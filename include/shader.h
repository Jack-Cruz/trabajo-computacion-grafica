#pragma once
#ifndef __SHADER__
#define __SHADER__

#include "typedef.h"

class Shader {
private:
	u32 program;
	Path* path;

	i32 ok;            // check for error status 
	char infoLog[512]; // get error status

public:
	Shader(std::string vertexFileName = "shader.vert",
		std::string fragmentFileName = "shader.frag",
		std::string shadersPath = "bin")
		: path(new Path(shadersPath, "resources/textures")){
		std::ifstream vertexFile(path->sp(vertexFileName));
		std::string vertexSrc;
		std::getline(vertexFile, vertexSrc, '\0');

		std::ifstream fragmentFile(path->sp(fragmentFileName));
		std::string fragmentSrc;
		std::getline(fragmentFile, fragmentSrc, '\0');

		ui32 vertex = mkShader(vertexSrc.c_str(), GL_VERTEX_SHADER);
		ui32 fragment = mkShader(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &ok);
		if (!ok) {
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cout << "Error::shader::program::link_failed\n"
				<< infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	~Shader() {
		glDeleteProgram(program);
		glfwTerminate();
	}

	void useProgram() {
		glUseProgram(program);
	}
	ui32 getProgram() { // might need to refactor this later ughhh
		return program;
	}

	// Set uniforms
	void setI32(const char* name, const int& value) const {
		glUniform1i(glGetUniformLocation(program, name), value);
	}
	void setMat4(const char* name, const glm::mat4& value) const {
		glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, &value[0][0]);
	}
	void setVec3(const char* name, const vec3& value) const {
		glUniform3fv(glGetUniformLocation(program, name), 1, &value[0]);
	}
	void setVec3(const char* name, f32 a, f32 b, f32 c) const {
		glUniform3f(glGetUniformLocation(program, name), a, b, c);
	}
	
private:
	ui32 mkShader(const char* source, GLenum type) {
		ui32 shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
		if (!ok) {
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Error::shader::compilation_failed\n of type"<<GLenum(type) <<endl
				<< infoLog << std::endl;
			return 0;
		}
		return shader;
	}
};

#endif

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

