#include <numbers>
#include <print>

#include "general.h"

#include "geo/geometry.h"
#include "geo/geometry_builder.h"
#include "geo/geometry_loader.h"
#include "logging/logging.h"

int main(i32 argc, char** argv) {
	if (argc < 2) {
		std::println("Usage: parser <file path>");
		return EXIT_SUCCESS;
	}

	// Parse

	auto shapes = GeometryLoader::LoadFromFile(argv[1], GeometryBuilder(GeometryConfig(1.0e-9)));

	f64 t = std::numbers::pi / 4.0f;
	std::println("\nFile parsed. t={:.2}:", t);
	std::println("\t{:<15} | {:<20} | {:<9} | {:<22} | {:<22}", "Geometry Type", "Label", "ID", "Point(t)", "Deriv(t)");

	for (auto& shape : shapes) {
		vec3 p = shape->GetPoint(t);
		vec3 d = shape->GetDerivative(t);
		std::println("\t{:<15} | {:<20} | {:<9} | ({:>6.2f},{:>6.2f},{:>6.2f}) | ({:>6.2f},{:>6.2f},{:>6.2f})",
								 shape->GetTypeName(), shape->GetLabel(), shape->GetId(), p.x, p.y, p.z, d.x, d.y, d.z);
	}

	// Get circles and sort by radius

	std::vector<std::shared_ptr<Circle>> circles;
	for (auto& shape : shapes) {
		if (auto circle = std::dynamic_pointer_cast<Circle>(shape)) {
			circles.push_back(circle);
		}
	}

	std::sort(circles.begin(), circles.end(),
						[](const auto& a, const auto& b) { return a->GetRadius() < b->GetRadius(); });

	std::println("\nCircles:");
	std::println("\t{:<20} | {:<9} | {:<8}", "Label", "ID", "Radius");

	// Get circles radius sum

	f64 radiusSum = 0;
	for (auto& circle : circles) {
		std::println("\t{:<20} | {:<9} | {:>6.2f}",
								 circle->GetLabel(), circle->GetId(), circle->GetRadius());

		radiusSum += circle->GetRadius();
	}

	std::println("\nRadius sum: {}", radiusSum);

	return EXIT_SUCCESS;
}
