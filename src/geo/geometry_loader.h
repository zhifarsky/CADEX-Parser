#pragma once
#include <filesystem>

#include "geometry.h"

namespace GeometryLoader {
std::vector<std::shared_ptr<Geometry>> LoadFromFile(const std::filesystem::path& path, const GeometryBuilder& geoBuilder);
};	// namespace GeometryLoader