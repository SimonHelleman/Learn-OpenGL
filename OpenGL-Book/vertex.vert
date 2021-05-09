#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec3 vertexPos;

uniform float offset;


void main()
{
	// Exercise 1 Make the triangle upside down
	// vec3 upsideDown = vec3(aPos.x, aPos.y * -1, aPos.z);
	// gl_Position = vec4(upsideDown, 1.0);

	// book solution is gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
	// gl_Position = vec4(aPos.x + offset, -aPos.y, aPos.z, 1.0); // Excercise 2, use a uniform to offset the xPos
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	vertexPos = aPos;
}