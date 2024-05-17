#include <iostream>
#include <string>

#include "forecast.h"
#include "weather_api.h"
#include "render.h"

int main(int argc, char** argv) {
    // Initialize the WeatherAPIClient
    api::WeatherAPIClient api_client;

    // Start the FTXUI application
    render::start_ftxui_application(api_client);

    return 0;
}
