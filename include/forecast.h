#ifndef include_forecast_h_
#define include_forecast_h_

#include <vector>
#include <string>
#include <array>
#include <nlohmann/json.hpp>

namespace forecast
{

struct ForecastStep {
    int hour;
    float wind;
    float gust;
    int direction;
    float temperature;
};

enum ForecastLevel {LOW, NORMAL, HIGH};

class Forecast
{
public:
    Forecast(){};

    // Copy constructor
    Forecast(const Forecast& other);

    // Move constructor
    Forecast(Forecast&& other) noexcept;

    // Function to get formatted forecast data for a specific day and hour
    std::vector<std::string> get_forecast_step(int day, int hour) const;

    // Function to update the forecast data for a specific day and hour
    void update_forecast_step(int day, int hour, float wind, float gust, int dir, float temp);

private:
    std::string start_of_forecast_date_;
    std::array<std::array<ForecastStep, 24>, 7> forecasts_day_;

    const std::array<float, 5> wind_levels_{0.0, 7.0, 12.0, 25.0};
    const std::array<float, 5> gust_levels_{0.0, 9.0, 14.0, 25.0};
    const std::array<float, 5> temp_levels_{-20.0, 15.0, 25.0, 45.0};

    // Function to format a float to a string with precision 1
    std::string format_float(float value) const;

    // Function to convert direction in degrees to a compass direction string
    std::string convert_direction(int degrees) const;
};

// Function to create a Forecast object from a JSON object
void update_forecast(Forecast& forecast, const nlohmann::json& weather_data);

// Function to create a Forecast object from a JSON object
Forecast create_forecast(const nlohmann::json& weather_data);

}  // namespace forecast

#endif  // include_forecast_h_
