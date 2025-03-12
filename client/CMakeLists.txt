project(Program1)

add_executable(prog1
	prog1.cpp
	ServerConnection.cpp ServerConnection.h
	BufferManager.cpp BufferManager.h)

target_include_directories(prog1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Указываем путь к заголовочным файлам (если не видит json.hpp)
target_include_directories(prog1 PRIVATE ${json_SOURCE_DIR}/single_include)
# Подключаем json.hpp
target_link_libraries(prog1 PRIVATE nlohmann_json::nlohmann_json)
