#include <gtest/gtest.h>

#include "weather_api.h"

// Test fixture for the get_day function
class WeatherAPIClientTest: public ::testing::Test {};

// Test edge cases
TEST_F(WeatherAPIClientTest, LocationTest)
{
    std::string const know_location{"New York"};
    api::WeatherAPIClient api{};
    auto resp = api.get_location(know_location);
    EXPECT_TRUE(not resp.empty());
    std::string const lat = resp["lat"].get<std::string>();
    std::string const lon = resp["lon"].get<std::string>();
    EXPECT_EQ(lat, "40.7127281");
    EXPECT_EQ(lon, "-74.0060152");

    std::string const know_nonlocation{"Poopyville"};
    resp = api.get_location(know_nonlocation);
    EXPECT_TRUE(resp.empty());
}

// Test edge cases
TEST_F(WeatherAPIClientTest, ForecastTest)
{
    api::WeatherAPIClient api{};
    auto resp = api.get_weather("0", "0");
    EXPECT_TRUE(not resp.empty());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
