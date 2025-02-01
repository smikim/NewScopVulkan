#pragma once
#include "../Includes/VulkanDevice.hpp"
#include "../Includes/VulkanBuffer.hpp"
#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Includes/Types.hpp"
#include "../Includes/Common/IVulkanModel.hpp"

#include "Common/Vertex.hpp"
#include "Common/ShaderData.hpp"

#include "VulkanDescriptor.hpp"

#define MAX_CONCURRENT_FRAMES 2

namespace vks
{
class VulkanRenderer;
class VulkanPipeline;
class VulkanTexture;


class VulkanAnimationModel : public IVulkanModel<::HumanVertex, ::ShaderHumanData>
{
public:
	VulkanAnimationModel();
	virtual ~VulkanAnimationModel();

	VulkanAnimationModel(const VulkanAnimationModel&) = delete;
	VulkanAnimationModel& operator=(const VulkanAnimationModel&) = delete;

	bool	Initialize(VulkanRenderer* renderer) override;
	//void	loadNodes(humanGL::Node& node);
	void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) override;
	void draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, ::ShaderHumanData& ubo) override;
	void drawNode(VkCommandBuffer commandBuffer, Node<::HumanVertex>& node, uint32_t currentFrame, ::ShaderHumanData& ubo);

	void EndCreateMesh(const std::string& BmpFilename) override;
	void EndCreateMesh();

	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets();
	void createPipelineLayout();

	void updateUniformBuffer(uint32_t currentFrame, ::ShaderHumanData* src);
	void updateModelMat(Node<::HumanVertex>& node, ::ShaderHumanData& ubo, uint32_t currentFrame);

	void traverseAndAddNodes(std::shared_ptr<Node<::HumanVertex>> node, std::vector<mymath::Mat4>& transformMats, mymath::Mat4& parent);
	void createSkinFromRootNode(std::shared_ptr<Node<::HumanVertex>> rootNode);

protected:

	//VulkanRenderer* _renderer;

private:
	//Buffer _VertexBuffer;
	//uint32_t vertexCount;
	//Buffer _IndexBuffer;

	//uint32_t indexCount;
	//std::vector<ScopVertex> _Vertices;
	//std::vector<uint32_t> _Indices;

	VulkanTexture* _texture = nullptr;

	//VkDescriptorSetLayout _basicDescriptorSetLayout{ VK_NULL_HANDLE };
	//VkDescriptorPool _basicDescriptorPool{ VK_NULL_HANDLE };
	VkPipelineLayout _basicPipelineLayout{ VK_NULL_HANDLE };

	std::unique_ptr<DescriptorPool> _DescriptorPool{};
	std::unique_ptr<DescriptorSetLayout> _DecriptorSetLayout{};
	std::unique_ptr<DescriptorSetLayout> _DecriptorSetLayoutSSBO{};
	

	// We use one UBO per frame, so we can have a frame overlap and make sure that uniforms aren't updated while still in use
	std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> _uniformBuffers;

	VulkanPipeline* _basicPipeline = nullptr;
	//Skin<::HumanVertex> _skin;
};
}
