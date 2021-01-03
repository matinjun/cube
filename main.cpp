#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include "myFrustum.h"
#include "myTree.h"
#include "human.h"
#include "sun.h"
#include "plane.h"

#include <iostream>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);



const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
// 相机参数
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// 机器人参数
Human* pHuman[2];
int currentRobot = 0;

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// 平衡速度的参数
float lastFrame = 0.0f;
float deltaTime = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 5.0f, 2.0f);
// glm::vec3 lightColor(0.93f, 0.78f, 0.06f);
Light light(lightPos);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "mtj-2018152102-finalhomework", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
	// -----------------------------
    glEnable(GL_DEPTH_TEST);
    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("6.1.coordinate_systems.vs", "6.1.coordinate_systems.fs");

    // initail position of cube
    glm::vec3 cubePositions[] = {
		glm::vec3(1.0f,  0.0f,  -1.0f),
		glm::vec3(4.0f,  0.0f, -1.0f),
		glm::vec3(-2.0f,  0.0f, -1.0f),
		glm::vec3(-3.8f, 0.0f, -12.3f),
		glm::vec3(2.4f, 0.0f, -3.5f),
		glm::vec3(-1.7f,  0.0f, -7.5f),
		glm::vec3(1.3f, 0.0f, -2.5f),
		glm::vec3(4.5f,  0.0f, -2.5f),
		glm::vec3(6.5f,  0.0f, -1.5f),
		glm::vec3(-7.3f,  0.0f, -1.5f)
    };

    // 自定义
    Frustum frustum(0.01f, 1.0f, 1.0f, glm::vec3(1.0f, 0.5f, 0.31f));
    MyTree tree(2.0f);
    Transform transform;
	// 机器人
	Human human(0.051f);
    pHuman[0] = &human;
    Human human1(0.051f);
    human1.set_pos(2.0f, 3.0f);
    pHuman[1] = &human1;
    // 太阳
    Sun sun(light.lightColor);
    // 平面
    Plane plane;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 计算时差，并更新
        auto time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 让太阳转动起来
        light.lightColor = glm::vec3(std::max((float)sin(0.1f * time), 0.3f));
        light.lightPos.x = 2.33f * sin(0.5 * time);
        light.lightPos.z = 2.33f * cos(0.5 * time);

        transform.reset();
        // create transformations
        transform.view = camera.GetViewMatrix();
        ourShader.setMat4("view", transform.view);
        // projection
        transform.projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", transform.projection);

        // 渲染树
        for (int i = 0; i < 10; ++i) {
			transform.model = glm::mat4(1.0f);
            transform.model = glm::translate(transform.model, cubePositions[i]);
			tree.draw(&transform, &light, camera);
        }
        // 让机器人动起来
        transform.model = glm::mat4(1.0f);
        human.draw(&transform, &light, camera);
        human1.draw(&transform, &light, camera);

        // 渲染太阳
        sun.SetColor(light.lightColor);
        transform.model = glm::translate(glm::mat4(1.0f), light.lightPos);
        transform.model = glm::scale(transform.model, glm::vec3(0.1f));
        sun.draw(transform);

        // 渲染草地
        transform.model = glm::mat4(1.0f);
        transform.model = glm::translate(transform.model, glm::vec3(0.0f, -0.01f, 0.0f));
        transform.model = glm::scale(transform.model, glm::vec3(70.f));
        plane.draw(transform);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        currentRobot = (currentRobot + 1) % 2;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pHuman[currentRobot]->move(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pHuman[currentRobot]->move(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        pHuman[currentRobot]->move(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        pHuman[currentRobot]->move(LEFT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    
}
// 键盘回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
