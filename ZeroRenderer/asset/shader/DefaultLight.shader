#shader vertex
#version 330 core

layout(location = 0) in vec4 originPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
uniform vec3 u_modPosition;
uniform vec3 u_modScale;

uniform vec3 u_lightPosition;

out vec2 v_texCoord;
out vec3 v_normal;
out vec4 v_relativePosition;
out vec4 v_glPos;

void main()
{
    v_texCoord = texCoord;
    vec4 relativePosition = u_modRotationMatrix * originPosition;
    relativePosition.xyz *= u_modScale;
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

uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_depthMap;

uniform vec3 u_lightColor;
uniform mat4 u_lightMVPMatrix;
uniform vec3 u_lightDirection;
uniform float u_nearPlane;
uniform float u_farPlane;
uniform vec3 u_mixedColor;
uniform float u_mixedFactor;

void main()
{
    vec4 diffuseColor = texture(u_diffuseMap, v_texCoord);
    vec4 specularColor = texture(u_specularMap, v_texCoord);
    specularColor.a = 1;
    vec4 texColor = diffuseColor + specularColor;

    vec4 outColor = texColor + vec4(u_mixedColor, 0.0) * u_mixedFactor;
     //- Lambert Light Model -
     float dot = dot(v_normal, u_lightDirection);
     float intensity = -min(dot, 0.0);
     vec3 diffuse = u_lightColor * intensity; 
     outColor.rgb *= diffuse;

    //  //------ Shadow -------
    //  vec4 light_glPos = u_lightMVPMatrix * v_relativePosition;
    //  vec2 depthCoord = light_glPos.xy / light_glPos.w * 0.5 + 0.5;
    //  float mapDepth = texture(u_depthMap, depthCoord).r;
    //  float curDepth = light_glPos.z / light_glPos.w;
    //  curDepth = curDepth * 0.5 + 0.5;
    //  curDepth = curDepth > 1.0 ? 1.0 : curDepth;
    //  float bias = min(0.00002 * (1 + dot), 0.00002);
    //  float shadowFactor = curDepth < mapDepth + bias ? 1.0 : 0.0;
    //  outColor = vec4(outColor.rgb * shadowFactor, outColor.a);

     // ------ Ambient Light -------
     vec3 ambientColor = texColor.xyz;
     float ambientStrength = 0.5;
     vec3 ambient = ambientColor * ambientStrength;
     outColor.rgb += ambient;
    
    color = outColor;
}

