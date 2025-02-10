#include "HumanGLObject.hpp"

#include "../Includes/VulkanRenderer.hpp"

#include "../Scop_App/Library/Math/Matrix.hpp"
#include "../Scop_App/Component/Animation.hpp"
#include "../Component/Camera.hpp"
#include "../Component/Transform.hpp"

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
		// 자식 노드 추가 예제
		//auto childNode = std::make_unique<Node>("ChildNode");
		//_root->add_child(*childNode);
		// childNode는 이제 _root의 자식이므로, childNode의 소유권을 _root가 가집니다.

		//_root = new Node( "body" );
		_root->get_transform().set_translation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		_root->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		_root->get_transform().set_scale(mymath::Vec3(1.0f, 1.0f, 1.0f));


		std::unique_ptr<Node<::HumanVertex>> head = std::make_unique<Node<::HumanVertex>>("head");
		head->get_transform().set_translation(mymath::Vec3(0.0f, 1.4f, 0.0f));
		head->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		head->get_transform().set_scale(mymath::Vec3(0.4f, 0.4f, 0.4f));

		//std::cout << "head : " << & head << std::endl;

		std::unique_ptr<Node<::HumanVertex>> left_upper_arm = std::make_unique<Node<::HumanVertex>>("left_upper_arm");
		std::cout << "left_upper_arm : " << &left_upper_arm << std::endl;
		left_upper_arm->get_transform().set_translation(mymath::Vec3(1.9f, 0.8f, 0.0f));
		left_upper_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_upper_arm->get_transform().set_scale(mymath::Vec3(0.8f, 0.3f, 0.3f));

		std::unique_ptr<Node<::HumanVertex>> right_upper_arm = std::make_unique<Node<::HumanVertex>>("right_upper_arm");
		std::cout << "right_upper_arm : " << &right_upper_arm << std::endl;
		right_upper_arm->get_transform().set_translation(mymath::Vec3(-1.9f, 0.8f, 0.0f));
		right_upper_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_upper_arm->get_transform().set_scale(mymath::Vec3(0.8f, 0.3f, 0.3f));

		std::unique_ptr<Node<::HumanVertex>> left_lower_arm = std::make_unique<Node<::HumanVertex>>("left_lower_arm");
		std::cout << "left_lower_arm : " << left_lower_arm.get() << std::endl;
		left_lower_arm->get_transform().set_translation(mymath::Vec3(1.6f, 0.0f, 0.0f));
		left_lower_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_lower_arm->get_transform().set_scale(mymath::Vec3(0.8f, 0.3f, 0.3f));

		std::unique_ptr<Node<::HumanVertex>> right_lower_arm = std::make_unique<Node<::HumanVertex>>("right_lower_arm");
		std::cout << "right_under_arm : " << right_lower_arm.get() << std::endl;
		right_lower_arm->get_transform().set_translation(mymath::Vec3(-1.6f, 0.0f, 0.0f));
		right_lower_arm->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_lower_arm->get_transform().set_scale(mymath::Vec3(0.8f, 0.3f, 0.3f));


		std::unique_ptr<Node<::HumanVertex>> left_upper_leg = std::make_unique<Node<::HumanVertex>>("left_upper_leg");
		std::cout << "left_upper_leg : " << left_upper_leg.get() << std::endl;
		left_upper_leg->get_transform().set_translation(mymath::Vec3(0.6f, -1.8f, 0.0f));
		left_upper_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_upper_leg->get_transform().set_scale(mymath::Vec3(0.4f, 1.0f, 0.4f));


		std::unique_ptr<Node<::HumanVertex>> right_upper_leg = std::make_unique<Node<::HumanVertex>>("right_upper_leg");
		std::cout << "right_upper_leg : " << right_upper_leg.get() << std::endl;
		right_upper_leg->get_transform().set_translation(mymath::Vec3(-0.6f, -1.8f, 0.0f));
		right_upper_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_upper_leg->get_transform().set_scale(mymath::Vec3(0.4f, 1.0f, 0.4f));


		std::unique_ptr<Node<::HumanVertex>> left_lower_leg = std::make_unique<Node<::HumanVertex>>("left_lower_leg");
		std::cout << "heleft_lower_legad : " << left_lower_leg.get() << std::endl;
		left_lower_leg->get_transform().set_translation(mymath::Vec3(0.0f, -2.0f, 0.0f));
		left_lower_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		left_lower_leg->get_transform().set_scale(mymath::Vec3(0.4f, 1.0f, 0.4f));


		std::unique_ptr<Node<::HumanVertex>> right_lower_leg = std::make_unique<Node<::HumanVertex>>("right_lower_leg");
		std::cout << "right_lower_leg : " << right_lower_leg.get() << std::endl;
		right_lower_leg->get_transform().set_translation(mymath::Vec3(0.0f, -2.0f, 0.0f));
		right_lower_leg->get_transform().set_rotation(mymath::Vec3(0.0f, 0.0f, 0.0f));
		right_lower_leg->get_transform().set_scale(mymath::Vec3(0.4f, 1.0f, 0.4f));


		left_upper_arm->add_child(std::move(left_lower_arm));
		right_upper_arm->add_child(std::move(right_lower_arm));

		left_upper_leg->add_child(std::move(left_lower_leg));
		right_upper_leg->add_child(std::move(right_lower_leg));

		
		_root->add_child(std::move(left_upper_arm));
		_root->add_child(std::move(right_upper_arm));
		_root->add_child(std::move(left_upper_leg));
		_root->add_child(std::move(right_upper_leg));
		
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
		// Texture 占쌩곤옙 占싹댐옙 占쌘듸옙

		_renderer->LoadNodes(_vulkanModel, *_root);
		_renderer->BeginCreateMesh(_vulkanModel);
		_renderer->InsertIndexBuffer(_vulkanModel);
		_renderer->EndCreateMesh(_vulkanModel);



		return _vulkanModel;

	}

	//void HumanGLObject::drawNode(Node<::HumanVertex>& node)
	//{
	//	if (_vulkanModel)
	//	{
	//		//_renderer->renderMeshObject(_vulkanModel, _transform.mat4(), _colorMode);
	//		//mymath::Mat4 _worldMatrix = node.get_transform().get_world_matrix();
	//		mymath::Mat4 empty{ 1.0f };
	//		mymath::Mat4 _worldMatrix = empty;
	//		
	//		_renderer->renderMeshObject<::HumanVertex>(_vulkanModel, _worldMatrix, 0);
	//	}

	//	for (const auto& child : node.get_children())
	//	{
	//		std::cout << child.get() << std::endl;
	//		drawNode(*child);
	//	}
	//}

	size_t HumanGLObject::fillVertexData(Node<::HumanVertex>& node, size_t partsID)
	{
		std::vector<HumanVertex> vertices;
		std::vector<uint32_t> indices;
		createCubeData(vertices, indices, partsID, node.get_name());

		node.set_vertices(vertices);
		node.set_indices(indices);

		size_t currentPartsID = partsID; // 현재 노드의 partsID를 저장

		for (const auto& child : node.get_children())
		{
			std::cout << child.get() << std::endl;
			currentPartsID = fillVertexData(*child, currentPartsID + 1); // 자식 노드에 대해 partsID를 증가시키고 재귀 호출
		}

		return currentPartsID; // 최종 partsID를 반환
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
		// 	humanObjects 의 LateUpdate() 호출;
		//	humanObjects의 FinalUpdate() 호출; run 에서 호출 가능
		
		//LateUpdate();
		//FixedUpdate();
	}


	void HumanGLObject::Render()
	{
		//if (_root)
		//	drawNode(*_root);

		if (_vulkanModel)
		{
			//_renderer->renderMeshObject(_vulkanModel, _transform.mat4(), _colorMode);
			mymath::Mat4 scaleMatrix(1.0f);
			mymath::Mat4 ViewMat = GetCamera()->get_ViewMat();
			//mymath::Mat4 ViewMat(1.0f);
			//mymath::Mat4 ProjMat(1.0f);
			mymath::Mat4 ProjMat = GetCamera()->get_ProjMat();
			//_renderer->renderMeshObject(_vulkanModel, scaleMatrix, 0);
			_renderer->renderMeshObject(_vulkanModel, scaleMatrix, ViewMat, ProjMat, 0);
		}
	}



	mymath::Vec3 HumanGLObject::pickColor(const std::string& name)
	{
		if (name == "head") {
			return mymath::Vec3(255 / 255.0f, 218 / 255.0f, 185 / 255.0f);
		}
		else if (name == "body") {
			return mymath::Vec3(139 / 255.0f, 0 / 255.0f, 0 / 255.0f);
		}
		else if (name == "left_upper_arm" || name == "right_upper_arm") {
			return mymath::Vec3(240 / 255.0f, 128 / 255.0f, 128 / 255.0f);
		}
		else if (name == "left_lower_arm" || name == "right_lower_arm") {
			return mymath::Vec3(255 / 255.0f, 218 / 255.0f, 185 / 255.0f);
		}
		else if (name == "left_upper_leg" || name == "right_upper_leg") {
			return mymath::Vec3(128 / 255.0f, 0 / 255.0f, 128 / 255.0f);
		}
		else if (name == "left_lower_leg" || name == "right_lower_leg") {
			return mymath::Vec3(0 / 255.0f, 128 / 255.0f, 0 / 255.0f);
		}
		else if (name == "custom") {
			return mymath::Vec3(225 / 255.0f, 228 / 255.0f, 196 / 255.0f);
		}
		
		else {
			throw std::invalid_argument("Unknown color name");
		}
	}


	void HumanGLObject::createCubeData(std::vector<HumanVertex>& vertices, std::vector<uint32_t>& indices, uint32_t bodyPartID, const std::string& name)
	{
		mymath::Vec3 color =  pickColor(name);


		vertices = {
				{ { -1.0f, -1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f, -1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f,  1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f,  1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
											 		 
				{ {  1.0f,  1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f,  1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f, -1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f, -1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
											 		 
				{ { -1.0f, -1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f, -1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f,  1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f,  1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
													  
				{ { -1.0f, -1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f, -1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f,  1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f,  1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
													  
				{ {  1.0f,  1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f,  1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f,  1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f,  1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
											 		  
				{ { -1.0f, -1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f, -1.0f, -1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ {  1.0f, -1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
				{ { -1.0f, -1.0f,  1.0f }, { color._x, color._y, color._z }, bodyPartID },
		};

		indices = {
			0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23
		};
	}

	void createCubeData(std::vector<HumanVertex>& vertices, std::vector<uint32_t>& indices)
	{
		vertices = {
			{ { -1.0f, -1.0f,  1.0f }, {.9f, .9f, .9f}},
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