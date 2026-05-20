cmake_minimum_required(VERSION 3.15)
project(SimpleSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем генерацию базы данных компиляции
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(sim_app
  sim/src/main.cpp
  sim/src/two-body-gravity.cpp
)

# Указываем CMake (и редактору), где лежат хедеры
target_include_directories(sim_app PRIVATE sim/include)
