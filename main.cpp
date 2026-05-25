#include "engine.hpp"

void printMatrix(glm::mat4 matrix) {
    std::cout << "Matrix: ";
    for (int i = 0; i < 4; i++) {
        std::cout << std::endl;
        std::cout << "[" << i << "] ";
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << ", ";
        }
    }
}

static void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "SPACE key pressed" << std::endl;
    }
}

int main() {
    Window w(960, 600, "does it work?");
    Shaders s;
    Render r(w);
    Textures t("textures\\texture.png");

    bool once = true;

    glfwSetKeyCallback(w.GetWindow(), keyCallback);

    while (!glfwWindowShouldClose(w.GetWindow())) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t.texture);

        glUseProgram(s.texturedShaderProgram);
        t.SetTextureSlot1(s);

        glBindVertexArray(r.GetVAO());

        r.ResetTransform();
        r.Scale(0.5f, 0.5f, 0.5f);
        r.Translate(0.66f, 0.75f);
        r.Transform(s.texturedShaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (once == true) {
            once = false;
            printMatrix(r.GetMatrix());
        }

        glfwPollEvents();
        glfwSwapBuffers(w.GetWindow());
    }

    glfwTerminate();
    return 0;
}

/*

what i did:
    made labels in engine.hpp but then deleted them
    made a key callback
    learned to handle input (somewhat)
    found out what is in matrices by printing them
    learned the order of transformation (Translation * Rotation * Scaling)

what i need to do:
    implement movement logic (at least a jump)
    find out if i need to modify the coordinates relative to the existing ones or just translate
*/