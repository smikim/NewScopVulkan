#pragma once
#include "../Includes/VulkanInstance.hpp"
#include "../Window/GlfwWindow.hpp"
#include "../Includes/VulkanDevice.hpp"
#include "../Includes/VulkanSwapChain.hpp"
#include "../Graphics/BasicPSO.hpp"
#include "../Includes/VulkanPipelineState.hpp"
#include "../Includes/VulkanCommandBuffer.hpp"

#include "../Includes/VulkanTexture.hpp"
#include "../Library/BmpLoader.hpp"
#include "../Common/Vertex.hpp"
#include "../Common/ShaderData.hpp"
#include "../Library/Math/math.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Includes/Types.hpp"
#include <type_traits>

#include <chrono>

#include <vector>

#define MAX_CONCURRENT_FRAMES 2

class ScopObject;

namespace humanGL
{
	template<typename VertexType>
	class Node;
}

namespace vks
{
	class VulkanDevice;
	template<typename VertexType, typename ShaderData>
	class IVulkanModel;

	class VulkanRenderer
	{
	public:
		VulkanRenderer(GlfwWindow& window);
		
		~VulkanRenderer();

		bool initVulkan();
		
		
		IVulkanModel<::ScopVertex, ::ShaderData>* CreateBasicMeshObject();
		// IVulkanModel<::ScopVertex, ::ShaderData>* CreateBasicMeshObject(std::string& ObjFilename);
		IVulkanModel<::HumanVertex, ::ShaderHumanData>* CreateHumanMeshObject();
		
		template <typename VertexType, typename ShaderData>
		void BeginCreateMesh(IVulkanModel<VertexType, ShaderData>* model);

		//template <typename VertexType, typename ShaderData>
		//void BeginCreateMesh(IVulkanModel<VertexType, ShaderData>* model, std::vector<VertexType>& vertices);
		
		//template <typename VertexType, typename ShaderData>
		//void InsertIndexBuffer(IVulkanModel<VertexType, ShaderData>* model, std::vector<uint32_t>& indices);
				
		template <typename VertexType, typename ShaderData>
		void InsertIndexBuffer(IVulkanModel<VertexType, ShaderData>* model);

		template <typename VertexType, typename ShaderData>
		void EndCreateMesh(IVulkanModel<VertexType, ShaderData>* model, std::string& BmpFilename);
		
		template <typename VertexType, typename ShaderData>
		void EndCreateMesh(IVulkanModel<VertexType, ShaderData>* model);

		template <typename VertexType, typename ShaderData>
		void DeleteMeshObject(IVulkanModel<VertexType, ShaderData>* model);

		template <typename VertexType, typename ShaderData>
		void LoadNodes(IVulkanModel<VertexType, ShaderData>* model, humanGL::Node<VertexType>& node);

		VulkanTexture* CreateTexture(const std::string& filename);
		void AddDescriptorSetLayout(VkDescriptorSetLayout layout);
		VulkanPipeline* createBasicPipeline(VkPipelineLayout pipelineLayout, const std::vector<VkVertexInputBindingDescription>& bindingDescription,
			const std::vector<VkVertexInputAttributeDescription>& attributeDescription,
			VkShaderModule vertShaderModule,
			VkShaderModule fragShaderModule);
		
		VkResult beginRender();
		void beginRenderPass();
		void endRenderPass();
		VkResult endRender();

		template <typename VertexType, typename ShaderData>
		void renderMeshObject(IVulkanModel<VertexType, ShaderData>* model, mymath::Mat4 worldMat, uint32_t colorMode);

		VkResult prepareFrame();
		VkResult submitFrame();
		

		uint32_t getWidth() { return _width; }
		uint32_t getHeight() { return _height; }

		VkFramebuffer get_frameBuffer_by_index(uint32_t index) const {
			return _FrameBuffers[index];
		}

		VkCommandBuffer getCurrentCommandBuffer() const {		
			return _drawCommandBuffer->get_commandBuffer_by_index(_currentImageIndex);
		}

		vks::VulkanDevice* getVulkanDevice() const {
			return _vulkanDevice;
		}

		float getAspectRatio() { 
			return static_cast<float>(_width) / static_cast<float>(_height); 
		};

		template <typename ShaderData>
		std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> createUniformBuffers(size_t bufferSize);
		
		void createSSBO(vks::Buffer& ssbo, size_t size);

		Graphics::BasicPSO* _basicPSO = nullptr;
		//VulkanPipeline* _basicPipeline = nullptr;

		


	private:
		// TODO
		void getEnabledFeatures();
		void create_surface();

		void add_device_extension(const char* extension, bool optional = false)
		{
			device_extensions[extension] = optional;
		}


		std::unordered_map<const char*, bool> const& get_device_extensions() const
		{
			return device_extensions;
		}

		
		void setupRenderPass();
		void setupDepthStencil(uint32_t& width, uint32_t& height);
		void setupFrameBuffer(uint32_t& width, uint32_t& height);
		void createCommandPool(); 
		
		void createSynchronizationPrimitives();

		VkResult submitCommandBuffer();
		void windowResize();
		
		template <typename T>
		typename std::enable_if<std::is_member_object_pointer<decltype(&T::colorMode)>::value>::type
			setColorMode(T& ubo, uint32_t colorMode)
		{
			ubo.colorMode = colorMode;
		}

		template <typename T>
		typename std::enable_if<!std::is_member_object_pointer<decltype(&T::colorMode)>::value>::type
			setColorMode(T&, uint32_t)
		{
			// Do nothing if T does not have colorMode
		}

		template <typename VertexType, typename ShaderData>
		void updateObjectUniformBuffer(IVulkanModel<VertexType, ShaderData>* model, mymath::Mat4 worldMat, uint32_t colorMode);

		template<typename ShaderData>
		ShaderData updateObjectMatrix(mymath::Mat4& worldMat);

		uint32_t _width;
		uint32_t _height;
		
		bool _prepared = false;
		bool _resized = false;

		VkClearColorValue _defaultClearColor = { { 0.0f, 0.0f, 0.2f, 1.0f } };

		// To select the correct sync objects, we need to keep track of the current frame
		uint32_t _currentFrame{ 0 };
		uint32_t _currentImageIndex{ 0 };

		vks::VulkanInstance _instance;
		GlfwWindow& _window;
		VkSurfaceKHR _Surface{ VK_NULL_HANDLE };
		vks::VulkanDevice* _vulkanDevice = nullptr;

		/** @brief Set of device extensions to be enabled for this example and whether they are optional (must be set in the derived constructor) */
		std::unordered_map<const char*, bool> device_extensions;

		// Wraps the swap chain to present images (framebuffers) to the windowing system
		std::unique_ptr<VulkanSwapChain> _swapChain;
		VkRenderPass _RenderPass{ VK_NULL_HANDLE };
		
		// Depth buffer format (selected during Vulkan initialization)
		VkFormat _DepthFormat;
		bool _requiresStencil{ false };
		/** @brief Default depth stencil attachment used by the default render pass */
		struct {
			VkImage image = VK_NULL_HANDLE;
			VkDeviceMemory memory = VK_NULL_HANDLE;
			VkImageView view = VK_NULL_HANDLE;
		} _depthStencil{};

		// List of available frame buffers (same as number of swap chain images)
		std::vector<VkFramebuffer>_FrameBuffers;

		// Command buffer pool
		VkCommandPool _CmdPool{ VK_NULL_HANDLE };

		// Semaphores are used to coordinate operations within the graphics queue and ensure correct command ordering
		std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> _PresentCompleteSemaphores{};
		std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> _RenderCompleteSemaphores{};

		std::array<VkFence, MAX_CONCURRENT_FRAMES> _WaitFences{};

		// Command buffers used for rendering
		VulkanCommandBuffer* _drawCommandBuffer = nullptr;
		std::vector<VkDescriptorSetLayout> _descriptorSetLayouts;
	};


	

	//template <typename VertexType, typename ShaderData>
	//void VulkanRenderer::BeginCreateMesh(IVulkanModel<VertexType, ShaderData>* model, std::vector<VertexType>& vertices)
	//{
	//	if (model) {
	//		model->createVertexBuffer(vertices);
	//	}
	//	else {
	//		// Handle error: model is not of type VulkanModel
	//		std::cerr << "model is nullptr" << std::endl;
	//	}
	//}

	template <typename VertexType, typename ShaderData>
	void VulkanRenderer::BeginCreateMesh(IVulkanModel<VertexType, ShaderData>* model)
	{
		if (model) {
			model->createVertexBuffer();
		}
		else {
			// Handle error: model is not of type VulkanModel
			std::cerr << "model is nullptr" << std::endl;
		}
	}

	//template <typename VertexType, typename ShaderData>
	//void VulkanRenderer::InsertIndexBuffer(IVulkanModel<VertexType, ShaderData>* model, std::vector<uint32_t>& indices)
	//{
	//	if (model) {
	//		model->createIndexBuffer(indices);
	//	}
	//	else {
	//		// Handle error: model is not of type VulkanModel
	//		std::cerr << "model is nullptr" << std::endl;
	//	}
	//}

	template <typename VertexType, typename ShaderData>
	void VulkanRenderer::InsertIndexBuffer(IVulkanModel<VertexType, ShaderData>* model)
	{
		if (model) {
			model->createIndexBuffer();
		}
		else {
			// Handle error: model is not of type VulkanModel
			std::cerr << "model is nullptr" << std::endl;
		}
	}

	template<typename VertexType, typename ShaderData>
	void VulkanRenderer::EndCreateMesh(IVulkanModel<VertexType, ShaderData>* model)
	{
		if (model) {
			try {
				model->EndCreateMesh();
			}
			catch (std::exception& e) {
				std::cerr << "Failed to create mesh: " << e.what() << std::endl;
				throw;
			}
		}
		else {
			// Handle error: model is not of type VulkanModel
			std::cerr << "model is nullptr" << std::endl;
		}
	}

	template <typename VertexType, typename ShaderData>
	void VulkanRenderer::EndCreateMesh(IVulkanModel<VertexType, ShaderData>* model, std::string& BmpFilename)
	{
		if (model) {
			try {
				model->EndCreateMesh(BmpFilename);
			}
			catch (std::exception& e) {
				std::cerr << "Failed to create mesh: " << e.what() << std::endl;
				throw;
			}
		}
		else {
			// Handle error: model is not of type VulkanModel
			std::cerr << "model is nullptr" << std::endl;
		}
	}

	template <typename VertexType, typename ShaderData>
	void VulkanRenderer::DeleteMeshObject(IVulkanModel<VertexType, ShaderData>* model)
	{
		if (model) {
			delete model;
		}
		else {
			// Handle error: model is not of type VulkanModel
			std::cerr << "model is nullptr" << std::endl;
		}
	}

	template<typename VertexType, typename ShaderData>
	inline void VulkanRenderer::LoadNodes(IVulkanModel<VertexType, ShaderData>* model, humanGL::Node<VertexType>& node)
	{
		if (model)
		{
			model->loadNodes(node);
			
			
		}
		else
		{
			std::cerr << "model is nullptr" << std::endl;
		}
	}


	template <typename VertexType, typename ShaderData>
	void VulkanRenderer::renderMeshObject(IVulkanModel<VertexType, ShaderData>* model, mymath::Mat4 worldMat, uint32_t colorMode)
	{
		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		if (model)
		{
			model->bind(commandBuffer, _currentFrame);

			//updateObjectUniformBuffer(model, worldMat, colorMode);
			
			ShaderData ubo = updateObjectMatrix<ShaderData>(worldMat);

			model->draw(commandBuffer, _currentFrame, ubo);
		}
	}

	template <typename VertexType, typename ShaderData>
	void VulkanRenderer::updateObjectUniformBuffer(IVulkanModel<VertexType, ShaderData>* model, mymath::Mat4 worldMat, uint32_t colorMode)
	{
		if (model) {

			ShaderData ubo{};

			ubo.modelMatrix = worldMat;

			ubo.viewMatrix = mymath::lookAt(mymath::Vec3(0.0f, 0.0f, 8.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, 1.0f, 0.0f));
			//ubo.viewMatrix = mymath::lookAtGLM(mymath::Vec3(2.0f, 2.0f, 2.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, -1.0f, 0.0f));


			ubo.projectionMatrix = mymath::perspective(mymath::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
			//ubo.projectionMatrix = mymath::perspectiveGLM(mymath::radians(45.0f), getAspectRatio(), 0.1f, 10.0f);
			//ubo.projectionMatrix[5] *= -1;

			//ubo.colorMode = colorMode;
			// ShaderData에 colorMode가 있는 경우에만 값을 대입합니다.
			//setColorMode(ubo, colorMode);

			model->updateUniformBuffer(_currentFrame, &ubo);
		}

	}

	template<typename ShaderData>
	ShaderData VulkanRenderer::updateObjectMatrix(mymath::Mat4& worldMat)
	{
		ShaderData ubo{};

		ubo.modelMatrix = worldMat;
		ubo.viewMatrix = mymath::lookAt(mymath::Vec3(0.0f, 0.0f, 8.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, 1.0f, 0.0f));
		//ubo.viewMatrix = mymath::lookAtGLM(mymath::Vec3(2.0f, 2.0f, 2.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, -1.0f, 0.0f));


		ubo.projectionMatrix = mymath::perspective(mymath::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
		//ubo.projectionMatrix = mymath::perspectiveGLM(mymath::radians(45.0f), getAspectRatio(), 0.1f, 10.0f);

		return ubo;
	}

	template <typename ShaderData>
	std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> VulkanRenderer::createUniformBuffers(size_t bufferSize)
	{
		std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> uniformBuffers;
		// Prepare and initialize the per-frame uniform buffer blocks containing shader uniforms
		// Single uniforms like in OpenGL are no longer present in Vulkan. All Shader uniforms are passed via uniform buffer blocks
		VkMemoryRequirements memReqs;

		// Vertex shader uniform buffer block
		VkBufferCreateInfo bufferInfo{};
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = 0;
		allocInfo.memoryTypeIndex = 0;

		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//bufferInfo.size = sizeof(ShaderData);
		bufferInfo.size = bufferSize;
		// This buffer will be used as a uniform buffer
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		// Create the buffers
		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			VK_CHECK_RESULT(vkCreateBuffer(_vulkanDevice->getLogicalDevice(), &bufferInfo, nullptr, &uniformBuffers[i].buffer));
			// Get memory requirements including size, alignment and memory type
			vkGetBufferMemoryRequirements(_vulkanDevice->getLogicalDevice(), uniformBuffers[i].buffer, &memReqs);
			allocInfo.allocationSize = memReqs.size;
			// Get the memory type index that supports host visible memory access
			// Most implementations offer multiple memory types and selecting the correct one to allocate memory from is crucial
			// We also want the buffer to be host coherent so we don't have to flush (or sync after every update.
			// Note: This may affect performance so you might not want to do this in a real world application that updates buffers on a regular base
			allocInfo.memoryTypeIndex = _vulkanDevice->get_gpu().getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			// Allocate memory for the uniform buffer
			VK_CHECK_RESULT(vkAllocateMemory(_vulkanDevice->getLogicalDevice(), &allocInfo, nullptr, &(uniformBuffers[i].memory)));
			// Bind memory to buffer
			VK_CHECK_RESULT(vkBindBufferMemory(_vulkanDevice->getLogicalDevice(), uniformBuffers[i].buffer, uniformBuffers[i].memory, 0));
			// We map the buffer once, so we can update it without having to map it again
			VK_CHECK_RESULT(vkMapMemory(_vulkanDevice->getLogicalDevice(), uniformBuffers[i].memory, 0, sizeof(ShaderData), 0, (void**)&uniformBuffers[i].mapped));
		}

		return uniformBuffers;
	}

}