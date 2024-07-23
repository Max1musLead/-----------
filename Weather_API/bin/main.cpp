#include <lib/weather.h>

int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8);

    City city;
    int freq;
    std::string cnt_days;

    time_t cur_time = time(nullptr);

    std::fstream f("config.json"); // Работа с конфигом
    json config = json::parse(f);
    config.at("cities_names").at(city.cnt_qur_city).get_to(city.name_qur_city);
    config.at("number_of_days").get_to(cnt_days);
    config.at("frequency").get_to(freq);

    city = ChangeLocation(city, config); // Api запрос местоположение
    if (city.status != 0) {
        std::cerr << "Отсутствует подключение к интернету или доступ к api";
        return 0;
    }

    system("cls");
    PrintWeather(city, cnt_days); // Первичный вывод погоды

    while(true) {
        if (char key = getch()) {
            system("cls");
            CheckConnection();
            cur_time = time(nullptr);
            if (key == 27) {
                return 0;
            } else if (key == 'n' || key == 'p') {
                city.cnt_qur_city = ChangeCity(key, city.cnt_qur_city, config["cnt_cities"]);
                city = ChangeLocation(city, config);
            } else if (key == '+' || key == '-') {
                cnt_days = ChangeCntDays(key, cnt_days);
            }
            PrintWeather(city, cnt_days);
            while(kbhit()) getch();
        } else if (time(nullptr) - cur_time == freq) {
            cur_time = time(nullptr);
            system("cls");
            PrintWeather(city, cnt_days);
        }
    }
}
