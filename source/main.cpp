#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <iostream>

#include "../include/obj_loader.h"
#include "../include/shader.h"
#include "../include/camera.h"
#include "../include/texture.h"
#include "../include/light.h"

int width, height;
float last_x, last_y;
bool rotation = false, wireframe = false, first_mouse = false;

Camera *camera = new Camera{glm::vec3{0,0,4}};
GLFWwindow *window;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow *window, double x, double y);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void uploadMatrices(GLuint shader);

void initialize();

void upload_lights_and_position(GLuint shader, Light* light);

int main(int argc, const char *argv[]) {

    initialize();

    Mesh *earth = ObjLoader::load_obj("../models_and_textures/models/sphere.obj");

    Texture *earth_texture = new Texture{"../models_and_textures/textures/earthmap1k.jpg"};

    earth->add_texture(earth_texture->get_texture());

    Light *sun = new Light{
            {0.0, 0.0, 10.0f},
            {255.0f / 255.0f, 255.0f / 255.0f, 160.0f / 255.0f},
            {255.0f / 255.0f, 255.0f / 255.0f, 160.0f / 255.0f},
            glm::mat4{1.0f}};

    //TODO 8: create a second light source

    Shader normal_shader = Shader{"../shader/textured.vert", "../shader/textured.frag"};

    // the main loop where the object are drawn
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);

        glClearColor(0.63f, 0.63f, 0.63f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        uploadMatrices(normal_shader.get_program());
        upload_lights_and_position(normal_shader.get_program(), sun);

        if (rotation) {
            //TODO 11: implement that one light source rotates (for this modify the model matrix of the light)
        }

        earth->draw(normal_shader.get_program());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // unbinding shader program
    glUseProgram(NULL);
    // window is no longer required
    glfwDestroyWindow(window);
    // finish glfw
    glfwTerminate();
    return 0;
}

// this function is called when a key is pressed
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // if the escape key is pressed the window will close
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // if this key is pressed the cube should start rotating around the y-axis
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        if (!rotation)
            rotation = true;
        else
            rotation = false;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (wireframe) {
            wireframe = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            wireframe = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        //TODO 7: if this key is pressed the shading model should change
    }

    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(FORWARD, 0.05f);
        }

    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(BACKWARD, 0.05f);
        }
    }
    //left and right
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(RIGHT, 0.05f);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(LEFT, 0.05f);
        }
    }
}

// this function captures and processes the mouse movement
void mouse_callback(GLFWwindow *window, double x, double y) {
    if (first_mouse) {
        last_x = x;
        last_y = y;
        first_mouse = false;
    }

    float xoffset = x - last_x;
    float yoffset = last_y - y; // reversed since y-coordinates go from bottom to top

    glfwSetCursorPos(window, last_x, last_y);
    camera->ProcessMouseMovement(xoffset, yoffset);
}

// this function is called when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// this function uploads the model, view and projection matrix to the shader if they are defined in the shader
void uploadMatrices(GLuint shader) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width / (float) height, 0.1f, 10000.0f);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
}

//this function uploads values to the shader for further computation
void upload_lights_and_position(GLuint shader, Light* light) {

    int light_position_location = glGetUniformLocation(shader, "light_position");
    int light_diffuse_color_location = glGetUniformLocation(shader, "light_diffuse_color");
    int camera_position_location = glGetUniformLocation(shader, "camera_position");

    glUniform3f(light_position_location, light->Position.x, light->Position.y, light->Position.z);
    glUniform3f(light_diffuse_color_location, light->diffuse_color.x, light->diffuse_color.y, light->diffuse_color.z);
    glUniform3f(camera_position_location, camera->Position.x, camera->Position.y, camera->Position.z);

    //TODO 1: add necessary uploads to implement Blinn-Phong in the fragment shader

    //TODO 4: upload a boolean to specify which shading model (blinn-phong or phong) should be used

    //TODO 9: upload the second light source

}

void initialize() {
    // initialize the GLFW library to be able create a window
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't init GLFW");
    }

    // set the opengl version
    int major = 3;
    int minor = 2;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // create the window
    window = glfwCreateWindow(640, 480, "Lab 2", NULL, NULL);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a window");
    }

    // set the window to the current context so that it is used
    glfwMakeContextCurrent(window);
    // set the frameBufferSizeCallback so that the window adjusts if it is scaled
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // set the keyCallback function so that keyboard input can be used
    glfwSetKeyCallback(window, key_callback);
    // set the mouse callback so that mouse input can be used
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // try to initialise glew to be able to use opengl commands
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error(
                std::string("Could initialize GLEW, error = ") + (const char *) glewGetErrorString(err));
    }

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;


    // opengl configuration
    glEnable(GL_DEPTH_TEST);        // enable depth-testing
    glDepthFunc(GL_LESS);           // depth-testing interprets a smaller value as "closer"
    glfwSwapInterval(false); // disables VSYNC
}