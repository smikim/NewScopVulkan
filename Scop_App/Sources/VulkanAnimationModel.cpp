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


		/*if (_VertexBuffer.buffer != VK_NULL_HANDLE)
			_VertexBuffer.destroy();
		if (_IndexBuffer.buffer != VK_NULL_HANDLE)
			_IndexBuffer.destroy();*/
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

	/*void VulkanAnimationModel::loadNodes(humanGL::Node& node)
	{
		std::shared_ptr<Node> newNode = std::make_shared<Node>();
		newNode->index = static_cast<uint32_t>(_nodes.size());
		newNode->translation = node.get_transform().get_translation();
		newNode->rotation = node.get_transform().get_rotation();
		newNode->scale = node.get_transform().get_scale();
		newNode->matrix = node.get_transform().get_matrix();
		newNode->parent = nullptr;
		newNode->name = node.get_name();

		_nodes.push_back(newNode);

		for (const auto& child : node.get_children())
		{
			std::cout << child.get() << std::endl;
			loadNodes(*child);
			newNode->children.push_back(_nodes.back());
			_nodes.back()->parent = newNode;
		}
	}*/

	void VulkanAnimationModel::bind(VkCommandBuffer commandBuffer, uint32_t currentFrame)
	{
		_basicPipeline->bind(commandBuffer);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 0, 1, &_uniformBuffers[currentFrame].descriptorSet, 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 1, 1, &_skin.descriptorSet, 0, nullptr);

		//////////
		//VkDeviceSize offsets[1]{ 0 };
		//vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_VertexBuffer.buffer, offsets);
		// Bind triangle index buffer

		//vkCmdBindIndexBuffer(commandBuffer, _IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanAnimationModel::draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, ::ShaderHumanData& ubo)
	{
		//std::cout << "draw Node" << std::endl;
		// Draw indexed triangle
		for (auto& node : _nodes) {
			drawNode(commandBuffer, *node, currentFrame, ubo);
		}
	}

	void VulkanAnimationModel::updateUniformBuffer(uint32_t currentFrame, ShaderHumanData* src)
	{
		memcpy(_uniformBuffers[currentFrame].mapped, src, sizeof(ShaderHumanData));
	}

	void VulkanAnimationModel::updateModelMat(Node<::HumanVertex>& node, ::ShaderHumanData& ubo, uint32_t currentFrame)
	{
		//mymath::Mat4 modelMat = ubo.modelMatrix;

		// 최상위 루트 
		//ubo.modelMatrix = ubo.modelMatrix * node.getLocalMatrix();
		
		//ubo.modelMatrix = ubo.modelMatrix;
		updateUniformBuffer(currentFrame, &ubo);
		// Update ssbo

		//_skin.ssbo.copyToWhere((void*)ubo.modelMatrix, sizeof(mymath::Mat4), sizeof(mymath::Mat4) + i);
		// mymath::Mat4의 주소를 void*로 캐스팅하여 전달합니다.
		//_skin.ssbo.copyToWhere(static_cast<void*>(&ubo.modelMatrix), modelMat.size(), modelMat.size() * i);

		//ubo.modelMatrix = modelMat * node.getTRMatrix();
	}

	void VulkanAnimationModel::drawNode(VkCommandBuffer commandBuffer, Node<::HumanVertex>& node, uint32_t currentFrame, ::ShaderHumanData& ubo)
	{
		VkDeviceSize offsets[1]{ 0 };

		updateModelMat(node, ubo, currentFrame);
		
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &node.mesh._VertexBuffer.buffer, offsets);
		// Bind triangle index buffer
		vkCmdBindIndexBuffer(commandBuffer, node.mesh._IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
		
		

		vkCmdDrawIndexed(commandBuffer, node.mesh.indexCount, 1, 0, 0, 1);

		
		

		for (auto& child : node.children) {
			drawNode(commandBuffer, *child, currentFrame, ubo);
		}
	}

	// Draw the glTF scene starting at the top-level-nodes
	//void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
	//{
	//	// All vertices and indices are stored in single buffers, so we only need to bind once
	//	VkDeviceSize offsets[1] = { 0 };
	//	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertices.buffer, offsets);
	//	vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
	//	// Render all nodes at top-level
	//	for (auto& node : nodes) {
	//		drawNode(commandBuffer, pipelineLayout, node);
	//	}
	//}
	//void VulkanAnimationModel::drawNode(VkCommandBuffer commandBuffer, Node<::HumanVertex>& node, uint32_t currentFrame, ::ShaderHumanData& ubo)
	//{
	//	VkDeviceSize offsets[1]{ 0 };

	//	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &node.mesh._VertexBuffer.buffer, offsets);
	//	// Bind triangle index buffer
	//	vkCmdBindIndexBuffer(commandBuffer, node.mesh._IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	//	vkCmdDrawIndexed(commandBuffer, node.mesh.indexCount, 1, 0, 0, 1);


	//	updateUniformBuffer(currentFrame, &ubo);

	//	for (auto& child : node.children) {
	//		drawNode(commandBuffer, *child, currentFrame, ubo);
	//	}
	//}
	//void VulkanAnimationModel::draw(VkCommandBuffer commandBuffer)
	//{
	//	// Draw indexed triangle

	//	for (auto& node : _nodes) {
	//		drawNode(commandBuffer, *node);
	//	}
	//	
	//}

	//void VulkanAnimationModel::drawNode(VkCommandBuffer commandBuffer, Node<::HumanVertex>& node)
	//{
	//	VkDeviceSize offsets[1]{ 0 };
	//	
	//	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &node.mesh._VertexBuffer.buffer, offsets);
	//	// Bind triangle index buffer
	//	vkCmdBindIndexBuffer(commandBuffer, node.mesh._IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	//	vkCmdDrawIndexed(commandBuffer, node.mesh.indexCount, 1, 0, 0, 1);

	//	for (auto& child : node.children) {
	//		drawNode(commandBuffer, *child);
	//	}
	//}

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
		printNodeHierarchy(_nodes[0], 0);
		
		createSkinFromRootNode(_nodes[0]);

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

		_DecriptorSetLayoutSSBO =
			DescriptorSetLayout::Builder(*(_renderer->getVulkanDevice()))
			.addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				VK_SHADER_STAGE_VERTEX_BIT)
			.build();

	}

	void VulkanAnimationModel::createDescriptorPool()
	{

		_DescriptorPool =
			DescriptorPool::Builder(*(_renderer->getVulkanDevice()))
			.setMaxSets(MAX_CONCURRENT_FRAMES + 1)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_CONCURRENT_FRAMES)
			.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1)
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

		DescriptorWriter(*_DecriptorSetLayoutSSBO, *_DescriptorPool)
			.writeBuffer(0, &_skin.ssbo.descriptor)
			.build(_skin.descriptorSet);
	}

	void VulkanAnimationModel::createPipelineLayout()
	{
		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
		VkDescriptorSetLayout descriptorSetLayout = _DecriptorSetLayout->getDescriptorSetLayout();

		std::array<VkDescriptorSetLayout, 2> setLayouts = {
		_DecriptorSetLayout->getDescriptorSetLayout(),
		_DecriptorSetLayoutSSBO->getDescriptorSetLayout() };
		
		VkPipelineLayoutCreateInfo pipelineLayoutCI{};
		pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCI.pNext = nullptr;
		pipelineLayoutCI.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
		pipelineLayoutCI.pSetLayouts = setLayouts.data();
		pipelineLayoutCI.pushConstantRangeCount = 0;
		pipelineLayoutCI.pPushConstantRanges = nullptr;

		VK_CHECK_RESULT(vkCreatePipelineLayout(vulkanDevice->getLogicalDevice(), &pipelineLayoutCI, nullptr, &_basicPipelineLayout));

	}

	void VulkanAnimationModel::traverseAndAddNodes(std::shared_ptr<Node<::HumanVertex>> node, std::vector<mymath::Mat4>& transformMats, mymath::Mat4& parent)
	{
		node->skin = _skin.joints.size();
		_skin.joints.push_back(node);

		mymath::Mat4 localMatrix = parent * node->getLocalMatrix();
		//mymath::Mat4 localMatrix{ 1.0f };
		
		std::cout << node->name << std::endl;

		transformMats.push_back(localMatrix);
		
		/*if(node->parent)
			parent = node->parent->getTRMatrix();*/
		//parent = localMatrix;
		for (auto& child : node->children)
		{
			parent = node->getTRMatrix();
			traverseAndAddNodes(child, transformMats, parent);
		}
	}

	void VulkanAnimationModel::createSkinFromRootNode(std::shared_ptr<Node<::HumanVertex>> rootNode)
	{
		_skin.skeletonRoot = rootNode;
	
		std::vector<mymath::Mat4> transformMats;
		mymath::Mat4 parent{ 1.0f };
		traverseAndAddNodes(rootNode, transformMats, parent);


		size_t numJoints = transformMats.size();
		size_t numAnimations = 0; // 예를 들어 4개의 애니메이션 데이터가 있다고 가정
		size_t ssboSize = (1 + numAnimations) * numJoints * sizeof(mymath::Mat4);

		_renderer->createSSBO(_skin.ssbo, ssboSize);
		_skin.ssbo.copyTo(static_cast<void*>(transformMats.data()), ssboSize);

		
	}

}