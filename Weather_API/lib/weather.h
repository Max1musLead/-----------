#pragma once

#include <conio.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string api_location = "https://api.api-ninjas.com/v1/city?name=";
const std::string api_weather = "https://api.open-meteo.com/v1/forecast?";
const std::string api_time = "https://api.api-ninjas.com/v1/worldtime?city=";

bool offline_status = false;
cpr::Response offline_api_weather;
cpr::Response offline_api_time;
cpr::Response offline_api_location;

struct City {
    int latitude;
    int longitude;
    int cnt_qur_city = 0;
    std::string name_qur_city;
    int status = 0;
};

struct WeatherStr {
    std::string time;
    float temperature;
    int relative_wetness;
    float precipitation;
    int weather_code;
    float wind_speed;
    float wind_direction;
};

void CheckConnection();

cpr::Response GetLocation(std::string name_qur_city);

cpr::Response GetTime(std::string name_qur_city);

cpr::Response GetWeather(int latitude, int longitude, std::string cnt_days);

int ChangeCity(char key, int qur_city, int max_cnt_city);

City ChangeLocation(City old_city, json config);

std::string ChangeCntDays(char key, std::string cnt_days);

void PrintWeather(City city, std::string cnt_days);
