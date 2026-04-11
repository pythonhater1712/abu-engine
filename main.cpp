#include "engine.hpp"

int main() {
    Window w(960, 600, "does it work?");
    Shaders s;
    Render r(w);
    Textures t("textures\\texture.png");

    while (!glfwWindowShouldClose(w.GetWindow())) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t.texture);

        glUseProgram(s.texturedShaderProgram);
        t.SetTextureSlot1(s);

        glBindVertexArray(r.GetVAO());

        r.ResetTransform();
        r.Transform(s.texturedShaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(w.GetWindow());
    }

    glfwTerminate();
    return 0;
}