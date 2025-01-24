#pragma once

//#include "../Includes/Common/Vertex.hpp"
//#include "../Includes/Common/IVulkanModel.hpp"

#include "Common/Vertex.hpp"
#include "Common/IVulkanModel.hpp"

#include "../Includes/VulkanDevice.hpp"
#include "../Includes/VulkanBuffer.hpp"
#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Includes/Types.hpp"

#include "../Includes/Common/ShaderData.hpp"

#define MAX_CONCURRENT_FRAMES 2

namespace vks
{
	class VulkanRenderer;
	class VulkanTexture;
	class VulkanPipeline;
	template <typename VertexType, typename ShaderData>
	class IVulkanModel;

	class VulkanModel : public IVulkanModel<::ScopVertex, ::ShaderData>
	{
	public:
		
		VulkanModel();
		virtual ~VulkanModel();

		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;


		bool	Initialize(VulkanRenderer* renderer) override;
		bool	Initialize(VulkanRenderer* renderer, std::string& ObjFilename) ;

		void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) override;
		virtual void draw(VkCommandBuffer commandBuffer) override;
		//void createVertexBuffer() override;
		//void createIndexBuffer() override;
		//void createVertexBuffer(const std::vector<::ScopVertex>& vertices) override;
		//void createIndexBuffer(const std::vector<uint32_t>& indices);
		void EndCreateMesh() override;
		void EndCreateMesh(const std::string& BmpFilename) override;
		
		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSets();
		void createPipelineLayout();
		
		void updateUniformBuffer(uint32_t currentFrame, ShaderData* src);
		
	
	protected:
		//VulkanRenderer* _renderer;

	private:
		//Buffer _VertexBuffer;
		//uint32_t vertexCount;
		//Buffer _IndexBuffer;

		//uint32_t indexCount;
		//std::vector<::ScopVertex> _Vertices;
		//std::vector<uint32_t> _Indices;

		VulkanTexture* _texture = nullptr;

		VkDescriptorSetLayout _basicDescriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool _basicDescriptorPool{ VK_NULL_HANDLE };
		VkPipelineLayout _basicPipelineLayout{ VK_NULL_HANDLE };

		
		// We use one UBO per frame, so we can have a frame overlap and make sure that uniforms aren't updated while still in use
		std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> _uniformBuffers;

		VulkanPipeline* _basicPipeline = nullptr;
	};

}

