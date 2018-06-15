#version 330


in vec2 texCoord0;
in vec3 normal0;

uniform vec2 lightBias;
uniform vec3 lightDirection;
uniform sampler2D tex;

out vec4 fragColor;

void main()
{

    if(lightBias.x <= 0.01 && lightBias.y <= 0.01){
        fragColor = texture2D(tex, texCoord0);
    }else{
        vec4 diffuseColour = texture2D(tex, texCoord0);
        vec3 unitNormal = normalize(normal0);
        float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;
        fragColor = vec4((diffuseColour * diffuseLight).xyz,1.0);
    }


}
