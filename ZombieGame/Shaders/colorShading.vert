#version 130
//The vertex shader operates on each vertex

//input data from the VBO.
in vec2 vertexPosition;  //Each vertex is 2 floats
in vec4 vertexColor;  //RGBA color
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor; //RGBA color
out vec2 fragmentUV;

uniform mat4 P;

void main() {
	//Set the x,y position on the screen
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy; 
	//the z position is 0 since we are in 2D
	gl_Position.z = 0.0; 
	//Indicate that the coordinates are normalized
	gl_Position.w = 1.0; 
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	
	//flip the uv coordinates (openGL has a wierd UV coordinates system that flips v so things appear upside down)
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y); 
}