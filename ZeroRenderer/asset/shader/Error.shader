#shader vertex
#version 330 core

layout(location = 0) in vec4 originPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
uniform vec3 u_modPosition;
uniform vec3 u_modScale;

void main()
{
    vec4 relativePosition = u_modRotationMatrix * originPosition;
    relativePosition.xyz *= u_modScale;
    vec4 glPos = u_mvp * relativePosition;
    gl_Position = glPos;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.5, 0.0, 0.5, 1.0);
}

