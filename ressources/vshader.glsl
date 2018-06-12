#version 330


uniform mat4 mvp;
uniform mat4 boneTransformations[30]; // BoneTransformations array

in vec3 vertexPosition; // Position of the vertex
in vec2 texCoord; // Texture Coordinates of the vertex

in uint bonesIndex[4];  // Index of the bones
in float weight[4];  // Weight of the bones


out vec2 texCoord0;

//! [0]
void main()
{


    vec4 newVertex;
    // Calculate vertex position with bones influences
    newVertex = (boneTransformations[bonesIndex[0]] * vec4(vertexPosition, 0.0)) * weight[0] +
            (boneTransformations[bonesIndex[1]] * vec4(vertexPosition, 0.0)) * weight[1] +
            (boneTransformations[bonesIndex[2]] * vec4(vertexPosition, 0.0)) * weight[2] +
            (boneTransformations[bonesIndex[3]] * vec4(vertexPosition, 0.0)) * weight[3];
   /* for(int i =1; i<4; ++i){
        newVertex = (boneTransformations[bonesIndex[i]] * vec4(vertexPosition, 0.0)) * weight[i] + newVertex;
    }*/

    // Calculate vertex position in screen space
    //gl_Position = mvp * vec4(newVertex.xyz, 1.0);
    gl_Position = mvp * vec4(vertexPosition, 1.0);

    texCoord0 = texCoord;
}
//! [0]

