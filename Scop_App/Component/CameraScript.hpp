#pragma once

#include "MonoBehaviour.hpp"

namespace vks
{
	class VulkanRenderer;
}

class Input;

class CameraScript : public MonoBehaviour
{
public:
	CameraScript(vks::VulkanRenderer* renderer);
	virtual ~CameraScript();
	
	virtual void LateUpdate(Input& input, float deltaTime) override;

private:
	float		_speed = 100.f;
};
