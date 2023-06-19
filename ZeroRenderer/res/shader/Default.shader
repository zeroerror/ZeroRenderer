#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 1) in vec3 normal;

uniform mat4 u_mvp;

void main() {
	vec4 glPos = u_mvp * position;
	gl_Position = glPos;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(1, 1, 1, 1);
}
