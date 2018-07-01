#include <stdio.h>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <HandmadeMath.h>

#include "shaders.h"

#include "Entity.h"
#include "Cube.h"
#include "MeshRenderComponent.h"
#include "FPSCam.h"

void TickTree(Entity *e, float deltaSeconds);
void ComputeModelMatrices(Entity *ep, hmm_mat4 parentModelMatrix);

using std::chrono::high_resolution_clock;

int main()
{
    // Initialise GLFW
    glewExperimental = true; // Needed for core profile
    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("src/vertex.glsl", "src/fragment.glsl");
    if (!programID) {
        return 1;
    }

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint uniformID_M = glGetUniformLocation(programID, "M");
    GLuint uniformID_V = glGetUniformLocation(programID, "V");
    GLuint uniformID_MVP = glGetUniformLocation(programID, "MVP");

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    Cube c1 = Cube();

    Entity monkey = Entity();
    monkey.position = HMM_Vec3(2.1f, 0.0f, 0.0f);
    monkey.renderComponent = new MeshRenderComponent("MonkeySmooth.obj");

    FPSCam fpsCam = FPSCam();
    fpsCam.position = HMM_Vec3(-3.0f, 1.0f, 1.0f);

    Entity *cam = &fpsCam.cam;

    // Cube c = Cube();
    // monkey.position = HMM_Vec3(2.1f, 0.0f, 0.0f);

    // monkey.AddChild(&c);
    
    c1.AddChild(&monkey);

    Entity root = Entity();
    root.AddChild(&c1);
    root.AddChild(&fpsCam);

    Entity axes = Entity();
    axes.renderComponent = new MeshRenderComponent("Axes.obj");
    root.AddChild(&axes);

    bool hasTicked = false;
    high_resolution_clock::time_point lastTickTime;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tick
        auto now = high_resolution_clock::now();
        if (hasTicked) {
            auto elapsedNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTickTime).count();
            float elapsedSeconds = elapsedNanoseconds / 1000000000.0f;
            TickTree(&root, elapsedSeconds);
        }
        lastTickTime = now;
        hasTicked = true;

        // Compute model positions for rendering
        ComputeModelMatrices(&root, HMM_Mat4d(1.0f));

        // Render!
        hmm_mat4 projection = cam->projectionMatrix();
        hmm_mat4 view = cam->viewMatrix();
        hmm_mat4 vp = projection * view;

        auto it = EntityIterator(&root);
        while (it.HasNext()) {
            Entity *e = it.Next();

            if (e->renderComponent) {
                // Use our shader
                glUseProgram(programID);

                // Send uniforms
                glUniformMatrix4fv(uniformID_M, 1, GL_FALSE, &e->modelMatrix.Elements[0][0]);

                glUniformMatrix4fv(uniformID_V, 1, GL_FALSE, &view.Elements[0][0]);

                hmm_mat4 mvp = vp * e->modelMatrix;
                glUniformMatrix4fv(uniformID_MVP, 1, GL_FALSE, &mvp.Elements[0][0]);

                e->renderComponent->Draw();
            }
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (
        // Check if the ESC key was pressed or the window was closed
        glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
        && glfwWindowShouldClose(window) == 0
    );
}

void TickTree(Entity *e, float deltaSeconds) {
    e->Tick(deltaSeconds);

    for (auto child : e->children) {
        TickTree(child, deltaSeconds);
    }
}

void ComputeModelMatrices(Entity *e, hmm_mat4 parentModelMatrix) {
    e->parentModelMatrix = parentModelMatrix;
    e->modelMatrix = parentModelMatrix * HMM_Translate(e->position) * HMM_QuaternionToMat4(e->rotation) * HMM_Scale(e->scale);

    for (int i = 0; i < e->children.size(); i++) {
        ComputeModelMatrices(e->children[i], e->modelMatrix);
    }
}
