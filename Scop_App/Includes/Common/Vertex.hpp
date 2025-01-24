#pragma once

#include <vector>
#include <vulkan/vulkan.h>

struct ScopVertex {
	float position[3];
	float color[3];
	float uv[2];
	float normal[3];
	uint32_t triangleID; // �ﰢ�� ID

	static std::vector<VkVertexInputBindingDescription> getBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
};

struct HumanVertex {
	float position[3];
	float color[3];
	static std::vector<VkVertexInputBindingDescription> getBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
};