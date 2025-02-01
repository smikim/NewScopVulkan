#include "../Scop_App/Includes/Common/Vertex.hpp"

std::vector<VkVertexInputBindingDescription> ScopVertex::getBindingDescription()
{
	// Vertex input binding
		// This example uses a single vertex input binding at binding point 0 (see vkCmdBindVertexBuffers)
	std::vector<VkVertexInputBindingDescription> vertexInputBinding(1);

	vertexInputBinding[0].binding = 0;
	vertexInputBinding[0].stride = sizeof(ScopVertex);
	vertexInputBinding[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return vertexInputBinding;
}

std::vector<VkVertexInputAttributeDescription> ScopVertex::getAttributeDescription()
{
	std::vector<VkVertexInputAttributeDescription> vertexInputAttributs{};

	vertexInputAttributs.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(ScopVertex, position) });
	vertexInputAttributs.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(ScopVertex, color) });
	vertexInputAttributs.push_back({ 2, 0, VK_FORMAT_R32G32_SFLOAT , offsetof(ScopVertex, uv) });
	vertexInputAttributs.push_back({ 3, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(ScopVertex, normal) });
	vertexInputAttributs.push_back({ 4, 0, VK_FORMAT_R32_UINT , offsetof(ScopVertex, triangleID) });

	return vertexInputAttributs;
}

std::vector<VkVertexInputBindingDescription> HumanVertex::getBindingDescription()
{
	std::vector<VkVertexInputBindingDescription> vertexInputBinding(1);

	vertexInputBinding[0].binding = 0;
	vertexInputBinding[0].stride = sizeof(HumanVertex);
	vertexInputBinding[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return vertexInputBinding;
}

std::vector<VkVertexInputAttributeDescription> HumanVertex::getAttributeDescription()
{
	std::vector<VkVertexInputAttributeDescription> vertexInputAttributs{};

	vertexInputAttributs.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(HumanVertex, position) });
	vertexInputAttributs.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(HumanVertex, color) });
	vertexInputAttributs.push_back({ 2, 0, VK_FORMAT_R32_UINT , offsetof(HumanVertex, bodyPartID) });

	return vertexInputAttributs;
}
