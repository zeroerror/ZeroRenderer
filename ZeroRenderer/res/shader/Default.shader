#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 1) in vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;

out vec2 v_texCoord;

void main() {
	vec4 glPos = u_mvp * u_modRotationMatrix * position;
	gl_Position = glPos;
	v_texCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

void main() {
	color = vec4(v_texCoord,1, 1);
}
