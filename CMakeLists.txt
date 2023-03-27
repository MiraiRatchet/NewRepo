cmake_minimum_required(VERSION 3.22)
set (ProjectName IMDB_data_analyze)
set(CMAKE_CXX_STANDARD 20)
project(${ProjectName})

add_subdirectory(App)

option(TestingEnabled ON)
if(${TestingEnabled})
    add_subdirectory(tests)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${ProjectName} src/imdbdataanalyze.cpp)
target_link_libraries(${ProjectName} App)