#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
uniform vec3 u_modPosition;

void main()
{
    v_texCoord = texCoord;
    vec4 rotatedPosition = u_modRotationMatrix * position;
    gl_Position = u_mvp * rotatedPosition;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
uniform sampler2D u_depthMapTexture;
uniform float u_nearPlane;
uniform float u_farPlane;

void main()
{
    float depthValue = texture(u_depthMapTexture, v_texCoord).r;
    float linearDepth = 2.0 * u_nearPlane / (u_farPlane + u_nearPlane - depthValue * (u_farPlane - u_nearPlane));
    vec3 depthColor = vec3(linearDepth);
    color = vec4(depthColor, 1.0);

}
