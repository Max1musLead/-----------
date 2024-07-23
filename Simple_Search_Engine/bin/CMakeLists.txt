add_executable(IndexProgram main.cpp)

target_link_libraries(IndexProgram PRIVATE SEARCH INDEX PARSE)

target_include_directories(IndexProgram PUBLIC ${PROJECT_SOURCE_DIR})