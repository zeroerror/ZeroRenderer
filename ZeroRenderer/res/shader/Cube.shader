#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

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
	
	vec3 pos = vec3(position.x, position.y, position.z);
	v_normal = normalize(pos - u_modPosition);
    v_lightDirection = normalize(pos - u_lightPosition);

    gl_Position = u_mvp * u_modRotationMatrix * position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_lightDirection;

uniform sampler2D u_texture;
uniform vec3 u_lightColor;

void main()
{
    vec4 textureColor = texture(u_texture, v_texCoord);
    float intensity = -min(dot(v_normal, v_lightDirection), 0.0);
    vec3 diffuse = u_lightColor * intensity; 

    color = vec4(textureColor.rgb * diffuse, textureColor.a);
    color = vec4(-dot(v_normal, v_lightDirection),-dot(v_normal, v_lightDirection),-dot(v_normal, v_lightDirection),1);
}
