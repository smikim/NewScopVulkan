#pragma once

// animation component�� �����
// �� ������Ʈ�� ���� �ϸ� �ִϸ��̼��� �ǰ� ����� ����
// �ִϸ��̼� ������Ʈ���� ���� ��带 ������ �־ ������ ���� ������ ������
#include "../Includes/Common/IVulkanModel.hpp"
#include "../Includes/Common/Vertex.hpp"
#include "Scop.hpp"

#include <array>
#include <memory>

namespace scop {
	class Scop;
}


namespace vks {
	class VulkanRenderer;
	class VulkanAnimationModel;
}

namespace humanGL
{
	template<typename VertexType>
	class Node;

	class HumanGLObject
	{
	private:
		scop::Scop* _scop;
		vks::VulkanRenderer* _renderer;
		vks::IVulkanModel<::HumanVertex, ::ShaderHumanData>* _vulkanModel;
		//Node*_root = nullptr;
		std::unique_ptr<Node<::HumanVertex>> _root;

		void	Cleanup();
		vks::IVulkanModel<::HumanVertex, ::ShaderHumanData>* CreateHumanMeshObject();
		void createCubeData(std::vector<HumanVertex>& vertices, std::vector<uint32_t>& indices, uint32_t bodyPartID);
		void drawNode(Node<::HumanVertex>& node);
		void fillVertexData(Node<::HumanVertex>& node, size_t partsID);
		
	public:
		HumanGLObject();
		~HumanGLObject();
		void createHuman();
		

		bool Initialize(scop::Scop* scop);
		void Run();

		void Render();

	protected:

	};


}
