#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"

// #define FULLSCREEN
// #define DISPLAY_WIREFRAME

#ifdef FULLSCREEN
static int windowWidth = 2560;
static int windowHeight = 1440;
#else
static int windowWidth = 800;
static int windowHeight = 600;
#endif

// Called whenever the window is resized -> changes fields and sets viewport
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

void PrintWindowDimensions()
{
	std::cout << "width: " << windowWidth << " height: " << windowHeight << std::endl;
}

void ProcessInput(GLFWwindow* window)
{
#ifdef DISPLAY_WIREFRAME
	static bool isWireFrameEnabled = 1;
#else
	static bool isWireFrameEnabled = 0;
#endif
	static bool isKeyDown = 0;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	if (!isKeyDown)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			isKeyDown = 1;
			if (!isWireFrameEnabled)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				isWireFrameEnabled = !isWireFrameEnabled;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				isWireFrameEnabled = !isWireFrameEnabled;
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
	{
		isKeyDown = 0;
	}
	
}

int main()
{
	std::cerr << "stderr is visible" << std::endl;
	if (!glfwInit())
	{
		std::cerr << "glfwInit() failed" << std::endl;
		return -1;
	}

	std::cout << "glfwInit() complete" << std::endl;
	
	// Set OpenGL version and set to core mode (modern)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window
#ifdef FULLSCREEN
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGl", glfwGetPrimaryMonitor(), NULL);
#else
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGl", nullptr, nullptr);
#endif
	if (!window)
	{
		std::cerr << "Failed ot create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLAD -> I can't say I completely understand this but it works like a black box
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set the view port to be the entire window and register the callback to handle resizeing
	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);


	float triangle[] = {
		0.5f, -0.5f, 0.0f, 0.22f, 1.0f, 0.78f,
		-0.5f, -0.5f, 0.0f, 0.04f, 0.38f, 0.93f,
		0.0f, 0.5f, 0.0f, 0.97f, 0.13f, 0.52f
	};

	// VBO = Vertex Buffer Object -> verticies we want to store in the GPU's memory

	// Create a buffer object, there are several different types of buffer objects
	// OpenGL allows us to bind several buffers at once as long as they have a different buffer type
	unsigned int triangleVBO;
	glGenBuffers(1, &triangleVBO);


	// VAO = Vertex Array Object -> Once bound any subsquent vertex attribute calls from that point will be stored inside the VAO
	//		which makes is simple to switch objects (just switch VAO)
	unsigned int triangleVAO;
	glGenVertexArrays(1, &triangleVAO);

	// 1. Bind vertex array object
	glBindVertexArray(triangleVAO);

	// 2. Copy verticies into a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO); // From this point, any calls to glBufferData() will be on the currently bound buffer (VBO)

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW); // Push buffer to GPU

	// 3. Set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Set the offset to 3 floats in since thats where color is
	glEnableVertexAttribArray(1);

	ShaderProgram shader = ShaderProgram("vertex.vert", "fragment.frag");
	shader.Use();

	shader.SetFloat("offset", 0.0f);

#ifdef DISPLAY_WIREFRAME
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		// rendering happens here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();

		glBindVertexArray(triangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}