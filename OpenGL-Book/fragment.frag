#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 vertexPos;
void main()
{
	// FragColor = vec4(ourColor, 1.0);
	// The bottom left corner of the triangle is black since the value of the position contains negative values
	// as such, they are clamped to 0.0f which is black.
	// This will happen all the way to the center sides of the triangle since they will be interpolated positvely again.
	FragColor = vec4(vertexPos, 1.0);
}