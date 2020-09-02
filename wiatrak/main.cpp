#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>

#include "ShaderProgram.h"
#include "Object.h"
#include "Primitive.h"
#include "Cube.h"
#include "Camera.h"
#include "PointLight.h"
#include "Cylinder.h"
#include "CubicPointLight.h"
#include "Sphere.h"
#include "SphericalPointLight.h"
#include "CylindricalPointLight.h"
#include "Skybox.h"
#include "Floor.h"
#include "Textures.h"
#include "Windmill.h"

using namespace std;

const float MAX_FPS = 60.0f;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void setUpShaders(ShaderProgram* shader, ShaderProgram* lampShader, ShaderProgram* skyboxShader);

int PointLight::pointLightsQuantity = 0; //init static variable of PointLight

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

Camera camera(glm::vec3(-1.0f, 2.0f, 10.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLuint WIDTH = 1200;
GLuint HEIGHT = 800;

bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

int main() {
	if (glfwInit() != GL_TRUE) {
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try {
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Wiatrak", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, WIDTH, HEIGHT);

		Textures textures;

		ShaderProgram ourShader("gl_05.vert", "gl_05.frag");
		ShaderProgram lampShader("LampShader.vert", "LampShader.frag");
		ShaderProgram skyboxShader("SkyboxShader.vert", "SkyboxShader.frag");
		
		Skybox skybox = Skybox(skyboxShader);
		Floor floor = Floor(glm::vec3(0, -0.1, 0), &ourShader, &textures);

		SphericalPointLight sun1 = SphericalPointLight(&ourShader, &textures, glm::vec3(-100.0, 100.0, 100.0), &lampShader, glm::vec3(90.0), glm::vec3(0.25), glm::vec3(0.2), glm::vec3(0.2), 1.0, 0.3, 0.0003);
		SphericalPointLight sun2 = SphericalPointLight(&ourShader, &textures, glm::vec3(100.0, 100.0, -100.0), &lampShader, glm::vec3(20.0), glm::vec3(0.25), glm::vec3(0.2), glm::vec3(0.2), 1.0, 0.3, 0.0003);
		SphericalPointLight sun3 = SphericalPointLight(&ourShader, &textures, glm::vec3(-10.0, -10.0, 10.0), &lampShader, glm::vec3(4.0), glm::vec3(0.25), glm::vec3(0.2), glm::vec3(0.2), 1.0, 0.3, 0.0003);

		Windmill windmill(glm::vec3(0), &ourShader, &textures);
		Windmill windmill2(glm::vec3(10, 0, 2), &ourShader, &textures);

		int frame_counter = 0;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// main event loop
		while (!glfwWindowShouldClose(window)) {
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			if (deltaTime < 1.0f / MAX_FPS) //limiting rendering to MAX_FPS
				continue;
			lastFrame = currentFrame;
			processInput(window);

			// Clear the colorbuffer
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			setUpShaders(&ourShader, &lampShader, &skyboxShader);
			
			windmill.set_fan_rot((float) (frame_counter % 720) / 720); // 3 * 60 fps = 3s pre rotation
			windmill2.set_fan_rot((float) ((frame_counter + 140) % 720) / 720); // 3 * 60 fps = 3s pre rotation

			skybox.draw();
			floor.draw();
			sun1.draw();
			sun2.draw();
			sun3.draw();
			windmill.draw();
			windmill2.draw();

			frame_counter++;

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}

	} catch (exception ex) {
		cout << ex.what() << endl;
	}
	glfwTerminate();
	return 0;
}

void setUpShaders(ShaderProgram* shader, ShaderProgram* lampShader, ShaderProgram* skyboxShader)
{
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	
	shader->Use();
	shader->setVec3("viewPos", camera.Position);
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);

	lampShader->Use();
	lampShader->setMat4("projection", projection);
	lampShader->setMat4("view", view);

	skyboxShader->Use();
	view = glm::mat4(glm::mat3(view));
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}