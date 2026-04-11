#version 330 core
in vec2 vertexTextureCoordinates;

out vec4 fragColor;

uniform sampler2D texture;

void main() {
    fragColor = texture(texture, vertexTextureCoordinates);
}