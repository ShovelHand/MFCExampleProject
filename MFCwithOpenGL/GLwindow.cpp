#pragma once

#include "stdafx.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLwindow.h"


MyWindow::MyWindow() {
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	m_window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);

	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(m_window);

	//Sets the key callback  
	glfwSetKeyCallback(m_window, key_callback);

	//Initialize GLEW  
	GLenum err = glewInit();

	//If GLEW hasn't initialized  
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	//	return -1;
	}

	//Set a background color  
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	//Main Loop  

	return;

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

MyWindow::~MyWindow() {

}

void MyWindow::Update()
{
	
}

void MyWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void MyWindow::error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}