#include <gtest/gtest.h>
#include "forecast.h"
#include <json.hpp>
#include <fstream>
#include <iostream>

using namespace forecast;

// Test fixture for the Forecast class
class ForecastTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load test data from test.json file
        std::ifstream file("test/aux/test.json");
        if (file.is_open()) {
            file >> weather_data;
            file.close();
        } else {
            std::cerr << " Failed to open test.json" << std::endl;
        }
    }

    nlohmann::json weather_data;
};

// Test get_forecast_step method
TEST_F(ForecastTest, GetForecastStep) {
    Forecast forecast = create_forecast(weather_data);

    // Test data for the first hour
    auto result = forecast.get_forecast_step(6, 23);
    std::cout <<  result[3] << std::endl;
    std::cout <<  result[0] << std::endl;
}

// Test update_forecast_step method
TEST_F(ForecastTest, UpdateForecastStep) {
    Forecast forecast;
    forecast.update_forecast_step(0, 0, 3.5, 6.5, 90, 10.0);

    // Verify the update
    auto result = forecast.get_forecast_step(0, 0);
}

// Test create_forecast function
TEST_F(ForecastTest, CreateForecast) {
    Forecast forecast = create_forecast(weather_data);

    // Check multiple forecast steps to ensure data is loaded correctly
    auto result1 = forecast.get_forecast_step(0, 0);
    auto result2 = forecast.get_forecast_step(0, 1);
    auto result3 = forecast.get_forecast_step(0, 2);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
