#pragma once

#include "../Window/GlfwWindow.hpp"
#include <vector>

enum class KEY_TYPE
{
	UP = GLFW_KEY_UP,
	DOWN = GLFW_KEY_DOWN,
	LEFT = GLFW_KEY_LEFT,
	RIGHT = GLFW_KEY_RIGHT,

	W = GLFW_KEY_W,
	A = GLFW_KEY_A,
	S = GLFW_KEY_S,
	D = GLFW_KEY_D,

	Q = GLFW_KEY_Q,
	E = GLFW_KEY_E
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32_t>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<int32_t>(KEY_STATE::END),
};

class Input
{

public:
	void Init(GLFWwindow* window);
	void Update();

	// 누르고 있을 때
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// 맨 처음 눌렀을 때
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// 맨 처음 눌렀다 뗐을 때
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8_t>(key)]; }

private:
	GLFWwindow*	_Window;
	std::vector<KEY_STATE> _states;
};

