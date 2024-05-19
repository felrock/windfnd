#ifndef include_render_h_
#define include_render_h_

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include <vector>
#include <functional>

#include "forecast.h"
#include "util.h"
#include "weather_api.h"

namespace render
{

using RenderMethod = std::function<ftxui::Element()>;

void get_location(const nlohmann::json& location_data, std::string& lat, std::string& lon);

ftxui::Component create_components(std::string& location_input, forecast::Forecast& forecast, api::WeatherAPIClient& api_client);

ftxui::Color get_color_from_forecastlevel(const forecast::ForecastLevel level);

RenderMethod create_render_method(std::vector<forecast::Forecast>& forecasts, ftxui::ScreenInteractive& screen, ftxui::Component component);

void start_ftxui_application(api::WeatherAPIClient& api_client);

}  // namespace render

#endif  // include_render_h_
