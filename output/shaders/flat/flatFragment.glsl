#version 330 core
in vec3 vertexcolor;

out vec4 fragcolor;

void main() {
    fragcolor = vec4(vertexcolor, 1.0);
}