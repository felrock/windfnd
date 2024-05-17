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

RenderMethod create_render_method(std::vector<forecast::Forecast>& forecasts, ftxui::ScreenInteractive& screen, ftxui::Component component);

void start_ftxui_application(api::WeatherAPIClient& api_client);

}  // namespace render

#endif  // include_render_h_
