#pragma once

#include <vulkan/vulkan.h>
#include "../../Library/Math/Matrix.hpp"

struct ShaderData {
	alignas(16) mymath::Mat4 modelMatrix;
	alignas(16) mymath::Mat4 viewMatrix;
	alignas(16) mymath::Mat4 projectionMatrix;
	alignas(16) uint32_t colorMode;
};

struct ShaderHumanData {
	alignas(16) mymath::Mat4 modelMatrix;
	alignas(16) mymath::Mat4 viewMatrix;
	alignas(16) mymath::Mat4 projectionMatrix;
};
