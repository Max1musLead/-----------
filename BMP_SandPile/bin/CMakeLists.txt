add_executable(labwork3 main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE bmp parser sandpile)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
