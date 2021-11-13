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
	//��ʼ��GLFW
	glfwInit();
	//����GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	//����һ������,������ڵĶ����������кʹ�����ص����ݣ����һᱻGLFW����������Ƶ���ص��á�
	GLFWwindow* glWindow = glfwCreateWindow(WIDTH, HEIGHT, "Hello Triangle", NULL, NULL);
	if (!glWindow)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//ָ����ǰ�̶߳�Ӧ�Ĵ���ΪOpenGL��openGlES�����������Ĵ��ڡ�
	//�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, framebuffer_size_callback);

	//GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	while (!glfwWindowShouldClose(glWindow))
	{
		//����û�أ��ʹ��������¼�
		processInput(glWindow);
		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(glWindow);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.ddd
	//����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ��
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