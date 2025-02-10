#pragma once
#include "Component.hpp"

namespace vks
{
	class VulkanRenderer;
}


class MonoBehaviour : public Component
{
public:
	MonoBehaviour(vks::VulkanRenderer* renderer);
	virtual ~MonoBehaviour();


private:
	virtual void FinalUpdate() sealed { }
};
	

