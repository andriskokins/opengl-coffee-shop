#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <array>

#include "animation.h"
#include "camera.h"
#include "collision.h"
#include "error.h"
#include "file.h"
#include "shader.h"
#include "shadow.h"
#include "texture.h"
#include "light.h"
#include "model.h"
#include "object_parser.h"
#include "torus.h"

SCamera Camera;

// Vector of interactable object IDs
std::vector<int> interactableObjects;

// Each vertex includes: position (3), color (3), normal (3), and texcoords (2)
std::vector<float> cube_vertices =
{
    // back face
    // pos                // col                // normal          // texcoords
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, -1.f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, -1.f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, -1.f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, -1.f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, -1.f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, -1.f,    0.0f, 0.0f,

    // front face
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, 1.f,     0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, 1.f,     1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, 1.f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, 1.f,     1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, 1.f,     0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,   0.f, 0.f, 1.f,     0.0f, 0.0f,

    // left face
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  -1.f, 0.f, 0.f,     0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  -1.f, 0.f, 0.f,     1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  -1.f, 0.f, 0.f,     1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  -1.f, 0.f, 0.f,     1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  -1.f, 0.f, 0.f,     0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  -1.f, 0.f, 0.f,     0.0f, 0.0f,

    // right face
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.f, 0.f, 0.f,     0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.f, 0.f, 0.f,     1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  1.f, 0.f, 0.f,     1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  1.f, 0.f, 0.f,     1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  1.f, 0.f, 0.f,     0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.f, 0.f, 0.f,     0.0f, 0.0f,

     // bottom face
     -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.f, -1.f, 0.f,    0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.f, -1.f, 0.f,    1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.f, -1.f, 0.f,    1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.f, -1.f, 0.f,    1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.f, -1.f, 0.f,    0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.f, -1.f, 0.f,    0.0f, 0.0f,

     // top face
     -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.f, 1.f, 0.f,     0.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.f, 1.f, 0.f,     1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  0.f, 1.f, 0.f,     1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  0.f, 1.f, 0.f,     1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  0.f, 1.f, 0.f,     0.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.f, 1.f, 0.f,     0.0f, 0.0f
};

#define NUM_BUFFERS 17
#define NUM_VAOS 17
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];

#define WIDTH 1920
#define HEIGHT 1080

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
        glUniform1i(glGetUniformLocation(program, (prefix + "isOn").c_str()), lights[i].isOn);

    }
}

void drawModels(unsigned int program)
{

    for (model obj : models)
    {
        glBindVertexArray(VAOs[obj.bufferIndex]);

        // Enable texturing
        //glUniform1i(glGetUniformLocation(program, "useTexture"), 1);
        //glUniform1i(glGetUniformLocation(program, "useSingleTexture"), 1);

        glUniform1f(glGetUniformLocation(program, "textureScale"), obj.textures.textureScale);

        // Bind the texture to unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj.textures.albedo);
        glUniform1i(glGetUniformLocation(program, "albedoMap"), 0);

        // Bind roughness if available, otherwise use a default value
        glActiveTexture(GL_TEXTURE1);
        if (obj.textures.hasRoughness)
        {
            glBindTexture(GL_TEXTURE_2D, obj.textures.roughness);
        }
        else glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1i(glGetUniformLocation(program, "roughnessMap"), 1);

        // Bind metallic if available, otherwise use a default value
        glActiveTexture(GL_TEXTURE2);
        if (obj.textures.hasMetallic)
        {
            glBindTexture(GL_TEXTURE_2D, obj.textures.metallic);
        }
        else glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1i(glGetUniformLocation(program, "metallicMap"), 2);

        // Bind normal if available, otherwise use a default value
        glActiveTexture(GL_TEXTURE3);
        if (obj.textures.hasNormal)
        {
            glBindTexture(GL_TEXTURE_2D, obj.textures.normal);
        }
        else glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1i(glGetUniformLocation(program, "normalMap"), 3);

        // Bind ao if available, otherwise use a default value
        glActiveTexture(GL_TEXTURE4);
        if (obj.textures.hasAO)
        {
            glBindTexture(GL_TEXTURE_2D, obj.textures.ao);
        }
        else glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1i(glGetUniformLocation(program, "aoMap"), 4);


        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, obj.position);
        model = glm::rotate(model, glm::radians(obj.rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(obj.rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(obj.rotation.z), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, obj.scale);

        // Set the model matrix uniform
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Draw the triangles
        // position (3), colour (3), normal (3), and texcoords (2) = 11 floats per vertex
        // length of vertices divided by floats per vertex gives number of vertices per object
        glDrawArrays(GL_TRIANGLES, 0, obj.vertices.size() / 11);
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
        lights[1].direction = Camera.Front;
        lights[1].position = Camera.Position;
        lights[1].shadow.updateShadow = true;
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
        Camera.Position.y = 2.5f; // ground camera for first person effect
    }
    if (state->crouchEnabled)
    {
        Camera.Position.y /= 2;
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
    drawModels(shadowShaderProgram);
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
    drawModels(shadowCubeMapProgram);

    // Reset framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderWithShadows(unsigned int renderShadowProgram, std::vector<glm::mat4> lightSpaceMatrices, std::vector<std::array<glm::mat4, 6>> transforms, State state)
{
    glViewport(0, 0, WIDTH, HEIGHT);
    static const GLfloat bgd[] = { 0.f, 0.f, 0.f, 1.f };
    glClearBufferfv(GL_COLOR, 0, bgd);
    glClear(GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glUseProgram(renderShadowProgram);
	updateLightUniforms(renderShadowProgram);

    //int posLightIndex = 0;

    for (int i = 0; i < lights.size(); i++) {
        if (lights[i].type == DIRECTIONAL || lights[i].type == SPOT) {
            glActiveTexture(GL_TEXTURE5 + i);
            glBindTexture(GL_TEXTURE_2D, lights[i].shadow.Texture);
            std::string uniformName = "shadowMaps[" + std::to_string(i) + "]";
            glUniform1i(glGetUniformLocation(renderShadowProgram, uniformName.c_str()), 5 + i);

            // Pass the matrix to the shader
            std::string matrixUniformName = "lightSpaceMatrices[" + std::to_string(i) + "]";
            glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, matrixUniformName.c_str()),
                1, GL_FALSE, glm::value_ptr(lightSpaceMatrices[i]));
        }
        else if (lights[i].type == POSITIONAL)
        {
            // Use texture unit 15 (or any safe starting point) + pointLightIndex
            int textureUnit = 15 + i;
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i].shadow.Texture);

            std::string uniformName = "shadowCubeMaps[" + std::to_string(i) + "]";
            glUniform1i(glGetUniformLocation(renderShadowProgram, uniformName.c_str()), textureUnit);

            // Also pass the position to an array
            std::string posUniformName = "pointLightPositions[" + std::to_string(i) + "]";
            glUniform3fv(glGetUniformLocation(renderShadowProgram, posUniformName.c_str()), 1, glm::value_ptr(lights[i].position));
            //++posLightIndex;
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

    drawModels(renderShadowProgram);
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

void initialiseBuffers(std::vector<float> vertices, int bufferIndex)
{
    glNamedBufferStorage(Buffers[bufferIndex], vertices.size() * sizeof(float), vertices.data(), 0);
    glBindVertexArray(VAOs[bufferIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[bufferIndex]);
    // Position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Colour (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normal (3 floats)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Texture (2 floats)
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
}

void handleInteraction(int modelId, float distance) {
    if (std::find(interactableObjects.begin(), interactableObjects.end(), modelId) != interactableObjects.end()) 
    {
        glm::vec3 currentRotation = models.at(modelId).rotation;
        glm::vec3 newRotation = currentRotation;
        glm::vec3 newPosition = models.at(modelId).position;

        if (static_cast<int>(round(fmod(currentRotation.x, 360.0f))) == 0) 
        {
            newRotation.x = 180.0f; // On state
            newPosition.y = 2.9f;   // Y axis needs to be adjusted, might be caused due to the order of transformations in drawModels()
        }
        else {
            newRotation.x = 0.0f;   // Off state
            newPosition.y = 2.f;
        }
        // Keep the original Z rotations
        newRotation.z = currentRotation.z;

        setTranformations(modelId, newPosition, newRotation, models.at(modelId).scale);

        // Toggle the lights
        if (modelId == interactableObjects.at(0))
        {
            lights.at(0).isOn = !lights.at(0).isOn;
            lights.at(0).shadow.updateShadow = true;
        }
        else if(modelId == interactableObjects.at(1))
        {
            lights.at(1).isOn = !lights.at(1).isOn;
            lights.at(2).isOn = !lights.at(2).isOn;
            lights.at(1).shadow.updateShadow = true;
            lights.at(2).shadow.updateShadow = true;
        }
        else if (modelId == interactableObjects.at(2))
        {
            lights.at(3).isOn = !lights.at(3).isOn;
            lights.at(3).shadow.updateShadow = true;
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        // Get ray origin and direction from camera
        glm::vec3 rayOrigin = Camera.Position;
        glm::vec3 rayDir = glm::normalize(Camera.Front);

        float closestIntersection = FLT_MAX;
        int clickedModelIndex = -1;

        // Iterate through all models
        for (int i = 0; i < models.size(); ++i) {
            AABB worldAABB = calculateWorldAABB(models.at(i));
            float intersectionDist;
            // Check for intersection
            if (intersectRayAABB(rayOrigin, rayDir, worldAABB, intersectionDist)) {
                // If this intersection is closer than the previous closest, update
                if (intersectionDist < closestIntersection) {
                    closestIntersection = intersectionDist;
                    clickedModelIndex = i;
                }
            }
        }

        // Output result
        if (clickedModelIndex != -1) {
            std::cout << "Clicked on model index: " << clickedModelIndex
                << " at distance: " << closestIntersection << std::endl;
            handleInteraction(clickedModelIndex, closestIntersection);
        }
        else {
            std::cout << "Clicked on empty space." << std::endl;
        }
    }
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

    // Store the calculated matrices here
    std::vector<glm::mat4> lightSpaceMatrices;
    std::vector<std::array<glm::mat4, 6>> cubeMapMatrices;

    // Use state to store variables like FOV and booleans for crouch / flashlight
    State state;

    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 2);

    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);  // Make window visible
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, &state);
    glfwSetWindowSizeCallback(window, SizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    gl3wInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallback, 0);
    // Disable mouse cursor 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    for (int i = 0; i < 32; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

	/////////////////////////////////////////////
	// Initialising objects and their textures //
	/////////////////////////////////////////////

    std::vector<float> lowpoly_torus_vertices, highpoly_torus_vertices;
    lowpoly_torus_vertices = generateTorus(1.f, 0.3f, 8, 4);
    highpoly_torus_vertices = generateTorus(1.f, 0.3f, 32, 16);

    // Model IDs
    int table, chair, floor, counter_table, counter_top, wall, grinder, coffee_machine, shelf, coffee_bag, light_bulb, light_fixture, light_switch,
	low_torus, high_torus, plate, mug;
    // Models paths
    std::string table_path = "../objects/table/", chair_path = "../objects/chair/", grinder_path = "../objects/grinder/", coffee_machine_path = "../objects/coffee_machine/",
	shelf_path = "../objects/shelf/", coffee_bag_path = "../objects/coffee_bag/", light_bulb_path = "../objects/light/", light_fixture_path = "../objects/light2/",
	light_switch_path = "../objects/light_switch/", plate_path = "../objects/plate/", mug_path = "../objects/mug/";
    // Texture paths
    std::string floor_path = "../textures/floor/", counter_table_path = "../textures/oak-wood-bare-ue/", counter_top_path = "../textures/cloudy-veined-quartz-ue/",
	brick_wall_path = "../textures/brick-wall-ue/", gold_path = "../textures/gold-scuffed/";

    table = load(table_path+"Round_table_low.obj",
        table_path + "Wood_beech_toas_brown_Diffuse_2k.jpg",
        table_path+"Wood_beech_toas_brown_Normal_2k.jpg",
        table_path+"Wood_beech_toas_brown_Roughness_2k.jpg",
        "",
        table_path+"internal_ground_ao_texture.jpeg");
    floor = addModel(cube_vertices, floor_path+"light-plank-flooring_albedo.png",
        floor_path+"light-plank-flooring_normal-dx.png",
        floor_path+"light-plank-flooring_roughness.png",
        floor_path+"light-plank-flooring_metallic.png",
        floor_path+"light-plank-flooring_metallic.png");
    chair = load(chair_path + "Stool2_LP.obj",
        chair_path + "Stool_2_Color.png",
        chair_path+"Stool_2_Normal.png",
        chair_path+"Stool_2_Roughness.png",
        chair_path+"Stool_2_Metallic.png");
    counter_table = addModel(cube_vertices, counter_table_path + "oak-wood-bare_albedo.png",
        counter_table_path + "oak-wood-bare_normal-dx.png",
        counter_table_path + "oak-wood-bare_roughness.png",
        counter_table_path + "oak-wood-bare_metallic.png",
        counter_table_path + "oak-wood-bare_ao.png");
    counter_top = addModel(cube_vertices, counter_top_path + "cloudy-veined-quartz_albedo.png",
        counter_top_path + "cloudy-veined-quartz_normal-dx.png",
        counter_top_path + "cloudy-veined-quartz_roughness.png",
        counter_top_path + "cloudy-veined-quartz_metallic.png",
        counter_top_path + "cloudy-veined-quartz_ao.png");
    wall = addModel(cube_vertices, brick_wall_path + "brick-wall_albedo.png",
        brick_wall_path + "brick-wall_normal-dx.png",
        brick_wall_path + "brick-wall_normal-dx.png",
        brick_wall_path + "brick-wall_metallic.png",
        brick_wall_path + "brick-wall_ao.png");
    grinder = load(grinder_path + "grinder.obj",
        grinder_path + "Grinder-color.png",
        grinder_path + "Grinder-normal.png",
        grinder_path+"Grinder-roughness.png",
        grinder_path+"Grinder-metal.png",
        "",
        grinder_path+"Grinder-opacity.jpeg");
    coffee_machine = load(coffee_machine_path + "coffee_machine.obj",
        coffee_machine_path + "Coffee-Machine-Color.png",
        coffee_machine_path + "Coffee-Machine-Normal2.png",
        coffee_machine_path + "Coffee-Machine-Roughness.png",
        coffee_machine_path + "Coffee-Machine-Metal1.png",
        "", coffee_machine_path + "Coffee-Machine-Opacity.jpg");
    shelf = load(shelf_path + "single_wood_shelf.obj",
        shelf_path + "T_shelf.002.png", "", "",
        shelf_path + "metalnessMap1.png","");
    coffee_bag = load(coffee_bag_path + "Coffee_Paper_Bag..obj",
        coffee_bag_path + "initialShadingGroup_BaseColor.1001.png",
        coffee_bag_path + "initialShadingGroup_Normal.1001.png",
        coffee_bag_path + "initialShadingGroup_Roughness.1001.png",
        coffee_bag_path + "initialShadingGroup_Metalness.1001.png");
    light_bulb = load(light_bulb_path + "simple_ceiling_light.obj", 
        light_bulb_path+"simple_ceiling_light_intake_diffuse.png");
    light_fixture = load(light_fixture_path + "LightFixtureRecessed.obj",
        light_fixture_path + "DefaultMaterial_BaseColor.jpg",
        light_fixture_path + "DefaultMaterial_Normal.png",
        light_fixture_path + "DefaultMaterial_Roughness.jpg",
        light_fixture_path + "DefaultMaterial_Metallic.jpg");
    light_switch = load(light_switch_path + "light_swich2.obj",
        light_switch_path + "None_ColorMap.png",
        light_switch_path + "None_NormalMap2.png",
        light_switch_path + "None_RoughnessMap.png",
        light_switch_path + "None_MetalnessMap.png");
    low_torus = addModel(lowpoly_torus_vertices,
        gold_path+"gold-scuffed_basecolor-boosted.png",
        gold_path+"gold-scuffed_normal.png",
        gold_path+"gold-scuffed_roughness.png",
        gold_path+"gold-scuffed_metallic.png");
    high_torus = addModel(highpoly_torus_vertices,
        gold_path + "gold-scuffed_basecolor-boosted.png",
        gold_path + "gold-scuffed_normal.png",
        gold_path + "gold-scuffed_roughness.png",
        gold_path + "gold-scuffed_metallic.png");
    plate = load(plate_path + "plate.obj", plate_path + "white.png");
    mug = load(mug_path + "cup.obj", mug_path + "albedo.jpg", mug_path+"normal.jpg", mug_path+"roughness.jpg");

    // Add light switch to the interactable list of models
    interactableObjects.push_back(light_switch);

    models.at(floor).textures.textureScale = 12.f;
    models.at(counter_top).textures.textureScale = 4.f;
    models.at(wall).textures.textureScale = 4.f;

    setTranformations(table, glm::vec3(5, 0, 0), glm::vec3(0), glm::vec3(0.0225f));
    setTranformations(chair, glm::vec3(7, 0, 0), glm::vec3(0, -90.f, 0), glm::vec3(0.002));
    setTranformations(floor, glm::vec3(0), glm::vec3(0), glm::vec3(100, 0.01, 100));
    setTranformations(counter_table, glm::vec3(0), glm::vec3(0), glm::vec3(1.5, 3, 10));
    setTranformations(counter_top, glm::vec3(0, 1.5, -0.37), glm::vec3(0), glm::vec3(1.75, 0.05, 11));
    setTranformations(coffee_machine, glm::vec3(0, 1.5f, -2.5), glm::vec3(0, -90.f, 0), glm::vec3(0.005));
    setTranformations(wall, glm::vec3(-8, 0, 0), glm::vec3(0), glm::vec3(0.1, 12, 15));
    setTranformations(grinder, glm::vec3(-2, 1.85, -5), glm::vec3(0), glm::vec3(0.0035));
    setTranformations(shelf, glm::vec3(-7.95, 3.5, 0), glm::vec3(0, 90.f, 0), glm::vec3(1.5));
    setTranformations(coffee_bag, glm::vec3(-7.8, 3.52, 0), glm::vec3(0, 90.f, 0), glm::vec3(0.03));
    setTranformations(light_bulb, glm::vec3(5, 7, 0), glm::vec3(0), glm::vec3(0.02f));
    setTranformations(light_fixture, glm::vec3(0, 7.f, -2.5), glm::vec3(0), glm::vec3(0.01));
    setTranformations(light_switch, glm::vec3(-7.94, 2, 7), glm::vec3(0, 0,-90), glm::vec3(0.15));
    setTranformations(low_torus, glm::vec3(4.1, 1.75, -0.1), glm::vec3(90,0,0), glm::vec3(0.085));
    setTranformations(high_torus, glm::vec3(6.01, 1.75, -0.02), glm::vec3(90, 0, 0), glm::vec3(0.085));
    setTranformations(plate, glm::vec3(4.1, -0.05, 0.5), glm::vec3(0), glm::vec3(0.07));
    setTranformations(mug, glm::vec3(4, 1.76, 0.5), glm::vec3(0), glm::vec3(0.0002));


    // Initialise AABB
    for (auto& model : models)
        calculateAABB(model);

    // Initialise the lights shadow maps
    //lights[0].position = glm::vec3(2.f, 6.f, 7.f);
    addDirectionalLight(glm::normalize(glm::vec3(0, -1.0f, -0.3f)), glm::vec3(1), 0.1f);
    addSpotLight(glm::vec3(0.001, -0.99, 0.001), glm::vec3(0, 6.95f, -2.5), rgb2vec(255, 212, 226), 0.75f);
    addSpotLight(glm::vec3(0.001, -0.99, 0.001), glm::vec3(0, 6.95f, 5), rgb2vec(210, 230, 255), 0.75f);
    addPositionalLight(glm::vec3(5, 6, 0), rgb2vec(255,223,142) , 1.f);
    //addPositionalLight(glm::vec3(-5, 0, 0), BLUE, 1.f);
    //addPositionalLight(glm::vec3(0, 0, -5), GREEN, 1.f);
    // Reserve memory for each light
    cubeMapMatrices.reserve(lights.size());
    lightSpaceMatrices.reserve(lights.size());
    printf("Lights size: %d\n", lights.size());

    GLuint program = CompileShader("pbr.vert", "pbr.frag");
    GLuint shadow_program = CompileShader("shadow.vert", "shadow.frag");
    GLuint shadow_cubemap_program = CompileShader("shadowCubeMap.vert", "shadowCubeMap.frag", "shadowCubeMap.geom");

    InitCamera(Camera);

    glCreateBuffers(NUM_BUFFERS, Buffers);
    glGenVertexArrays(NUM_VAOS, VAOs);

    // Initialising buffers
    for (auto& model : models)
        initialiseBuffers(model.vertices, model.bufferIndex);
    /*initialiseBuffers(models.at(table).vertices, models.at(table).bufferIndex);
    initialiseBuffers(models.at(chair).vertices, models.at(chair).bufferIndex);
    initialiseBuffers(models.at(floor).vertices, models.at(floor).bufferIndex);
    initialiseBuffers(models.at(counter_table).vertices, models.at(counter_table).bufferIndex);
    initialiseBuffers(models.at(counter_top).vertices, models.at(counter_top).bufferIndex);
    initialiseBuffers(models.at(wall).vertices, models.at(wall).bufferIndex);
    initialiseBuffers(models.at(grinder).vertices, models.at(grinder).bufferIndex);
    initialiseBuffers(models.at(coffee_machine).vertices, models.at(coffee_machine).bufferIndex);
    initialiseBuffers(models.at(shelf).vertices, models.at(shelf).bufferIndex);
    initialiseBuffers(models.at(coffee_bag).vertices, models.at(coffee_bag).bufferIndex);
    initialiseBuffers(models.at(light_bulb).vertices, models.at(light_bulb).bufferIndex);
    initialiseBuffers(models.at(light_fixture).vertices, models.at(light_fixture).bufferIndex);
    initialiseBuffers(models.at(light_switch).vertices, models.at(light_switch).bufferIndex);
    initialiseBuffers(models.at(torus).vertices, models.at(torus).bufferIndex);*/


    int duplicateID;
	duplicateID = duplicateModel(chair);
    setTranformations(duplicateID, glm::vec3(3, 0, 0), glm::vec3(0, 90.f, 0), glm::vec3(0.002));
    duplicateID = duplicateModel(counter_table);
    setTranformations(duplicateID, glm::vec3(-2.25, 0, -5), glm::vec3(0, -90.f, 0), glm::vec3(1.5, 3, 6));
    duplicateID = duplicateModel(counter_top);
    setTranformations(duplicateID, glm::vec3(-3.125, 1.5, -5), glm::vec3(0, -90.f, 0), glm::vec3(1.75, 0.05, 4.5));
    duplicateID = duplicateModel(shelf);
    setTranformations(duplicateID, glm::vec3(-7.95, 3.5, 5), glm::vec3(0, 90.f, 0), glm::vec3(1.5));
    duplicateID = duplicateModel(shelf);
    setTranformations(duplicateID, glm::vec3(-7.95, 3.5, -5), glm::vec3(0, 90.f, 0), glm::vec3(1.5));
    duplicateID = duplicateModel(coffee_bag);
    setTranformations(duplicateID, glm::vec3(-7.8, 3.52, 0.5), glm::vec3(0, 115.f, 0), glm::vec3(0.03));
    duplicateID = duplicateModel(coffee_bag);
    setTranformations(duplicateID, glm::vec3(-7.8, 3.52, 5), glm::vec3(0, 85.f, 0), glm::vec3(0.03));
    duplicateID = duplicateModel(coffee_bag);
    setTranformations(duplicateID, glm::vec3(-7.65, 3.52, -5), glm::vec3(0), glm::vec3(0.03));
    // Initialise animations
    createAnimation(duplicateID, 10, 20, glm::vec3(-6, 0.1, -4), glm::vec3(-90, 0, 0));
    duplicateID = duplicateModel(light_fixture);
    setTranformations(duplicateID, glm::vec3(0, 7.f, 5), glm::vec3(0), glm::vec3(0.01));
    duplicateID = duplicateModel(light_switch);
    interactableObjects.push_back(duplicateID);
    setTranformations(duplicateID, glm::vec3(-7.94, 2, 6.5), glm::vec3(0, 0, -90), glm::vec3(0.15));
    duplicateID = duplicateModel(light_switch);
    interactableObjects.push_back(duplicateID);
    setTranformations(duplicateID, glm::vec3(-7.94, 2, 6), glm::vec3(0, 0, -90), glm::vec3(0.15));
    duplicateID = duplicateModel(plate);
    setTranformations(duplicateID, glm::vec3(6, -0.05, 0.55), glm::vec3(0), glm::vec3(0.07));

    glEnable(GL_DEPTH_TEST);
    // Resize the vector to match the number of lights
    lightSpaceMatrices.resize(lights.size());
    cubeMapMatrices.resize(lights.size());

    glfwSetTime(0); // Reset glfw time to account for loading time

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
            fpsString = std::to_string((int)(frameCount / (currentTime - lastFpsTime)));
            msString = std::to_string((float)(((currentTime - lastFpsTime) / frameCount) * 1000));

            std::string time = std::to_string(glfwGetTime());
            std::string posX = std::to_string(Camera.Position.x);
            std::string posY = std::to_string(Camera.Position.y);
            std::string posZ = std::to_string(Camera.Position.z);
            std::string newTitle = "Coffee Shop Scene - " + fpsString + "FPS / " + msString.substr(0, 5) + "ms" +
                "\t\t\t" + "X: " + posX + " Y: " + posY + " Z: " + posZ +"\t\t" + +" seconds: "+time;
            glfwSetWindowTitle(window, newTitle.c_str());

            // Reset FPS counter variables
            lastFpsTime = currentTime;
            frameCount = 0;
        }

        // Process keyboard input
        processKeyboard(window, frameTime);

        // Update animation
        updateAnimations(frameTime);

        // Generate a depth map for each light
        for (int i = 0; i < lights.size(); i++)
        {
            // Check if shadow map needs updating i.e. after light has moved
            if (lights[i].shadow.updateShadow)
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
	        		float lightCutoffRadians = acos(sin(glm::radians(25.f)));
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

	        		cubeMapMatrices[i][0] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	        		cubeMapMatrices[i][1] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	        		cubeMapMatrices[i][2] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	        		cubeMapMatrices[i][3] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	        		cubeMapMatrices[i][4] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	        		cubeMapMatrices[i][5] = shadowProj * glm::lookAt(lights[i].position, lights[i].position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	        		// Generate cubemap for this positional light
	        		generateCubeMap(shadow_cubemap_program, lights[i].shadow, cubeMapMatrices[i].data(), far_plane, i);
	        		//++posLightIndex;
	        	}
                lights[i].shadow.updateShadow = false; // Stop the shadow map updating every 
	        }
        }

        renderWithShadows(program, lightSpaceMatrices, cubeMapMatrices, state);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}