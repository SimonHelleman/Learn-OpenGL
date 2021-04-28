#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define FULLSCREEN

#ifdef FULLSCREEN
static int windowWidth = 2560;
static int windowHeight = 1440;
#else
static int windowWidth = 800;
static int windowHeight = 600;
#endif

static const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

static const char* fragmentShaderSource = "#version 330 core\nout vec4 FragColor;\nvoid main()\n{\nFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}\n";

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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main()
{
	if (!glfwInit())
	{
		std::cerr << "glfwInit() failed" << std::endl;
		return -1;
	}

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

	// xyz coordinates for a triangle
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	// VBO = Vertex Buffer Object -> verticies we want to store in the GPU's memory

	// Create a buffer object, there are several different types of buffer objects
	// OpenGL allows us to bind several buffers at once as long as they have a different buffer type
	unsigned int VBO;
	glGenBuffers(1, &VBO);


	// VAO = Vertex Array Object -> Once bound any subsquent vertex attribute calls from that point will be stored inside the VAO
	//		which makes is simple to switch objects (just switch VAO)

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	

	// 1. Bind vertex array object
	glBindVertexArray(VAO);

	// 2. Copy verticies into a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // From this point, any calls to glBufferData() will be on the currently bound buffer (VBO)
	
	// glBufferData() is a function specifically targeted to copy user-defined data into the currently bound buffer.
	// Its first argument is the type of the buffer we want to copy data into: the vertex buffer object currently bound to the GL_ARRAY_BUFFER target.
	// The second argument specifies the size of the data (in bytes) we want to pass to the buffer; a simple sizeof of the vertex data suffices.
	// The third parameter is the actual data we want to send.
	// The fourth parameter specifies how we want the graphics card to manage the given data. This can take 3 forms:
	//		GL_STREAM_DRAW: the data is set only onceand used by the GPU at most a few times.
	//		GL_STATIC_DRAW : the data is set only onceand used many times.
	//		GL_DYNAMIC_DRAW : the data is changed a lotand used many times.
	// The position data of the triangle does not change, is used a lot, and stays the same for every render call so its usage type should best be 
	// GL_STATIC_DRAW.If, for instance, one would have a buffer with data that is likely to change frequently, a usage type of GL_DYNAMIC_DRAW ensures
	// the graphics card will place the data in memory that allows for faster writes.
	// As of now we stored the vertex data within memory on the graphics card as managed by a vertex buffer object named VBO.Next we want to create a
	// vertexand fragment shader that actually processes this data, so let's start building those.
 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Push buffer to GPU


	// 3. Set the vertex attributes pointers

	// Tell OpenGL how our vertex buffer data is formatted
	// Paramater 1:	vertex attribute to configure. In this case we specified the location of the position vertex attribute
	//				in the in the vertex shader with location = 0.
	// Paramater 2:	Size of vertex attribute, in this case it is a vec3 which is composed of 3 floats
	// Paramater 3: Type of the data
	// Paramater 4: Should the data be normalized (not relavent here)
	// Paramater 5: Stide = space between consecutive vertex attribs. In this case they are spaced out by 3 floats
	// Paramater 6: Offset wehre position data begins in the buffer (0 in this case)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Create fragment shader -> basically the same as the vertex
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Create a shader program
	// When linking the shaders into a program it links the outputs of each shader to the inputs of the next shader.
	// This is also where you'll get linking errors if your outputs and inputs do not match.
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	// Delete the shaders since we don't need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		// rendering happens here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// Numbers represnent start index and how many verticies
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}