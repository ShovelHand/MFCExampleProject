#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLwindow.h"


class MyWindow {

public:
	MyWindow();
	~MyWindow();

	GLFWwindow* m_window;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void error_callback(int error, const char* description);
	void SetShouldClose(bool b) { glfwSetWindowShouldClose(m_window, 1); }
//	void Update();
	bool ShouldClose(){ return glfwWindowShouldClose(m_window); }

};