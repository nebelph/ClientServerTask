cmake_minimum_required(VERSION 3.10)
project(App)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(client)
add_subdirectory(server)

include(FetchContent)
# Загружаем nlohmann/json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/latest/download/json.tar.xz
)
FetchContent_MakeAvailable(json)
