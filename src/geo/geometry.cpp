#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "geometry.h"

//
// vec3
//

vec3::vec3() : x(0), y(0), z(0) {};
vec3::vec3(f64 x_, f64 y_, f64 z_) : x(x_), y(y_), z(z_) {};

vec3 vec3::operator+(vec3 other) const {
	return {x + other.x, y + other.y, z + other.z};
}
vec3 vec3::operator-(vec3 other) const {
	return {x - other.x, y - other.y, z - other.z};
}
vec3 vec3::operator*(f64 value) const {
	return {x * value, y * value, z * value};
}
vec3 vec3::operator/(f64 value) const {
	return {x / value, y / value, z / value};
}

f64 vec3::Length() {
	return std::sqrt(x * x + y * y + z * z);
}
vec3 vec3::Normalized(f64 epsilon) {
	if (Length() < epsilon) {
		return {0, 0, 0};
	}
	return *this / Length();
}

//
// Geometry Config
//

GeometryConfig::GeometryConfig(f64 epsilon_) : epsilon(epsilon_) {}

//
// Geometry
//

Geometry::Geometry() {};

Geometry::Geometry(std::string label_, u64 id_, vec3 origin_)
		: label(std::move(label_)),
			id(id_),
			origin(origin_) {}

Geometry::~Geometry() {}

void Geometry::CheckRadius(f64 radius, const GeometryConfig& cfg) {
	if (radius < 0) {
		throw std::invalid_argument("Radius < 0");
	}
	if (std::abs(radius) < cfg.epsilon) {
		throw std::invalid_argument("|radius| < epsilon");
	}
}

const std::string& Geometry::GetLabel() {
	return label;
}

vec3 Geometry::GetOrigin() {
	return origin;
}

u64 Geometry::GetId() {
	return id;
}

//
// Circle
//

Circle::Circle(std::string label_, u64 id_, vec3 origin_, f64 radius_, const GeometryConfig& cfg)
		: Geometry(std::move(label_), id_, origin_), radius(radius_) {
	CheckRadius(radius, cfg);
}

f64 Circle::GetRadius() {
	return radius;
}

std::string_view Circle::GetTypeName() {
	return "Circle";
}

vec3 Circle::GetPoint(f64 t) {
	return vec3(radius * cos(t), radius * sin(t), 0) + origin;
};

vec3 Circle::GetDerivative(f64 t) {
	return {-radius * sin(t), radius * cos(t), 0};
}

//
// Ellipse
//

Ellipse::Ellipse(std::string label_, u64 id_, vec3 origin_, f64 radiusX_, f64 radiusY_, const GeometryConfig& cfg)
		: Geometry(std::move(label_), id_, origin_),
			radiusX(radiusX_),
			radiusY(radiusY_) {
	CheckRadius(radiusX, cfg);
	CheckRadius(radiusY, cfg);
}

std::string_view Ellipse::GetTypeName() {
	return "Ellipse";
}

vec3 Ellipse::GetPoint(f64 t) {
	return vec3(radiusX * cos(t), radiusY * sin(t), 0) + origin;
};

vec3 Ellipse::GetDerivative(f64 t) {
	return {-radiusX * sin(t), radiusY * cos(t), 0};
}

//
// Helix
//

Helix::Helix(std::string label_, u64 id_, vec3 origin_, f64 radius_, f64 step_, const GeometryConfig& cfg)
		: Geometry(std::move(label_), id_, origin_),
			radius(radius_),
			step(step_) {
	CheckRadius(radius, cfg);
	if (std::abs(step) < cfg.epsilon) {
		throw std::invalid_argument("Step < epsilon");
	}
}

std::string_view Helix::GetTypeName() {
	return "Helix";
}

vec3 Helix::GetPoint(f64 t) {
	return vec3(radius * cos(t), radius * sin(t), step * t) + origin;
};

vec3 Helix::GetDerivative(f64 t) {
	return {-radius * sin(t), radius * cos(t), step};
}

//
// Point
//

Point::Point(std::string label_, u64 id_, vec3 origin_) : Geometry(std::move(label_), id_, origin_) {}

std::string_view Point::GetTypeName() {
	return "Point";
}

vec3 Point::GetPoint(f64 t) {
	return origin;
};

vec3 Point::GetDerivative(f64 t) {
	return {0, 0, 0};
}

//
// Line
//

Line::Line(std::string label_, u64 id_, vec3 origin_, vec3 direction_, const GeometryConfig& cfg)
		: Geometry(std::move(label_), id_, origin_) {
	if (direction_.Length() < cfg.epsilon) {
		throw std::invalid_argument("Direction length < epsilon");
	}

	direction = direction_.Normalized();
}

std::string_view Line::GetTypeName() {
	return "Line";
}

vec3 Line::GetPoint(f64 t) {
	return (direction * t) + origin;
}

vec3 Line::GetDerivative(f64 t) {
	return direction;
}

//
// Line Segment
//

LineSegment::LineSegment(std::string label_, u64 id_, vec3 start, vec3 end, const GeometryConfig& cfg)
		: Geometry(std::move(label_), id_, start) {
	vec3 diff = end - start;

	length = diff.Length();

	if (length < cfg.epsilon) {
		throw std::invalid_argument("Length < epsilon");
	}

	direction = diff.Normalized();
}

std::string_view LineSegment::GetTypeName() {
	return "Line Segment";
}

vec3 LineSegment::GetPoint(f64 t) {
	t = std::clamp(t, 0.0, 1.0);
	return (direction * length * t) + origin;
}

vec3 LineSegment::GetDerivative(f64 t) {
	return direction * length;
}