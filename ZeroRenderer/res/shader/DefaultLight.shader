#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_lightDirection;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
uniform vec3 u_modPosition;
uniform vec3 u_lightPosition;

void main()
{
    v_texCoord = texCoord;
    vec4 rotatedPosition = u_modRotationMatrix * position;
    gl_Position = u_mvp * rotatedPosition;

    mat3 u_modRotationMatrix3 = mat3(u_modRotationMatrix);
	v_normal = u_modRotationMatrix3 * normal;
    
	vec3 localPos = vec3(rotatedPosition.x, rotatedPosition.y, rotatedPosition.z);
    v_lightDirection = normalize(localPos - u_lightPosition);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_lightDirection;

uniform sampler2D u_texture;
uniform vec3 u_lightColor;

uniform sampler2D u_depthMapTexture;
uniform mat4 u_lightSpaceMatrix;
uniform float u_nearPlane;
uniform float u_farPlane;

float calculateShadowFactor(vec4 fragPosition, vec3 lightDirection)
{
    vec4 lightSpacePosition = u_lightSpaceMatrix * fragPosition;
    lightSpacePosition /= lightSpacePosition.w;

    float currentDepth = lightSpacePosition.z;
    float depthBias = 0.005; // 调整偏差以解决阴影失真问题

    vec2 texCoord = lightSpacePosition.xy * 0.5 + 0.5;
    float shadowMapDepth = texture(u_depthMapTexture, texCoord).r;
    float linearMapDepth = (2.0 * u_nearPlane) / (u_farPlane + u_nearPlane - shadowMapDepth * (u_farPlane - u_nearPlane));
    return linearMapDepth;

    if (currentDepth - depthBias > linearMapDepth)
        return 0.0; // 片段在阴影中
    else
        return 1.0; // 片段不在阴影中
}

void main()
{
    vec4 outColor = texture(u_texture, v_texCoord);

    // - Lambert Light Model -
    float intensity = -min(dot(v_normal, v_lightDirection), 0.0);
    vec3 diffuse = u_lightColor * intensity; 
    outColor = vec4(outColor.rgb * diffuse, outColor.a);

    // - Shadow -
    vec4 fragPosition = vec4(gl_FragCoord.xyz, 1.0); 
    float shadowFactor = calculateShadowFactor(fragPosition, v_lightDirection);
    outColor = vec4(outColor.rgb * shadowFactor, outColor.a);

    color = outColor;
}
