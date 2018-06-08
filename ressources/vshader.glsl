#version 330

//Texture shader

/*
uniform mat4 mvp;

in vec3 vertexPosition; // Position of the vertex
in vec2 texCoord; // Texture Coordinates of the vertex


out vec2 texCoord0;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp * vec4( vertexPosition, 1.0 );

    texCoord0 = texCoord;
}
//! [0]
*/

//Color shader : should be replaced by the texture shader
uniform mat4 mvp;

in vec3 vertexPosition;
in vec3 color;

out vec3 fColor;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp * vec4( vertexPosition, 1.0 );

    fColor = color;
}
//! [0]
