#include "../Includes/Input.hpp"

void Input::Init(GLFWwindow* window)
{
	_Window = window;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void Input::Update()
{
    if (!glfwGetWindowAttrib(_Window, GLFW_FOCUSED))
    {
        for (uint32_t key = 0; key < KEY_TYPE_COUNT; key++)
            _states[key] = KEY_STATE::NONE;

        return;
    }

    for (uint32_t key = 0; key < KEY_TYPE_COUNT; key++)
    {
        // Ű�� ���� ������ true
        if (glfwGetKey(_Window, key) == GLFW_PRESS)
        {
            KEY_STATE& state = _states[key];

            // ���� �����ӿ� Ű�� ���� ���¶�� PRESS
            if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
                state = KEY_STATE::PRESS;
            else
                state = KEY_STATE::DOWN;
        }
        else
        {
            KEY_STATE& state = _states[key];

            // ���� �����ӿ� Ű�� ���� ���¶�� UP
            if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
                state = KEY_STATE::UP;
            else
                state = KEY_STATE::NONE;
        }
    }
}
