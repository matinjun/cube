#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;


void main()
{
	vec4 realPos;
	float k;

	//  µœ÷“ı”∞
	realPos = model * vec4(aPos, 1.0f);
	k = -lightPos.y / (realPos.y - lightPos.y);
	
	realPos.x = k * realPos.x + (1.0f - k) * lightPos.x;
	realPos.y = k * realPos.y + (1.0f - k) * lightPos.y;
	realPos.z = k * realPos.z + (1.0f - k) * lightPos.z;

	gl_Position = projection * view * realPos;
}
