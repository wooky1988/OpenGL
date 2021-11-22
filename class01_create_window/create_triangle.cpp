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
	//����glfwInit��������ʼ��GLFW
	glfwInit();
	//ʹ��glfwWindowHint����������GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//�������ڶ���
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Create Triangle", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(glWindow);
	//GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(glWindow, framebuffer_size_callback);
	
	//01,������ɫ��
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
	//02,Ƭ����ɫ��
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

	/*������ɫ�����ڶ������ˣ�ʣ�µ������ǰ�������ɫ���������ӵ�һ��������Ⱦ����ɫ������(Shader Program)��*/
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
	//��ʼ����������
	float vertices[] = {
						-0.5f, -0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f,
						 0.0f,  0.5f, 0.0f
	};

	//03, ��������������.
	/*
	����ԭ�ͣ�void glGenBuffers(GLsizei n,GLuint * buffers);
		��������һ��������Ҫ���ɵĻ��������������ڶ�����Ҫ���������洢����������Ƶ�����.
	  ����ֵ���ú�������buffers�ﷵ��n�������������ơ�

	  GLuint vbo;
	  glGenBuffers(1, &vbo);

	  GLuint vbo2[3];
	  glGenBuffers(3, vbo2);
	*/
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//04,�󶨻������
	/*
	����ԭ�ͣ�void glBindBuffer(GLenum target,GLuint buffer);
		��������һ�����ǻ����������ͣ�˵������������������洢ʲô�����������Ǵ洢�������飨GL_ARRAY_BUFFER) �������������飨GL_ELEMENT_ARRAY_BUFFER)
			  �ڶ�����������Ҫ�󶨵Ļ����������ƣ�Ҳ������������һ�����������ɵ����ơ�
	ʹ�øú������������󶨵�OpenGL�����Ļ������Ա�ʹ�á�����������ʼ�����֮�󣬿���ʹ��glBufferData������д�����ݡ�
	OpenGL��������ͬʱ�󶨶���������ͣ�ֻҪ��Щ���������ǲ�ͬ�ġ�
	*/
	//������ͺ��������󶨣���������������ϵĻ�����ö����������õ�ǰ�Ļ������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//05,д������
	//glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨵Ļ������ĺ���
	/*
	  ��һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	  �ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
	  ����������������ϣ�����͵�ʵ�����ݡ�
	  ���ĸ�����ָ��������ϣ���Կ���ι������������
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*���������Ѿ��Ѷ������ݴ������Կ����ڴ��У���VBO������㻺��������*/
	//ÿ���趨��һ��location��ֵ֮�󣬼ǵ�Ҫ������Ӧ��λ������glEnableVertexAttribArray����ΪOpenglĬ����ȫ�رյ�
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
	cout << "OpenGL �����β���" << endl;
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