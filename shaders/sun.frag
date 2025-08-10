#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D objectTexture;

void main() {
    vec4 texColor = texture(objectTexture, TexCoord);
    if (texColor.rgb == vec3(0.0)) {
        FragColor = vec4(1.0, 0.8, 0.0, 1.0);
    } else {
        FragColor = texColor * 1.5;
    }
}
