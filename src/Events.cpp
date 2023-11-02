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

void ProcessKeyEvents(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static float speed = 0.1f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_camera.Move(glm::vec3(0, 0, speed));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_camera.Move(glm::vec3(0, 0, -speed));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_camera.Move(glm::vec3(-speed, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_camera.Move(glm::vec3(speed, 0, 0));
}
