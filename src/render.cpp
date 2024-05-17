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

ftxui::Component create_components(std::string& location_input, forecast::Forecast& forecast, api::WeatherAPIClient& api_client)
{
    auto input_component = Input(&location_input, "Enter location");
    auto query_button = Button("Query", [&] {
        // Fetch location
        std::string lat, lon;
        auto location_json = api_client.get_location(location_input);
        get_location(location_json, lat, lon);

        auto weather_json = api_client.get_weather(lat, lon);
        if (!weather_json.is_null()) {
          forecast::update_forecast(forecast, weather_json);
        }
    });

    return Container::Horizontal({input_component, query_button});
}

RenderMethod create_render_method(Component& component, forecast::Forecast& forecast)
{
    return [&]() -> ftxui::Element {
        std::vector<Element> forecast_elements;
        for (int day = 0; day < 7; ++day)
        {
            std::string day_name = "Day " + std::to_string(day);  // Placeholder for day name
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
                if (!forecast_step.empty()) {
                    wind_mean_elements.push_back(text(forecast_step[0]) | size(WIDTH, EQUAL, 6));
                    wind_gust_elements.push_back(text(forecast_step[1]) | size(WIDTH, EQUAL, 6));
                    wind_direction_elements.push_back(text(forecast_step[2]) | size(WIDTH, EQUAL, 6));
                    temperature_air_elements.push_back(text(forecast_step[3]) | size(WIDTH, EQUAL, 6));
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
        auto menu = vbox({
            text("Menu"),
            hbox({
                text("Enter location: "),
                component->Render()
            })
        });

        auto content = hbox({
            vbox(forecast_elements),
            menu
        });
        return window(text("--Windfnd--"), vbox(content));
    };
}

void start_ftxui_application(api::WeatherAPIClient& api_client)
{
    ScreenInteractive screen = ScreenInteractive::TerminalOutput();
    forecast::Forecast forecast;
    std::string location_input{"New York"};

    auto components = create_components(location_input, forecast, api_client);
    RenderMethod render_method = create_render_method(components, forecast);
    auto renderer = Renderer(components, render_method);
    screen.Loop(renderer);
}
}  // namespace render
