#pragma once

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "Event.h"

struct MouseWheelEventArg
{
	double value;
};

struct MouseButtonEventArg
{
	int button;
	int action;
};

struct MouseEventArg
{
	short x;
	short y;
};

struct KeyEventArg
{
	int key;
	int action;
};

class OpenGL
{
	bool m_render_flag;

	int m_window_width;
	int m_window_height;

	int m_framebuffer_width;
	int m_framebuffer_height;

	GLFWwindow* m_window;

	double m_old_time = 0;

	Event<OpenGL*, const MouseEventArg&> m_mouseMoveEvent;
	Event<OpenGL*, const MouseWheelEventArg&> m_wheelEvent;
	Event<OpenGL*, const MouseButtonEventArg&> m_mouseButtonEvent;
	Event<OpenGL*, const KeyEventArg&> m_keyEvent;

	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void PreRender();
	void resize();

public:
	auto& mouseMoveEvent() { return m_mouseMoveEvent; }
	auto& wheelEvent() { return m_wheelEvent; }
	auto& mouseButtonEvent (){ return m_mouseButtonEvent; }
	auto& keyEvent() { return m_keyEvent; }
	GLFWwindow* getWindow() { return m_window; }

	void initWindow();

	int window_height(){ return m_window_height; }
	int window_width() { return m_window_width; }

	int fb_height() { return m_framebuffer_height; }
	int fb_width() { return m_framebuffer_width; }

	bool isKeyPressed(int key)
	{
		if (key == GLFW_MOUSE_BUTTON_1 ||
			key == GLFW_MOUSE_BUTTON_2 ||
			key == GLFW_MOUSE_BUTTON_3 ||
			key == GLFW_MOUSE_BUTTON_4 ||
			key == GLFW_MOUSE_BUTTON_5 ||
			key == GLFW_MOUSE_BUTTON_6 ||
			key == GLFW_MOUSE_BUTTON_7 ||
			key == GLFW_MOUSE_BUTTON_8)
			return glfwGetMouseButton(m_window, key) == GLFW_PRESS;
		else
			return glfwGetKey(m_window, key) == GLFW_PRESS;
	}

	void Render();
};