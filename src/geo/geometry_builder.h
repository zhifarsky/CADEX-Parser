#pragma once
#include <memory>

#include "../logging/logging.h"
#include "geometry.h"

// исправляем все параметры кривых перед вызовом конструкторов
// если какой-то параметр не исправлен, конструктор вызовет исключение
//
// отрицательный радиус берем по модулю
// если радиус/шаг близок к 0, преобразуем в фигуру другого типа

class GeometryBuilder {
	GeometryConfig cfg;

 public:
	GeometryBuilder(const GeometryConfig& cfg_);

	std::unique_ptr<Geometry> CreateCircle(std::string label, u64 id, vec3 origin, f64 radius) const;
	std::unique_ptr<Geometry> CreateEllipse(std::string label, u64 id, vec3 origin, f64 radiusX, f64 radiusY) const;
	std::unique_ptr<Geometry> CreateHelix(std::string label, u64 id, vec3 origin, f64 radius, f64 step) const;
};