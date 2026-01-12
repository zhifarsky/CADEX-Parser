#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "geometry_builder.h"
#include "geometry_loader.h"

// - выкидывать исключения вместо записи ошибок в лог?
// - можно уменьшить вложенность, если понадобится

namespace GeometryLoader {
std::vector<std::shared_ptr<Geometry>> LoadFromFile(const std::filesystem::path& path, const GeometryBuilder& geoBuilder) {
	std::vector<std::shared_ptr<Geometry>> shapes;	// если произошла ошибка, вернется пустой вектор
	std::ifstream file(path);

	if (file.is_open()) {
		std::string line;
		if (std::getline(file, line)) {
			i64 shapesCount = std::atoll(line.c_str());
			shapes.reserve(shapesCount);

			for (i64 i = 0; i < shapesCount; i++) {
				if (std::getline(file, line)) {
					if (line.length() > 0) {
						std::string shapeLabel;
						u64 shapeID = 0;
						char shapeType = 0;
						f64 x, y, z;

						std::istringstream stream(line);

						if (stream >> shapeType >> shapeID >> std::quoted(shapeLabel) >> x >> y >> z) {
							switch (shapeType) {
								// circle
								case 'C': {
									f64 radius;

									if (stream >> radius) {
										shapes.emplace_back(geoBuilder.CreateCircle(std::move(shapeLabel), shapeID, vec3(x, y, z), radius));
									} else {
										LogError("Unable to parse circle: %s", line.c_str());
									}
								} break;

								// ellipse
								case 'E': {
									f64 radiusX, radiusY;

									if (stream >> radiusX >> radiusY) {
										shapes.emplace_back(geoBuilder.CreateEllipse(std::move(shapeLabel), shapeID, vec3(x, y, z), radiusX, radiusY));
									} else {
										LogError("Unable to parse ellipse: %s", line.c_str());
									}
								} break;

								// helix
								case 'H': {
									f64 radius, step;

									if (stream >> radius >> step) {
										shapes.emplace_back(geoBuilder.CreateHelix(std::move(shapeLabel), shapeID, vec3(x, y, z), radius, step));
									} else {
										LogError("Unable to parse helix: %s", line.c_str());
									}
								} break;
								default:
									LogError("Unknown shape: %c", shapeType);
									break;
							}
						} else {
							LogError("Wrong shape format: %s", line.c_str());
						}
					} else {
						LogWarning("Empty line");
					}
				} else {
					LogError("Unexpected end of file");
					break;
				}
			}
		}
	} else {
		LogError("File \"%s\" cannot be opened", path.string().c_str());
	}

	return shapes;
}
};	// namespace GeometryLoader