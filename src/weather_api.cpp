#include "weather_api.h"

namespace api
{

WeatherAPIClient::WeatherAPIClient()
    : location_client("https://nominatim.openstreetmap.org")
    , weather_client("https://api.open-meteo.com")
{
}

nlohmann::json WeatherAPIClient::get_location(const std::string& location_arg)
{
    nlohmann::json json;
    if (location_arg.empty())
    {
        return json;
    }

    std::string query = "/search?q=" + location_arg + "&format=json&limit=1";
    auto res = location_client.Get(query.c_str());

    if (res && res->status == 200) {
        json = nlohmann::json::parse(res->body);
        if (!json.empty()) {
            json = json[0];
        }
    }

    return json;
}

nlohmann::json WeatherAPIClient::get_weather(std::string const lat, std::string const lon)
{
    nlohmann::json json;
    std::string query = "/v1/forecast?latitude=" + lat+ "&longitude=" + lon
                        + "&hourly=temperature_2m,wind_speed_10m,wind_direction_10m,wind_gusts_10m&wind_speed_unit=ms&timezone=auto&days=7";
    auto res = weather_client.Get(query.c_str());

    if (res && res->status == 200)
    {
        json = nlohmann::json::parse(res->body);
    }

    return json;
}

}  // namespace api
