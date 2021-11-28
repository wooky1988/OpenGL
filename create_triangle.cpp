#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

#ifdef ANNOTATION

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* glWindow, int width, int height);
void processInput(GLFWwindow* glWindow);

int main()
{
	//调用glfwInit函数来初始化GLFW
	glfwInit();
	//使用glfwWindowHint函数来配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//创建窗口对象
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Create Triangle", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(glWindow);
	//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(glWindow, framebuffer_size_callback);
	
	//01,顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	//02,片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	/*两个着色器现在都编译了，剩下的事情是把两个着色器对象链接到一个用来渲染的着色器程序(Shader Program)中*/
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	std::cout << "111111111111\n" << std::endl;
	//初始化顶点数据
	float vertices[] = {
						-0.5f, -0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f,
						 0.0f,  0.5f, 0.0f
	};

	//03, 创建缓冲区对象.
	/*
	函数原型：void glGenBuffers(GLsizei n,GLuint * buffers);
		参数：第一个参数是要生成的缓冲对象的数量，第二个是要输入用来存储缓冲对象名称的数组.
	  返回值：该函数会在buffers里返回n个缓冲对象的名称。

	  GLuint vbo;
	  glGenBuffers(1, &vbo);

	  GLuint vbo2[3];
	  glGenBuffers(3, vbo2);
	*/
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//04,绑定缓冲对象
	/*
	函数原型：void glBindBuffer(GLenum target,GLuint buffer);
		参数：第一个就是缓冲对象的类型，说明这个缓冲区是用来存储什么东西，可以是存储顶点数组（GL_ARRAY_BUFFER) 或者是索引数组（GL_ELEMENT_ARRAY_BUFFER)
			  第二个参数就是要绑定的缓冲对象的名称，也就是我们在上一个函数里生成的名称。
	使用该函数将缓冲对象绑定到OpenGL上下文环境中以便使用。当缓冲区初始化完成之后，可以使用glBufferData往里面写入数据。
	OpenGL允许我们同时绑定多个缓冲类型，只要这些缓冲类型是不同的。
	*/
	//这个类型和这个对象绑定，后续在这个类型上的缓冲调用都会用来配置当前的缓冲对象。
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//05,写入数据
	//glBufferData是一个专门用来把用户定义的数据复制到当前绑定的缓冲对象的函数
	/*
	  第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	  第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	  第三个参数是我们希望发送的实际数据。
	  第四个参数指定了我们希望显卡如何管理给定的数据
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*现在我们已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理*/
	//每次设定好一个location的值之后，记得要开启对应的位置数据glEnableVertexAttribArray，因为Opengl默认是全关闭的
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	

	std::cout << "22222222222222222222\n" << std::endl;
	while (!glfwWindowShouldClose(glWindow))
	{
		processInput(glWindow);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(glWindow);
		glfwPollEvents();
	}
	cout << "OpenGL 三角形测试" << endl;
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* glWindow)
{
	if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glWindow, true);
	}
}

#endif