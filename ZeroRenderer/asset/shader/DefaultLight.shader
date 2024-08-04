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

void main()
{
    // ============== 贴图
    vec4 diffuseColor = texture(u_diffuseMap, v_texCoord);
    vec4 specularColor = texture(u_specularMap, v_texCoord);
    specularColor.a = 1;
    vec4 texColor = diffuseColor + specularColor;
    vec4 outColor = texColor + vec4(u_mixedColor, 0.0) * u_mixedFactor;
    
    // ============== 漫反射
    // vec3 normal = normalize(v_normal); // 规范化法线向量
    // float dotNL = max(dot(normal, -u_lightDirection), 0.0); // 计算法线与光方向的点积，并取最大值
    // vec3 diffuse = u_lightColor * dotNL; // 计算漫反射分量
    // outColor.rgb *= diffuse; // 应用漫反射到输出颜色
    
    // ============== 深度贴图阴影
    // 计算光源空间的片段位置
    vec4 lightSpacePos = u_lightMVPMatrix * v_worldPos;
    // 将光源空间的位置转化为标准化设备坐标
    vec3 shadowCoord = lightSpacePos.xyz / lightSpacePos.w;
    shadowCoord = shadowCoord * 0.5 + 0.5; // 从 [-1, 1] 范围转换到 [0, 1]
    // 取样深度贴图
    float sampleDepth = texture(u_depthMap, shadowCoord.xy).r;
    // 判断当前片段是否在阴影中
    float curDepth = -shadowCoord.z;
    float shadow = (curDepth > sampleDepth + 0.005) ? 0.5 : 1.0;
    // 应用阴影
    outColor.rgb *= shadow;

    // 输出
    color = outColor;

if (isnan(lightSpacePos.x))  color.rgb = vec3(1, 0, 0); // 表示 NaN 值

}
