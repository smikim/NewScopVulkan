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
class Node;
class HumanGLObject
{
private:
	scop::Scop* _scop;
	vks::VulkanRenderer* _renderer;
	vks::IVulkanModel<::HumanVertex, ::ShaderHumanData>* _vulkanModel;
	//Node*_root = nullptr;
	std::unique_ptr<Node> _root;
	
	void	Cleanup();
	vks::IVulkanModel<::HumanVertex, ::ShaderHumanData>* CreateBoxMeshObject();
	void drawNode(Node& node);

public:
	HumanGLObject();
	~HumanGLObject();
	void createHuman();
	bool Initialize(scop::Scop* scop);
	void Run();
	
	void Render();

protected:

};

void createCubeData(std::vector<HumanVertex>& vertices,
	std::vector<uint32_t>& indices);
}