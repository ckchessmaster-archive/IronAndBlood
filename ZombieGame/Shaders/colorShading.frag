#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor; //RGBA color
in vec2 fragmentUV;

//This is the 3 component float vector that gets outputted to the screen for each pixel.
out vec4 color;

uniform sampler2D mySampler;

void main() {
	
	
	vec4 textureColor = texture(mySampler, fragmentUV);
	
	//Make crazy colors using time and position (cos(x) returns a number between -1 and 1.
	//To convert it into the range 0 to 1 you simply do (cos(x) + 1.0) * 0.5)
	color = fragmentColor * textureColor;
}