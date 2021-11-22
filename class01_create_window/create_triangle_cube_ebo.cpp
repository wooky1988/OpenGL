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

void framesize_update_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow *);


int main()
{
	//��ʼglfw����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//��������
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Triangle Cube EBO", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		return -1;
	}
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, framesize_update_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to loader glfw" << endl;
		return -1;
	}
	unsigned int vertextShader;
	vertextShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertextShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertextShader);
	int success;
	char infoLog[256];
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		return -1;
	}
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertextShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
			0.5f, 0.5f, 0.0f,   // ���Ͻ�
			0.5f, -0.5f, 0.0f,  // ���½�
			-0.5f, -0.5f, 0.0f, // ���½�
			-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};
	unsigned int indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	unsigned int  VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/*
	Ĭ������£��������ܿ��ǣ����ж�����ɫ�������ԣ�Attribute���������ǹرյģ���ζ����������ɫ�����ǲ��ɼ��ģ�
	���������Ѿ��ϴ���GPU����glEnableVertexAttribArray����ָ�����ԣ��ſ��ڶ�����ɫ���з����𶥵���������ݡ�
	glVertexAttribPointer��VBOֻ�ǽ���CPU��GPU֮����߼����ӣ��Ӷ�ʵ����CPU�����ϴ���GPU��
	���ǣ�������GPU���Ƿ�ɼ���������ɫ���ܷ��ȡ�����ݣ����Ƿ������˶�Ӧ�����Ծ����������glEnableVertexAttribArray�Ĺ��ܣ���������ɫ����ȡGPU���������ˣ����ݡ�
	*/
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(glWindow))
	{
		processInput(glWindow);
		glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(glWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

void framesize_update_callback(GLFWwindow* glWindow, int width, int height)
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