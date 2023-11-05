#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

extern Camera g_camera;
extern bool g_mouse_move_clear_flag;

void MouseMoveCallback(GLFWwindow* window, double x, double y);

void ProcessKeyEvents(GLFWwindow* window);

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
