#pragma once
#include <string>

#include "../general.h"

struct vec3 {
	union {
		f64 v[3];
		struct {
			f64 x, y, z;
		};
	};

	vec3();
	vec3(f64 x_, f64 y_, f64 z_);

	vec3 operator+(vec3 other) const;
	vec3 operator-(vec3 other) const;
	vec3 operator*(f64 value) const;
	vec3 operator/(f64 value) const;

	f64 Length();
	vec3 Normalized(f64 epsilon = 1.0e-9);
};

struct GeometryConfig {
	f64 epsilon;
	GeometryConfig(f64 epsilon_ = 1.0e-9);
};

class Geometry {
 protected:
	std::string label;
	vec3 origin;
	u64 id;

	void CheckRadius(f64 radius, const GeometryConfig& cfg);

 public:
	Geometry();
	Geometry(std::string label_, u64 id_, vec3 origin_);

	virtual ~Geometry();	// понадобится, если у производных классов появятся поля, требующие деструктора

	const std::string& GetLabel();	// не используем string_view, чтобы не следить за временем жизни
	vec3 GetOrigin();
	u64 GetId();

	virtual std::string_view GetTypeName() = 0;	 // время жизни не проблема, т.к. возвращаем статичные строки
	virtual vec3 GetPoint(f64 t) = 0;
	virtual vec3 GetDerivative(f64 t) = 0;
};

class Circle : public Geometry {
	f64 radius;

 public:
	Circle(std::string label_, u64 id_, vec3 origin_, f64 radius_, const GeometryConfig& cfg);

	f64 GetRadius();
	std::string_view GetTypeName() override;
	vec3 GetPoint(f64 t) override;
	vec3 GetDerivative(f64 t) override;
};

class Ellipse : public Geometry {
	f64 radiusX, radiusY;

 public:
	Ellipse(std::string label_, u64 id_, vec3 origin_, f64 radiusX_, f64 radiusY_, const GeometryConfig& cfg);

	std::string_view GetTypeName() override;
	vec3 GetPoint(f64 t) override;
	vec3 GetDerivative(f64 t) override;
};

class Helix : public Geometry {
	f64 radius, step;

 public:
	Helix(std::string label_, u64 id_, vec3 origin_, f64 radius_, f64 step_, const GeometryConfig& cfg);

	std::string_view GetTypeName() override;
	vec3 GetPoint(f64 t) override;
	vec3 GetDerivative(f64 t) override;
};

//
// Другие геометрия может быть преобразована в точку, линию или отрезок
//

class Point : public Geometry {
 public:
	Point(std::string label_, u64 id_, vec3 origin_);

	std::string_view GetTypeName() override;
	vec3 GetPoint(f64 t) override;
	vec3 GetDerivative(f64 t) override;
};

class Line : public Geometry {
	vec3 direction;

 public:
	Line(std::string label_, u64 id_, vec3 origin_, vec3 direction_, const GeometryConfig& cfg);

	std::string_view GetTypeName() override;
	vec3 GetPoint(f64 t) override;
	vec3 GetDerivative(f64 t) override;
};

class LineSegment : public Geometry {
	vec3 direction;
	f64 length;

 public:
	LineSegment(std::string label_, u64 id_, vec3 start, vec3 end, const GeometryConfig& cfg);

	std::string_view GetTypeName() override;
	vec3 GetPoint(f64 t) override;
	vec3 GetDerivative(f64 t) override;
};