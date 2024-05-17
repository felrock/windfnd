#include <gtest/gtest.h>
#include "forecast.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using namespace forecast;

// Test fixture for the Forecast class
class ForecastTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load test data from test.json file
        std::ifstream file("test.json");
        if (file.is_open()) {
            file >> weather_data;
            file.close();
        } else {
            std::cerr << "Failed to open test.json" << std::endl;
        }
    }

    nlohmann::json weather_data;
};

// Test get_forecast_step method
TEST_F(ForecastTest, GetForecastStep) {
    Forecast forecast = create_forecast(weather_data);

    // Test data for the first hour
    auto result = forecast.get_forecast_step(0, 0);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "5.0");
    EXPECT_EQ(result[1], "7.0");
    EXPECT_EQ(result[2], "N");
    EXPECT_EQ(result[3], "15.0");
}

// Test update_forecast_step method
TEST_F(ForecastTest, UpdateForecastStep) {
    Forecast forecast;
    forecast.update_forecast_step(0, 0, 3.5, 6.5, 90, 10.0);

    // Verify the update
    auto result = forecast.get_forecast_step(0, 0);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "3.5");
    EXPECT_EQ(result[1], "6.5");
    EXPECT_EQ(result[2], "E");
    EXPECT_EQ(result[3], "10.0");
}

// Test create_forecast function
TEST_F(ForecastTest, CreateForecast) {
    Forecast forecast = create_forecast(weather_data);

    // Check multiple forecast steps to ensure data is loaded correctly
    auto result1 = forecast.get_forecast_step(0, 0);
    auto result2 = forecast.get_forecast_step(0, 1);
    auto result3 = forecast.get_forecast_step(0, 2);

    ASSERT_EQ(result1.size(), 4);
    ASSERT_EQ(result2.size(), 4);
    ASSERT_EQ(result3.size(), 4);

    EXPECT_EQ(result1[0], "5.0");
    EXPECT_EQ(result1[1], "7.0");
    EXPECT_EQ(result1[2], "N");
    EXPECT_EQ(result1[3], "15.0");

    EXPECT_EQ(result2[0], "4.2");
    EXPECT_EQ(result2[1], "6.2");
    EXPECT_EQ(result2[2], "NE");
    EXPECT_EQ(result2[3], "14.8");

    EXPECT_EQ(result3[0], "4.1");
    EXPECT_EQ(result3[1], "6.1");
    EXPECT_EQ(result3[2], "E");
    EXPECT_EQ(result3[3], "14.5");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
