#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;
/*
��������������
*/
#ifdef DEBUG



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
	//01,��ʼ��glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//02,����glWindow
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Two Triangle", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, framesize_update_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to loader glfw" << endl;
		return -1;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[256];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//����3�� �������Ҫ�����ַ����ĳ��ȣ��������length�����д���NULLֵ��
		cout << "Create Vertex shader failed" << endl;
		return -1;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "Create fragment shader failed" << endl;
		return -1;
	}
	unsigned int glProgram = 0;
	glProgram = glCreateProgram();
	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);
	glLinkProgram(glProgram);
	glGetProgramiv(glProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(glProgram, 512, NULL, infoLog);
		cout << "link glProgram failed" << endl;
		return -1;
	}
	float vertex[] = 
	{
		-0.5f,  .0f, .0f,
		  .0f,  .0f, .0f,
	   -0.25f, 0.5f, .0f,
		  .0f,  .0f, .0f,
		 0.5f,  .0f, .0f,
		0.25f, 0.5f, .0f,
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	/*
	����ԭ�ͣ�����OpenGL����ν�����������
	glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
	����1��
		ָ������Ҫ���õĶ������ԣ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
	����2��
		ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	����3��
		ָ�����ݵ����ͣ�������GL_FLOAT,Ĭ��vec3��float�͡�
	����4��
		���������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
	����5��
	    ����(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
	����6��
	    ����ָ�룬 ���ֵ�ܵ�VBO��Ӱ�죻
		1���ڲ�ʹ��VBO������£�����һ��ָ�룬ָ�������Ҫ�ϴ�����������ָ�룬��Ŀ��ͨ���ڲ�ʹ��VBO������£�
		   ����֮ǰ��ִ��glBindBuffer(GL_ARRAY_BUFFER, 0)������ᵼ�����鶥����Ч�������޷���ʾ��
		2��ʹ��VBO������£���Ҫִ��glBindBuffer(GL_ARRAY_BUFFER, 1)�����һ�����Ʒ���Ļ�����󱻰���GL_ARRAY_BUFFERĿ�꣨��glBindBuffer���Ҵ�ʱһ�������������鱻ָ���ˣ�
		   ��ôpointer�������û���������ݴ洢�����ֽ�ƫ���������ң��������󶨣�GL_ARRAY_BUFFER_BINDING���ᱻ��Ϊ����Ϊindex�Ķ�����������ͻ���״̬����ʱָ��ָ��ľͲ��Ǿ���������ˡ�
		   ��Ϊ�����Ѿ������ڻ������ˡ������ָ���ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0��
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT),  (void*)0);
	//�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õģ������Ժ����ݴ��붥����ɫ��
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(glWindow))
	{
		processInput(glWindow);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(glProgram);
		glBindVertexArray(VAO);
		/*
		����ԭ�ͣ��ṩ���ƹ��ܣ���������������ȡ������Ⱦ����ͼԪ��
		void glDrawArrays(GLenum mode, GLint first, GLsizei count);
		����1��
			��Ҫ��Ⱦ��ͼԪ���ͣ����� GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN ��GL_TRIANGLES��
		����2��
			�����黺���е���һλ��ʼ���ƣ�һ��Ϊ0.
		����3��
			�����ж��������.
			https://www.cnblogs.com/lxb0478/p/6381677.html
		*/
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(glWindow);
		glfwPollEvents();
	}

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
#endif // DEBUG