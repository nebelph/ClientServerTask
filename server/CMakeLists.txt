project(Program2)

add_executable(prog2 prog2.cpp Server.cpp Server.h)

target_include_directories(prog2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Указываем путь к заголовочным файлам (если не видит json.hpp)
target_include_directories(prog2 PRIVATE ${json_SOURCE_DIR}/single_include)
# Подключаем json.hpp
target_link_libraries(prog2 PRIVATE nlohmann_json::nlohmann_json)
