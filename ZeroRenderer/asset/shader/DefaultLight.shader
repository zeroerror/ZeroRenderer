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
uniform sampler2D u_depthMapTexture;

uniform vec3 u_lightColor;
uniform mat4 u_lightMVPMatrix;
uniform vec3 u_lightDirection;
uniform float u_nearPlane;
uniform float u_farPlane;

void main()
{
    vec4 textureColor = texture(u_texture, v_texCoord);
    if(textureColor.a == 0)
    {
        textureColor = vec4(v_texCoord, 0, 1);
    }
    vec4 outColor = textureColor;

    // // - Lambert Light Model -
    // float dot = dot(v_normal, u_lightDirection);
    // float intensity = -min(dot, 0.0);
    // vec3 diffuse = u_lightColor * intensity; 
    // outColor.rgb *= diffuse;

    // // ------ Shadow -------
    // vec4 light_glPos = u_lightMVPMatrix * v_relativePosition;
    // vec2 depthCoord = light_glPos.xy / light_glPos.w * 0.5 + 0.5;
    // float mapDepth = texture(u_depthMapTexture, depthCoord).r;
    // float curDepth = light_glPos.z / light_glPos.w;
    // curDepth = curDepth * 0.5 + 0.5;
    // curDepth = curDepth > 1.0 ? 1.0 : curDepth;
    // float bias = min(0.00002 * (1 + dot), 0.00002);
    // float shadowFactor = curDepth < mapDepth + bias ? 1.0 : 0.0;
    // outColor = vec4(outColor.rgb * shadowFactor, outColor.a);

    // // ------ Ambient Light -------
    // vec3 ambientColor = textureColor.xyz;
    // float ambientStrength = 0.3;
    // vec3 ambient = ambientColor * ambientStrength;
    // outColor.rgb += ambient;

    color = outColor;
}

