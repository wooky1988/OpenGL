#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;
/*
绘制两个三角形
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
	//01,初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//02,创建glWindow
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
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//参数3： 如果不需要返回字符串的长度，则可以在length参数中传递NULL值。
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
	函数原型：告诉OpenGL该如何解析顶点数据
	glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
	参数1：
		指定我们要配置的顶点属性，我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值，因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
	参数2：
		指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	参数3：
		指定数据的类型，这里是GL_FLOAT,默认vec3是float型。
	参数4：
		定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
	参数5：
	    步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
	参数6：
	    数据指针， 这个值受到VBO的影响；
		1：在不使用VBO的情况下，就是一个指针，指向的是需要上传到顶点数据指针，项目中通常在不使用VBO的情况下，
		   绘制之前，执行glBindBuffer(GL_ARRAY_BUFFER, 0)，否则会导致数组顶点无效，界面无法显示；
		2：使用VBO的情况下，先要执行glBindBuffer(GL_ARRAY_BUFFER, 1)，如果一个名称非零的缓冲对象被绑定至GL_ARRAY_BUFFER目标（见glBindBuffer）且此时一个顶点属性数组被指定了，
		   那么pointer被当做该缓冲对象数据存储区的字节偏移量。并且，缓冲对象绑定（GL_ARRAY_BUFFER_BINDING）会被存为索引为index的顶点属性数组客户端状态；此时指针指向的就不是具体的数据了。
		   因为数据已经缓存在缓冲区了。这里的指针表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT),  (void*)0);
	//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的，启用以后数据传入顶点着色器
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
		函数原型：提供绘制功能，从数组数据中提取数据渲染基本图元。
		void glDrawArrays(GLenum mode, GLint first, GLsizei count);
		参数1：
			需要渲染的图元类型，包括 GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN ，GL_TRIANGLES。
		参数2：
			从数组缓存中的哪一位开始绘制，一般为0.
		参数3：
			数组中顶点的数量.
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