#ifndef include_weather_api_h_
#define include_weather_api_h_

#include <nlohmann/json.hpp>
#include <httplib.h>
#include <string>

namespace api
{

class WeatherAPIClient {
public:
    WeatherAPIClient();

    nlohmann::json get_location(const std::string& location_arg);
    nlohmann::json get_weather(std::string const lat, std::string const lon);

private:
    httplib::Client location_client;
    httplib::Client weather_client;
};

}  // namespace api

#endif  // include_weather_api_h_
