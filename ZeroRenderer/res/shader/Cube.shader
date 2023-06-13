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
    v_TexCoord = texCoord;
	
	vec3 pos = vec3(position.x, position.y, position.z);
	v_Normal = normalize(u_ModPosition - pos);
	v_lightDirection = normalize(pos - u_LightPosition);

    gl_Position = u_MVP * u_ModRotationMatrix * position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_lightDirection;

uniform sampler2D u_Texture;
uniform vec3 u_LightColor;

void main()
{
    vec4 textureColor = texture(u_Texture, v_TexCoord);
    float intensity = max(dot(v_Normal, v_lightDirection), 0.0);
    vec3 diffuse = u_LightColor * intensity; 

    color = vec4(textureColor.rgb * diffuse, textureColor.a);
}
