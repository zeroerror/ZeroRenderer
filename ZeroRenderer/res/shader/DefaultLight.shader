#shader vertex
#version 330 core

layout(location = 0) in vec4 originPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_texCoord;
out vec3 v_normal;
out vec4 v_relativePosition;
out vec4 v_glPos;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
uniform vec3 u_modPosition;
uniform vec3 u_lightPosition;

void main()
{
    v_texCoord = texCoord;
    vec4 relativePosition = u_modRotationMatrix * originPosition;
    vec4 glPos = u_mvp * relativePosition;
    gl_Position = glPos;

    mat3 u_modRotationMatrix3 = mat3(u_modRotationMatrix);
    v_normal = u_modRotationMatrix3 * normal;
    
    v_relativePosition = relativePosition;

    v_glPos = glPos;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec4 v_relativePosition;
in vec4 v_glPos;

uniform sampler2D u_texture;
uniform vec3 u_lightColor;

uniform sampler2D u_depthMapTexture;
uniform mat4 u_lightMVPMatrix;
uniform vec3 u_lightDirection;
uniform float u_nearPlane;
uniform float u_farPlane;

void main()
{
    vec4 outColor = texture(u_texture, v_texCoord);

    // - Lambert Light Model -
    float intensity = -min(dot(v_normal, u_lightDirection), 0.0);
    vec3 diffuse = u_lightColor * intensity; 
    outColor.rgb *= diffuse;

    // ------ Shadow -------
    vec4 light_glPos = u_lightMVPMatrix * v_relativePosition;
    vec2 depthCoord = light_glPos.xy / light_glPos.w * 0.5 + 0.5;
    float mapDepth = texture(u_depthMapTexture, depthCoord).r;

    float curDepth = light_glPos.z / light_glPos.w;
    curDepth = curDepth * 0.5 + 0.5;
    curDepth = curDepth > 1.0 ? 1.0 : curDepth;

    float bias = 0.0002;
    float shadow = curDepth < mapDepth + bias ? 0.0 : 1.0;

    outColor = vec4(outColor.rgb * (1-shadow), outColor.a);

    color = outColor;
}
