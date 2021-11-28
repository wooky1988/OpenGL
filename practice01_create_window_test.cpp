#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

#ifdef OPEN_GL
const static unsigned WIDTH = 800;
const static unsigned HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	//初始化GLFW
	glfwInit();
	//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	//创建一个窗口,这个窗口的对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地调用。
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Hello Triangle", NULL, NULL);
	if (!glWindow)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//指定当前线程对应的窗口为OpenGL或openGlES关联的上下文窗口。
	//将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, framebuffer_size_callback);

	//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	while (!glfwWindowShouldClose(glWindow))
	{
		//窗口没关，就处理输入事件
		processInput(glWindow);
		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(glWindow);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.ddd
	//当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。
	glfwTerminate();
	return 0;
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
#endif