#pragma once
#include <vector>
#include <string>
#include "../VulkanTools.hpp"
#include "vulkan/vulkan.h"
#include "../VulkanRenderer.hpp"
#include "../Scop_App/Component/Animation.hpp"
#include "../Scop_App/Library/Math/Matrix.hpp"

namespace humanGL
{
	template<typename VertexType>
	class Node;
}

namespace vks
{
class VulkanRenderer;
class VulkanDevice;
class VulkanQueue;
struct Buffer;


//template<typename VertexType>
//class Mesh
//{
//public:
//	Mesh() = default;
//	~Mesh() = default;
//
//	void createVertexBuffer(VulkanRenderer* renderer, const std::vector<VertexType>& vertices);
//	void createVertexBuffer(VulkanRenderer* renderer);
//	void createIndexBuffer(VulkanRenderer* renderer);
//	void createIndexBuffer(VulkanRenderer* renderer, const std::vector<uint32_t>& indices);
//
//	Buffer _VertexBuffer;
//	uint32_t _vertexCount;
//	Buffer _IndexBuffer;
//
//	uint32_t indexCount;
//	std::vector<VertexType> _Vertices;
//	std::vector<uint32_t> _Indices;
//};

template<typename VertexType>
struct Mesh
{
	Buffer _VertexBuffer;
	uint32_t vertexCount;
	Buffer _IndexBuffer;

	uint32_t indexCount;
	std::vector<VertexType> _Vertices;
	std::vector<uint32_t> _Indices;

};

template<typename VertexType>
struct Node
{
	std::shared_ptr<Node<VertexType>> parent;
	std::string				name;
	uint32_t				index;
	std::vector<std::shared_ptr<Node<VertexType>>>		children;
	Mesh<VertexType>				mesh;
	mymath::Vec3			_translation{};
	mymath::Vec3			_scale{ 1.0f };
	mymath::Vec3			_rotation{};
	int32_t					skin = -1;
	mymath::Mat4			matrix;
	mymath::Mat4			getLocalMatrix();
	mymath::Mat4			getTRMatrix();
};
 
template<typename VertexType>
struct Skin
{
	std::string            name;
	std::shared_ptr<Node<VertexType>> skeletonRoot = nullptr;
	std::vector<std::shared_ptr<Node<VertexType>>>    joints;
	vks::Buffer            ssbo;
	VkDescriptorSet        descriptorSet;
};

template<typename VertexType, typename ShaderData>
class IVulkanModel
{
public:
	IVulkanModel() = default;
	virtual ~IVulkanModel() = default;

	virtual bool Initialize(VulkanRenderer* renderer) = 0;
	virtual void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) = 0;

	virtual void draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, ShaderData& ubo) = 0;

	virtual void drawNode(VkCommandBuffer commandBuffer, Node<VertexType>& node, uint32_t currentFrame, ShaderData& ubo) = 0;
	
	virtual void loadNodes(humanGL::Node<VertexType>& node);

	//virtual void createVertexBuffer(const std::vector<VertexType>& vertices);
	virtual void createVertexBuffer();
	virtual void createIndexBuffer();
	//virtual void createIndexBuffer(const std::vector<uint32_t>& indices);
	
	virtual void EndCreateMesh() = 0;
	virtual void EndCreateMesh(const std::string& BmpFilename) = 0;
	
	virtual void updateUniformBuffer(uint32_t currentFrame, ShaderData* src) = 0;
protected:
	VulkanRenderer* _renderer;
	
	std::vector<std::shared_ptr<Node<VertexType>>> _nodes;
	Skin<VertexType> _skin;
};

//template<typename VertexType, typename ShaderData>
//inline void IVulkanModel<VertexType, ShaderData>::draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, ShaderData& ubo)
//{
//	// Draw indexed triangle
//	for (auto& node : _nodes) {
//		drawNode(commandBuffer, *node, currentFrame, ubo);
//	}
//}
//
//template<typename VertexType, typename ShaderData>
//inline void IVulkanModel<VertexType, ShaderData>::drawNode(VkCommandBuffer commandBuffer, Node<VertexType>& node, uint32_t currentFrame, ShaderData& ubo)
//{
//	VkDeviceSize offsets[1]{ 0 };
//
//	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &node.mesh._VertexBuffer.buffer, offsets);
//	// Bind triangle index buffer
//	vkCmdBindIndexBuffer(commandBuffer, node.mesh._IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
//	vkCmdDrawIndexed(commandBuffer, node.mesh.indexCount, 1, 0, 0, 1);
//
//
//	updateUniformBuffer(currentFrame, ubo);
//
//	for (auto& child : node.children) {
//		drawNode(commandBuffer, *child, currentFrame, ubo);
//	}
//}

template<typename VertexType, typename ShaderData>
inline void IVulkanModel<VertexType, ShaderData>::loadNodes(humanGL::Node<VertexType>& node)
{
	std::shared_ptr<Node<VertexType>> newNode = std::make_shared<Node<VertexType>>();
	newNode->index = static_cast<uint32_t>(_nodes.size());
	newNode->_translation = node.get_transform().get_translation();
	newNode->_rotation = node.get_transform().get_rotation();
	newNode->_scale = node.get_transform().get_scale();
	newNode->matrix = node.get_transform().get_matrix();
	newNode->parent = nullptr;
	newNode->name = node.get_name();
	newNode->mesh._Vertices = node.get_vertices();
	newNode->mesh._Indices = node.get_indices();
	
	_nodes.push_back(newNode);

	for (const auto& child : node.get_children())
	{
		std::cout << child.get() << std::endl;
		loadNodes(*child);
		newNode->children.push_back(_nodes.back());
		_nodes.back()->parent = newNode;
	}
}

template<typename VertexType, typename ShaderData>
inline void IVulkanModel<VertexType, ShaderData>::createVertexBuffer()
{
	for (auto& node : _nodes)
	{
		uint32_t vertexBufferSize = static_cast<uint32_t>(node->mesh._Vertices.size()) * sizeof(VertexType);
		node->mesh.vertexCount = node->mesh._Vertices.size();

		// Create buffers and upload data to the GPU
		struct StagingBuffers {
			vks::Buffer vertices;
		} stagingBuffers;

		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		// Host visible source buffers (staging)
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.vertices, vertexBufferSize, node->mesh._Vertices.data()));

		// Device local destination buffers
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &node->mesh._VertexBuffer, node->mesh._Vertices.size() * sizeof(VertexType)));

		const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

		// Copy from host to device
		vulkanDevice->copyBuffer(&stagingBuffers.vertices, &node->mesh._VertexBuffer, queue.get_queue());

		// Clean up
		stagingBuffers.vertices.destroy();
	}
}

template<typename VertexType, typename ShaderData>
inline void IVulkanModel<VertexType, ShaderData>::createIndexBuffer()
{
	for (auto& node : _nodes)
	{
		node->mesh.indexCount = static_cast<uint32_t>(node->mesh._Indices.size());

		// Create buffers and upload data to the GPU
		struct StagingBuffers {
			vks::Buffer indices;
		} stagingBuffers;

		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		// Host visible source buffers (staging)
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.indices, node->mesh._Indices.size() * sizeof(uint32_t), node->mesh._Indices.data()));

		// Device local destination buffers
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &node->mesh._IndexBuffer, node->mesh._Indices.size() * sizeof(uint32_t)));

		const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

		// Copy from host to device
		vulkanDevice->copyBuffer(&stagingBuffers.indices, &node->mesh._IndexBuffer, queue.get_queue());

		// Clean up
		stagingBuffers.indices.destroy();
	}
}



template<typename VertexType>
inline mymath::Mat4 Node<VertexType>::getLocalMatrix()
{
	mymath::Mat4 transMat{ 1.0f };
	mymath::Mat4 rotMat{ 1.0f };
	mymath::Mat4 scaleMat{ 1.0f };

	transMat = mymath::translate(transMat, _translation);
	
	rotMat = mymath::rotate(rotMat, _rotation._y, mymath::Vec3(0.0f, 1.0f, 0.0f));
	rotMat = mymath::rotate(rotMat, _rotation._x, mymath::Vec3(1.0f, 0.0f, 0.0f));
	rotMat = mymath::rotate(rotMat, _rotation._z, mymath::Vec3(0.0f, 0.0f, 1.0f));
	
	scaleMat = mymath::scale(scaleMat, _scale);

	return transMat * rotMat * scaleMat;
}

template<typename VertexType>
inline mymath::Mat4 Node<VertexType>::getTRMatrix()
{
	mymath::Mat4 transMat{ 1.0f };
	mymath::Mat4 rotMat{ 1.0f };

	transMat = mymath::translate(transMat, _translation);

	rotMat = mymath::rotate(rotMat, _rotation._y, mymath::Vec3(0.0f, 1.0f, 0.0f));
	rotMat = mymath::rotate(rotMat, _rotation._x, mymath::Vec3(1.0f, 0.0f, 0.0f));
	rotMat = mymath::rotate(rotMat, _rotation._z, mymath::Vec3(0.0f, 0.0f, 1.0f));

	return transMat * rotMat;
}

}