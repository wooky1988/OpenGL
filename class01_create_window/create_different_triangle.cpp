#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;
#ifdef DEBUG

/*
Ҫ�󣺴�����ͬ�����������Σ��������ǵ�����ʹ�ò�ͬ��VAO��VBO
*/

const unsigned int WIDTH  = 800;
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

void frameSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

int main()
{
	//01,��ʼ��glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//02,����Window����
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "CreateDifferentTrianlge", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "create glWindow failed" << endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, frameSizeCallback);

	//���ڰ�context����������
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "load proc failed" << endl;
		return -1;
	}

	//03,����shader
	unsigned int vertextShader = 0;
	vertextShader = glCreateShader(GL_VERTEX_SHADER);
	//����������ɫ���ַ�Դ�루ͨ�����������������͸����ǵ���ɫ������
	glShaderSource(vertextShader, 1, &vertexShaderSource, NULL);//���ĸ�����ָ��������ÿ���ַ����ĳ��ȣ����Ϊ NULL�������ÿ���ַ��������� '\0' ��β��
	glCompileShader(vertextShader);
	int  success;
	char infoLog[256];
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
		glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);//����3�� �������Ҫ�����ַ����ĳ��ȣ��������length�����д���NULLֵ��
		return -1;
	}

	unsigned int fragmentShader = 0;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		return -1;
	}
	//04,������ɫ��
	unsigned int glProgram = 0;
	glProgram = glCreateProgram();
	glAttachShader(glProgram, vertextShader);
	glAttachShader(glProgram, fragmentShader);
	glLinkProgram(glProgram);
	glGetProgramiv(glProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(glProgram, 512, NULL, infoLog);
		return -1;
	}
	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ���ǣ�������
	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader);

	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};
	unsigned int VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	/*����ԭ�ͣ���������������
		glGenBuffers(GLsizei n, GLuint *buffers)
		1����buffers�����з���n����ǰδʹ�õ����ƣ���ʾ������������buffers�����з��ص����Ʋ�����Ҫ��������������
		2�����ص����Ʊ����Ϊ��ʹ�ã��Ա��������������󡣵��ǵ����Ǳ���֮������ֻ���һ���Ϸ���״̬��
		3������һ���������Ļ������������ƣ��������ᱻglGenBuffers()��Ϊ���������󷵻ء�
	*/
	glGenBuffers(2, VBOs);

	glBindVertexArray(VAOs[0]);
	/*����ԭ�ͣ������������
		glBindBuffer(GLenum target, GLuint buffer)
		1��Ϊ�˼����������������Ҫ�����󶨡��󶨻����������ʾѡ��δ���Ĳ���(�����ݽ��г�ʼ������ʹ�û��������������Ⱦ)��Ӱ���Ǹ�����������.
		Ҳ����˵�����Ӧ�ó����ж�����������󣬾���Ҫ��ε���glBindBuffer()������һ�����ڳ�ʼ�������������Լ��������ݣ��Ժ�ĵ���Ҫôѡ��������Ⱦ��
		����������Ҫô�Ի�������������ݽ��и��¡�
		2��Ϊ�˽��û��������󣬿�����0��Ϊ����������ı�־��������glBindBuffers()�������⽫��OpenGL�л�ΪĬ�ϵĲ�ʹ�û����������ģʽ��
		3������һ���������Ļ������������ƣ��������ᱻglGenBuffers()��Ϊ���������󷵻ء�
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	/*����ԭ�ͣ������������
		glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
		1��һ������һ�����������󣬾���Ҫ�����ռ��Դ洢���ݣ�ͨ��glBufferData����ʵ�֡�
		2������size���洢��Ԫ(ͨ�����ֽ�)��OpenGL�������ڴ棬���ڴ洢�������ݻ���������ǰ�����뵱ǰ�󶨶�������������ݶ���ɾ����
		����1��
			�������������͡�
		����2:
			�洢�����������Ҫ�ĵ��ڴ�������ͨ��ʱ����Ԫ�صĸ����������Ǹ��ԵĴ洢���ȡ�
		����3��
			������һ��ָ��ͻ����ڴ��ָ��(���ڳ�ʼ������������)��Ҳ������NULL����������ݵ���һ����Ч��ָ�룬size����λ�Ĵ洢�ռ�ʹӿͻ������Ƶ���������
			������ݵ���NULL������������ᱣ��size����λ�Ĵ洢�ռ乩�Ժ�ʹ�ã�������������г�ʼ����
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(glWindow))
	{
		processInput(glWindow);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(glProgram);
		//����VAO[0]����ֵ���Ƶ�һ��������
		glBindVertexArray(VAOs[0]);
		/*����ԭ�ͣ�
				glDrawArrays(GLenum mode, GLint first, GLsizei count)��
				����mode��ͼԪ���ͣ�ʹ��ÿ�������õ������д�first��ʼ����first+count-1����������Ԫ�أ�����һ������ͼԪ���С�
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//����VAO[1]����ֵ���Ƶڶ���������
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(glProgram);
	glfwTerminate();
	return 0;
}

//�������ڱ仯
void frameSizeCallback(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

//����������
void processInput(GLFWwindow* glWindow)
{
	if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glWindow, true);
	}
}
#endif // DEBUG