#include <iostream>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "ShaderClass.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

#define PIXEL_SIZE 20

float Normalize(float input, float min, float max)
{
	return (input - min) / (max - min);
}

void DrawPixel(int posX, int posY)
{
	GLfloat vertices[] = {
		Normalize(posX, 0, 400), Normalize(posY, 0, 400), 0
	};

	//Drawing The initial coordinate
	VertexArrayObject VAO;
	VAO.Bind();
	VertexBufferObject VBO(vertices, sizeof(vertices));
	VAO.LinkVBO(VBO, 0);
	VAO.Unbind();
	VBO.Unbind();

	VAO.Bind();
	glPointSize(PIXEL_SIZE);
	glDrawArrays(GL_POINTS, 0, 1);

	VAO.Unbind();
	VAO.Delete();
	VBO.Delete();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void BLDDisplayLine(float x1, float y1, float x2, float y2)
{
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = PIXEL_SIZE;
	if (x2 < x1) incx = -PIXEL_SIZE;
	incy = PIXEL_SIZE;
	if (y2 < y1) incy = -PIXEL_SIZE;
	x = x1; y = y1;
	if (dx > dy)
	{
		DrawPixel(x, y);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx / PIXEL_SIZE; i++)
		{
			if (e >= 0)
			{
				y += incy;
				e += inc1;
			}
			else
				e += inc2;
			x += incx;
			DrawPixel(x, y);
		}
	}
	else {
		DrawPixel(x, y);
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy / PIXEL_SIZE; i++)
		{
			if (e >= 0)
			{
				x += incx;
				e += inc1;
			}
			else
				e += inc2;
			y += incy;
			DrawPixel(x, y);
		}
	}

}

void DDADisplayLine(float x1, float y1, float x2, float y2)
{
	float dy, dx, step, x, y, k, Xin, Yin;
	dx = x2 - x1;
	dy = y2 - y1;
	if (abs(dx) > abs(dy))
		step = abs(dx) / PIXEL_SIZE;
	else
		step = abs(dy) / PIXEL_SIZE;
	Xin = (dx) / step;
	Yin = dy / step;
	x = x1;
	y = y1;

	DrawPixel(x, y);


	for (k = 1; k <= step; k++)
	{
		x += Xin;
		y += Yin;
		DrawPixel(x, y);
	}
}

void MPLDrawLine(float x1, float y1, float x2, float y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dy <= dx) {
		//Decision Parameters
		int d = dy - (dx / 2);
		int x = x1, y = y1;

		// Plot initial given point
		DrawPixel(x, y);

		// Iterate over value of X
		while (x < x2)
		{
			x += PIXEL_SIZE;

			// E or East is chosen
			if (d < 0)
				d = d + dy;

			// NE or North East is chosen
			else
			{
				d += (dy - dx);
				y += PIXEL_SIZE;
			}

			// Plot intermediate points
			DrawPixel(x, y);
		}
	}

	else if (dx < dy)
	{
		// initial value of decision parameter d
		int d = dx - (dy / 2);
		int x = x1, y = y1;

		// Plot initial given point
		DrawPixel(x, y);

		// iterate through value of X
		while (y < y2)
		{
			y += PIXEL_SIZE;

			// E or East is chosen
			if (d < 0)
				d = d + dx;

			// NE or North East is chosen
			// NE or North East is chosen
			else
			{
				d += (dx - dy);
				x += PIXEL_SIZE;
			}

			// Plot intermediate points
			DrawPixel(x, y);
		}
	}
}


int main()
{
	//Initialized the window
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Get Display Resolutions
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* screen = glfwGetVideoMode(primaryMonitor);
	std::cout << screen->width << ", " << screen->height << std::endl;

	GLFWwindow* window = glfwCreateWindow(800, 800, "EndlessRunner", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed  to create" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OPENGL
	gladLoadGL();
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("./Shaders/Vertex.GLSL", "./Shaders/Fragment.GLSL");

	double mouseX = 0, mouseY = 0;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();

		glfwGetCursorPos(window, &mouseX, &mouseY);
		//DDADisplayLine(0, 0, mouseX - 400, -1 * (mouseY - 400));
		BLDDisplayLine(0, 0, mouseX - 400, -1 * (mouseY - 400));
		//MPLDrawLine(0, 0, mouseX - 400, -1 * (mouseY - 400));
		glfwSwapBuffers(window);
		//Take Care of all glfw Events
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
