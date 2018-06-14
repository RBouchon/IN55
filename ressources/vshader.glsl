#version 330


uniform mat4 mvp;
uniform mat4 boneTransformations[30]; // BoneTransformations array

in vec3 vertexPosition; // Position of the vertex
in vec2 texCoord; // Texture Coordinates of the vertex

in vec4 bonesIndex;  // Index of the bones
in vec4 weight;  // Weight of the bones


out vec2 texCoord0;

//! [0]
void main()
{


    vec4 newVertex;
    // Calculate vertex position with bones influences
    newVertex = (boneTransformations[int(bonesIndex.x)] * vec4(vertexPosition, 1.0)) * weight.x;
    newVertex = (boneTransformations[int(bonesIndex.y)] * vec4(vertexPosition, 1.0)) * weight.y + newVertex;
    newVertex = (boneTransformations[int(bonesIndex.z)] * vec4(vertexPosition, 1.0)) * weight.z + newVertex;
    newVertex = (boneTransformations[int(bonesIndex.w)] * vec4(vertexPosition, 1.0)) * weight.w + newVertex;

    // Calculate vertex position in screen space
    gl_Position = mvp * vec4(newVertex.xyz, 1.0);
    //gl_Position = mvp * vec4(vertexPosition, 1.0);

    texCoord0 = texCoord;
}
//! [0]

