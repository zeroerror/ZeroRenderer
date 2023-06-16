#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_texCoord;
out vec3 v_normal;
out vec4 v_worldPosition;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
uniform vec3 u_modPosition;
uniform vec3 u_lightPosition;

void main()
{
    v_texCoord = texCoord;
    vec4 rotatedPosition = u_modRotationMatrix * position;
    vec4 glPos = u_mvp * rotatedPosition;
    gl_Position = glPos;

    mat3 u_modRotationMatrix3 = mat3(u_modRotationMatrix);
    v_normal = u_modRotationMatrix3 * normal;
    
    vec4 worldPosition = rotatedPosition + vec4(u_modPosition, 0.0);
    v_worldPosition = worldPosition;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec4 v_worldPosition;

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
    vec4 lightViewPos = (u_lightMVPMatrix * v_worldPosition);
    vec3 lightViewMapPos  = lightViewPos.xyz / lightViewPos.w;
    lightViewMapPos  = lightViewMapPos * 0.5f + 0.5f;
    float currentDepth = lightViewMapPos.z ;
    float mapDepth = texture(u_depthMapTexture,lightViewMapPos.xy).r;
    float bias = 0.0f;
    float shadow = currentDepth < (mapDepth + bias) ? 0.f : 1.f;// 当前深度值小于_DepthMap说明不在阴影中
    outColor.rgb *= (1-shadow);

    color = outColor;
}
