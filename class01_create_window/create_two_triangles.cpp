#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void frame_size_callback(GLFWwindow*, int ,int);
void processInput(GLFWwindow *);

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


int main()
{
	//01,��ʼ��glfw����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//02,����glWindow
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Two triangles", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "create glwindow failed" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, frame_size_callback);
	//03,���س�ʼ��gl����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "load gl function failed" << endl;
		return -1;
	}

	//04,��ʼ����ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[256];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//����Ҫ�����ַ����ĳ��ȣ���������������ΪNULL
		cout << "init vertext shader error" << endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);//����Ҫ�����ַ����ĳ��ȣ���������������ΪNULL
		cout << "init fragment shader error" << endl;
	}
	//05,������ɫ������
	unsigned int glProgram;
	glProgram = glCreateProgram();
	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);
	glLinkProgram(glProgram);

	glGetProgramiv(glProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(glProgram, 512, NULL, infoLog);//����Ҫ�����ַ����ĳ��ȣ���������������ΪNULL
		cout << "init fragment shader error" << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertexs[] = 
	{
		-0.5f, .0f, .0f,
		  .0f, -0.5f, .0f,
		 0.5f,  0.0f, .0f,
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*
	����1����Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	����2��ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
	����3��������ϣ�����͵�ʵ�����ݡ�
    ����4��ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
			GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
			GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
			GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	/*
	����1��
		ָ������Ҫ���õĶ������ԡ����ǵ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)��
		�����԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
    ����2��
	    ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	����3��
	    ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
    ����4��
	    ���������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE��
		�������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
    ����5��
	   ��������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
	   Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
	   һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ����������ӣ���ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
    ����6��
	   ������void*��������Ҫ���ǽ��������ֵ�ǿ������ת��������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)��
	   ����λ������������Ŀ�ͷ������������0�����ǻ��ں�����ϸ�������������
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(glWindow))
	{
		processInput(glWindow);
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(glProgram);
		glBindVertexArray(VAO);//�����ɻ�Ϊʲô�����Ѿ�glBindVertexArray(0)�ˣ���߻�Ҫ��bindһ�飿����
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(glProgram);
	glfwTerminate();
	return 0;
}

void frame_size_callback(GLFWwindow* glWindow, int width, int height)
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
