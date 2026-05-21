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

FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  GIT_TAG v3.0
)
FetchContent_MakeAvailable(argparse)

# Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
# Для GTest лучше отключить установку в систему
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Создаем библиотеку
# Теперь Body.cpp и Integrator.cpp живут здесь
add_library(simlib STATIC
  sim/src/Body.cpp
  sim/src/Integrator.cpp
)
target_include_directories(simlib PUBLIC sim/include)
target_link_libraries(simlib PUBLIC Eigen3::Eigen)

# что собирать
add_executable(sim
  sim/main.cpp
)

# Связываем проект с библиотеками
target_link_libraries(sim PRIVATE
  simlib
  Eigen3::Eigen
  argparse
)

# Тесты
enable_testing()
add_executable(test_physics tests/test_physics.cpp)
target_include_directories(test_physics PRIVATE sim/include)
target_link_libraries(test_physics PRIVATE simlib GTest::gtest_main)