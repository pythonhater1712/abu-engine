#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

/*-------------------------*/
/* --- WINDOWING/INPUT --- */
/*-------------------------*/

class Window {
    public:
        Window(uint32_t _width, uint32_t _height, const char* _title) {
            if (!glfwInit()) {
                throw std::runtime_error("glfwInit");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
            if (!window) {
                glfwTerminate();
                throw std::runtime_error("\nwindow");
            }

            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                glfwTerminate();
                throw std::runtime_error("\nglad");
            }

            Window::width = _width;
            Window::height = _height;
        }

        GLFWwindow* GetWindow() {
            return window;
        }

        uint32_t GetHeight() {
            return Window::height;
        }

        uint32_t GetWidth() {
            return Window::width;
        }

        ~Window() {
            glfwDestroyWindow(window);
            Window::window = nullptr;

            glfwTerminate();
        }

    private:
        GLFWwindow* window = nullptr;
        uint32_t width;
        uint32_t height;
};

/*-------------------*/
/* --- RENDERING --- */
/*-------------------*/

class Vertices {
    public:
        float QUAD[20] = {
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, // left top
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f, // right top
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // left bottom
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f  // right bottom
        };

        uint32_t indices[6] {
            0, 2, 3,
            0, 1, 3
        };
};

class Shaders {
    public:
        Shaders() {
                TEXTUREDvertexShaderSrc = readShaderFromFile("shaders\\textured\\texturedVertex.glsl");
                TEXTUREDfragmentShaderSrc = readShaderFromFile("shaders\\textured\\texturedFragment.glsl");

                FLATvertexShaderSrc = readShaderFromFile("shaders\\flat\\flatVertex.glsl");
                FLATfragmentShaderSrc = readShaderFromFile("shaders\\flat\\flatFragment.glsl");

                shaderCompile();

                glAttachShader(texturedShaderProgram, VertexShadertextured);
                glAttachShader(texturedShaderProgram, FragmentShadertextured);
                glLinkProgram(texturedShaderProgram);

                glAttachShader(flatShaderProgram, VertexShaderflat);
                glAttachShader(flatShaderProgram, FragmentShaderflat);
                glLinkProgram(flatShaderProgram);

                glDeleteShader(VertexShaderflat);
                glDeleteShader(FragmentShaderflat);
                glDeleteShader(VertexShadertextured);
                glDeleteShader(FragmentShadertextured);
        }

        ~Shaders() {
            free(FLATvertexShaderSrc);
            free(FLATfragmentShaderSrc);
            free(TEXTUREDvertexShaderSrc);
            free(TEXTUREDfragmentShaderSrc);
        }

    GLuint texturedShaderProgram = glCreateProgram();
    GLuint flatShaderProgram = glCreateProgram();

    private:
        GLuint VertexShadertextured = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShadertextured = glCreateShader(GL_FRAGMENT_SHADER);

        GLuint VertexShaderflat = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderflat = glCreateShader(GL_FRAGMENT_SHADER);

        char* TEXTUREDvertexShaderSrc;
        char* TEXTUREDfragmentShaderSrc;

        char* FLATvertexShaderSrc;
        char* FLATfragmentShaderSrc;
    
        char* readShaderFromFile(const char* shaderPath) {
            FILE* fptr = fopen(shaderPath, "rb");
            if (!fptr) {
                std::cout << "failed to open file, please check " << shaderPath << std::endl;
                return nullptr;
            }

            fseek(fptr, 0, SEEK_END);
            long length = ftell(fptr);
            fseek(fptr, 0, SEEK_SET);

            char* shadersrc = (char*)malloc(length + 1);
            if (!shadersrc) {
                std::cout << "failed to allocate memory for shader source" << std::endl;
                fclose(fptr);
                return nullptr;
            }

            fread(shadersrc, 1, length, fptr);
            shadersrc[length] = '\0';

            return shadersrc;
        }

        void shaderCompile() {
            glShaderSource(VertexShadertextured, 1, &TEXTUREDvertexShaderSrc, nullptr);
            glShaderSource(FragmentShadertextured, 1, &TEXTUREDfragmentShaderSrc, nullptr);

            glShaderSource(VertexShaderflat, 1, &FLATvertexShaderSrc, nullptr);
            glShaderSource(FragmentShaderflat, 1, &FLATfragmentShaderSrc, nullptr);

            glCompileShader(VertexShadertextured); checkShaderCompilation(VertexShadertextured);
            glCompileShader(FragmentShadertextured); checkShaderCompilation(FragmentShadertextured);

            glCompileShader(VertexShaderflat); checkShaderCompilation(VertexShaderflat);
            glCompileShader(FragmentShaderflat); checkShaderCompilation(FragmentShaderflat);

            return;
        }

        void checkShaderCompilation(GLuint shader) {
            int success;
            char info[512];

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 512, nullptr, info);
                std::cout << info << std::endl;
                throw std::runtime_error("Shader couldn't compile.");
            } else {
                std::cout << "Shader is fine" << shader << std::endl;
                std::cout << 
                "(1 = TexturedVertex, 2 = TexturedFragment, 3 = FlatVertex, 4 = FlatFragment)" << std::endl;
            }
        }
};

class Textures {
    public:
        Textures(const char* pathToTexture) {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            stbi_set_flip_vertically_on_load(true);

            data = stbi_load(pathToTexture, &width, &height, &channels, 0);
            if (!data) {
                throw std::runtime_error("failed to load texture");
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        };

        uint32_t GetWidth() {
            return width;
        }

        uint32_t GetHeight() {
            return height;
        }

        void SetTextureSlot1(Shaders& s) {
            uniformLocation = glGetUniformLocation(s.texturedShaderProgram, "texture");
            glUniform1i(uniformLocation, 0);
            return;
        }

        ~Textures() {
            stbi_image_free(data);
        }

    GLuint texture;
    
    private:
        unsigned char* data;
        int width, height, channels;
        GLuint uniformLocation;
};

class Render {
    public:
        Render(Window& w) {
            Vertices v;

            glViewport(0, 0, w.GetWidth(), w.GetHeight());

            glGenBuffers(1, &VBO);
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(v.QUAD), v.QUAD, GL_STATIC_DRAW);
                        
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(v.indices), v.indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0);
        }

        GLuint GetVAO() {
            return VAO;
        }

        void ResetTransform() {
            transform = glm::mat4(1.0f);
        }

        void Scale(float x, float y, float z) {
            transform = glm::scale(transform, glm::vec3(x, y, z));
        }

        void Rotate(float degrees) {
            transform = glm::rotate(transform, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        void Translate(float x, float y) {
            transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
        }

        void Transform(GLuint shaderProgram) {
            GLuint transformLocation = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        }

        ~Render() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

    private:
        GLuint VBO;
        GLuint VAO;
        GLuint EBO;

        glm::mat4 transform = glm::mat4(1.0f);
};

#endif