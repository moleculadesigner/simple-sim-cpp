#pragma once
namespace sim {

// Физические константы
constexpr double GRAVITY_CONSTANT = 6.67430e-11;      // m³/(kg·s²)
constexpr double LJ_SCALE_DEFAULT = 1e-10;            // m (компенсация столкновений))

// Численные параметры
constexpr double EPSILON = 1e-12;                     // защита от деления на ноль
} // namespace sim