#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 myColor;
layout (location = 2) in vec2 mytexCoord;
layout (location = 3) in vec3 vertNormal;

out vec3 fragColor;
out vec2 texCoord;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() { 
	gl_Position =  proj * view * model * vec4(aPos, 1.0);
	fragColor = myColor;
	texCoord = mytexCoord;

	fragPos = vec3(model * vec4(aPos, 1.0));
	fragNormal = vertNormal;
}