#pragma once

#include <GLFW/glfw3.h>

#include "Camera.h"
extern Camera g_camera;

void MouseMoveCallback(GLFWwindow* window, double x, double y);

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
