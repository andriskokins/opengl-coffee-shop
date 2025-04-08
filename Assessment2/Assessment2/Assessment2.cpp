#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <array>

#include "camera.h"
#include "error.h"
#include "file.h"
#include "shader.h"
#include "shadow.h"
#include "texture.h"
#include "light.h"

SCamera Camera;

// Each vertex now includes: position (3), color (3), normal (3), and texcoords (2)
float vertices[] =
{
    // back face
    // pos                // col           // normal          // texcoords
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, -1.f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, -1.f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, -1.f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, -1.f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, -1.f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, -1.f,    0.0f, 0.0f,

    // front face
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, 1.f,     0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, 1.f,     1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, 1.f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, 1.f,     1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, 1.f,     0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 0.f, 1.f,     0.0f, 0.0f,

    // left face
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,   -1.f, 0.f, 0.f,     0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,   -1.f, 0.f, 0.f,     1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,   -1.f, 0.f, 0.f,     1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,   -1.f, 0.f, 0.f,     1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,   -1.f, 0.f, 0.f,     0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,   -1.f, 0.f, 0.f,     0.0f, 0.0f,

    // right face
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    1.f, 0.f, 0.f,     0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,    1.f, 0.f, 0.f,     1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    1.f, 0.f, 0.f,     1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    1.f, 0.f, 0.f,     1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,    1.f, 0.f, 0.f,     0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    1.f, 0.f, 0.f,     0.0f, 0.0f,

     // bottom face
     -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, -1.f, 0.f,    0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, -1.f, 0.f,    1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, -1.f, 0.f,    1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, -1.f, 0.f,    1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, -1.f, 0.f,    0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, -1.f, 0.f,    0.0f, 0.0f,

     // top face
     -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 1.f, 0.f,     0.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 1.f, 0.f,     1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 1.f, 0.f,     1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 1.f, 0.f,     1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,    0.f, 1.f, 0.f,     0.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,    0.f, 1.f, 0.f,     0.0f, 0.0f
};

GLuint albedo;
GLuint ao;
GLuint metallic;
GLuint normal;
GLuint roughness;

#define NUM_BUFFERS 1
#define NUM_VAOS 1
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];

#define WIDTH 1024
#define HEIGHT 768

struct State
{
    float FOV = 45.f;
    bool firstMouse = true;
    double lastX = WIDTH / 2.0;
    double lastY = HEIGHT / 2.0;
    bool flashlightEnabled = false;
    bool noClipEnabled = false;
    bool crouchEnabled = false;
};

void updateLightUniforms(GLuint program) {
    glUniform1i(glGetUniformLocation(program, "numLights"), lights.size());

    for (int i = 0; i < lights.size(); i++) {
        std::string prefix = "lights[" + std::to_string(i) + "].";

        glUniform1i(glGetUniformLocation(program, (prefix + "type").c_str()), lights[i].type);
        glUniform3fv(glGetUniformLocation(program, (prefix + "position").c_str()), 1, glm::value_ptr(lights[i].position));
        glUniform3fv(glGetUniformLocation(program, (prefix + "direction").c_str()), 1, glm::value_ptr(lights[i].direction));
        glUniform3fv(glGetUniformLocation(program, (prefix + "colour").c_str()), 1, glm::value_ptr(lights[i].colour));
        glUniform1f(glGetUniformLocation(program, (prefix + "intensity").c_str()), lights[i].intensity);
        glUniform1f(glGetUniformLocation(program, (prefix + "constant").c_str()), lights[i].constant);
        glUniform1f(glGetUniformLocation(program, (prefix + "linear").c_str()), lights[i].linear);
        glUniform1f(glGetUniformLocation(program, (prefix + "quadratic").c_str()), lights[i].quadratic);
        glUniform1f(glGetUniformLocation(program, (prefix + "cutOff").c_str()), lights[i].cutOff);
        glUniform1f(glGetUniformLocation(program, (prefix + "outerCutOff").c_str()), lights[i].outerCutOff);
    }
}

void drawFloorAndCubes(unsigned int shaderProgram)
{
    glBindVertexArray(VAOs[0]);

    // Draw floor without textures
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0, -3, 0));
    model = glm::scale(model, glm::vec3(100, 0.1, 100));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // Set useTexture to false for floor
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw cubes with textures
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            for (int k = -1; k < 2; k++)
            {
                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, glm::vec3(float(i * 2), float(j * 2), float(k * 2)));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

                // Set useTexture to true for cubes
                glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);

                // Activate texture units and bind textures accordingly.
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, albedo);
                glUniform1i(glGetUniformLocation(shaderProgram, "albedoMap"), 0);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, roughness);
                glUniform1i(glGetUniformLocation(shaderProgram, "roughnessMap"), 1);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, metallic);
                glUniform1i(glGetUniformLocation(shaderProgram, "metallicMap"), 2);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, normal);
                glUniform1i(glGetUniformLocation(shaderProgram, "normalMap"), 3);

                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, ao);
                glUniform1i(glGetUniformLocation(shaderProgram, "aoMap"), 4);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }
}

void processKeyboard(GLFWwindow* window, double deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool keyPressed[GLFW_KEY_LAST + 1] = { false };
    State* state = (State*)glfwGetWindowUserPointer(window);

    // Helper function for "just pressed" detection
    auto keyJustPressed = [&](int key)
	{
        bool pressed = glfwGetKey(window, key) == GLFW_PRESS;
        bool justPressed = pressed && !keyPressed[key];
        keyPressed[key] = pressed;
        return justPressed;
	};

    // Process movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboard(Camera, SCamera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboard(Camera, SCamera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboard(Camera, SCamera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboard(Camera, SCamera::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        ProcessKeyboard(Camera, SCamera::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        ProcessKeyboard(Camera, SCamera::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        Camera.MovementSpeed = SPRINT_SPEED;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        Camera.MovementSpeed = WALK_SPEED;
    else
        Camera.MovementSpeed = BASE_SPEED;

    // Toggle flashlight
    if (keyJustPressed(GLFW_KEY_F)) 
    {
        state->flashlightEnabled = !state->flashlightEnabled;
        std::cout << "Spotlight: " << (state->flashlightEnabled ? "ON" : "OFF") << std::endl;
        lights[0].direction = Camera.Front;
        lights[0].position = Camera.Position;
        //saveShadowMapToBitmap(lights[0].shadow.Texture, SH_MAP_WIDTH, SH_MAP_HEIGHT);

    }
    // Toggle flight / fly through camera
    if (keyJustPressed(GLFW_KEY_G)) 
    {
        state->noClipEnabled = !state->noClipEnabled;
        std::cout << "No Clip: " << (state->noClipEnabled ? "ON" : "OFF") << std::endl;
    }
    // Toggle crouch
    if (keyJustPressed(GLFW_KEY_LEFT_CONTROL))
    {
        state->crouchEnabled = !state->crouchEnabled;
        std::cout << "Crouch: " << (state->crouchEnabled ? "ON" : "OFF") << std::endl;
    }

    if (!state->noClipEnabled)
    {
        Camera.Position.y = 0.f; // ground camera for first person effect
    }
    if (state->crouchEnabled)
    {
        Camera.Position.y = -1.f;
        Camera.MovementSpeed /= 4; // crouch slows movement speed
    }

    // Move selected light with IJKL keys
    static int selectedLight = 0;

    // Select light with number keys
    for (int i = 0; i < lights.size(); i++) {
        if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS) {
            selectedLight = i;
            printf("Selected light: %d\n", selectedLight);
        }
    }

    float lightSpeed = 0.01f;

    // Move selected light
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lights[selectedLight].position.z -= lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lights[selectedLight].position.z += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lights[selectedLight].position.x -= lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lights[selectedLight].position.x += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        lights[selectedLight].position.y += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lights[selectedLight].position.y -= lightSpeed;
}

void SizeCallback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

void generateDepthMap(unsigned int shadowShaderProgram, ShadowStruct shadow, glm::mat4 LightSpaceMatrix)
{
    glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(shadowShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "LightSpaceMatrix"),
        1, GL_FALSE, glm::value_ptr(LightSpaceMatrix));
    drawFloorAndCubes(shadowShaderProgram);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void generateCubeMap(unsigned int shadowCubeMapProgram, ShadowStruct cubeMap, glm::mat4 transforms[], float far_plane, int lightIndex)
{
    // Set viewport for shadow mapping
    glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, cubeMap.FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Use the shadow cubemap shader program
    glUseProgram(shadowCubeMapProgram);

    // Pass the transforms for each face of the cubemap
    glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram, "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(transforms[0]));
    glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram, "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(transforms[1]));
    glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram, "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(transforms[2]));
    glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram, "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(transforms[3]));
    glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram, "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(transforms[4]));
    glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram, "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(transforms[5]));

    // Use the correct light position based on index
    glUniform3f(glGetUniformLocation(shadowCubeMapProgram, "lightPos"),
        lights[lightIndex].position.x,
        lights[lightIndex].position.y,
        lights[lightIndex].position.z);
    glUniform1f(glGetUniformLocation(shadowCubeMapProgram, "farPlane"), far_plane);

    // Draw the scene
    drawFloorAndCubes(shadowCubeMapProgram);

    // Reset framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderWithShadows(unsigned int renderShadowProgram, std::vector<glm::mat4> lightSpaceMatrices, std::vector<std::array<glm::mat4, 6>> transforms, State state)
{
    glViewport(0, 0, WIDTH, HEIGHT);
    static const GLfloat bgd[] = { 0.8f, 0.8f, 0.8f, 1.f };
    glClearBufferfv(GL_COLOR, 0, bgd);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderShadowProgram);
    updateLightUniforms(renderShadowProgram);

    int posLightIndex = 0;

    for (int i = 0; i < lights.size(); i++) {
        if (lights[i].type == DIRECTIONAL || lights[i].type == SPOT) {
            glActiveTexture(GL_TEXTURE5 + i);
            glBindTexture(GL_TEXTURE_2D, lights[i].shadow.Texture);
            std::string uniformName = "shadowMaps[" + std::to_string(i) + "]";
            glUniform1i(glGetUniformLocation(renderShadowProgram, uniformName.c_str()), 5 + i);

            // Calculate light space matrix - same as in your main loop
            //glm::mat4 lightSpaceMatrix;
            //if (lights[i].type == DIRECTIONAL) {
            //    float near_plane = 1.f, far_plane = 70.f;
            //    glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
            //    glm::mat4 lightView = glm::lookAt(lights[i].position, lights[i].position + lights[i].direction, glm::vec3(0.f, 1.f, 0.f));
            //    lightSpaceMatrix = lightProjection * lightView;
            //}
            //else { // SPOT
            //    float near_plane = 1.f, far_plane = 70.f;
            //    float lightCutoffRadians = acos(lights[i].outerCutOff);
            //    float fov = glm::degrees(lightCutoffRadians * 2.0f);
            //    glm::mat4 lightProjection = glm::perspective(glm::radians(fov), (float)SH_MAP_WIDTH / (float)SH_MAP_HEIGHT, near_plane, far_plane);
            //    glm::mat4 lightView = glm::lookAt(lights[i].position, lights[i].position + lights[i].direction, glm::vec3(0.f, 1.f, 0.f));
            //    lightSpaceMatrix = lightProjection * lightView;
            //}

            // Pass the matrix to the shader
            std::string matrixUniformName = "lightSpaceMatrices[" + std::to_string(i) + "]";
            glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, matrixUniformName.c_str()),
                1, GL_FALSE, glm::value_ptr(lightSpaceMatrices[i]));
        }
        else if (lights[i].type == POSITIONAL)
        {
            // Use texture unit 15 (or any safe starting point) + pointLightIndex
            int textureUnit = 15 + posLightIndex;
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i].shadow.Texture);

            std::string uniformName = "shadowCubeMaps[" + std::to_string(posLightIndex) + "]";
            glUniform1i(glGetUniformLocation(renderShadowProgram, uniformName.c_str()), textureUnit);

            // Also pass the position to an array
            std::string posUniformName = "pointLightPositions[" + std::to_string(posLightIndex) + "]";
            glUniform3fv(glGetUniformLocation(renderShadowProgram, posUniformName.c_str()), 1, glm::value_ptr(lights[i].position));
            ++posLightIndex;
        }
    }

    // Set up camera matrices
    glm::mat4 view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
    glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "view"),
        1, GL_FALSE, glm::value_ptr(view));

    glUniform1f(glGetUniformLocation(renderShadowProgram, "farPlane"), 25.0f);

    glm::mat4 projection = glm::perspective(glm::radians(state.FOV),
        (float)WIDTH / (float)HEIGHT, 0.01f, 100.f);
    glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "projection"),
        1, GL_FALSE, glm::value_ptr(projection));

    drawFloorAndCubes(renderShadowProgram);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    State* state = (State*)glfwGetWindowUserPointer(window);

    if (state->firstMouse)
    {
        state->lastX = xpos;
        state->lastY = ypos;
        state->firstMouse = false;
    }

    float xoffset = xpos - state->lastX;
    float yoffset = state->lastY - ypos; // Reversed since y-coordinates go from bottom to top
    state->lastX = xpos;
    state->lastY = ypos;

    ProcessMouseMovement(Camera, xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    State* state = (State*)glfwGetWindowUserPointer(window);

    state->FOV -= (float)yoffset;
    if (state->FOV < 1.0f)
        state->FOV = 1.0f;
    if (state->FOV > 45.0f)
        state->FOV = 45.0f;
    printf("FOV: %0.f\n", state->FOV);
}

int main(int argc, char** argv)
{
    // Variables for FPS counter
    double lastFpsTime = 0.0;
    int frameCount = 0;
    std::string fpsString = "0";
    std::string msString = "0";

    // Variables for movement timing
    double lastFrameTime = glfwGetTime();
    double frameTime;

    std::vector<glm::mat4> lightSpaceMatrices;
    std::vector<std::array<glm::mat4, 6>> cubeMapMatrices;

    State state;

    glfwInit();

    // Create a temporary context to query max samples
    {
        // Create a temporary hidden window
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        GLFWwindow* tempWindow = glfwCreateWindow(1, 1, "", NULL, NULL);
        glfwMakeContextCurrent(tempWindow);

        gl3wInit();

        // Query maximum sample count
        int max_antialiasing;
        glGetIntegerv(GL_MAX_SAMPLES, &max_antialiasing);
        printf("Max antialiasing supported: %d samples\n", max_antialiasing);

        glfwDestroyWindow(tempWindow);

        printf("Setting Antialiasing to: %d\n", max_antialiasing);
        glfwWindowHint(GLFW_SAMPLES, max_antialiasing);
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);  // Make window visible
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, &state);
    glfwSetWindowSizeCallback(window, SizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    gl3wInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallback, 0);
    // Disable mouse cursor 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    for (int i = 0; i < 32; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    // Initialise the lights shadow maps
    //lights[0].position = glm::vec3(2.f, 6.f, 7.f);
    //addDirectionalLight(glm::normalize(glm::vec3(0.2f, -1.0f, -0.3f)), glm::vec3(1), 1.f);
    //addSpotLight(glm::vec3(0, -1, 0.5), glm::vec3(0, 7, -10), GREEN, 5.f);
    //addSpotLight(glm::vec3(0, -1, 0.5), glm::vec3(0, 7, -10), RED, 5.f);
    addPositionalLight(glm::vec3(5, 0, 0), RED, 1.f);
    addPositionalLight(glm::vec3(-5, 0, 0), BLUE, 1.f);
    addPositionalLight(glm::vec3(0, 0, -5), GREEN, 1.f);
    // Reserve memory for each light
    cubeMapMatrices.reserve(lights.size());
    lightSpaceMatrices.reserve(lights.size());


    GLuint program = CompileShader("pbr.vert", "pbr.frag");
    GLuint shadow_program = CompileShader("shadow.vert", "shadow.frag");
    GLuint shadow_cubemap_program = CompileShader("shadowCubeMap.vert", "shadowCubeMap.frag", "shadowCubeMap.geom");

    // Load pbr textures
    albedo = setup_texture("textures/ocean-rock_albedo.png");
    metallic = setup_texture("textures/ocean-rock_metallic.png");
    normal = setup_texture("textures/ocean-rock_normal-dx.png");
    roughness = setup_texture("textures/ocean-rock_roughness.png");
    ao = setup_texture("textures/ocean-rock_ao.png");

    InitCamera(Camera);

    glCreateBuffers(NUM_BUFFERS, Buffers);
    glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, 0);
    glGenVertexArrays(NUM_VAOS, VAOs);
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        // Get current time for this frame
        double currentTime = glfwGetTime();

        // Calculate deltaTime for movement (this gets updated every frame)
        frameTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        // FPS counter logic (updated less frequently)
        frameCount++;
        if (currentTime - lastFpsTime >= 1.0 / 30.0) 
        {
            // Calculate FPS
            fpsString = std::to_string(round(frameCount / (currentTime - lastFpsTime)));
            msString = std::to_string(((currentTime - lastFpsTime) / frameCount) * 1000);

            std::string posX = std::to_string(Camera.Position.x);
            std::string posY = std::to_string(Camera.Position.y);
            std::string posZ = std::to_string(Camera.Position.z);
            std::string newTitle = "YoutubeOpenGL - " + fpsString + "FPS / " + msString + "ms" +
                "\t\t\t" + "X: " + posX + " Y: " + posY + " Z: " + posZ;
            glfwSetWindowTitle(window, newTitle.c_str());

            // Reset FPS counter variables
            lastFpsTime = currentTime;
            frameCount = 0;
        }

        // Process keyboard input
        processKeyboard(window, frameTime);

        // Resize the vector to match the number of lights
        lightSpaceMatrices.resize(lights.size());
        cubeMapMatrices.resize(lights.size());
        int posLightIndex = 0;

        // Generate a depth map for each light
        for (int i = 0; i < lights.size(); i++)
        {
            // Directional light depth map (orthogonal projection)
	        if (lights[i].type == DIRECTIONAL)
	        {
                float near_plane = 1.f, far_plane = 70.f;
                glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
                glm::mat4 lightView = glm::lookAt(lights[i].position, lights[i].position + lights[i].direction, glm::vec3(0.f, 1.f, 0.f));
                lightSpaceMatrices[i] = lightProjection * lightView;
                generateDepthMap(shadow_program, lights[i].shadow, lightSpaceMatrices[i]);
            }
            // Spot light depth map (perspective projection)
            else if (lights[i].type == SPOT)
            {
                float near_plane = 1.f, far_plane = 70.f;
                float lightCutoffRadians = acos(lights[i].outerCutOff);
                float fov = glm::degrees(lightCutoffRadians * 2.0f);
                glm::mat4 lightProjection = glm::perspective(glm::radians(fov), (float)SH_MAP_WIDTH / (float)SH_MAP_HEIGHT, near_plane, far_plane);
                glm::mat4 lightView = glm::lookAt(lights[i].position, lights[i].position + lights[i].direction, glm::vec3(0.f, 1.f, 0.f));
                lightSpaceMatrices[i] = lightProjection * lightView;
                generateDepthMap(shadow_program, lights[i].shadow, lightSpaceMatrices[i]);
            }
            else if (lights[i].type == POSITIONAL)
            {
                // Create shadow transforms for this light
                float far_plane = 25.0f; float near_plane = 1.0f;
                glm::mat4 shadowProj = glm::perspective(glm::radians(90.f), 1.f, near_plane, far_plane);

                cubeMapMatrices[posLightIndex][0] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
                cubeMapMatrices[posLightIndex][1] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
                cubeMapMatrices[posLightIndex][2] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                cubeMapMatrices[posLightIndex][3] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                cubeMapMatrices[posLightIndex][4] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
                cubeMapMatrices[posLightIndex][5] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
                // Generate cubemap for this positional light
                generateCubeMap(shadow_cubemap_program, lights[i].shadow, cubeMapMatrices[posLightIndex].data(), far_plane, i);
                ++posLightIndex;
            }
        }

        renderWithShadows(program, lightSpaceMatrices, cubeMapMatrices, state);

        // Positional Light Shadow
        //renderWithShadow(program, cubemap, far_plane, state);

        //saveShadowMapToBitmap(shadow.Texture, SH_MAP_WIDTH, SH_MAP_HEIGHT);

        //renderWithShadow(program, spot_lightSpaceMatrix);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}