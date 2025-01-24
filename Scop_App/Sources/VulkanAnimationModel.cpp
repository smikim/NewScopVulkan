#include "../Scop_App/Includes/VulkanAnimationModel.hpp"
#include "../Scop_App/Includes/VulkanRenderer.hpp"
#include "../Scop_App/Includes/VulkanPipeline.hpp"

namespace vks
{
	VulkanAnimationModel::VulkanAnimationModel()
	{
	}

	VulkanAnimationModel::~VulkanAnimationModel()
	{
		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			vkDestroyBuffer(vulkanDevice->getLogicalDevice(), _uniformBuffers[i].buffer, nullptr);
			vkFreeMemory(vulkanDevice->getLogicalDevice(), _uniformBuffers[i].memory, nullptr);
		}

		vkDestroyPipelineLayout(vulkanDevice->getLogicalDevice(), _basicPipelineLayout, nullptr);

		if (_basicPipeline)
			delete _basicPipeline;


		if (_VertexBuffer.buffer != VK_NULL_HANDLE)
			_VertexBuffer.destroy();
		if (_IndexBuffer.buffer != VK_NULL_HANDLE)
			_IndexBuffer.destroy();
	}
	
	bool VulkanAnimationModel::Initialize(VulkanRenderer* renderer)
	{
		_renderer = renderer;

		try {
			createDescriptorSetLayout();
			createPipelineLayout();

			VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
			VkShaderModule vertShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/human_vert.vert.spv", *vulkanDevice);
			VkShaderModule fragShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/human_frag.frag.spv", *vulkanDevice);

			_basicPipeline = _renderer->createBasicPipeline(_basicPipelineLayout, HumanVertex::getBindingDescription(),
				HumanVertex::getAttributeDescription(), vertShaderModule, fragShaderModule);

			if (_basicPipeline == nullptr)
				return false;

			_uniformBuffers = _renderer->createUniformBuffers<::ShaderHumanData>(sizeof(::ShaderHumanData));
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to load OBJ mesh: " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	void VulkanAnimationModel::bind(VkCommandBuffer commandBuffer, uint32_t currentFrame)
	{
		_basicPipeline->bind(commandBuffer);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 0, 1, &_uniformBuffers[currentFrame].descriptorSet, 0, nullptr);

		VkDeviceSize offsets[1]{ 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_VertexBuffer.buffer, offsets);
		// Bind triangle index buffer

		vkCmdBindIndexBuffer(commandBuffer, _IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanAnimationModel::draw(VkCommandBuffer commandBuffer)
	{
		// Draw indexed triangle
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 1);
	}

	void VulkanAnimationModel::EndCreateMesh(const std::string& BmpFilename)
	{
		try {
			_texture = _renderer->CreateTexture(BmpFilename);

		}
		catch (std::exception& e) {
			std::cerr << "Failed to load texture: " << e.what() << std::endl;
			throw;
		}

		createDescriptorPool();
		createDescriptorSets();
	}

	void VulkanAnimationModel::EndCreateMesh()
	{
		createDescriptorPool();
		createDescriptorSets();
	}

	void VulkanAnimationModel::createDescriptorSetLayout()
	{
		_DecriptorSetLayout =
			DescriptorSetLayout::Builder(*(_renderer->getVulkanDevice()))
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();
	}

	void VulkanAnimationModel::createDescriptorPool()
	{

		_DescriptorPool =
			DescriptorPool::Builder(*(_renderer->getVulkanDevice()))
			.setMaxSets(MAX_CONCURRENT_FRAMES)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_CONCURRENT_FRAMES)
			.build();
	}

	void VulkanAnimationModel::createDescriptorSets()
	{
		for (int i = 0; i < _uniformBuffers.size(); i++) {
			VkDescriptorBufferInfo bufferInfo = { _uniformBuffers[i].buffer, 0, sizeof(ShaderHumanData) };
			DescriptorWriter(*_DecriptorSetLayout, *_DescriptorPool)
				.writeBuffer(0, &bufferInfo)
				.build(_uniformBuffers[i].descriptorSet);

		}
	}

	void VulkanAnimationModel::createPipelineLayout()
	{
		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
		VkDescriptorSetLayout descriptorSetLayout = _DecriptorSetLayout->getDescriptorSetLayout();

		VkPipelineLayoutCreateInfo pipelineLayoutCI{};
		pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCI.pNext = nullptr;
		pipelineLayoutCI.setLayoutCount = 1;
		pipelineLayoutCI.pSetLayouts = &descriptorSetLayout;
		pipelineLayoutCI.pushConstantRangeCount = 0;
		pipelineLayoutCI.pPushConstantRanges = nullptr;

		VK_CHECK_RESULT(vkCreatePipelineLayout(vulkanDevice->getLogicalDevice(), &pipelineLayoutCI, nullptr, &_basicPipelineLayout));

	}

	void VulkanAnimationModel::updateUniformBuffer(uint32_t currentFrame, ShaderHumanData* src)
	{
		memcpy(_uniformBuffers[currentFrame].mapped, src, sizeof(ShaderHumanData));
	}

}