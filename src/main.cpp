#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include <nlohmann/json.hpp>

#include <httplib.h>

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <cctype>
#include <algorithm>

#include "forecast.h"
#include "util.h"

using namespace ftxui;

int get_day(const std::string& date_str, std::string& day)
{
    std::tm tm = {};
    std::istringstream ss(date_str);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail())
    {
        std::cerr << "Failed to parse date string" << std::endl;
        return 1;
    }

    std::time_t time = std::mktime(&tm);
    if (time == -1)
    {
        std::cerr << "Failed to convert to time_t" << std::endl;
        return 1;
    }

    std::tm* time_tm = std::localtime(&time);
    day = util::days_of_week[time_tm->tm_wday];
    return 0;
}

int parse_location(const std::string& location_arg, float& lat, float& lon)
{
    if (location_arg.empty())
    {
        return 0;
    }

    if (location_arg.front() == '(' && location_arg.back() == ')')
    {
        auto coords = location_arg.substr(1, location_arg.size() - 2);
        std::replace(coords.begin(), coords.end(), ',', ' ');
        std::istringstream iss(coords);
        if (!(iss >> lat >> lon))
        {
            return 1;
        }
    }
    else
    {
        httplib::Client cli("https://nominatim.openstreetmap.org");
        std::string query = "/search?q=" + location_arg + "&format=json&limit=1";
        auto res = cli.Get(query.c_str());

        if (!res || res->status != 200)
        {
            std::cerr << "Failed to get location from openstreetmap, location: " << location_arg << std::endl;
            return 1;
        }

        auto json = nlohmann::json::parse(res->body);

        if (json.empty())
        {
            std::cerr << "Failed to parse location from openstreetmap, location: " << location_arg << std::endl;
            return 1;
        }

        lat = std::stod(json[0]["lat"].get<std::string>());
        lon = std::stod(json[0]["lon"].get<std::string>());
    }

    return 0;
}

int get_weather_data(float& lat, float &lon, int days, nlohmann::json& weather_data)
{
    httplib::Client cli("https://api.open-meteo.com");
    std::string query = "/v1/forecast?latitude=" + std::to_string(lat) + "&longitude=" + std::to_string(lon)
                        + "&hourly=temperature_2m,wind_speed_10m,wind_direction_10m,wind_gusts_10m&wind_speed_unit=ms&timezone=auto&days=" + std::to_string(days);
    auto const res = cli.Get(query.c_str());

    if (!res || res->status != 200)
    {
        std::cerr << "Failed to get weather data from meteo, lat: " << lat << ", lon: " << lon<< std::endl;
        return 1;
    }

    weather_data = nlohmann::json::parse(res->body);
    if (weather_data.empty())
    {
        std::cerr << "Failed to parse weather data from meteo, lat: " << lat << ", lon: " << lon<< std::endl;
        return 1;
    }

    return 0;
}

int handle_arguments(int argc, char** argv, std::string& location_arg, int& forecast_days, float& lat, float& lon)
{
    location_arg = "";
    forecast_days = 7;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h")
        {
            std::cerr << "Usage: " << argv[0] << " [--location <location>] [--forecast <days>]" << std::endl;
            return 0;
        }
        else if (arg == "--location" || arg == "-l")
        {
            location_arg = argv[++i];
        }
        else if (arg == "--forecast" || arg == "-f")
        {
            forecast_days = std::stoi(argv[++i]);
            if (forecast_days != 1 && forecast_days != 7 && forecast_days != 14)
            {
                forecast_days = 7;
                std::cerr << "--forecast option requires an argument of 1, 7, or 14. Defaulted to 7." << std::endl;
            }
        }
        else
        {
            std::cerr << "Usage: " << argv[0] << " [--location <location>] [--forecast <days>]" << std::endl;
            return 1;
        }
    }

    return parse_location(location_arg, lat, lon);
}

std::vector<Forecast> parse_weather_data(const nlohmann::json& weather_data)
{
    std::vector<Forecast> forecast_objects;
    const std::string wm{"wind_speed_10m"};
    const std::string wg{"wind_gusts_10m"};
    const std::string wd{"wind_direction_10m"};
    const std::string tmp{"temperature_2m"};
    const std::string hr{"hourly"};

    for (size_t i = 0; i < weather_data[hr]["time"].size(); i += 24)
    {
        std::string date = weather_data[hr]["time"][i].get<std::string>().substr(0, 10);
        std::vector<float> wms(weather_data[hr][wm].begin() + i, weather_data[hr][wm].begin() + i + 24);
        std::vector<float> wgs(weather_data[hr][wg].begin() + i, weather_data[hr][wg].begin() + i + 24);
        std::vector<float> wds(weather_data[hr][wd].begin() + i, weather_data[hr][wd].begin() + i + 24);
        std::vector<float> tmps(weather_data[hr][tmp].begin() + i, weather_data[hr][tmp].begin() + i + 24);
        forecast_objects.push_back(Forecast(date, wms, wgs, wds, tmps));
    }
    return forecast_objects;
}

std::vector<Element> create_spaced_elements(const std::string& unit, const std::vector<std::string>& values, const std::vector<Color>& colors)
{
    std::vector<Element> elements{text(unit) | size(WIDTH, EQUAL, 20)};
    for (size_t i = 0; i < values.size(); ++i)
    {
        elements.push_back(text(values[i]) | size(WIDTH, EQUAL, 6) | color(colors[i]));
    }
    return elements;
};

int main(int argc, char** argv)
{
    std::string location_arg;
    float lat{0.0}, lon{0.0};
    int forecast_days = 7;
    if (handle_arguments(argc, argv, location_arg, forecast_days, lat, lon))
    {
        return 1;
    }

    nlohmann::json weather_data;
    if (get_weather_data(lat, lon, forecast_days, weather_data))
    {
        std::cerr << "failed to parse the json_data from meteo" << std::endl;
        return 1;
    }

    const std::vector<Forecast> forecast_objects = parse_weather_data(weather_data);

    std::vector<Element> forecasts;

    for (const auto& forecast_obj : forecast_objects)
    {
        std::string day_name;
        if (get_day(forecast_obj.getDate(), day_name))
        {
            return 1;
        }

        // add hourly header
        std::vector<Element> time_elements = create_spaced_elements("", util::hours, std::vector<Color>(24, Color::White));

        std::vector<Element> wind_mean_elements = create_spaced_elements("Wind Mean (m/s)", forecast_obj.getWindMean(), forecast_obj.getWindMeanColorScheme());
        std::vector<Element> wind_gust_elements = create_spaced_elements("Wind Gusts (m/s)", forecast_obj.getWindGust(), forecast_obj.getWindGustColorScheme());
        std::vector<Element> wind_direction_elements = create_spaced_elements("Direction", forecast_obj.getWindDirection(), std::vector<Color>(24, Color::White));
        std::vector<Element> temperature_air_elements = create_spaced_elements("Temperature (°C)", forecast_obj.getTemperatureAir(), forecast_obj.getTemperatureAirColorScheme());

        auto day_box = vbox({
            hbox(time_elements),
            hbox(wind_mean_elements),
            hbox(wind_gust_elements),
            hbox(wind_direction_elements),
            hbox(temperature_air_elements),
        });
        forecasts.push_back(window(text(day_name), day_box));
    }

    if (!location_arg.empty())
    {
        forecasts.push_back(window(text("Location"), vbox(text(location_arg))));
    }
    else
    {
        forecasts.push_back(window(text("Location"), vbox(text("lat: " + std::to_string(lat) + ", lon: " + std::to_string(lon)))));
    }

    // Add location input element
    std::string location_input;
    auto input_component = Input(&location_input, "location");


    // Create menu layout
    auto menu = vbox({
        text("menu"),
        hbox({
            text("Enter location: "),
            input_component->Render()
        })
    });
    auto content = hbox({
        vbox(forecasts),
        menu
    });
    auto grid = window(text("--Windfnd--"), vbox(content));

    auto screen = ScreenInteractive::TerminalOutput();
    auto renderer = Renderer([&] { return grid; });
    screen.Loop(renderer);

    return 0;
}
