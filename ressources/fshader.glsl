#version 330

//FOR TEXTURES
/*
in vec2 texCoord0;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
    fragColor = texture2D(tex, texCoord0);

}
*/

//Color : should be replaced with texture shader
in vec3 fColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4( fColor, 1.0 );
}
