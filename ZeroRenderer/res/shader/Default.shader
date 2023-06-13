#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;

void main()
{
    v_texCoord = texCoord;
	vec4 glPos = u_mvp * u_modRotationMatrix * position;
    gl_Position = glPos;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
    vec4 textureColor = texture(u_texture, v_texCoord);
    color = textureColor;
}
