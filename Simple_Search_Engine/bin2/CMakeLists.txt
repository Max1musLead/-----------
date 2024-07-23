add_executable(SearchProgram main.cpp)

target_link_libraries(SearchProgram PRIVATE SEARCH INDEX PARSE)

target_include_directories(SearchProgram PUBLIC ${PROJECT_SOURCE_DIR})