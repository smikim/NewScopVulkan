#include "../Includes/KeyboardMovementController.hpp"
#include "../App/ScopObject.hpp"
#include "../Library/Math/math.hpp"

namespace scop
{
	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject)
	{
		mymath::Vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate._y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate._y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate._x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate._x -= 1.f;

		if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon()) {
			gameObject._transform.rotation += lookSpeed * dt * rotate.normalize();
		}


		// limit pitch values between about +/- 85ish degrees
		gameObject._transform.rotation._x = mymath::clamp(gameObject._transform.rotation._x, -1.5f, 1.5f);
		gameObject._transform.rotation._y = mymath::mod(gameObject._transform.rotation._y, mymath::two_pi<float>());

		float yaw = gameObject._transform.rotation._y;
		const mymath::Vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const mymath::Vec3 rightDir{ forwardDir._z, 0.f, -forwardDir._x };
		const mymath::Vec3 upDir{ 0.f, -1.f, 0.f };

		mymath::Vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;
		
		if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon()) {
			gameObject._transform.translation += moveSpeed * dt * moveDir.normalize();
		}
	}

	void KeyboardMovementController::myMoveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject)
	{
		mymath::Vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate._y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate._y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate._x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate._x -= 1.f;

		if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon()) {
			mymath::Vec3 offset = lookSpeed * dt * rotate.normalize();
			gameObject.moveRotation(offset._x, offset._y, offset._z);
		}


		// limit pitch values between about +/- 85ish degrees
		gameObject._transform.rotation._x = mymath::clamp(gameObject._transform.rotation._x, -1.5f, 1.5f);
		gameObject._transform.rotation._y = mymath::mod(gameObject._transform.rotation._y, mymath::two_pi<float>());

		float yaw = gameObject._transform.rotation._y;
		const mymath::Vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const mymath::Vec3 rightDir{ forwardDir._z, 0.f, -forwardDir._x };
		const mymath::Vec3 upDir{ 0.f, -1.f, 0.f };

		mymath::Vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

		if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon()) {
			mymath::Vec3 offset = moveSpeed * dt * moveDir.normalize();
			gameObject.moveTranslation(offset._x, offset._y, offset._z);
		}
	}

	void KeyboardMovementController::moveObjects(GLFWwindow* window, float dt, std::vector<ScopObject*>& gameObjects)
	{
		for (size_t i = 0; i < gameObjects.size(); ++i) {

			auto obj = gameObjects[i];

			if (glfwGetKey(window, moves.spinOnX) == GLFW_PRESS && obj->_transform.spinOnX == false)
			{
				obj->_transform.spinAtX = !obj->_transform.spinAtX;
				obj->_transform.spinOnX = true;
			}
			if (glfwGetKey(window, moves.spinOnX) == GLFW_RELEASE)
				obj->_transform.spinOnX = false;

			if (glfwGetKey(window, moves.spinOnY) == GLFW_PRESS && obj->_transform.spinOnY == false)
			{
				obj->_transform.spinAtY = !obj->_transform.spinAtY;
				obj->_transform.spinOnY = true;
			}
			if (glfwGetKey(window, moves.spinOnY) == GLFW_RELEASE)
				obj->_transform.spinOnY = false;

			if (glfwGetKey(window, moves.spinOnZ) == GLFW_PRESS && obj->_transform.spinOnZ == false)
			{
				obj->_transform.spinAtZ = !obj->_transform.spinAtZ;
				obj->_transform.spinOnZ = true;
			}
			if (glfwGetKey(window, moves.spinOnZ) == GLFW_RELEASE)
				obj->_transform.spinOnZ = false;

			if (glfwGetKey(window, moves.resetAll) == GLFW_PRESS)
			{
				obj->_transform.spinAtX = false;
				obj->_transform.spinAtY = false;
				obj->_transform.spinAtZ = false;
				obj->setRotation(0.0f, 0.0f, 0.0f);
				obj->setTranslation(0.0f, 0.0f, 0.0f);
				obj->resetRTMatrix();

				return;
			}

			mymath::Vec3 rotate{ 0 };
			if (obj->_transform.spinAtX)
				rotate._x += 0.1f;
			if (obj->_transform.spinAtY)
				rotate._y += 0.1f;
			if (obj->_transform.spinAtZ)
				rotate._z += 0.1f;

			if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon()) {
				
				//obj->_transform.rotation += moveSpeed * dt * rotate.normalize();
				
				///////////////////////////////////////////////////////////////////

				mymath::Vec3 offset = moveSpeed * dt * rotate.normalize();			
				obj->moveRotation(offset._x, offset._y, offset._z);
				
			}


			const mymath::Vec3 forwardDir{ 0.f, 0.f, -1.f };
			mymath::Vec3 upDir{ 0.f, -1.f, 0.f };

			const mymath::Vec3 rightDir = upDir.cross(forwardDir).normalize();		
			//const mymath::Vec3 rightDir{ 1.0f, 0.0f, 0.0f };
			upDir = forwardDir.cross(rightDir).normalize();
			//upDir = rightDir.cross(forwardDir).normalize();

			mymath::Vec3 moveDir{ 0.f };
			if (glfwGetKey(window, moves.moveUpInZ) == GLFW_PRESS) {
				//std::cout << forwardDir._x << forwardDir._y << forwardDir._z << std::endl;
				moveDir += forwardDir;
				//std::cout << moveDir._x << moveDir._y << moveDir._z << std::endl;
			}

			if (glfwGetKey(window, moves.moveDownInZ) == GLFW_PRESS) moveDir -= forwardDir;
			if (glfwGetKey(window, moves.moveRightInX) == GLFW_PRESS) moveDir += rightDir;
			if (glfwGetKey(window, moves.moveLeftInX) == GLFW_PRESS) moveDir -= rightDir;
			if (glfwGetKey(window, moves.moveRightInY) == GLFW_PRESS) moveDir += upDir;
			if (glfwGetKey(window, moves.moveLeftInY) == GLFW_PRESS) moveDir -= upDir;

			if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon()) {
				//obj->_transform.translation += moveSpeed * dt * moveDir.normalize();
				//std::cout << obj->_transform.translation._x << obj->_transform.translation._y << obj->_transform.translation._z << std::endl;
			
				////////////////////////////////////////////////////////////////////////////
				mymath::Vec3 offset = moveSpeed * dt * moveDir.normalize();
				obj->moveTranslation(offset._x, offset._y, offset._z);
			}
		}
	}
	void KeyboardMovementController::switchColoring(GLFWwindow* window, std::vector<ScopObject*>& gameObjects)
	{

		static bool keyPressed = false;

    	if (glfwGetKey(window, coloring.switchKey) == GLFW_PRESS)
		{
			if (!keyPressed)
			{
				keyPressed = true;
				for (size_t i = 0; i < gameObjects.size(); ++i)
				{
					auto obj = gameObjects[i];
					obj->_colorMode = (obj->_colorMode + 1) % 4; // 0: original, 1: texture, 2: rainbow, 3: gray
				}
			}
		}
		else
		{
			keyPressed = false;
		}
		
	}
}