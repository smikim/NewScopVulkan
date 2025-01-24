#pragma once
#include <vector>
#include <string>
#include "../VulkanTools.hpp"
#include "vulkan/vulkan.h"
#include "../VulkanRenderer.hpp"

namespace vks
{
class VulkanRenderer;
class VulkanDevice;
class VulkanQueue;
struct Buffer;

template<typename VertexType, typename ShaderData>
class IVulkanModel
{
public:
	IVulkanModel() = default;
	virtual ~IVulkanModel() = default;

	virtual bool Initialize(VulkanRenderer* renderer) = 0;
	virtual void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) = 0;
	virtual void draw(VkCommandBuffer commandBuffer) = 0;
	virtual void createVertexBuffer(const std::vector<VertexType>& vertices);
	virtual void createVertexBuffer();
	virtual void createIndexBuffer();
	virtual void createIndexBuffer(const std::vector<uint32_t>& indices);
	virtual void EndCreateMesh() = 0;
	virtual void EndCreateMesh(const std::string& BmpFilename) = 0;
	virtual void updateUniformBuffer(uint32_t currentFrame, ShaderData* src) = 0;
protected:
	VulkanRenderer* _renderer;
	Buffer _VertexBuffer;
	uint32_t _vertexCount;
	Buffer _IndexBuffer;

	uint32_t indexCount;
	std::vector<VertexType> _Vertices;
	std::vector<uint32_t> _Indices;
};

template<typename VertexType, typename ShaderData>
inline void IVulkanModel<VertexType, ShaderData>::createVertexBuffer(const std::vector<VertexType>& vertices)
{
	_Vertices = vertices;

	uint32_t vertexBufferSize = static_cast<uint32_t>(_Vertices.size()) * sizeof(VertexType);
	_vertexCount = _Vertices.size();

	// Create buffers and upload data to the GPU
	struct StagingBuffers {
		vks::Buffer vertices;
		vks::Buffer indices;
	} stagingBuffers;

	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

	// Host visible source buffers (staging)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.vertices, vertexBufferSize, _Vertices.data()));

	// Device local destination buffers
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_VertexBuffer, _Vertices.size() * sizeof(VertexType)));

	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	// Copy from host do device
	vulkanDevice->copyBuffer(&stagingBuffers.vertices, &_VertexBuffer, queue.get_queue());

	// Clean up
	stagingBuffers.vertices.destroy();
}

template<typename VertexType, typename ShaderData>
inline void IVulkanModel<VertexType, ShaderData>::createVertexBuffer()
{
	uint32_t vertexBufferSize = static_cast<uint32_t>(_Vertices.size()) * sizeof(::ScopVertex);
	_vertexCount = _Vertices.size();

	// Create buffers and upload data to the GPU
	struct StagingBuffers {
		vks::Buffer vertices;
		vks::Buffer indices;
	} stagingBuffers;

	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

	// Host visible source buffers (staging)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.vertices, vertexBufferSize, _Vertices.data()));

	// Device local destination buffers
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_VertexBuffer, _Vertices.size() * sizeof(::ScopVertex)));

	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	// Copy from host do device
	vulkanDevice->copyBuffer(&stagingBuffers.vertices, &_VertexBuffer, queue.get_queue());

	// Clean up
	stagingBuffers.vertices.destroy();
}

template<typename VertexType, typename ShaderData>
inline void IVulkanModel<VertexType, ShaderData>::createIndexBuffer()
{
	indexCount = static_cast<uint32_t>(_Indices.size());

	struct StagingBuffers {
		vks::Buffer vertices;
		vks::Buffer indices;
	} stagingBuffers;

	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
	// Host visible source buffers (staging)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.indices, _Indices.size() * sizeof(uint32_t), _Indices.data()));

	// Device local destination buffers
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_IndexBuffer, _Indices.size() * sizeof(uint32_t)));

	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	// Copy from host do device
	vulkanDevice->copyBuffer(&stagingBuffers.indices, &_IndexBuffer, queue.get_queue());

	// Clean up
	stagingBuffers.indices.destroy();
}

template<typename VertexType, typename ShaderData>
inline void IVulkanModel<VertexType, ShaderData>::createIndexBuffer(const std::vector<uint32_t>& indices)
{
	_Indices = indices;

	indexCount = static_cast<uint32_t>(_Indices.size());

	struct StagingBuffers {
		vks::Buffer vertices;
		vks::Buffer indices;
	} stagingBuffers;

	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
	// Host visible source buffers (staging)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.indices, _Indices.size() * sizeof(uint32_t), _Indices.data()));

	// Device local destination buffers
	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_IndexBuffer, _Indices.size() * sizeof(uint32_t)));

	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	// Copy from host do device
	vulkanDevice->copyBuffer(&stagingBuffers.indices, &_IndexBuffer, queue.get_queue());

	// Clean up
	stagingBuffers.indices.destroy();
}



}