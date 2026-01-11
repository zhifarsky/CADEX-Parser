#include "geometry_builder.h"

GeometryBuilder::GeometryBuilder(const GeometryConfig& cfg_) : cfg(cfg_) {}

std::unique_ptr<Geometry> GeometryBuilder::CreateCircle(std::string label, u64 id, vec3 origin, f64 radius) const {
	std::unique_ptr<Geometry> geo;

	if (radius < 0) {
		LogWarning("%s: radius < 0", label.c_str());
		radius = std::abs(radius);
	}

	// точка
	if (radius < cfg.epsilon) {
		LogWarning("%s: |radius| < epsilon. Converting to Point", label.c_str());
		geo = std::make_unique<Point>(std::move(label), id, origin);
	}
	// окружность
	else {
		geo = std::make_unique<Circle>(std::move(label), id, origin, radius, cfg);
	}

	return geo;
}

std::unique_ptr<Geometry> GeometryBuilder::CreateEllipse(std::string label, u64 id, vec3 origin, f64 radiusX, f64 radiusY) const {
	std::unique_ptr<Geometry> geo;

	if (radiusX < 0 || radiusY < 0) {
		LogWarning("%s: radius < 0", label.c_str());
		radiusX = std::abs(radiusX);
		radiusY = std::abs(radiusY);
	}

	bool xZero = radiusX < cfg.epsilon;
	bool yZero = radiusY < cfg.epsilon;

	// точка
	if (xZero && yZero) {
		LogWarning("%s: |radius| < epsilon. Converting to Point", label.c_str());
		geo = std::make_unique<Point>(std::move(label), id, origin);
	}
	// отрезок, параллельный оси X или Y
	else if (xZero || yZero) {
		LogWarning("%s: |radius| < epsilon. Converting to Line Segment", label.c_str());

		radiusX = xZero ? 0 : radiusX;
		radiusY = yZero ? 0 : radiusY;

		vec3 direction(radiusX, radiusY, 0);
		vec3 start = origin - direction;
		vec3 end = origin + direction;

		geo = std::make_unique<LineSegment>(std::move(label), id, start, end, cfg);
	}
	// эллипс
	else {
		geo = std::make_unique<Ellipse>(std::move(label), id, origin, radiusX, radiusY, cfg);
	}

	return geo;
}

std::unique_ptr<Geometry> GeometryBuilder::CreateHelix(std::string label, u64 id, vec3 origin, f64 radius, f64 step) const {
	std::unique_ptr<Geometry> geo;

	if (radius < 0) {
		LogWarning("%s: radius < 0", label.c_str());
		radius = std::abs(radius);
	}

	bool radiusZero = radius < cfg.epsilon;
	bool stepZero = std::abs(step) < cfg.epsilon;

	if (radiusZero) {
		LogWarning("%s: |radius| < epsilon", label.c_str());
		// точка
		if (stepZero) {
			LogWarning("%s: |step| < epsilon. Converting to Point", label.c_str());
			geo = std::make_unique<Point>(std::move(label), id, origin);
		}
		// прямая
		// всегда будет параллельна оси Z
		else {
			LogWarning("%s: Converting to Line", label.c_str());
			vec3 direction = step > 0 ? vec3(0, 0, 1) : vec3(0, 0, -1);
			geo = std::make_unique<Line>(std::move(label), id, origin, direction, cfg);
		}
	}
	// окружность
	else if (stepZero) {
		LogWarning("%s: |step| < epsilon. Converting to Ellipse", label.c_str());
		geo = std::make_unique<Circle>(std::move(label), id, origin, radius, cfg);
	}
	// спираль
	else {
		geo = std::make_unique<Helix>(std::move(label), id, origin, radius, step, cfg);
	}

	return geo;
}