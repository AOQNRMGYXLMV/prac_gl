#include <iostream>
#include <array>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Events.h"

unsigned int VAO, VBO;

Camera g_camera;

void PrepareTriangle() {
	std::array<float, 36> vertices {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.0f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	// config VAO, VBO
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main() {
	spdlog::set_level(spdlog::level::debug);

	if (!glfwInit()) {
		spdlog::error("Init glfw failed.");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	auto window = glfwCreateWindow(800, 600, "Practicle OpenGL", nullptr, nullptr);

	glfwSetCursorPosCallback(window, MouseMoveCallback);
	glfwSetKeyCallback(window, KeyCallback);

	if (!window) {
		spdlog::error("GLFW create window failed.");
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		spdlog::error("GLAD init failed.");
	}

	Shader shader;
	shader.Init();
	shader.LoadFromFile("triangle.vert", "triangle.frag");

	// Set camera params
	g_camera.SetCamera({ 0.0, 0.0, -1.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 });
	g_camera.SetPerspective(60.0, 8.0 / 6.0, 0.1, 100.0);

	shader.Use();
	shader.SetUniformMatrix4fv("projection", g_camera.GetProjectionMatrix());

	PrepareTriangle();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		shader.SetUniformMatrix4fv("view", g_camera.GetViewMatrix());
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
