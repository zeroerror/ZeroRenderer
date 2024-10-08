#shader vertex
#version 330 core

layout(location = 0) in vec4 localPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
uniform vec3 u_modPosition;
uniform vec3 u_modScale;

uniform vec3 u_lightPosition;

out vec2 v_texCoord;
out vec3 v_normal;
out vec4 v_worldPos;
out vec4 v_glPos;

void main()
{
    vec4 worldPos = u_modRotationMatrix * localPos;
    worldPos.xyz *= u_modScale;
    vec4 glPos = u_mvp * worldPos;
    gl_Position = glPos;

    v_texCoord = texCoord;
    mat3 u_modRotationMatrix3 = mat3(u_modRotationMatrix);
    v_normal = u_modRotationMatrix3 * normal;
    v_worldPos = worldPos;
    v_glPos = glPos;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec4 v_worldPos;
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

// 贴图采样
vec4 getTextureColor()
{
    vec4 diffuseColor = texture(u_diffuseMap, v_texCoord);
    vec4 specularColor = texture(u_specularMap, v_texCoord);
    specularColor.a = 1;
    vec4 texColor = diffuseColor + specularColor;
    return texColor + vec4(u_mixedColor, 0.0) * u_mixedFactor;
}

// 获取环境光颜色
vec3 getAmbientColor(float ambientIntensity)
{
    vec4 color = getTextureColor();
    vec3 factor = color.rgb * ambientIntensity;
    return factor;
}

// 获取阴影颜色因子
float getShadowFactor()
{
    // 计算光源空间的片段位置
    vec4 lightSpacePos = u_lightMVPMatrix * v_worldPos;
    // 将光源空间的位置转化为标准化设备坐标
    vec3 shadowCoord = lightSpacePos.xyz / lightSpacePos.w;
    shadowCoord = shadowCoord * 0.5 + 0.5; // 从 [-1, 1] 范围转换到 [0, 1]

    float shadow = 0.0;
    float totalSamples = 0.0;

    // 定义采样偏移量
    vec2 texelSize = 1.0 / textureSize(u_depthMap, 0);

    // 进行 PCF 采样，例如 3x3 采样
    int samples = 1;
    for (int x = -samples; x <= samples; x++) 
    {
        for (int y = -samples; y <= samples; y++) 
        {
            vec2 offset = vec2(x, y) * texelSize;
            float sampleDepth = texture(u_depthMap, shadowCoord.xy + offset).r;
            shadow += (shadowCoord.z > sampleDepth + 0.001)? 0.1 : 1.0;
            totalSamples++;
        }
    }

    // 计算平均阴影值
    return shadow / totalSamples;
}

// 计算漫反射
float getDiffuseFactor()
{
    vec3 normal = normalize(v_normal);
    float factor = max(-dot(normal, u_lightDirection), 0.0);
    return factor;
}

void main()
{
    // ============== 贴图采样
    vec4 outColor = vec4(1.0);
    outColor = getTextureColor();
    // // ============== 漫反射
    float diffuseFactor = getDiffuseFactor();
    outColor.rgb *= diffuseFactor;
    // ============== 阴影
    float shadowFactor = getShadowFactor();
    outColor.rgb *= shadowFactor;
    // ============== 环境光
    float ambientIntensity = 0;
    if(shadowFactor==0) ambientIntensity = 0.16;
    else ambientIntensity = 0.3;
    vec3 ambientColor = getAmbientColor(ambientIntensity);
    outColor.rgb += ambientColor;
    // ============== 输出
    color = outColor;
}

