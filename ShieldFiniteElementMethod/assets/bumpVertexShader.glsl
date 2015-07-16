#version 310 
//es
//precision highp float;

//varying vec3 lightVec,lightVec2; 
//varying vec3 eyeVec;
//varying vec2 texCoord;
//attribute vec3 tangent;


layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 texCoord;  
layout (location = 2) in vec3 tangent; 


uniform mat4 mvp;
 
out vec2 tex_coord;

void main(void)
{

	gl_Position = mvp * vec4(position, 1.0);
	tex_coord = texCoord;
	
	//vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	//vec3 t = normalize(gl_NormalMatrix * tangent);
	//vec3 b = cross(n, t);
	
	//vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	//vec3 tmpVec = gl_LightSource[0].position.xyz - vVertex;

	//lightVec.x = dot(tmpVec, t);
	//lightVec.y = dot(tmpVec, b);
	//lightVec.z = dot(tmpVec, n);
    //lightVec = normalize(lightVec);
    //lightVec = t;

	//tmpVec = gl_LightSource[1].position.xyz - vVertex;

	//lightVec2.x = dot(tmpVec, t);
	//lightVec2.y = dot(tmpVec, b);
	//lightVec2.z = dot(tmpVec, n);
    //lightVec2 = normalize(lightVec2);

	//tmpVec = -vVertex;
	//eyeVec.x = dot(tmpVec, t);
	//eyeVec.y = dot(tmpVec, b);
	//eyeVec.z = dot(tmpVec, n);
}
