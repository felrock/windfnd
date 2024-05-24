#include "render.h"

namespace render
{
using namespace ftxui;

//TODO(felrock): Add tests for this method later, using "seach_location.json"
void get_location(const nlohmann::json& location_data, std::string& lat, std::string& lon)
{
    if (location_data.is_null() || location_data.empty()) {
        return;
    }
    lat = location_data["lat"].get<std::string>();
    lon = location_data["lon"].get<std::string>();
}

ftxui::Component create_components(std::string& location_input, std::string& error_msg, forecast::Forecast& forecast, api::WeatherAPIClient& api_client)
{
    auto input_component = Input(&location_input, "", InputOption::Default());
    // ignore enter keys from the user
    input_component |= CatchEvent([](Event event) {
        if (event == Event::Return) {
            return true;
        }
        return false;
    });

    auto query_button = Button("Query", [&] {
        // nothing to see here
        if (location_input.empty())
        {
            return;
        }

        // Fetch location
        std::string lat, lon;
        auto location_json = api_client.get_location(location_input);
        get_location(location_json, lat, lon);

        if (location_json.empty() || lat.size() == 0)
        {
            error_msg = "Failed to find location";
        }

        auto weather_json = api_client.get_weather(lat, lon);
        if (!weather_json.empty())
        {
            forecast::update_forecast(forecast, weather_json);
        }
        else
        {
            error_msg = "Failed to get the weather forecast";
        }
    }, ButtonOption::Ascii());

    return Container::Horizontal({input_component | size(HEIGHT, EQUAL, 1),
        query_button | size(HEIGHT, EQUAL, 1)});
}

ftxui::Color get_color_from_forecastlevel(const forecast::ForecastLevel level)
{
    if (level == forecast::ForecastLevel::LOW)
    {
        return Color::Blue;
    }
    else if(level == forecast::ForecastLevel::NORMAL)
    {
        return Color::Green;
    }
    else if(level == forecast::ForecastLevel::HIGH)
    {
        return Color::Red;
    }
    else
    {
        return Color::White;
    }
}

// TODO(felrock): Add different rendering stages, menu, forecast, load from config...
RenderMethod create_render_method(Component& component, forecast::Forecast& forecast, std::string const& error_msg)
{
    return [&]() -> ftxui::Element {
        std::vector<Element> forecast_elements;
        for (int day = 0; day < 7; ++day)
        {
            std::string day_name{""};
            std::string const start_date{forecast.get_start_date()};
            if (!start_date.empty())
            {
                util::get_day(start_date, day_name, day);
            }

            std::vector<Element> day_elements;
            std::vector<Element> time_elements{text("") | size(WIDTH, EQUAL, 20)};
            for (const auto& hour : util::hours) {
                time_elements.push_back(text(hour) | size(WIDTH, EQUAL, 6));
            }

            std::vector<Element> wind_mean_elements{text("Wind Mean (m/s)") | size(WIDTH, EQUAL, 20)};
            std::vector<Element> wind_gust_elements{text("Wind Gusts (m/s)") | size(WIDTH, EQUAL, 20)};
            std::vector<Element> wind_direction_elements{text("Direction") | size(WIDTH, EQUAL, 20)};
            std::vector<Element> temperature_air_elements{text("Temperature (°C)") | size(WIDTH, EQUAL, 20)};

            for (int hour = 0; hour < 24; ++hour)
            {
                auto forecast_step = forecast.get_forecast_step(day, hour);
                auto forecast_intensity = forecast.get_forecast_level(day, hour);
                if (!forecast_step.empty()) {
                    wind_mean_elements.push_back(text(forecast_step[0]) | size(WIDTH, EQUAL, 6)
                                                 |  color(get_color_from_forecastlevel(forecast_intensity[0])));
                    wind_gust_elements.push_back(text(forecast_step[1]) | size(WIDTH, EQUAL, 6)
                                                 |  color(get_color_from_forecastlevel(forecast_intensity[1])));
                    wind_direction_elements.push_back(text(forecast_step[2]) | size(WIDTH, EQUAL, 6));
                    temperature_air_elements.push_back(text(forecast_step[3]) | size(WIDTH, EQUAL, 6)
                                                 |  color(get_color_from_forecastlevel(forecast_intensity[2])));
                }
            }

            day_elements.push_back(hbox(time_elements));
            day_elements.push_back(hbox(wind_mean_elements));
            day_elements.push_back(hbox(wind_gust_elements));
            day_elements.push_back(hbox(wind_direction_elements));
            day_elements.push_back(hbox(temperature_air_elements));

            auto day_box = vbox(day_elements);
            forecast_elements.push_back(window(text(day_name), day_box));
        }
        // Menu layout
        auto menu = window(text("Menu"),
            hbox({
                text("Enter location: "),
                component->Render(),
                separator(),
                text(error_msg) | color(Color::Red)
            })
        );

        auto content = vbox({
            menu,
            separatorEmpty() | size(HEIGHT, EQUAL, 2),
            vbox(forecast_elements),
        });
        return window(text("--Windfnd--"), vbox(content) | center);
    };
}

void start_ftxui_application(api::WeatherAPIClient& api_client)
{
    ScreenInteractive screen = ScreenInteractive::Fullscreen();
    forecast::Forecast forecast;
    std::string location_input{""};
    std::string error_msg{""};

    auto components = create_components(location_input, error_msg, forecast, api_client);
    RenderMethod render_method = create_render_method(components, forecast, error_msg);
    auto renderer = Renderer(components, render_method);
    screen.Loop(renderer);
}
}  // namespace render
