#version 330 core
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

void main()
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;

	vec3 vertexPosition_worldspace =
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	// Output position of the vertex
	gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0f);

	// UV of the vertex. No special space for this one.
	TexCoords = squareVertices.xy + vec2(0.5, 0.5);
	ParticleColor = color;
}
