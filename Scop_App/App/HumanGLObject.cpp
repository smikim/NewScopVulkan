#include "HumanGLObject.hpp"

#include "../Includes/VulkanRenderer.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Animation.hpp"
#include "../Scop_App/Library/Math/Matrix.hpp"
#include "../Scop_App/Component/Animation.hpp"

#include <cassert>

namespace humanGL
{
	HumanGLObject::HumanGLObject()
	{
	}
	
	HumanGLObject::~HumanGLObject()
	{
		Cleanup();
	}

	void HumanGLObject::createHuman()
	{
		_root = std::make_unique<Node<::HumanVertex>>("body");
		// ÀÚ½Ä ³ëµå Ãß°¡ ¿¹Á¦
		//auto childNode = std::make_unique<Node>("ChildNode");
		//_root->add_child(*childNode);
		// childNode´Â ÀÌÁ¦ _rootÀÇ ÀÚ½ÄÀÌ¹Ç·Î, childNodeÀÇ ¼ÒÀ¯±ÇÀ» _root°¡ °¡Áý´Ï´Ù.
	
		//_root = new Node( "body" );
		_root->get_transform().set_translation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		_root->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		_root->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));

		
		std::unique_ptr<Node<::HumanVertex>> head = std::make_unique<Node<::HumanVertex>>("head");
		head->get_transform().set_translation(mymath::Vec3(0.0f, 1.4f, 0.0f));
		head->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		head->get_transform().set_scale(mymath::Vec3(0.4f, 0.4f, 0.4f));

		//std::cout << "head : " << & head << std::endl;

		/*std::unique_ptr<Node<::HumanVertex>> left_upper_arm = std::make_unique<Node<::HumanVertex>>("left_upper_arm");
		std::cout << "left_upper_arm : " << &left_upper_arm << std::endl;
		left_upper_arm->get_transform().set_translation(mymath::Vec3(4.0f, 4.f, 4.0f));
		left_upper_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_upper_arm->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));*/
		
		
		//

		/*std::unique_ptr<Node<::HumanVertex>> right_upper_arm = std::make_unique<Node<::HumanVertex>>("right_upper_arm");
		std::cout << "right_upper_arm : " << &right_upper_arm << std::endl;
		right_upper_arm->get_transform().set_translation(mymath::Vec3(0.65f, 0.15f, 0.0f));
		right_upper_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_upper_arm->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));*/
	
		
		

		/*std::unique_ptr<Node<::HumanVertex>> left_lower_arm = std::make_unique<Node<::HumanVertex>>("left_lower_arm");
		std::cout << "left_lower_arm : " << left_lower_arm.get() << std::endl;
		left_lower_arm->get_transform().set_translation(mymath::Vec3(0.0f, -0.7f, 0.0f));
		left_lower_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_lower_arm->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));*/
		
		
		



		/*std::unique_ptr<Node<::HumanVertex>> right_under_arm = std::make_unique<Node<::HumanVertex>>("right_under_arm");
		std::cout << "right_under_arm : " << right_under_arm.get() << std::endl;
		right_under_arm->get_transform().set_translation(mymath::Vec3(0.0f, -0.7f, 0.0f));
		right_under_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_under_arm->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));
		
		*/
		


		/*std::unique_ptr<Node<::HumanVertex>> left_upper_leg = std::make_unique<Node<::HumanVertex>>("left_upper_leg");
		std::cout << "left_upper_leg : " << left_upper_leg.get() << std::endl;
		left_upper_leg->get_transform().set_translation(mymath::Vec3(-0.25f, -0.875f, 0.0f));
		left_upper_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_upper_leg->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));*/
	
		



		/*std::unique_ptr<Node<::HumanVertex>> right_upper_leg = std::make_unique<Node<::HumanVertex>>("right_upper_leg");
		std::cout << "right_upper_leg : " << right_upper_leg.get() << std::endl;
		right_upper_leg->get_transform().set_translation(mymath::Vec3(0.25f, -0.875f, 0.0f));
		right_upper_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_upper_leg->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));*/
	
		
		


		/*std::unique_ptr<Node<::HumanVertex>> left_lower_leg = std::make_unique<Node<::HumanVertex>>("left_lower_leg");
		std::cout << "heleft_lower_legad : " << left_lower_leg.get() << std::endl;
		left_lower_leg->get_transform().set_translation(mymath::Vec3(0.0f, -0.75f, 0.0f));
		left_lower_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_lower_leg->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));*/

		

		/*std::unique_ptr<Node<::HumanVertex>> right_lower_leg = std::make_unique<Node<::HumanVertex>>("right_lower_leg");
		std::cout << "right_lower_leg : " << right_lower_leg.get() << std::endl;
		right_lower_leg->get_transform().set_translation(mymath::Vec3(0.0f, -0.75f, 0.0f));
		right_lower_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_lower_leg->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));*/


		//left_upper_arm->add_child(std::move(left_lower_arm));
		//right_upper_arm->add_child(std::move(right_under_arm));

		//left_upper_leg->add_child(std::move(left_lower_leg));
		//right_upper_leg->add_child(std::move(right_lower_leg));
		
		//_root->add_child(std::move(left_upper_arm));
		//_root->add_child(std::move(right_upper_arm));
		//_root->add_child(std::move(left_upper_leg));
		//_root->add_child(std::move(right_upper_leg));
		_root->add_child(std::move(head));

	}

	void HumanGLObject::Cleanup()
	{
		if (_vulkanModel)
		{
			_renderer->DeleteMeshObject(_vulkanModel);
		}
	}

	vks::IVulkanModel<::HumanVertex, ::ShaderHumanData>* HumanGLObject::CreateHumanMeshObject()
	{
		

		_vulkanModel = _renderer->CreateHumanMeshObject();
		if (_vulkanModel == nullptr)
			return nullptr;
		// TODO
		// Texture ï¿½ß°ï¿½ ï¿½Ï´ï¿½ ï¿½Úµï¿½

		_renderer->LoadNodes(_vulkanModel, *_root);
		_renderer->BeginCreateMesh(_vulkanModel);
		_renderer->InsertIndexBuffer(_vulkanModel);
		_renderer->EndCreateMesh(_vulkanModel);
		
		

		return _vulkanModel;
	
	}

	void HumanGLObject::drawNode(Node<::HumanVertex>& node)
	{
		if (_vulkanModel)
		{
			//_renderer->renderMeshObject(_vulkanModel, _transform.mat4(), _colorMode);
			mymath::Mat4 _worldMatrix = node.get_transform().get_world_matrix();
			_renderer->renderMeshObject<::HumanVertex>(_vulkanModel, _worldMatrix, 0);
		}

		for (const auto& child : node.get_children())
		{
			std::cout << child.get() << std::endl;
			drawNode(*child);
		}
	}

	void HumanGLObject::fillVertexData(Node<::HumanVertex>& node, size_t partsID)
	{
		std::vector<HumanVertex> vertices;
		std::vector<uint32_t> indices;
		createCubeData(vertices, indices, partsID);

		node.set_vertices(vertices);
		node.set_indices(indices);

		for (const auto& child : node.get_children())
		{
			partsID++;
			std::cout << child.get() << std::endl;
			fillVertexData(*child, partsID);
		}
	}
	
	bool HumanGLObject::Initialize(scop::Scop* scop)
	{
		_scop = scop;
		_renderer = scop->getVulkanRenderer();

		// create hiargrachy
		createHuman();
		fillVertexData(*_root, 0);
		
		_vulkanModel = CreateHumanMeshObject();
		
		
		if (_vulkanModel == nullptr)
			return false;
		return true;

	}

	void HumanGLObject::Run()
	{
		//TODO
	}


	void HumanGLObject::Render() 
	{
		//if (_root)
		//	drawNode(*_root);
		
		if (_vulkanModel)
		{
			//_renderer->renderMeshObject(_vulkanModel, _transform.mat4(), _colorMode);
			mymath::Mat4 scaleMatrix(1.0f);
			_renderer->renderMeshObject(_vulkanModel, scaleMatrix, 0);
		}
	}

	void HumanGLObject::createCubeData(std::vector<HumanVertex>& vertices, std::vector<uint32_t>& indices, uint32_t bodyPartID)
	{
		vertices = {
				{ { -1.0f, -1.0f,  1.0f }, { .9f, .9f, .9f }, bodyPartID },
				{ {  1.0f, -1.0f,  1.0f }, { .9f, .9f, .9f }, bodyPartID },
				{ {  1.0f,  1.0f,  1.0f }, { .9f, .9f, .9f }, bodyPartID },
				{ { -1.0f,  1.0f,  1.0f }, { .9f, .9f, .9f }, bodyPartID },

				{ {  1.0f,  1.0f,  1.0f }, { .8f, .8f, .1f }, bodyPartID },
				{ {  1.0f,  1.0f, -1.0f }, { .8f, .8f, .1f }, bodyPartID },
				{ {  1.0f, -1.0f, -1.0f }, { .8f, .8f, .1f }, bodyPartID },
				{ {  1.0f, -1.0f,  1.0f }, { .8f, .8f, .1f }, bodyPartID },

				{ { -1.0f, -1.0f, -1.0f }, { .9f, .6f, .1f }, bodyPartID },
				{ {  1.0f, -1.0f, -1.0f }, { .9f, .6f, .1f }, bodyPartID },
				{ {  1.0f,  1.0f, -1.0f }, { .9f, .6f, .1f }, bodyPartID },
				{ { -1.0f,  1.0f, -1.0f }, { .9f, .6f, .1f }, bodyPartID },

				{ { -1.0f, -1.0f, -1.0f }, { .8f, .1f, .1f }, bodyPartID },
				{ { -1.0f, -1.0f,  1.0f }, { .8f, .1f, .1f }, bodyPartID },
				{ { -1.0f,  1.0f,  1.0f }, { .8f, .1f, .1f }, bodyPartID },
				{ { -1.0f,  1.0f, -1.0f }, { .8f, .1f, .1f }, bodyPartID },

				{ {  1.0f,  1.0f,  1.0f }, { .1f, .1f, .8f }, bodyPartID },
				{ { -1.0f,  1.0f,  1.0f }, { .1f, .1f, .8f }, bodyPartID },
				{ { -1.0f,  1.0f, -1.0f }, { .1f, .1f, .8f }, bodyPartID },
				{ {  1.0f,  1.0f, -1.0f }, { .1f, .1f, .8f }, bodyPartID },

				{ { -1.0f, -1.0f, -1.0f }, { .1f, .8f, .1f }, bodyPartID },
				{ {  1.0f, -1.0f, -1.0f }, { .1f, .8f, .1f }, bodyPartID },
				{ {  1.0f, -1.0f,  1.0f }, { .1f, .8f, .1f }, bodyPartID },
				{ { -1.0f, -1.0f,  1.0f }, { .1f, .8f, .1f }, bodyPartID },
		};

		indices = {
			0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23
		};
	}

	void createCubeData(std::vector<HumanVertex>& vertices, std::vector<uint32_t>& indices)
	{
		vertices = {
			{ { -1.0f, -1.0f,  1.0f }, {.9f, .9f, .9f} },
			{ {  1.0f, -1.0f,  1.0f }, {.9f, .9f, .9f}},
			{ {  1.0f,  1.0f,  1.0f }, {.9f, .9f, .9f}},
			{ { -1.0f,  1.0f,  1.0f }, {.9f, .9f, .9f}},

			{ {  1.0f,  1.0f,  1.0f }, {.8f, .8f, .1f}},
			{ {  1.0f,  1.0f, -1.0f }, {.8f, .8f, .1f}},
			{ {  1.0f, -1.0f, -1.0f }, {.8f, .8f, .1f}},
			{ {  1.0f, -1.0f,  1.0f }, {.8f, .8f, .1f}},

			{ { -1.0f, -1.0f, -1.0f }, {.9f, .6f, .1f}},
			{ {  1.0f, -1.0f, -1.0f }, {.9f, .6f, .1f}},
			{ {  1.0f,  1.0f, -1.0f }, {.9f, .6f, .1f}},
			{ { -1.0f,  1.0f, -1.0f }, {.9f, .6f, .1f}},

			{ { -1.0f, -1.0f, -1.0f }, {.8f, .1f, .1f}},
			{ { -1.0f, -1.0f,  1.0f }, {.8f, .1f, .1f}},
			{ { -1.0f,  1.0f,  1.0f }, {.8f, .1f, .1f}},
			{ { -1.0f,  1.0f, -1.0f }, {.8f, .1f, .1f}},

			{ {  1.0f,  1.0f,  1.0f }, {.1f, .1f, .8f}},
			{ { -1.0f,  1.0f,  1.0f }, {.1f, .1f, .8f}},
			{ { -1.0f,  1.0f, -1.0f }, {.1f, .1f, .8f}},
			{ {  1.0f,  1.0f, -1.0f }, {.1f, .1f, .8f}},

			{ { -1.0f, -1.0f, -1.0f }, {.1f, .8f, .1f}},
			{ {  1.0f, -1.0f, -1.0f }, {.1f, .8f, .1f}},
			{ {  1.0f, -1.0f,  1.0f }, {.1f, .8f, .1f}},
			{ { -1.0f, -1.0f,  1.0f }, {.1f, .8f, .1f}},
		};
		indices = {
			0,1,2, 0,2,3, 4,5,6,  4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23
		};
	}
}