#include "../Includes/VulkanRenderer.hpp"
#include "../Includes/VulkanTools.hpp"
#include "../App/Scop.hpp"

#include "../Includes/VulkanModel.hpp"
#include "../Includes/VulkanAnimationModel.hpp"


// VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#ifndef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
	#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#endif

namespace vks
{
	VulkanRenderer::VulkanRenderer(GlfwWindow& window)
		: _window(window)
	{
		add_device_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		#ifdef __APPLE__
			add_device_extension(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
		#endif
	}

	vks::VulkanRenderer::~VulkanRenderer()
	{
		_swapChain->cleanup();
		
		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		vkDestroyImageView(logicalDevice, _depthStencil.view, nullptr);
		vkDestroyImage(logicalDevice, _depthStencil.image, nullptr);
		vkFreeMemory(logicalDevice, _depthStencil.memory, nullptr);

		if (_RenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(logicalDevice, _RenderPass, nullptr);
		}

		for (uint32_t i = 0; i < _FrameBuffers.size(); i++)
		{
			vkDestroyFramebuffer(logicalDevice, _FrameBuffers[i], nullptr);
		}

		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			vkDestroyFence(logicalDevice, _WaitFences[i], nullptr);
			vkDestroySemaphore(logicalDevice, _PresentCompleteSemaphores[i], nullptr);
			vkDestroySemaphore(logicalDevice, _RenderCompleteSemaphores[i], nullptr);
		}
		if (_drawCommandBuffer)
			delete _drawCommandBuffer;

		if (_basicPSO)
			delete _basicPSO;
		

		vkDestroyCommandPool(_vulkanDevice->getLogicalDevice(), _CmdPool, nullptr);

		//delete _texture;
		if (_vulkanDevice)
			delete _vulkanDevice;
	}

	IVulkanModel<::ScopVertex, ::ShaderData>* VulkanRenderer::CreateBasicMeshObject()
	{
		VulkanModel* vulkanModel = new VulkanModel();

		if (vulkanModel->Initialize(this))
		{
			return vulkanModel;
		}
		else
		{
			delete vulkanModel;
			return nullptr;
		}

	}


	//IVulkanModel<::ScopVertex, ::ShaderData>* VulkanRenderer::CreateBasicMeshObject(std::string& ObjFilename)
	//{
	//	VulkanModel* vulkanModel = new VulkanModel();

	//	if (vulkanModel->Initialize(this, ObjFilename))
	//	{
	//		return vulkanModel;
	//	}
	//	else
	//	{
	//		delete vulkanModel;
	//		return nullptr;
	//	}

	//}

	IVulkanModel<::HumanVertex, ::ShaderHumanData>* VulkanRenderer::CreateHumanMeshObject()
	{
		VulkanAnimationModel* vulkanModel = new VulkanAnimationModel();

		if (vulkanModel->Initialize(this))
		{
			return vulkanModel;
		}
		else
		{
			delete vulkanModel;
			return nullptr;
		}
	}

	bool VulkanRenderer::initVulkan()
	{
		try {
			VK_CHECK_RESULT(_instance.createInstance());
			create_surface();
			_vulkanDevice = new vks::VulkanDevice(_instance.get_first_gpu(), _Surface, get_device_extensions());
			_swapChain = std::make_unique<VulkanSwapChain>(_instance, *_vulkanDevice, _Surface);
			_swapChain->initSurface(_window.getGLFWwindow());
			_width = _window.getWidth();
			_height = _window.getHeight();
			_swapChain->create(_width, _height);
			// Find a suitable depth and/or stencil format
			VkBool32 validFormat{ false };
			VkPhysicalDevice physicalDevice = _vulkanDevice->get_gpu().get_physical_device();
			// Samples that make use of stencil will require a depth + stencil format, so we select from a different list
			if (_requiresStencil) {
				validFormat = vks::tools::getSupportedDepthStencilFormat(physicalDevice, &_DepthFormat);
			}
			else {
				validFormat = vks::tools::getSupportedDepthFormat(physicalDevice, &_DepthFormat);
			}
			assert(validFormat); 
			setupDepthStencil(_width, _height);
			setupRenderPass();
			setupFrameBuffer(_width, _height);
			createSynchronizationPrimitives();
			createCommandPool();

		
			_drawCommandBuffer = new VulkanCommandBuffer(*_vulkanDevice, _CmdPool, _swapChain->_imageCount);

			_basicPSO = new Graphics::BasicPSO(*_vulkanDevice, _RenderPass);
		
			/////////////////////////
			// Allocate data for the dynamic uniform buffer object
		// We allocate this manually as the alignment of the offset differs between GPUs

		// Calculate required alignment based on minimum device offset alignment
	

			
			size_t maxUniformBufferSize = _vulkanDevice->get_gpu().get_device_properties().limits.maxUniformBufferRange;

			size_t minUboAlignment = _vulkanDevice->get_gpu().get_device_properties().limits.minUniformBufferOffsetAlignment;
			size_t maxCount = _vulkanDevice->get_gpu().get_device_properties().limits.maxDescriptorSetUniformBuffersDynamic;
			
			VkDeviceSize maxSSBOSize = _vulkanDevice->get_gpu().get_device_properties().limits.maxStorageBufferRange;

			std::cout << "Max SSBO size: " << maxSSBOSize << " bytes" << std::endl;

			std::cout << "maxCount :" << maxCount << std::endl;;
			size_t dynamicAlignment = 64; // glm::mat4의 크기는 64바이트
			if (minUboAlignment > 0) {
				dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
			}
			std::cout << "maxUniformBufferSize : " << maxUniformBufferSize << std::endl;
			// 최대 할당 가능한 mat4의 개수 계산
			size_t maxMat4Count = 65536 / dynamicAlignment; // 65536바이트 = 64KB

			std::cout << "Max mat4 count: " << maxMat4Count << std::endl;

			// 실제로 할당할 개수 (예: 256개)
			size_t objectInstances = 256;
			size_t bufferSize = objectInstances * dynamicAlignment;

			std::cout << "Buffer size for 256 mat4s: " << bufferSize << " bytes" << std::endl;
			_prepared = true;

			return true;
		} catch (const std::exception& e) {
			 std::cerr << "Vulkan initialization failed: " << e.what() << std::endl;

        	// ?먯썝 ?댁젣
        	delete _vulkanDevice;
        	_vulkanDevice = nullptr;

			return false;
		}
		
	}

	

	

	

	VulkanTexture* VulkanRenderer::CreateTexture(const std::string& filename)
	{
		VulkanTexture* texture = nullptr;
		const VulkanQueue& queue = _vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

		try {
			texture = new VulkanTexture(filename, queue.get_queue(), _vulkanDevice);

		} catch (std::exception& e) {
			delete texture;
			std::cerr << "Failed to create texture: " << e.what() << std::endl;
			throw;
		}
		
		return texture;
	}

	void VulkanRenderer::AddDescriptorSetLayout(VkDescriptorSetLayout layout)
	{
		_descriptorSetLayouts.push_back(layout);
	}
	
	VulkanPipeline* VulkanRenderer::createBasicPipeline(VkPipelineLayout pipelineLayout,
		const std::vector<VkVertexInputBindingDescription>& bindingDescription,
		const std::vector<VkVertexInputAttributeDescription>& attributeDescription,
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule
		)
	{

		try {
			_basicPSO->setPipelineLayout(pipelineLayout);
			//VkShaderModule vertShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/vert.spv", _vulkanDevice);
			//VkShaderModule fragShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/frag.spv", _vulkanDevice);
			//std::vector<VkVertexInputBindingDescription> bindingDescription = 
			//std::vector<VkVertexInputAttributeDescription> attributeDescription
			_basicPSO->initPSO(bindingDescription, attributeDescription, vertShaderModule, fragShaderModule);

			// Scene rendering with shadows applied
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

			VkPipelineShaderStageCreateInfo shaderStage{};
			shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

			shaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
			shaderStage.module = _basicPSO->_VertShaderModule;
			shaderStage.pName = "main";

			shaderStage.pNext = nullptr;
			shaderStage.pSpecializationInfo = nullptr;

			shaderStages.push_back(shaderStage);

			shaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			shaderStage.module = _basicPSO->_FragShaderModule;

			shaderStages.push_back(shaderStage);

			VulkanPipeline* basicPipeline = new VulkanPipeline(*_vulkanDevice, shaderStages, _basicPSO->_pipelineState);
			return basicPipeline;
		} catch (const std::exception& e) {
        	std::cerr << "Error : create VulkanPipeline : " << e.what() << std::endl;
			return nullptr;
		}
			
	}

	VkResult VulkanRenderer::beginRender()
	{
		auto result = _swapChain->acquireNextImage(_WaitFences[_currentFrame], _PresentCompleteSemaphores[_currentFrame], &_currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			_window.resetWindowResizedFlag();
			windowResize();
			std::cout << "beginFrame(): return nullptr" << std::endl;
			return result;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			return result;
		}

		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
		
		// Build the command buffer
		// Unlike in OpenGL all rendering commands are recorded into command buffers that are then submitted to the queue
		// This allows to generate work upfront in a separate thread
		// For basic command buffers (like in this sample), recording is so fast that there is no need to offload this

		vkResetCommandBuffer(commandBuffer, 0);

		_drawCommandBuffer->begin(commandBuffer);

		return VK_SUCCESS;
	}

	void VulkanRenderer::beginRenderPass()
	{
		std::vector<VkClearValue> clearValues;
		VkClearValue clearValue;

		clearValue.color = _defaultClearColor;
		clearValues.push_back(clearValue);

		clearValue.depthStencil = { 1.0f, 0 };
		clearValues.push_back(clearValue);

		uint32_t width = getWidth();
		uint32_t height = getHeight();
		
		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		_drawCommandBuffer->begin_renderpass(commandBuffer, get_frameBuffer_by_index(_currentImageIndex), _RenderPass, width, height, clearValues);

		_drawCommandBuffer->set_viewport(commandBuffer, width, height);
		_drawCommandBuffer->set_scissor(commandBuffer, width, height);

	}

	void VulkanRenderer::endRenderPass()
	{
		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		_drawCommandBuffer->end_renderpass(commandBuffer);

		_drawCommandBuffer->end(commandBuffer);
	}

	VkResult VulkanRenderer::endRender()
	{
		auto result = submitCommandBuffer();

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized()) {
			if (_window.wasWindowResized()) {
				result = VK_ERROR_OUT_OF_DATE_KHR;
			}
			_window.resetWindowResizedFlag();
			windowResize();
			return result;
		}
		else if (result != VK_SUCCESS) {
			return result;
			//throw std::runtime_error("failed to present swap chain image!");
		}

		return VK_SUCCESS;
	}

	VkResult VulkanRenderer::prepareFrame()
	{
		auto result = _swapChain->acquireNextImage(_WaitFences[_currentFrame], _PresentCompleteSemaphores[_currentFrame], &_currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			_window.resetWindowResizedFlag();
			windowResize();
			std::cout << "beginFrame(): return nullptr" << std::endl;
			return result;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			//throw std::runtime_error("failed to acquire swap chain image!");
			return result;
		}

		return VK_SUCCESS;
	}

	VkResult VulkanRenderer::submitFrame()
	{
		auto result = submitCommandBuffer();

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized()) {
			if (_window.wasWindowResized()) {
				result = VK_ERROR_OUT_OF_DATE_KHR;
			}
			_window.resetWindowResizedFlag();
			windowResize();
			return result;
		}
		else if (result != VK_SUCCESS) {
			return result;
			//throw std::runtime_error("failed to present swap chain image!");
		}
		return VK_SUCCESS;
	}

	void VulkanRenderer::createSSBO(vks::Buffer& ssbo, size_t size)
	{
		// Store inverse bind matrices for this skin in a shader storage buffer object
			// To keep this sample simple, we create a host visible shader storage buffer
		VK_CHECK_RESULT(_vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&ssbo,
			size));
		VK_CHECK_RESULT(ssbo.map());
	}

	void VulkanRenderer::getEnabledFeatures()
	{

	}

	void VulkanRenderer::create_surface()
	{
		if (glfwCreateWindowSurface(_instance.getInstance(), _window.getGLFWwindow(), nullptr, &_Surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

	void VulkanRenderer::setupRenderPass()
	{
		std::array<VkAttachmentDescription, 2> attachments = {};
		// Color attachment
		attachments[0].format = _swapChain->_ColorFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		// Depth attachment
		attachments[1].format = _DepthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;
		subpassDescription.pDepthStencilAttachment = &depthReference;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;
		subpassDescription.pResolveAttachments = nullptr;

		// Subpass dependencies for layout transitions
		std::array<VkSubpassDependency, 2> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		dependencies[0].dependencyFlags = 0;

		dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].dstSubpass = 0;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].srcAccessMask = 0;
		dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		dependencies[1].dependencyFlags = 0;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();
		VK_CHECK_RESULT(vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &_RenderPass));
	}

	void VulkanRenderer::setupDepthStencil(uint32_t& width, uint32_t& height)
	{
		VkImageCreateInfo imageCI{};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = _DepthFormat;
		imageCI.extent = { width, height, 1 };
		imageCI.mipLevels = 1;
		imageCI.arrayLayers = 1;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		VK_CHECK_RESULT(vkCreateImage(logicalDevice, &imageCI, nullptr, &_depthStencil.image));
		VkMemoryRequirements memReqs{};
		vkGetImageMemoryRequirements(logicalDevice, _depthStencil.image, &memReqs);


		VkMemoryAllocateInfo memAllloc{};
		memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllloc.allocationSize = memReqs.size;
		memAllloc.memoryTypeIndex = _vulkanDevice->get_gpu().getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(logicalDevice, &memAllloc, nullptr, &_depthStencil.memory));
		VK_CHECK_RESULT(vkBindImageMemory(logicalDevice, _depthStencil.image, _depthStencil.memory, 0));

		VkImageViewCreateInfo imageViewCI{};
		imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCI.image = _depthStencil.image;
		imageViewCI.format = _DepthFormat;
		imageViewCI.subresourceRange.baseMipLevel = 0;
		imageViewCI.subresourceRange.levelCount = 1;
		imageViewCI.subresourceRange.baseArrayLayer = 0;
		imageViewCI.subresourceRange.layerCount = 1;
		imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		// Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
		if (_DepthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
			imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		VK_CHECK_RESULT(vkCreateImageView(logicalDevice, &imageViewCI, nullptr, &_depthStencil.view));

	}

	void VulkanRenderer::setupFrameBuffer(uint32_t& width, uint32_t& height)
	{
		VkImageView attachments[2];

		// Depth/Stencil attachment is the same for all frame buffers
		attachments[1] = _depthStencil.view;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = NULL;
		frameBufferCreateInfo.renderPass = _RenderPass;
		frameBufferCreateInfo.attachmentCount = 2;
		frameBufferCreateInfo.pAttachments = attachments;
		frameBufferCreateInfo.width = width;
		frameBufferCreateInfo.height = height;
		frameBufferCreateInfo.layers = 1;

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		// Create frame buffers for every swap chain image
		_FrameBuffers.resize(_swapChain->_imageCount);
		for (uint32_t i = 0; i < _FrameBuffers.size(); i++)
		{
			attachments[0] = _swapChain->_buffers[i].view;
			VK_CHECK_RESULT(vkCreateFramebuffer(logicalDevice, &frameBufferCreateInfo, nullptr, &_FrameBuffers[i]));
		}
	}

	void VulkanRenderer::createCommandPool()
	{
		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = _swapChain->_queueNodeIndex;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(_vulkanDevice->getLogicalDevice(), &cmdPoolInfo, nullptr, &_CmdPool));
	}


	void VulkanRenderer::createSynchronizationPrimitives()
	{
		// Semaphores are used for correct command ordering within a queue
		VkSemaphoreCreateInfo semaphoreCI{};
		semaphoreCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		// Fences are used to check draw command buffer completion on the host
		VkFenceCreateInfo fenceCI{};
		fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// Create the fences in signaled state (so we don't wait on first render of each command buffer)
		fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			// Semaphore used to ensure that image presentation is complete before starting to submit again
			VK_CHECK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreCI, nullptr, &_PresentCompleteSemaphores[i]));
			// Semaphore used to ensure that all commands submitted have been finished before submitting the image to the queue
			VK_CHECK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreCI, nullptr, &_RenderCompleteSemaphores[i]));

			// Fence used to ensure that command buffer has completed exection before using it again
			VK_CHECK_RESULT(vkCreateFence(logicalDevice, &fenceCI, nullptr, &_WaitFences[i]));
		}
	}

	VkResult VulkanRenderer::submitCommandBuffer()
	{
		// Submit the command buffer to the graphics queue

		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		// Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		// The submit info structure specifies a command buffer queue submission batch
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &waitStageMask;      // Pointer to the list of pipeline stages that the semaphore waits will occur at
		submitInfo.pCommandBuffers = &commandBuffer;		// Command buffers(s) to execute in this batch (submission)
		submitInfo.commandBufferCount = 1;                  // We submit a single command buffer

		// Semaphore to wait upon before the submitted command buffer starts executing
		submitInfo.pWaitSemaphores = &_PresentCompleteSemaphores[_currentFrame];
		submitInfo.waitSemaphoreCount = 1;
		// Semaphore to be signaled when command buffers have completed
		submitInfo.pSignalSemaphores = &_RenderCompleteSemaphores[_currentFrame];
		submitInfo.signalSemaphoreCount = 1;


		// Submit to the graphics queue passing a wait fence
		//std::cout << _Queue.get_queue() << std::endl;
		const VulkanQueue& queue = _vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);
		VK_CHECK_RESULT(vkQueueSubmit(queue.get_queue(), 1, &submitInfo, _WaitFences[_currentFrame]));

		VkResult result = _swapChain->queuePresent(queue.get_queue(), _currentImageIndex, _RenderCompleteSemaphores[_currentFrame]);

		_currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES;

		return result;

	}

	void VulkanRenderer::windowResize()
	{
		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();
		std::cout << "windowResize()" << std::endl;
		if (!_prepared)
		{

			return;
		}
		_prepared = false;
		_resized = true;

		vkDeviceWaitIdle(logicalDevice);

		auto extent = _window.getExtent();

		while (extent.width == 0 || extent.height == 0) {
			extent = _window.getExtent();
			glfwWaitEvents();
		}
		
		_width = extent.width;
		_height = extent.height;

		_swapChain->create(_width, _height);
		// Recreate the frame buffers
		vkDestroyImageView(logicalDevice, _depthStencil.view, nullptr);
		vkDestroyImage(logicalDevice, _depthStencil.image, nullptr);
		vkFreeMemory(logicalDevice, _depthStencil.memory, nullptr);
		setupDepthStencil(_width, _height);
		for (uint32_t i = 0; i < _FrameBuffers.size(); i++) {
			vkDestroyFramebuffer(logicalDevice, _FrameBuffers[i], nullptr);
		}
		setupFrameBuffer(_width, _height);

		delete _drawCommandBuffer;
		_drawCommandBuffer = new VulkanCommandBuffer(*_vulkanDevice, _CmdPool, _swapChain->_imageCount);

		vkDeviceWaitIdle(logicalDevice);


		_prepared = true;
	}

	
	
	

}

