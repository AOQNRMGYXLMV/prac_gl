#include "Events.h"

void MouseMoveCallback(GLFWwindow* window, double x, double y) {
	static bool first_call = true;
	static double last_x;
	static double last_y;

	auto state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state == GLFW_PRESS) {
		if (first_call) {
			last_x = x;
			last_y = y;
			first_call = false;
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
		g_camera.Rotate(glm::vec3(0, 0, 1.0), speed * 5.0);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		g_camera.Rotate(glm::vec3(0, 0, 1.0), -speed * 5.0);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		g_camera.Move(glm::vec3(0, 0.5, 0));
}
