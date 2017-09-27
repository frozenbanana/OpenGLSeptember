#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vertexData
{
	vec3 normal;
	vec2 texCoord;
} g_inData[3];

out vertexData
{
	vec3 normal;
	vec2 texCoord;
} g_outData;

uniform vec3 camPos;

void processPrimitive();
bool cullPrimitive();

void main(){
	if(cullPrimitive()){
		processPrimitive();
	}
}

void processPrimitive(){
	for(int i = 0; i < gl_in.length(); i++){

		gl_Position = gl_in[i].gl_Position;								

		g_outData.normal = g_inData[i].normal;
		g_outData.texCoord = g_inData[i].texCoord;

		EmitVertex();
	}
	EndPrimitive();

}

bool cullPrimitive(){

	bool doCull = true;		//A variable to hold the return value

	vec3 cornerVer;			//The vertex that is placed in the corner of the triangle
	vec3 edge1;				//First edge of the triangle
	vec3 edge2;				//Second edge of the triangle
	vec3 cullNorm;			//The normal of the triangle
	vec3 cullCam;			//The vector from the corner to the camera
	float cullResult;		//coefficient that tells if normal is seen by camera

	cornerVer = vec3(gl_in[0].gl_Position);
	
	edge1 = vec3(gl_in[1].gl_Position) - cornerVer;
	edge2 = vec3(gl_in[2].gl_Position) - cornerVer;

	cullNorm = normalize(cross(edge1, edge2));

	cullCam = normalize(cornerVer - camPos);	// here should cam-front go

	cullResult = dot(cullNorm, cullCam);

	if(cullResult > 0.000001f){
		//If the dot-product is positive the triangle should not be culled
		doCull = false;
	}

	return doCull;
}