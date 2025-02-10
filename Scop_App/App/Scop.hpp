#pragma once

#include "vulkan/vulkan.h"
#include "../Includes/VulkanRenderer.hpp"
#include "../Includes/Input.hpp"
#include "ScopObject.hpp"
#include "HumanGLObject.hpp"

class GlfwWindow;

#define MAX_FRAME_TIME 60

namespace humanGL
{
	class HumanGLObject;
}

class GameObject;

namespace scop
{
	class Scop
	{
	public:
		Scop();
		~Scop();

		void run();
		bool prepare(std::string& ObjFilename, std::string& BmpFilename);
		void update();
		void render();
		ScopObject* createScopObject(std::string& ObjFilename, std::string& BmpFilename);
		std::shared_ptr<humanGL::HumanGLObject> createHumanObject();

	private:
		
		uint32_t _width = 1280;
		uint32_t _height = 720;
		std::string _windowName = "Scop";
		
		GlfwWindow _window{ _width, _height, _windowName };
		Input	_input;

		vks::VulkanRenderer _renderer{ _window };
		std::vector<ScopObject*> ScopObjects;
		//std::vector<humanGL::HumanGLObject*> humanObjects;
		std::vector<std::shared_ptr<humanGL::HumanGLObject>> humanObjects;
		// Camera
		std::shared_ptr<GameObject> _Camera;

		void deleteScopObjects();
		void deleteHumanObjects();
	public: 
		vks::VulkanRenderer* getVulkanRenderer() { return &_renderer; }
	};

}


