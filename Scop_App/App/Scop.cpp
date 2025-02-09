#include "Scop.hpp"
#include "../Includes/KeyboardMovementController.hpp"

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
		_input.Update();

		if (_input.GetButton(KEY_TYPE::W))
			std::cout << "W" << std::endl;
		if (_input.GetButton(KEY_TYPE::S))
			std::cout << "S" << std::endl;
		if (_input.GetButton(KEY_TYPE::A))
			std::cout << "A" << std::endl;
		if (_input.GetButton(KEY_TYPE::D))	
			std::cout << "D" << std::endl;

		for (size_t i = 0; i < ScopObjects.size(); i++)
		{
			ScopObjects[i]->Run();
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

	humanGL::HumanGLObject* Scop::createHumanObject()
	{
		humanGL::HumanGLObject* obj = new humanGL::HumanGLObject;

		if (!obj->Initialize(this))
		{
			delete obj;
			throw std::runtime_error("Failed to initialize ScopObject");
		}

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
		for (humanGL::HumanGLObject* obj : humanObjects) {
			delete obj;
		}
		humanObjects.clear();
	}
	

}
