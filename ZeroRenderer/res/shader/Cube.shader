#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_lightDirection;

uniform mat4 u_MVP;
uniform mat4 u_ModRotationMatrix;
uniform vec3 u_ModPosition;
uniform vec3 u_LightPosition;

void main()
{
    gl_Position = u_MVP * u_ModRotationMatrix * position;
    v_TexCoord = texCoord;
	
	vec3 pos = vec3(position.x, position.y, position.z);
	v_Normal = normalize(pos - u_ModPosition);
	v_lightDirection = normalize(pos - u_LightPosition);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_lightDirection;

uniform vec4 u_BlendColor;
uniform sampler2D u_Texture;
uniform vec3 u_AmbientColor;
uniform vec3 u_DiffuseColor;
uniform vec3 u_SpecularColor;

void main()
{
    // 计算漫反射光照
    float diffuseIntensity = max(dot(v_Normal, v_lightDirection), 0.0);
    vec3 diffuse = u_DiffuseColor * diffuseIntensity;

    // 计算环境光照
    vec3 ambient = u_AmbientColor;

    // 计算镜面反射光照
    vec3 viewDirection = normalize(vec3(0.0, 0.0, -1.0)); // 观察者方向
    vec3 reflectionDirection = reflect(-v_lightDirection, v_Normal);
    float specularIntensity = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32.0);
    vec3 specular = u_SpecularColor * specularIntensity;

    // 最终颜色输出
    color = vec4(diffuse + ambient + specular, 1.0) * texture(u_Texture, v_TexCoord);
}
