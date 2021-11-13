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
	//01,初始化glfw环境
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//02,创建glWindow
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Two triangles", NULL, NULL);
	if (glWindow == NULL)
	{
		cout << "create glwindow failed" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, frame_size_callback);
	//03,加载初始化gl函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "load gl function failed" << endl;
		return -1;
	}

	//04,初始化着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[256];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//不需要返回字符串的长度，第三个参数传递为NULL
		cout << "init vertext shader error" << endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);//不需要返回字符串的长度，第三个参数传递为NULL
		cout << "init fragment shader error" << endl;
	}
	//05,创建着色器程序
	unsigned int glProgram;
	glProgram = glCreateProgram();
	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);
	glLinkProgram(glProgram);

	glGetProgramiv(glProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(glProgram, 512, NULL, infoLog);//不需要返回字符串的长度，第三个参数传递为NULL
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
	参数1：是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	参数2：指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	参数3：是我们希望发送的实际数据。
    参数4：指定了我们希望显卡如何管理给定的数据。它有三种形式：
			GL_STATIC_DRAW ：数据不会或几乎不会改变。
			GL_DYNAMIC_DRAW：数据会被改变很多。
			GL_STREAM_DRAW ：数据每次绘制时都会改变。
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	/*
	参数1：
		指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？
		它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
    参数2：
	    指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	参数3：
	    指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
    参数4：
	    定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，
		所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
    参数5：
	   叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
	   要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
	   一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
    参数6：
	   类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。
	   由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。
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
		glBindVertexArray(VAO);//这块很疑惑，为什么上面已经glBindVertexArray(0)了，这边还要再bind一遍？？？
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
