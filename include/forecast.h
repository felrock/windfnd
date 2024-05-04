#ifndef include_forecast_h_
#define include_forecast_h_

#include <vector>
#include <string>
#include <iomanip>

#include <ftxui/dom/elements.hpp>

class Forecast
{
public:
    Forecast(const std::string& date, const std::vector<float>& wind_mean, const std::vector<float>& wind_gust,
             const std::vector<float>& wind_direction, const std::vector<float>& temperature_air)
        : date(date), wind_mean(wind_mean), wind_gust(wind_gust), wind_direction(wind_direction),
          temperature_air(temperature_air)
    {}

    std::string getDate() const
    {
        return date;
    }

    std::vector<std::string> getWindMean() const
    {
        return format_values(wind_mean);
    }

    std::vector<std::string> getWindGust() const
    {
        return format_values(wind_gust);
    }

    std::vector<std::string> getWindDirection() const
    {
        std::vector<std::string> formatted_values;
        std::vector<std::string> directions = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
                                               "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
        for (float value : wind_direction)
        {
            int val = static_cast<int>((value / 22.5) + 0.5);
            formatted_values.push_back(directions[val % 16]);
        }
        return formatted_values;
    }

    std::vector<std::string> getTemperatureAir() const
    {
        return format_values(temperature_air);
    }

    std::vector<ftxui::Color> getWindMeanColorScheme() const
    {
        return generate_color_scheme(wind_mean, 1, 7, 7, 12, 25);
    }

    std::vector<ftxui::Color> getWindGustColorScheme() const
    {
        return generate_color_scheme(wind_gust, 1, 10, 10, 14, 25);
    }

    std::vector<ftxui::Color> getTemperatureAirColorScheme() const
    {
        return generate_color_scheme(temperature_air, -20, 5, 5, 25, 45);
    }

private:
    std::string date;
    std::vector<float> wind_mean;
    std::vector<float> wind_gust;
    std::vector<float> wind_direction;
    std::vector<float> temperature_air;

    std::vector<std::string> format_values(const std::vector<float>& values) const
    {
        std::vector<std::string> formatted_values;
        for (float value : values)
        {
            std::ostringstream out;
            out << std::fixed << std::setprecision(1) << value;
            formatted_values.push_back(out.str());
        }
        return formatted_values;
    }

    std::vector<ftxui::Color> generate_color_scheme(const std::vector<float>& values, float low_min, float low_max, float mid_min, float mid_max, float high_max) const
    {
        std::vector<ftxui::Color> color_scheme;
        for (float value : values)
        {
            if (value >= low_min && value <= low_max)
            {
                color_scheme.push_back(ftxui::Color::Blue);
            }
            else if (value > mid_min && value <= mid_max)
            {
                color_scheme.push_back(ftxui::Color::Green);
            }
            else if (value > mid_max && value <= high_max)
            {
                color_scheme.push_back(ftxui::Color::Red);
            }
            else
            {
                color_scheme.push_back(ftxui::Color::Default); // Default color for out-of-range values
            }
        }
        return color_scheme;
    }
};

#endif  // include_forecast_h_
