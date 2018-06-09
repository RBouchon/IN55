#version 330

in vec2 texCoord0;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
    fragColor = texture2D(tex, texCoord0);

}
