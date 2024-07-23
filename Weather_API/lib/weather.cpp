#include "weather.h"

void CheckConnection() { // Проверка соединения
    cpr::Response r = cpr::Get(cpr::Url{"https://ya.ru/"});
    if (r.status_code == 200) {
        return;
    } else {
        offline_status = true;
        return; 
    }
}

cpr::Response GetTime(std::string name_qur_city) { 
    cpr::Response r = cpr::Get(cpr::Url{api_time + name_qur_city},
                            cpr::Header{{"X-Api-Key", "ocfL2h++4wIndF023qzpkA==mztabuqxCYFAN2Ec"}});
    if (r.status_code == 200) {
        std::cout << "Time Sucess connection" << std::endl;
        offline_api_time = r;
        offline_status = false;
        return r;
    } else {
        std::cerr << "Time - отсутствует подключение к интернету или доступ к api" << std::endl;
        offline_status = true;
        return offline_api_time; 
    }
}    

cpr::Response GetLocation(std::string name_qur_city) {
    cpr::Response r = cpr::Get(cpr::Url{api_location + name_qur_city},
                            cpr::Header{{"X-Api-Key", "ocfL2h++4wIndF023qzpkA==mztabuqxCYFAN2Ec"}});
    if (r.status_code == 200) {
        std::cout << "Location Sucess connection" << std::endl;
        offline_api_location = r;
        offline_status = false;
        return r;
    } else {
        std::cerr << "Location - отсутствует подключение к интернету или доступ к api" << std::endl;
        offline_status = true;
        return offline_api_location; 
    }
}

cpr::Response GetWeather(int latitude, int longitude, std::string cnt_days) {
    cpr::Response g = cpr::Get(cpr::Url{api_weather + "latitude=" + std::to_string(latitude) + "&longitude=" + std::to_string(longitude)
                                + "&current=temperature_2m,relative_humidity_2m,precipitation,weather_code,wind_speed_10m,wind_direction_10m"
                                + "&daily=weather_code,temperature_2m_max,temperature_2m_min,precipitation_sum,precipitation_probability_max,wind_speed_10m_max"
                                + "&timezone=auto&forecast_days=" + cnt_days});
    if (g.status_code == 200) {
        std::cout << "Weather Sucess connection" << std::endl;
        offline_api_weather = g;
        offline_status = false;
        return g;
    } else {
        std::cerr << "Weather - отсутствует подключение к интернету или доступ к api" << std::endl;
        offline_status = true;
        return offline_api_weather;
    }
}

int ChangeCity(char key, int qur_city, int max_cnt_city) { // Переключение между городами
    if (offline_status) {
        return qur_city;
    }

    if (key == 'p') {
        if (qur_city == 0) {
            return (max_cnt_city - 1);
        }
        return --qur_city;
    } else {
        if (qur_city == (max_cnt_city - 1)) {
            return 0;
        }
        return ++qur_city;
    }
}

City ChangeLocation(City old_city, json config) { // Смена координат города
    config.at("cities_names").at(old_city.cnt_qur_city).get_to(old_city.name_qur_city);

    cpr::Response r = GetLocation(old_city.name_qur_city);

    if (r.status_code != 200) {
        old_city.status = 1;
        return old_city;
    }

    json j = json::parse(r.text.substr(1, r.text.length() - 2));

    j.at("latitude").get_to(old_city.latitude);
    j.at("longitude").get_to(old_city.longitude);

    return old_city;
}

std::string ChangeCntDays(char key, std::string cnt_days) { // Кол-во дней прогноза
    if (offline_status) {
        return cnt_days;
    }

    int num = std::stoi(cnt_days);

    if (key == '+') {
        if (num == 10) {
            std::cerr << "It's maximum, pls press minus or don't change count of days" << std::endl;
            return "10";
        }
        num++;
    } else {
        if (num == 0) {
            std::cerr << "It's minimum, pls press plus or don't change count of days" << std::endl;
            return "0";
        }
        num--;
    }

    return std::to_string(num);
}

void PrintSky(int code) {
    switch (code) {
    case 0:
        std::cout << "Чистое небо";
        break;
    case 1:
        std::cout << "Преимущественно ясно";
        break;
    case 2:
        std::cout << "Переменная облачность";
        break;
    case 3:
        std::cout << "Пасмурно";
        break;
    case 45:
        std::cout << "Туман";
        break;
    case 48:
        std::cout << "Отложение инея";
        break;
    case 51:
        std::cout << "Легкая морось";
        break;
    case 53:
        std::cout << "Средняя морось";
        break;
    case 55:
        std::cout << "Сильная морось";
        break;
    case 56:
        std::cout << "Моросящий дождь";
        break;
    case 57:
        std::cout << "Моросящий дождь";
        break;
    case 61:
        std::cout << "Небольшой дождь";
        break;
    case 63:
        std::cout << "Умеренный дождь";
        break;
    case 65:
        std::cout << "Сильный дождь";
        break;
    case 66:
        std::cout << "Небольшой ледяной дождь";
        break;
    case 67:
        std::cout << "Интенсивный ледяной дождь";
        break;
    case 71:
        std::cout << "Слабый снегопад";
        break;
    case 73:
        std::cout << "Средний снегопад";
        break;
    case 75:
        std::cout << "Сильный снегопад";
        break;
    case 77:
        std::cout << "Снежные зерна";
        break;
    case 80:
        std::cout << "Слабый ливень";
        break;
    case 81:
        std::cout << "Средний ливень";
        break;
    case 82:
        std::cout << "Сильный ливень";
        break;
    case 85:
        std::cout << "Небольшой снегопад";
        break;
    case 86:
        std::cout << "Сильный снегопад";
        break;
    case 95:
        std::cout << "Гроза";
        break;
    case 96:
        std::cout << "Гроза с небольшим градом";
        break;
    case 99 :
        std::cout << "Гроза с сильным градом";
        break;
    default:
        break;
    }
}

std::string NumDay(int cnt_days) { // Красивый вывод дней
    switch (cnt_days) {
    case 1:
        return "1 день";
        break;
    case 2:
        return "2 дня";
        break;
    case 3:
        return "3 дня";
        break;
    case 4:
        return "4 дня";
        break;
    default:
        return (std::to_string(cnt_days) + " дней");
        break;
    }
}

void PrintWeather(City city, std::string cnt_days) {
    SetConsoleOutputCP(CP_UTF8);
    WeatherStr cur_weather;
    WeatherStr days_weather;

    cpr::Response g = GetWeather(city.latitude, city.longitude, cnt_days);
    json j = json::parse(g.text);

    cpr::Response t = GetTime(city.name_qur_city);
    json time = json::parse(t.text);

    time.at("datetime").get_to(cur_weather.time);
    j.at("current").at("temperature_2m").get_to(cur_weather.temperature);
    j.at("current").at("relative_humidity_2m").get_to(cur_weather.relative_wetness);
    j.at("current").at("precipitation").get_to(cur_weather.precipitation);
    j.at("current").at("weather_code").get_to(cur_weather.weather_code);
    j.at("current").at("wind_speed_10m").get_to(cur_weather.wind_speed);
    j.at("current").at("wind_direction_10m").get_to(cur_weather.wind_direction);

    std::cout << "\n" << "Прогноз погоды для " << city.name_qur_city << ", данные приведены для даты и времени " << cur_weather.time << "\n" 
    << "-----------------------------------------------------------------" << "\n" << "Погода в городе сейчас:";
    std::cout << "\n" << "Температура воздуха " << cur_weather.temperature << " °C," << " относительная влажность " << cur_weather.relative_wetness << " %"<< "\n";
    PrintSky(cur_weather.weather_code);
    std::cout << ", количество осадков " << cur_weather.precipitation << " mm" << "\n";
    std::cout << "Скорость ветра " << cur_weather.wind_speed << " km/h," << " и его направление " << cur_weather.wind_direction << " относительно севера" << "\n";
    std::cout << "-----------------------------------------------------------------" << "\n" << "\n";
    
    std::cout << "Прогноз погоды на " << NumDay(stoi(cnt_days)) << " вперёд:" << "\n" << "\n";

    for (int i = 0; i < std::stoi(cnt_days); i++) {
        j.at("daily").at("time").at(i).get_to(days_weather.time);
        j.at("daily").at("weather_code").at(i).get_to(days_weather.weather_code);
        j.at("daily").at("temperature_2m_min").at(i).get_to(days_weather.wind_direction);
        j.at("daily").at("temperature_2m_max").at(i).get_to(days_weather.temperature);
        j.at("daily").at("precipitation_sum").at(i).get_to(days_weather.precipitation);
        j.at("daily").at("wind_speed_10m_max").at(i).get_to(days_weather.wind_speed);
        j.at("daily").at("precipitation_probability_max").at(i).get_to(days_weather.relative_wetness);

        std::cout << days_weather.time << "\n";
        PrintSky(days_weather.weather_code);
        std::cout << ", темература воздуха макс(мин) " << days_weather.temperature << "(" << days_weather.wind_direction << ")" << " °C" << "\n";
        std::cout << "Максимальная вероятность выпадения осадков " << days_weather.relative_wetness << " %, сумма осадков " << days_weather.precipitation << " mm" << "\n";
        std::cout << "Скорость ветра " << days_weather.wind_speed << " km/h" << "\n";
        std::cout << "\n";
    }

    //mute.unlock();
}
