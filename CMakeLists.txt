cmake_minimum_required(VERSION 3.15)
project(SimpleSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем генерацию базы данных компиляции
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Добавляем внешние библиотеки
include(FetchContent)
FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0
)

set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE) # Это отключит поиск Boost в Eigen
FetchContent_MakeAvailable(eigen)

# что собирать
add_executable(sim
  sim/main.cpp
  sim/src/Body.cpp
  sim/src/Integrator.cpp
)

# Связываем проект с Eigen
target_link_libraries(sim PRIVATE Eigen3::Eigen)

# Указываем CMake (и редактору), где лежат хедеры
target_include_directories(sim PRIVATE sim/include)
