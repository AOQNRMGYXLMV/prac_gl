#include "Events.h"

bool g_mouse_move_clear_flag = true;

// 
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		g_mouse_move_clear_flag = true;
	}
}

void MouseMoveCallback(GLFWwindow* window, double x, double y) {
	static double last_x;
	static double last_y;

	auto state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state == GLFW_PRESS) {
		if (g_mouse_move_clear_flag) {
			last_x = x;
			last_y = y;
			g_mouse_move_clear_flag = false;
			return;
		}

		g_camera.Rotate(glm::vec3(0, 1.0, 0), (x - last_x) * 0.3);
		g_camera.Rotate(glm::vec3(1.0, 0, 0), (y - last_y) * 0.3);
		last_x = x;
		last_y = y;
	}
}

void ProcessKeyEvents(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static float speed = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_camera.Move(glm::vec3(0, 0, speed));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_camera.Move(glm::vec3(0, 0, -speed));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_camera.Move(glm::vec3(-speed, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_camera.Move(glm::vec3(speed, 0, 0));

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		g_camera.Rotate(glm::vec3(0, 0, 1.0), speed * 3.0);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		g_camera.Rotate(glm::vec3(0, 0, 1.0), -speed * 3.0);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		g_camera.Move(glm::vec3(0, 0.5, 0));
}
