#include "forecast.h"
#include <sstream>
#include <iomanip>

namespace forecast
{

Forecast::Forecast()
{
    for (size_t day = 0; day < 7; ++day)
    {
        for (size_t hour = 0; hour < 24; ++hour)
        {
            size_t index = day * 24 + hour;
            this->update_forecast_step(day, hour,0.0, 0.0, 0, 0.0);
        }
    }
}

Forecast::Forecast(const Forecast& other)
    : start_of_forecast_date_(other.start_of_forecast_date_),
      forecasts_day_(other.forecasts_day_),
      wind_levels_(other.wind_levels_),
      gust_levels_(other.gust_levels_),
      temp_levels_(other.temp_levels_)
{
}

Forecast::Forecast(Forecast&& other) noexcept
    : start_of_forecast_date_(std::move(other.start_of_forecast_date_)),
      forecasts_day_(std::move(other.forecasts_day_)),
      wind_levels_(std::move(other.wind_levels_)),
      gust_levels_(std::move(other.gust_levels_)),
      temp_levels_(std::move(other.temp_levels_))
{
}

std::vector<std::string> Forecast::get_forecast_step(int day, int hour) const
{
    if (day < 0 || day >= 7 || hour < 0 || hour >= 24) {
        return {};
    }
    const ForecastStep& step = forecasts_day_[day][hour];
    return {
        format_float(step.wind),
        format_float(step.gust),
        convert_direction(step.direction),
        format_float(step.temperature)
    };
}

void Forecast::update_forecast_step(int day, int hour, float wind, float gust, int dir, float temp)
{
    if (day < 0 || day >= 7 || hour < 0 || hour >= 24) {
        return; // Out of bounds check
    }
    forecasts_day_[day][hour] = {hour, wind, gust, dir, temp};
}

std::string Forecast::format_float(float value) const
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << value;
    return out.str();
}

std::string Forecast::convert_direction(int degrees) const
{
    static const std::array<std::string, 8> directions{
        "N", "NE", "E", "SE", "S", "SW", "W", "NW"
    };
    int index = static_cast<int>((degrees / 45.0) + 0.5) % 8;
    return directions[index];
}

void Forecast::set_start_date(const std::string& start_date)
{
    start_of_forecast_date_ = start_date;
}

std::vector<ForecastLevel> Forecast::get_forecast_level(int day, int hour) const
{
    const ForecastStep& step = forecasts_day_[day][hour];

    ForecastLevel wind_level{ForecastLevel::LOW};
    if (step.wind > wind_levels_[0])
    {
        wind_level = ForecastLevel::NORMAL;
    }
    if (step.wind > wind_levels_[1])
    {
        wind_level = ForecastLevel::HIGH;
    }

    ForecastLevel gust_level{ForecastLevel::LOW};
    if (step.gust > gust_levels_[0])
    {
        gust_level = ForecastLevel::NORMAL;
    }
    if (step.gust > gust_levels_[1])
    {
        gust_level = ForecastLevel::HIGH;
    }

    ForecastLevel temp_level{ForecastLevel::LOW};
    if (step.temperature > temp_levels_[0])
    {
        temp_level = ForecastLevel::NORMAL;
    }
    if (step.temperature > temp_levels_[1])
    {
        temp_level = ForecastLevel::HIGH;
    }
    return {wind_level, gust_level, temp_level};
}

std::string Forecast::get_start_date() const
{
    return start_of_forecast_date_;
}

void update_forecast(Forecast& forecast, const nlohmann::json& weather_data)
{
    const std::string wm{"wind_speed_10m"};
    const std::string wg{"wind_gusts_10m"};
    const std::string wd{"wind_direction_10m"};
    const std::string tmp{"temperature_2m"};
    const std::string hr{"hourly"};

    // first data of the forecast, take that time
    forecast.set_start_date(weather_data[hr]["time"][0].get<std::string>());

    for (size_t day = 0; day < 7; ++day)
    {
        for (size_t hour = 0; hour < 24; ++hour)
        {
            size_t index = day * 24 + hour;
            forecast.update_forecast_step(day, hour,
                weather_data[hr][wm][index].get<float>(),
                weather_data[hr][wg][index].get<float>(),
                weather_data[hr][wd][index].get<int>(),
                weather_data[hr][tmp][index].get<float>()
            );
        }
    }
}

Forecast create_forecast(const nlohmann::json& weather_data)
{
    Forecast forecast;
    update_forecast(forecast, weather_data);
    return forecast;
}

}  // namespace forecast
