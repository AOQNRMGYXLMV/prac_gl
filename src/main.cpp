#include <iostream>
#include <array>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Shader.h"
#include "Camera.h"
#include "Events.h"
#include "Mesh.h"

unsigned int VAO, VBO;
Shader g_shader;
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

void WindowResizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	g_camera.UpdateAspectRatio(static_cast<double>(width) / height);
	g_shader.SetUniformMatrix4fv("projection", g_camera.GetProjectionMatrix());
}

void InitImGui(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
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
	auto window = glfwCreateWindow(1200, 900, "Practicle OpenGL", nullptr, nullptr);

	glfwSetCursorPosCallback(window, MouseMoveCallback);
	glfwSetWindowSizeCallback(window, WindowResizeCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	if (!window) {
		spdlog::error("GLFW create window failed.");
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		spdlog::error("GLAD init failed.");
	}

	InitImGui(window);

	Mesh mesh;
	mesh.LoadFromFile("../../assets/3d_material_ball.glb");

	g_shader.Init();
	g_shader.LoadFromFile("triangle.vert", "triangle.frag");

	// Set camera params
	glm::vec3 cam_pos(24.397, -83.063, -73.5177);
	glm::vec3 up(0.055583, 0.256135, -0.965038);
	glm::vec3 lookat(24.553335, -82.105133, -73.276779);
	g_camera.SetCamera(cam_pos, lookat, up);
	g_camera.SetPerspective(60.0, 8.0 / 6.0, 0.1, 1000.0);

	g_shader.Use();
	g_shader.SetUniformMatrix4fv("projection", g_camera.GetProjectionMatrix());

	// …Ë÷√π‚’’
	g_shader.SetUniform3fv("light_dir", glm::normalize(glm::vec3(1.0f, 1.0f, 0)));
	g_shader.SetUniform3fv("light_color", glm::vec3(1.0, 1.0, 1.0));
	g_shader.SetUniform3fv("camera_pos", cam_pos);

	PrepareTriangle();
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ProcessKeyEvents(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_shader.Use();
		g_shader.SetUniformMatrix4fv("view", g_camera.GetViewMatrix());
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		mesh.Draw();

		ImGui::Text("Hello, world %d", 123);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
