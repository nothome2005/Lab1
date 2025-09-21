#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <windows.h>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
const float moveStep = 0.01f;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderOutlineSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

float offsetX = 0.0f;
float offsetY = 0.0f;
float baseVertices[] = {
    // триткутник 1
    -0.5f,  0.3f, 0.0f,
    -0.5f,  0.1f, 0.0f,
    -0.6f,  0.2f, 0.0f,
    // трикутник 2
    -0.5f,  0.6f, 0.0f,
    -0.1f,  0.2f, 0.0f,
    -0.5f,  0.2f, 0.0f,
    // трикутник 3
    -0.7f,  0.6f, 0.0f,
    -0.5f,  0.8f, 0.0f,
    -0.5f,  0.6f, 0.0f,
    // трикутник 4
    -0.3f, 0.2f, 0.0f,
    0.0f, 0.2f, 0.0f,
    0.0f, -0.1f, 0.0f,
    // Квадрат
    -0.3f, 0.2f, 0.0f,
    -0.447f, 0.05f, 0.0f,
    -0.297f, -0.097f, 0.0f,
    -0.15f, 0.053f, 0.0f,
    // Треугольник
    -0.15f, 0.053f, 0.0f,
    -0.444f, -0.247f, 0.0f,
    0.15f, -0.247f, 0.0f,
    // паралелограм
    0.15f, -0.247f, 0.0f,
    0.4f, -0.247f, 0.0f,
    0.3f, -0.1f, 0.0f,
    0.0086f, -0.1f, 0.0f
};
float vertices[sizeof(baseVertices)/sizeof(float)];

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw init
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //vertex comp sh
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // frag compile sh
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // outline
    int fragmentShaderOutline = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderOutline, 1, &fragmentShaderOutlineSource, NULL);
    glCompileShader(fragmentShaderOutline);

    //link sh
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int shaderProgramOutline = glCreateProgram();
    glAttachShader(shaderProgramOutline, vertexShader);
    glAttachShader(shaderProgramOutline, fragmentShaderOutline);
    glLinkProgram(shaderProgramOutline);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderOutline);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.101f, 0.847f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        // Перед рендером копируем baseVertices с учетом смещения
        for (int i = 0; i < sizeof(baseVertices)/sizeof(float); i += 3) {
            vertices[i] = baseVertices[i] + offsetX;
            vertices[i+1] = baseVertices[i+1] + offsetY;
            vertices[i+2] = baseVertices[i+2];
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);   
        glDrawArrays(GL_TRIANGLES, 3, 3);   
        glDrawArrays(GL_TRIANGLES, 6, 3);   
        glDrawArrays(GL_TRIANGLES, 9, 3);  

        glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

        
        glUseProgram(shaderProgramOutline);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);

        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glDrawArrays(GL_TRIANGLES, 6, 3);
        glDrawArrays(GL_TRIANGLES, 9, 3);

    
        glDrawArrays(GL_LINE_LOOP, 12, 4);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 16, 3);

        glUseProgram(shaderProgramOutline);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        glDrawArrays(GL_TRIANGLES, 16, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

        glUseProgram(shaderProgramOutline);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINE_LOOP, 19, 4);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        offsetY += moveStep;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        offsetY -= moveStep;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        offsetX -= moveStep;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        offsetX += moveStep;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}