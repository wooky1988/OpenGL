#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;
#ifdef DEBUG

/*
要求：创建相同的两个三角形，但对它们的数据使用不同的VAO和VBO
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
	//01,初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//02,创建Window窗口
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "CreateDifferentTrianlge", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "create glWindow failed" << endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, frameSizeCallback);

	//放在绑定context上下文下面
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "load proc failed" << endl;
		return -1;
	}

	//03,创建shader
	unsigned int vertextShader = 0;
	vertextShader = glCreateShader(GL_VERTEX_SHADER);
	//将真正的着色器字符源码（通过第三个参数）传送给我们的着色器对象
	glShaderSource(vertextShader, 1, &vertexShaderSource, NULL);//第四个参数指明数组中每个字符串的长度，如果为 NULL，则表明每个字符串都是以 '\0' 结尾的
	glCompileShader(vertextShader);
	int  success;
	char infoLog[256];
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
		glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);//参数3： 如果不需要返回字符串的长度，则可以在length参数中传递NULL值。
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
	//04,创建着色器
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
	//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们！！！！
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
	/*函数原型：创建缓冲区对象
		glGenBuffers(GLsizei n, GLuint *buffers)
		1，在buffers数组中返回n个当前未使用的名称，表示缓冲区对象。在buffers数组中返回的名称并不需要是连续的整数。
		2，返回的名称被标记为已使用，以便分配给缓冲区对象。但是当他们被绑定之后，他们只获得一个合法的状态。
		3，零是一个被保留的缓冲区对象名称，从来不会被glGenBuffers()作为缓冲区对象返回。
	*/
	glGenBuffers(2, VBOs);

	glBindVertexArray(VAOs[0]);
	/*函数原型：激活缓冲区对象
		glBindBuffer(GLenum target, GLuint buffer)
		1，为了激活缓冲区对象，首先需要将它绑定。绑定缓冲区对象表示选择未来的操作(对数据进行初始化或者使用缓冲区对象进行渲染)将影响那个缓冲区对象.
		也就是说，如果应用程序有多个缓冲区对象，就需要多次调用glBindBuffer()函数：一次用于初始化缓冲区对象以及它的数据，以后的调用要么选择用于渲染的
		缓冲区对象，要么对缓冲区对象的数据进行更新。
		2，为了禁用缓冲区对象，可以用0作为缓冲区对象的标志符来调用glBindBuffers()函数。这将把OpenGL切换为默认的不使用缓冲区对象的模式。
		3，零是一个被保留的缓冲区对象名称，从来不会被glGenBuffers()作为缓冲区对象返回。
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	/*函数原型：激活缓冲区对象
		glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
		1，一旦绑定了一个缓冲区对象，就需要保留空间以存储数据，通过glBufferData函数实现。
		2，分配size个存储单元(通常是字节)的OpenGL服务器内存，用于存储顶点数据或索引。以前所有与当前绑定对象相关联的数据都将删除。
		参数1：
			缓冲区对象类型。
		参数2:
			存储相关数据所需要的的内存数量。通常时数据元素的个数乘以它们各自的存储长度。
		参数3：
			可以是一个指向客户机内存的指针(用于初始化缓冲区对象)，也可以是NULL。如果它传递的是一个有效的指针，size个单位的存储空间就从客户机复制到服务器。
			如果传递的是NULL，这个函数将会保留size个单位的存储空间供以后使用，但不会对他进行初始化。
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
		//依据VAO[0]的数值绘制第一个三角形
		glBindVertexArray(VAOs[0]);
		/*函数原型：
				glDrawArrays(GLenum mode, GLint first, GLsizei count)‘
				按照mode的图元类型，使用每个被启用的数组中从first开始，到first+count-1结束的数组元素，创建一个几何图元序列。
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//依据VAO[1]的数值绘制第二个三角形
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

//监听窗口变化
void frameSizeCallback(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

//处理窗口输入
void processInput(GLFWwindow* glWindow)
{
	if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glWindow, true);
	}
}
#endif // DEBUG