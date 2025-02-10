#pragma once

// animation component�� �����
// �� ������Ʈ�� ���� �ϸ� �ִϸ��̼��� �ǰ� ����� ����
// �ִϸ��̼� ������Ʈ���� ���� ��带 ������ �־ ������ ���� ������ ������
#include "../Includes/Common/IVulkanModel.hpp"
#include "../Includes/Common/Vertex.hpp"
#include "Scop.hpp"
#include "GameObject.hpp"

#include <array>




namespace vks {
	class VulkanRenderer;
	class VulkanAnimationModel;
}

namespace humanGL
{
	template<typename VertexType>
	class Node;

	class HumanGLObject : public GameObject
	{
	private:
		scop::Scop* _scop;
		vks::VulkanRenderer* _renderer;
		vks::IVulkanModel<::HumanVertex, ::ShaderHumanData>* _vulkanModel;
		//Node*_root = nullptr;
		std::unique_ptr<Node<::HumanVertex>> _root;

		void	Cleanup();
		vks::IVulkanModel<::HumanVertex, ::ShaderHumanData>* CreateHumanMeshObject();
		void createCubeData(std::vector<HumanVertex>& vertices, std::vector<uint32_t>& indices, uint32_t bodyPartID, const std::string& name);
		//void drawNode(Node<::HumanVertex>& node);
		size_t fillVertexData(Node<::HumanVertex>& node, size_t partsID);
		mymath::Vec3 pickColor(const std::string& name);

	public:
		HumanGLObject();
		~HumanGLObject();
			
		void createHuman();
		
		bool Initialize(scop::Scop* scop);
		void Run();

		//void LateUpdate();
		//void FinalUpdate();


		void Render();

		//std::shared_ptr<scop::Component> GetFixedComponent(scop::COMPONENT_TYPE type);
		//std::shared_ptr<scop::Transform> GetTransform();
		//shared_ptr<MeshRenderer> GetMeshRenderer();
		//std::shared_ptr<scop::Camera> GetCamera();

		//void AddComponent(std::shared_ptr<scop::Component> component);

	private:
		
		//std::array<std::shared_ptr<scop::Component>, scop::FIXED_COMPONENT_COUNT> _components;
		//std::vector<std::shared_ptr<scop::MonoBehaviour>> _scripts;
	protected:

	};


}
