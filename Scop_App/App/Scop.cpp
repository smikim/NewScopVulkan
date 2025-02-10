#include "Scop.hpp"

#include "../Includes/KeyboardMovementController.hpp"
#include "GameObject.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Camera.hpp"
#include "../Component/CameraScript.hpp"

namespace scop
{
	Scop::Scop()
	{
		_input.Init(_window.getGLFWwindow());
	}

	Scop::~Scop()
	{
		//deleteScopObjects();
		deleteHumanObjects();
	}

	void Scop::run()
	{
		KeyboardMovementController keyboardController{};

		// Camera
		_Camera = std::make_shared<GameObject>();
		_Camera->AddComponent(std::make_shared<Transform>(&_renderer));
		_Camera->AddComponent(std::make_shared<Camera>(&_renderer)); // Near=1, Far=1000, FOV=45도
		_Camera->AddComponent(std::make_shared<CameraScript>(&_renderer));
		_Camera->GetTransform()->SetLocalPosition(mymath::Vec3(0.f, 0.0f, -14.0f));

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!_window.shouldClose()) {
			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime =
				std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = std::min(frameTime, (float)MAX_FRAME_TIME);

			glfwPollEvents();
			keyboardController.moveObjects(_window.getGLFWwindow(), frameTime, ScopObjects);
			keyboardController.switchColoring(_window.getGLFWwindow(), ScopObjects);

			_input.Update();
			_Camera->LateUpdate(_input, frameTime);
			_Camera->FixedUpdate();

			update();
			render();
		}

		vkDeviceWaitIdle(_renderer.getVulkanDevice()->getLogicalDevice());
	}

	bool Scop::prepare(std::string& ObjFilename, std::string& BmpFilename)
	{
		try {
			_renderer.initVulkan();
			//createScopObject(ObjFilename, BmpFilename);
			createHumanObject();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
		return true;
	}

	void Scop::update()
	{
		

		

		{
			if (_input.GetButton(KEY_TYPE::W))
				std::cout << "W" << std::endl;
			if (_input.GetButton(KEY_TYPE::S))
				std::cout << "S" << std::endl;
			if (_input.GetButton(KEY_TYPE::A))
				std::cout << "A" << std::endl;
			if (_input.GetButton(KEY_TYPE::D))
				std::cout << "D" << std::endl;
		}	
		
		for (size_t i = 0; i < ScopObjects.size(); i++)
		{
			ScopObjects[i]->Run();
		}

		// 	humanObjects 의 LateUpdate() 호출;
		//	humanObjects의 FinalUpdate() 호출; run 에서 호출 가능
		
		for (size_t i = 0; i < humanObjects.size(); i++)
		{
			humanObjects[i]->Run();
		}
	}

	void Scop::render()
	{

		VkResult result = _renderer.beginRender();
		if (result != VK_SUCCESS) return;
		_renderer.beginRenderPass();
		/*for (size_t i = 0; i < ScopObjects.size(); i++)
		{
			ScopObjects[i]->Render();
		}*/

		for (size_t i = 0; i < humanObjects.size(); i++)
		{
			humanObjects[i]->Render();
		}

		_renderer.endRenderPass();
		result = _renderer.endRender();
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			return ;
	}

	ScopObject* Scop::createScopObject(std::string& ObjFilename, std::string& BmpFilename)
	{
		ScopObject* obj = new ScopObject;
		
		if (!obj->Initialize(this, ObjFilename, BmpFilename))
		{
			delete obj;
			throw std::runtime_error("Failed to initialize ScopObject with filename: " + ObjFilename);
		}
		
		ScopObjects.push_back(obj);
		obj->_transform.translation = { .0f, .0f, 0.0f };
		//obj->_transform.scale = { 0.5f, 0.5f, 0.5f };
		//obj->setTranslation(0.f, 2.f, 0.f);
		//obj->setScale(0.5f, 0.5f, 0.5f);
		//obj->setRotation(0.0f, 180.f, 0.0f);
		
		return obj;
	}

	std::shared_ptr<humanGL::HumanGLObject> Scop::createHumanObject()
	{
		auto obj = std::make_shared<humanGL::HumanGLObject>();

		if (!obj->Initialize(this))
		{
			throw std::runtime_error("Failed to initialize ScopObject");
		}

		obj->AddComponent(std::make_shared<Transform>(&_renderer));
		std::shared_ptr<Transform> transform = obj->GetTransform();

		humanObjects.push_back(obj);

		return obj;
	}

	void Scop::deleteScopObjects()
	{
		for (ScopObject* obj : ScopObjects) {
			delete obj;
		}
		ScopObjects.clear();
	}

	void Scop::deleteHumanObjects()
	{
		humanObjects.clear();
	}
	

}
