#include <glutil2.h>

#include <vector>
#include <stdlib.h> // aleatorios
#include <time.h> // aleatorios
#include <math.h>

#include <vao.h>
#include <vbo.h>
#include <ebo.h>
#include <shader.h>
#include <cube.h>
#include <texture.h>
#include <camera.h>

// settings
const i32 SCR_WIDTH = 960;
const i32 SCR_HEIGHT = 540;
const f32  ASPECT = 16.0f / 9.0f;

// camera
Cam cam(0.0f, 16.0f, 0.0f);
f32 lastx;
f32 lasty;
bool firstMouse = true;

// timing
f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

// light
vec3 lightPos(10.0f, 10.0f, 25.0f);

// input processing
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		deltaTime *= 10;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.processKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
		cam.processKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
		cam.processKeyboard(RIGTH, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) 
		cam.processKeyboard(UP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) 
		cam.processKeyboard(DOWN, deltaTime);

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastx = xpos;
		lasty = ypos;
		firstMouse = false;
	}
	f32 xoffset = xpos - lastx;
	f32 yoffset = lasty - ypos;

	cam.processMouse(xoffset, yoffset);
	lastx = xpos;
	lasty = ypos;
	
}
void scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
	cam.processScroll((f32)yoffset);
}

f32 matrix[101][101];

/***********************************************/
f32 noise(f32 precision = 100.0f) {
	return (rand() % (int)precision + 1.0f) / precision;
}
i32 rescale(f32 num, f32 min, f32 max) {
	return (int)((max - min) * (num) / ((float)(1.0f)) + min);
}
void bilinear_interpolation(u32& n, u32 offset) {
	int xNearest, zNearest;
	float nearest, farest;
	//interpolamos valores en horizontal
	for (int x = 0; x < n; ++x) {
		if (x % offset == 0) {
			for (int z = 0; z < n - 1; ++z) {
				if (z % offset == 0) {
					zNearest = z;
					nearest = matrix[x][z];
					farest = matrix[x][z + offset];
					continue;
				}
				float point = z - zNearest;
				matrix[x][z] =
					(nearest * (offset - point) / (float)offset) +
					(farest * (point) / (float)offset);
			}
		}
	}
	//interpolamos valores en vertical
	for (int z = 0; z < n; ++z) {
		if (true) {
			for (int x = 0; x < n-1; ++x) {
				if (x%offset==0) {
					xNearest = x;
					nearest = matrix[x][z];
					farest = matrix[x+offset][z];
					continue;
				}
				float point = x-xNearest;
				matrix[x][z] =
					(nearest*(offset-point)/(float)offset)+
					(farest*(point)/(float)offset);
			}
		}
	}
}
void generate_random(u32& n, i32 min, i32 max, u32 offset) {
	//Solo generamos n?meros randoms, cada cierto intervalo
	//Aquellos que no se encuentren en el intervalo, quedan vacios
	float precision = 100.0f;
	for (int x = 0; x < n; ++x)
		for (int z = 0; z < n; ++z) {
			matrix[x][z] = 0.0f;
			if (x % offset == 0 && z % offset == 0)
				matrix[x][z] = noise();
		}

	bilinear_interpolation(n, offset);

	//Como este no es un espacio continuo, hay que truncar los valores
	for (int x = 0; x < n; ++x)
		for (int z = 0; z < n; ++z)
			matrix[x][z] = rescale(matrix[x][z], min, max);
	
	/*for (u32 x = 0; x < n; ++x) {
		for (u32 z = 0; z < n; ++z) {
			//std::cout << matrix[x][z] << "\t";
		}
		//std::cout << "\n";
	}
	for (u32 x = 0; x < n; ++x)
		for (u32 z = 0; z < n; ++z)
			matrix[x][z] = (int)(matrix[x][z]);
			*/
}
void load_positions(u32& n, std::vector<glm::vec3>& positions) {
	for (u32 x = 0; x < n; ++x) {
		for (u32 z = 0; z < n; ++z) {
			positions[x * n + z] = glm::vec3(x, matrix[x][z], z);
		}
	}
}
/***********************************************/

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Generacion procedural de terrenos");
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	string shader_path = "bin";
	Shader* lightingShader = new Shader("colors.vert", "colors.frag", shader_path);
	Shader* lightCubeShader = new Shader("light_cube.vert", "light_cube.frag", shader_path);

	Cube* cube = new Cube();

	srand(time(NULL));
	u32 n = 101;
	std::vector<glm::vec3> positions(n * n);
	generate_random(n, -5, 15, 20);
	load_positions(n, positions);


	// configuracion del vbo y ebo
	Vbo* vbo = new Vbo();
	Ebo* ebo = new Ebo();
	vbo->SetBufferData(cube->getVSize(), cube->getVertices());
	ebo->SetBufferData(cube->getISize(), cube->getIndices());

	Vao* cubeVao = new Vao();
	vbo->Bind();
	ebo->Bind();
	cubeVao->SetVertexAttributes(0, 3, 11 * FSIZE, 0); // posicion
	cubeVao->SetVertexAttributes(1, 3, 11 * FSIZE, 3); // color
	cubeVao->SetVertexAttributes(2, 2, 11 * FSIZE, 6); // texture
	cubeVao->SetVertexAttributes(3, 3, 11 * FSIZE, 8); // texture
	cubeVao->Unbind();

	Vao* lightCubeVao = new Vao();
	vbo->Bind();
	ebo->Bind();
	lightCubeVao->SetVertexAttributes(0, 3, 11 * FSIZE, 0); // posicion
	lightCubeVao->Unbind();

	// load and create a texture
	string textures_path = "";
	Texture* texture0 = new Texture(GL_TEXTURE0, "agua.jpeg", textures_path + "resources/textures");
	Texture* texture1 = new Texture(GL_TEXTURE0, "dirt.jpeg", textures_path + "resources/textures");
	Texture* texture2 = new Texture(GL_TEXTURE0, "piedra.jpg", textures_path + "resources/textures");

	lightingShader->useProgram();
	lightingShader->setI32("texture0", 0);

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Cube objeto
		lightingShader->useProgram();
		lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader->setVec3("lightPos", lightPos);
		lightingShader->setVec3("viewPos", cam.getPos());


		// uniforms
		mat4 projection = perspective(cam.getZoom(), ASPECT, 0.1f, 100.0f);
		lightingShader->setMat4("proj", projection);
		lightingShader->setMat4("view", cam.getViewM4());

		// draw
		cubeVao->Bind();
		for (u32 i = 0; i < positions.size(); ++i) {
			if(positions[i].y < 0)
				texture0->Bind();
			else if (positions[i].y < 5)
				texture1->Bind();
			else if (positions[i].y < 15)
				texture2->Bind();

			mat4 model = mat4(1.0f);
			model = translate(model, positions[i]);
			lightingShader->setMat4("model", model);

			glDrawElements(GL_TRIANGLES, cube->getISize(), GL_UNSIGNED_INT, 0);
		}



		// Cube de luz
		lightCubeShader->useProgram();
		lightCubeShader->setMat4("proj", projection);
		lightCubeShader->setMat4("view", cam.getViewM4());

		// uniforms
		mat4 model = mat4(1.0f);
		model = translate(model, lightPos);
		model = scale(model, vec3(0.2f)); // a smaller cube
		lightCubeShader->setMat4("model", model);

		// draw
		lightCubeVao->Bind();
		glDrawElements(GL_TRIANGLES, cube->getISize(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete cubeVao;
	delete lightCubeVao;
	delete vbo;
	delete ebo;

	delete cube;
	delete lightingShader;
	delete lightCubeShader;

	return 0;
}
