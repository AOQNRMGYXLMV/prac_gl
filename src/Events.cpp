#include "Events.h"

void MouseMoveCallback(GLFWwindow* window, double x, double y) {
	static bool first_call = true;
	static double last_x;
	static double last_y;

	if (first_call) {
		last_x = x;
		last_y = y;
		first_call = false;
		return;
	}

	g_camera.ProcessMouseMove(x - last_x, y - last_y);
}

void KeyCallback(
	GLFWwindow* window,
	int key,
	[[maybe_unused]] int scancode,
	int action,
	[[maybe_unused]] int mods) {
	static float speed = 0.1f;
	auto pos = g_camera.GetPosition();
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_W:
			pos.z += speed;
			break;
		case GLFW_KEY_S:
			pos.z -= speed;
			break;
		case GLFW_KEY_A:
			pos.x -= speed;
			break;
		case GLFW_KEY_D:
			pos.x += speed;
			break;
		default:
			break;
		}
		g_camera.SetPosition(pos);
	}
}
