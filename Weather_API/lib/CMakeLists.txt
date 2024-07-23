add_library(weather weather.cpp)

target_link_libraries(weather PRIVATE cpr::cpr)
target_link_libraries(weather PRIVATE nlohmann_json::nlohmann_json)