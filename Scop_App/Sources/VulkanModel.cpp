#include "../Includes/VulkanModel.hpp"
#include "../Includes/VulkanTools.hpp"
#include "../Includes/VulkanRenderer.hpp"
#include "../Library/ObjMeshLoader.hpp"

namespace vks
{
	VulkanModel::VulkanModel()
	{
	}

	VulkanModel::~VulkanModel()
	{

		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
		vkDestroyDescriptorPool(vulkanDevice->getLogicalDevice(), _basicDescriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(vulkanDevice->getLogicalDevice(), _basicDescriptorSetLayout, nullptr);

		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			vkDestroyBuffer(vulkanDevice->getLogicalDevice(), _uniformBuffers[i].buffer, nullptr);
			vkFreeMemory(vulkanDevice->getLogicalDevice(), _uniformBuffers[i].memory, nullptr);
		}

		vkDestroyPipelineLayout(vulkanDevice->getLogicalDevice(), _basicPipelineLayout, nullptr);

		if (_basicPipeline)
			delete _basicPipeline;

		if (_texture)
			delete _texture;

		/*if (_VertexBuffer.buffer != VK_NULL_HANDLE)
			_VertexBuffer.destroy();
		if (_IndexBuffer.buffer != VK_NULL_HANDLE)	
			_IndexBuffer.destroy();*/
	}

	bool VulkanModel::Initialize(VulkanRenderer* renderer)
	{
		_renderer = renderer;

		try {
			createDescriptorSetLayout();
			createPipelineLayout();

			VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
			VkShaderModule vertShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/shader.vert.spv", *vulkanDevice);
			VkShaderModule fragShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/shader.frag.spv", *vulkanDevice);

			_basicPipeline = _renderer->createBasicPipeline(_basicPipelineLayout, ::ScopVertex::getBindingDescription(),
				::ScopVertex::getAttributeDescription(), vertShaderModule, fragShaderModule);
			if (_basicPipeline == nullptr)
				return false;

			_uniformBuffers = _renderer->createUniformBuffers<::ShaderData>(sizeof(::ShaderData));
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to load OBJ mesh: " << e.what() << std::endl;
			return false;
		}
		


		return true;
	}

	void VulkanModel::bind(VkCommandBuffer commandBuffer, uint32_t currentFrame)
	{
		_basicPipeline->bind(commandBuffer);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 0, 1, &_uniformBuffers[currentFrame].descriptorSet, 0, nullptr);

	}

	void VulkanModel::updateUniformBuffer(uint32_t currentFrame, ShaderData* src)
	{
		//mymath::Mat4 worldMat = src->modelMatrix;

		memcpy(_uniformBuffers[currentFrame].mapped, src, sizeof(::ShaderData));
	}


	void VulkanModel::draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, ::ShaderData& ubo)
	{
		// Draw indexed triangle
		for (auto& node : _nodes) {
			drawNode(commandBuffer, *node, currentFrame, ubo);
		}
	}

	void VulkanModel::drawNode(VkCommandBuffer commandBuffer, Node<::ScopVertex>& node, uint32_t currentFrame, ::ShaderData& ubo)
	{
		VkDeviceSize offsets[1]{ 0 };

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &node.mesh._VertexBuffer.buffer, offsets);
		// Bind triangle index buffer
		vkCmdBindIndexBuffer(commandBuffer, node.mesh._IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, node.mesh.indexCount, 1, 0, 0, 1);

		updateUniformBuffer(currentFrame, &ubo);


		for (auto& child : node.children) {
			drawNode(commandBuffer, *child, currentFrame, ubo);
		}
	}

	//bool VulkanModel::Initialize(VulkanRenderer* renderer, std::string& ObjFilename)
	//{
	//	_renderer = renderer;

	//	try {
	//		ObjMeshLoader objLoader{ ObjFilename };
	//		//_Vertices = objLoader.vertices;
	//		//_Indices = objLoader.indices;

	//		createDescriptorSetLayout();
	//		createPipelineLayout();

	//		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
	//		VkShaderModule vertShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/shader.vert.spv", *vulkanDevice);
	//		VkShaderModule fragShaderModule = utils::loadSPIRVShader("./Scop_App/shaders/shader.frag.spv", *vulkanDevice);
	//		
	//		_basicPipeline = _renderer->createBasicPipeline(_basicPipelineLayout, ::ScopVertex::getBindingDescription(),
	//			::ScopVertex::getAttributeDescription(), vertShaderModule, fragShaderModule);
	//		if (_basicPipeline == nullptr)
	//			return false;
	//	
	//		_uniformBuffers = _renderer->createUniformBuffers<::ShaderData>(sizeof(::ShaderData));
	//	}
	//	catch (const std::exception& e)
	//	{
	//		std::cerr << "Failed to load OBJ mesh: " << e.what() << std::endl;
	//		return false;
	//	}
	//	


	//	return true;
	//}


	//void VulkanModel::bind(VkCommandBuffer commandBuffer, uint32_t currentFrame)
	//{
	//	_basicPipeline->bind(commandBuffer);

	//	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 0, 1, &_uniformBuffers[currentFrame].descriptorSet, 0, nullptr);

	//	VkDeviceSize offsets[1]{ 0 };
	//	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_VertexBuffer.buffer, offsets);
	//	// Bind triangle index buffer

	//	vkCmdBindIndexBuffer(commandBuffer, _IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	//}

	//void VulkanModel::draw(VkCommandBuffer commandBuffer)
	//{
	//	// Draw indexed triangle
	//	vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 1);
	//}

	//void VulkanModel::createVertexBuffer()
	//{
	//	uint32_t vertexBufferSize = static_cast<uint32_t>(_Vertices.size()) * sizeof(::ScopVertex);
	//	vertexCount = _Vertices.size();

	//	// Create buffers and upload data to the GPU
	//	struct StagingBuffers {
	//		vks::Buffer vertices;
	//		vks::Buffer indices;
	//	} stagingBuffers;

	//	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

	//	// Host visible source buffers (staging)
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.vertices, vertexBufferSize, _Vertices.data()));

	//	// Device local destination buffers
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_VertexBuffer, _Vertices.size() * sizeof(::ScopVertex)));

	//	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	//	// Copy from host do device
	//	vulkanDevice->copyBuffer(&stagingBuffers.vertices, &_VertexBuffer, queue.get_queue());

	//	// Clean up
	//	stagingBuffers.vertices.destroy();
	//}

	//void VulkanModel::createIndexBuffer()
	//{
	//	indexCount = static_cast<uint32_t>(_Indices.size());

	//	struct StagingBuffers {
	//		vks::Buffer vertices;
	//		vks::Buffer indices;
	//	} stagingBuffers;

	//	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
	//	// Host visible source buffers (staging)
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.indices, _Indices.size() * sizeof(uint32_t), _Indices.data()));
	//	
	//	// Device local destination buffers
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_IndexBuffer, _Indices.size() * sizeof(uint32_t)));

	//	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	//	// Copy from host do device
	//	vulkanDevice->copyBuffer(&stagingBuffers.indices, &_IndexBuffer, queue.get_queue());

	//	// Clean up
	//	stagingBuffers.indices.destroy();
	//}

	//void VulkanModel::createVertexBuffer(const std::vector<::ScopVertex>& vertices)
	//{
	//	_Vertices = vertices;

	//	uint32_t vertexBufferSize = static_cast<uint32_t>(_Vertices.size()) * sizeof(::ScopVertex);
	//	vertexCount = _Vertices.size();

	//	// Create buffers and upload data to the GPU
	//	struct StagingBuffers {
	//		vks::Buffer vertices;
	//		vks::Buffer indices;
	//	} stagingBuffers;

	//	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

	//	// Host visible source buffers (staging)
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.vertices, vertexBufferSize, _Vertices.data()));

	//	// Device local destination buffers
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_VertexBuffer, _Vertices.size() * sizeof(::ScopVertex)));

	//	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	//	// Copy from host do device
	//	vulkanDevice->copyBuffer(&stagingBuffers.vertices, &_VertexBuffer, queue.get_queue());

	//	// Clean up
	//	stagingBuffers.vertices.destroy();
	//}

	//void VulkanModel::createIndexBuffer(const std::vector<uint32_t>& indices)
	//{
	//	_Indices = indices;

	//	indexCount = static_cast<uint32_t>(_Indices.size());

	//	struct StagingBuffers {
	//		vks::Buffer vertices;
	//		vks::Buffer indices;
	//	} stagingBuffers;

	//	VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
	//	// Host visible source buffers (staging)
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.indices, _Indices.size() * sizeof(uint32_t), _Indices.data()));

	//	// Device local destination buffers
	//	VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_IndexBuffer, _Indices.size() * sizeof(uint32_t)));

	//	const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

	//	// Copy from host do device
	//	vulkanDevice->copyBuffer(&stagingBuffers.indices, &_IndexBuffer, queue.get_queue());

	//	// Clean up
	//	stagingBuffers.indices.destroy();
	//}



	void VulkanModel::EndCreateMesh()
	{
		createDescriptorPool();
		createDescriptorSets();
	}


	void VulkanModel::EndCreateMesh(const std::string& BmpFilename)
	{
		try {
			_texture = _renderer->CreateTexture(BmpFilename);
	
		} catch (std::exception& e) {
			std::cerr << "Failed to load texture: " << e.what() << std::endl;
			throw;
		}
	
		createDescriptorPool();
		createDescriptorSets();
	}

	void VulkanModel::createDescriptorSetLayout()
	{
		std::array<VkDescriptorSetLayoutBinding, 2> setLayoutBindings{};

		// Binding 0: Uniform buffer (Vertex shader)
		setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		setLayoutBindings[0].binding = 0;
		setLayoutBindings[0].descriptorCount = 1;
		setLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		setLayoutBindings[0].pImmutableSamplers = nullptr;

		/*
			Binding 1: Combined image sampler (used to pass per object texture information)
		*/
		setLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		setLayoutBindings[1].binding = 1;
		// Accessible from the fragment shader only
		setLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		setLayoutBindings[1].descriptorCount = 1;
		setLayoutBindings[1].pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorLayoutCI{};
		descriptorLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorLayoutCI.pNext = nullptr;
		descriptorLayoutCI.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorLayoutCI.pBindings = setLayoutBindings.data();

		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(vulkanDevice->getLogicalDevice(), &descriptorLayoutCI, nullptr, &_basicDescriptorSetLayout));

	}

	// Descriptors are allocated from a pool, that tells the implementation how many and what types of descriptors we are going to use (at maximum)
	void VulkanModel::createDescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes{};

		// We need to tell the API the number of max. requested descriptors per type

		// This example only one descriptor type (uniform buffer)
		descriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		// We have one buffer (and as such descriptor) per frame
		descriptorPoolSizes[0].descriptorCount = MAX_CONCURRENT_FRAMES;

		// Combined image samples : 1 per object texture
		descriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorPoolSizes[1].descriptorCount = MAX_CONCURRENT_FRAMES;


		// For additional types you need to add new entries in the type count list
		// E.g. for two combined image samplers :
		// typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		// typeCounts[1].descriptorCount = 2;

		// Create the global descriptor pool
		// All descriptors used in this example are allocated from this pool
		VkDescriptorPoolCreateInfo descriptorPoolCI{};
		descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCI.pNext = nullptr;
		descriptorPoolCI.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
		descriptorPoolCI.pPoolSizes = descriptorPoolSizes.data();
		// Set the max. number of descriptor sets that can be requested from this pool (requesting beyond this limit will result in an error)
		// Our sample will create one set per uniform buffer per frame
		descriptorPoolCI.maxSets = MAX_CONCURRENT_FRAMES;

		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
		VK_CHECK_RESULT(vkCreateDescriptorPool(vulkanDevice->getLogicalDevice(), &descriptorPoolCI, nullptr, &_basicDescriptorPool));

	}

	// Shaders access data using descriptor sets that "point" at our uniform buffers
	// The descriptor sets make use of the descriptor set layouts created above 
	void VulkanModel::createDescriptorSets()
	{
		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		// Allocate one descriptor set per frame from the global descriptor pool
		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = _basicDescriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &_basicDescriptorSetLayout;
			VK_CHECK_RESULT(vkAllocateDescriptorSets(vulkanDevice->getLogicalDevice(), &allocInfo, &_uniformBuffers[i].descriptorSet));

			// Update the descriptor set determining the shader binding points
			// For every binding point used in a shader there needs to be one
			// descriptor set matching that binding point
			std::array<VkWriteDescriptorSet, 2> writeDescriptorSets{};

			// The buffer's information is passed using a descriptor info structure
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = _uniformBuffers[i].buffer;
			bufferInfo.range = sizeof(::ShaderData);

			// Setup a descriptor image info for the current texture to be used as a combined image sampler
			VkDescriptorImageInfo textureDescriptor;
			// The image's view (images are never directly accessed by the shader, but rather through views defining subresources)
			textureDescriptor.imageView = _texture->_TextureImageView;
			// The sampler (Telling the pipeline how to sample the texture, including repeat, border, etc.)
			textureDescriptor.sampler = _texture->_TextureSampler;
			// The current layout of the image(Note: Should always fit the actual use, e.g.shader read)
			textureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			// Binding 0 : Uniform buffer
			writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSets[0].dstSet = _uniformBuffers[i].descriptorSet;
			writeDescriptorSets[0].descriptorCount = 1;
			writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeDescriptorSets[0].pBufferInfo = &bufferInfo;
			writeDescriptorSets[0].dstBinding = 0;

			/*
				Binding 1: Object texture
			*/
			writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSets[1].dstSet = _uniformBuffers[i].descriptorSet;
			writeDescriptorSets[1].dstBinding = 1;
			writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			// Images use a different descriptor structure, so we use pImageInfo instead of pBufferInfo
			writeDescriptorSets[1].pImageInfo = &textureDescriptor;
			writeDescriptorSets[1].descriptorCount = 1;

			vkUpdateDescriptorSets(vulkanDevice->getLogicalDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}
	}

	void VulkanModel::createPipelineLayout()
	{
		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		VkPipelineLayoutCreateInfo pipelineLayoutCI{};
		pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCI.pNext = nullptr;
		pipelineLayoutCI.setLayoutCount = 1;
		pipelineLayoutCI.pSetLayouts = &_basicDescriptorSetLayout;
		pipelineLayoutCI.pushConstantRangeCount = 0;
		pipelineLayoutCI.pPushConstantRanges = nullptr;

		VK_CHECK_RESULT(vkCreatePipelineLayout(vulkanDevice->getLogicalDevice(), &pipelineLayoutCI, nullptr, &_basicPipelineLayout));

	}

	
	

}
