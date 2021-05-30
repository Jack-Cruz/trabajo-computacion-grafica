#ifndef __GLUTIL_H__
#define __GLUTIL_H__

#include "typedef.h"

GLFWwindow* glutilInit(i32 major, i32 minor,
		i32 width, i32 height,
		const char* title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW Window\n";
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Not GLAD at all!\n";
		return nullptr;
	}

	return window;
}
#endif

void util() {
	// generacion de terreno Oscar
	/*
	i32 n = 10;
	vector<vec3> positions(n * n);

	u32 funcion = 0, guardado = 0;
	std::vector<u32> lastposition(n);
	u32 k;
	for (u32 i = 0; i < n; ++i) {
		for (u32 j = 0; j < n; ++j) {
			if (i > 0) {
				if (lastposition.at(j) >= guardado) {
					if (lastposition.at(j) >= guardado + 1.2)guardado = lastposition.at(j);
				}
				else if (lastposition.at(j) <= guardado) {
					if (lastposition.at(j) + 1.2 <= guardado)guardado = lastposition.at(j);
				}
			}
			u32 aleatorio = rand() % 2;
			if (aleatorio == 0) {
				k = -1;
			}
			else k = 1;

			funcion = guardado + k * (sin(j) + sin(j * 2) / 1.5 + sin(j * 4) / 2 + sin(j * 20) / 10);
			positions[i * n + j] = glm::vec3((f32)(i - n / 2.0f), funcion, (f32)(j - n / 2.0f));
			guardado = funcion;
			lastposition.at(j) = funcion;
		}
	}
	*/
	/* generacion de terreno Luis
	
	auto rndb = [](f32 a, f32 b) -> f32 {
		f32 d = b - a;
		return (rand() % 1000) / 1000.0f * d + a;
	};

	i32 n = 10;
	vector<vec3> positions(n * n);
	for (u32 i = 0; i < positions.size(); ++i) {
		positions[i] = vec3(rndb(-3.0f, 3.0f), rndb(-2.0f, 2.0f), rndb(-10.0f, 0.0f));
	}
	*/
}


/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

