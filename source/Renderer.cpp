#ifndef Renderer_h__
#define Renderer_h__

#include "Renderer.h"
 #include "File.h"
//#include "HelperFunctions.h"
//#include "ShaderManager.h"

#ifdef CUDA_ENABLED
#include <cuda_gl_interop.h>
#endif



Camera Renderer::m_camera;

//Renderer::Renderer(float screenWidth, float screenHeight)
//	:m_screenWidth(screenWidth), m_screenHeight(screenHeight)
//{
//
//}

void Renderer::init(float screenWidth, float screenHeight)
{ 

	m_currVolume = 0;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_aspectRatio = screenWidth / screenHeight;
	m_clearColor = CLEAR_COLOR;
	m_clearMask = CLEAR_MASK;
	m_constructIntersectRay = false;
	//m_camera.setPosition(INITIAL_CAMERA_POS);
	m_camera.projection = glm::perspective(CAMERA_FOV, screenWidth / screenHeight, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);
	/*m_crosshairPts.push_back(glm::vec3(-0.03f,0,0));
	m_crosshairPts.push_back(glm::vec3(0.03f,0,0));
	m_crosshairPts.push_back(glm::vec3(0,-0.03f,0));
	m_crosshairPts.push_back(glm::vec3(0,0.03f,0));*/
#ifdef CUDA_ENABLED
	m_blockSize = dim3(BLOCK_SIZE, BLOCK_SIZE);
	m_gridSize = dim3(HelperFunctions::iDivUp(m_screenWidth, m_blockSize.x), HelperFunctions::iDivUp(m_screenHeight, m_blockSize.y));
	m_cudaPBO = 0;
	cuda_pbo_resource = NULL;
	initCuda();
	initPixelBufferCuda();
#endif
}


Renderer::~Renderer()
{
#ifdef CUDA_ENABLED
	exitCuda();
#endif
}
#ifdef CUDA_ENABLED
void Renderer::initPixelBufferCuda()
{

	if (m_cudaPBO)
	{
		// unregister this buffer object from CUDA C
		checkCudaErrorsLog(cudaGraphicsUnregisterResource(cuda_pbo_resource));

		// delete old buffer
		glDeleteBuffers(1, &m_cudaPBO);
		glDeleteTextures(1, &m_cudaTex);
	}

	// create pixel buffer object for display
	glGenBuffers(1, &m_cudaPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_cudaPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, m_screenWidth*m_screenHeight*sizeof(GLubyte)*4, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	// register this buffer object with CUDA
	checkCudaErrorsLog(cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, m_cudaPBO, cudaGraphicsMapFlagsWriteDiscard));

	// create texture for display
	glGenTextures(1, &m_cudaTex);
	glBindTexture(GL_TEXTURE_2D, m_cudaTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_screenWidth, m_screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}
#endif
//void Renderer::getMinMaxPointsVertices(const PTVEC3 positions, const Mesh &cubeMesh, glm::vec3 &minPos, glm::vec3 &maxPos) const
//{
//	//assume first point as both min and max
//	float currMin, currMax, distanceToCamera ;
//	currMin = currMax = positions.at(0).z;
//	int currMinIndex = 0, currMaxIndex = 0;
//	for (unsigned int i = 1; i < cubeMesh.m_Vertices.size(); ++i)
//	{
//		const glm::vec3 &vertexView = positions[i];
//		//distanceToCamera = abs(vertexView.z);
//		distanceToCamera = vertexView.z;
//		if(distanceToCamera < currMin)
//		{
//			currMin = distanceToCamera;
//			currMinIndex = i;
//		}
//		else if(distanceToCamera > currMax)
//		{
//			currMax = distanceToCamera;
//			currMaxIndex = i;
//		}
//	}
//	//maxPos = glm::vec3(0, 0, positions.at(currMinIndex).z);
//	//minPos = glm::vec3(0, 0, positions.at(currMaxIndex).z);	
//	maxPos = positions.at(currMinIndex);
//	minPos = positions.at(currMaxIndex);
//
//}

//void Renderer::getMinMaxPointsCube(const PTVEC3 positions, const Mesh &cubeMesh, glm::vec3 &minPos, glm::vec3 &maxPos) const
//{
//	PTVEC3 intersectioPts;
//	glm::vec3 origin = glm::vec3(0);
//	glm::vec3 direction  = glm::vec3(0,0,-1);
//	for (unsigned int i = 0; i < cubeMesh.m_Indices.size(); i +=3)
//	{
//
//		glm::vec3 v1 = positions[cubeMesh.m_Indices[i]];
//		glm::vec3 v2 = positions[cubeMesh.m_Indices[i+1]];
//		glm::vec3 v3 = positions[cubeMesh.m_Indices[i+2]];
//
//		glm::vec3 uvtCoord;
//		//if(glm::intersectRayTriangle(origin, direction , vertexView1, vertexView2, vertexView3, intersectPos))
//		if(HelperFunctions::intersectRayTriangle(v1, v2, v3, true, origin, direction , uvtCoord))
//		{
//			glm::vec3 d1 = (v2 - v1) * uvtCoord.x;
//			glm::vec3 d2 = (v3 - v1) * uvtCoord.y;
//			glm::vec3 intersecPt = v1 + (d1 + d2); 
//			intersectioPts.push_back(intersecPt);
//		}
//	}
//	if(intersectioPts.size() < 2)
//		return;
//
//	float distance1 = glm::length2(intersectioPts.at(0));
//	float distance2 = glm::length2(intersectioPts.at(1));
//	if(distance1 < distance2)
//	{
//		minPos = intersectioPts.at(0);
//		maxPos = intersectioPts.at(1);
//	}
//	else
//	{
//		minPos = intersectioPts.at(1);
//		maxPos = intersectioPts.at(0);
//	}
//
//}
//
//void Renderer::getMinMaxPointsView(const PTVEC3 positions, const Mesh &cubeMesh, glm::vec3 &minPos, glm::vec3 &maxPos) const
//{
//	//assume first point as both min and max
//	float currMin, currMax, distanceToCamera ;
//	currMin = currMax = positions.at(0).z;
//	int currMinIndex = 0, currMaxIndex = 0;
//	for (unsigned int i = 1; i < cubeMesh.m_Vertices.size(); ++i)
//	{
//		const glm::vec3 &vertexView = positions[i];
//		//distanceToCamera = abs(vertexView.z);
//		distanceToCamera = vertexView.z;
//		if(distanceToCamera < currMin)
//		{
//			currMin = distanceToCamera;
//			currMinIndex = i;
//		}
//		else if(distanceToCamera > currMax)
//		{
//			currMax = distanceToCamera;
//			currMaxIndex = i;
//		}
//	}
//	maxPos = glm::vec3(0, 0, positions.at(currMinIndex).z);
//	minPos = glm::vec3(0, 0, positions.at(currMaxIndex).z);	
//	//maxPos = positions.at(currMinIndex);
//	//minPos = positions.at(currMaxIndex);
//
//}

void Renderer::loadDebugShader()
{
	File vs("", std::string(DEBUG_VERTEX_SHADER_NAME));
	File fs("", std::string(DEBUG_FRAGMENT_SHADER_NAME));
	//m_debugShader = ShaderManager::get().getShader(vs,fs);
}



void Renderer::setUpdateCallback(void(*updateCallback)(unsigned int, unsigned int, unsigned int))
{
	this->updateCallback = updateCallback;
} 

void Renderer::renderBasic( Shaders::Shader *shader, Mesh &mesh, const glm::mat4 &MVP, bool renderWireframe) const
{
	glEnable(GL_DEPTH_TEST);
	shader->Use();
	shader->SetUniform("MVP",MVP);
	
	/*if(renderWireframe)
		mesh.renderWireframe();
	else*/
		mesh.Draw(*shader);
	glDisable(GL_DEPTH_TEST);

}
 
#ifdef CUDA_ENABLED
void Renderer::renderRaycastVRCUDA(const Shader *shader, const Mesh &planeMesh, const Volume &volume, float maxRaySteps, float rayStepSize, float gradientStepSize)
{


	glm::mat4 modelMatrix = planeMesh.transform.getMatrix();
	const glm::mat4 &viewMatrix = m_camera.GetViewMatrix();
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	glm::mat3x4 invViewMatrix = glm::mat3x4(glm::transpose(glm::inverse( modelViewMatrix)));


	//copyInvViewMatrix(glm::value_ptr(invViewMatrix), sizeof(float4)*3);


	// map PBO to get CUDA device pointer
	uint *d_output;
	// map PBO to get CUDA device pointer
	checkCudaErrorsLog(cudaGraphicsMapResources(1, &cuda_pbo_resource, 0));
	size_t num_bytes;
	checkCudaErrorsLog(cudaGraphicsResourceGetMappedPointer((void **)&d_output, &num_bytes, cuda_pbo_resource));
	//printf("CUDA mapped PBO: May access %ld bytes\n", num_bytes);

	// clear image
	checkCudaErrorsLog(cudaMemset(d_output, 0, m_screenWidth*m_screenHeight*4));

	// call CUDA kernel, writing results to PBO
	render_kernel(m_gridSize, m_blockSize, d_output, m_screenWidth, m_screenHeight, glm::value_ptr(invViewMatrix), m_aspectRatio, maxRaySteps, rayStepSize);

	getLastCudaErrorLog("kernel failed");

	checkCudaErrorsLog(cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0));

	// display results
	//glClear(GL_COLOR_BUFFER_BIT);

	// draw image from PBO
	//glDisable(GL_DEPTH_TEST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// draw using texture

	// copy from pbo to texture
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_cudaPBO);
	//glBindTexture(GL_TEXTURE_2D, m_cudaTex);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_screenWidth, m_screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_cudaPBO);
	glBindTexture(GL_TEXTURE_2D, m_cudaTex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_screenWidth, m_screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	GLuint shaderId = shader->getId();
	glUseProgram(shaderId);
	writeUniform2DTex(shaderId, "tex", 0, m_cudaTex);
	writeUniform(shaderId, "transformMat", glm::mat4());
	planeMesh.render();

	glBindTexture(GL_TEXTURE_2D, 0);


}
#endif 
//
//void Renderer::drawCrosshair(const glm::vec4 &color) const
//{
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//	drawObject(glm::mat4(), m_crosshairPts, GL_LINES, color);
//}

//
//void Renderer::incrementRenderType()
//{
//	m_currRenderType++;
//	if(m_currRenderType > m_availableRenderTypes.size())
//		m_currRenderType = 0;
//}
//
//void Renderer::incrementTransferFnType()
//{
//	m_currTransferFnType++;
//	if(m_currTransferFnType > m_availableTransferFnType.size()-1)
//		m_currTransferFnType = 0;
//}
//
//void Renderer::incrementVolumeNumber()
//{
//	m_currVolume++;
//	if(m_currVolume > 2)
//		m_currVolume = 0;
//}
//
//void Renderer::drawObject(const glm::mat4 &transformMatrix, const PTVEC3 &points, GLenum mode, const glm::vec4 &color) const
//{
//	GLint currShader;
//	glGetIntegerv(GL_CURRENT_PROGRAM, &currShader);
//	GLuint shaderId = m_debugShader->m_Id;
//	glUseProgram(shaderId);
//
//	GLuint uniformLoc = glGetUniformLocation (shaderId, "MVP");
//	glUniformMatrix4fv (uniformLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
//
//	GLuint vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
//	GLuint position = glGetAttribLocation(shaderId,"position");
//	glEnableVertexAttribArray(position);
//	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
//
//	if(color.w != 0.0f)
//		writeUniform(shaderId, "color", color);
//	
//	glDrawArrays(mode,0, points.size());
//	glBindBuffer(GL_ARRAY_BUFFER,0);
//	glDeleteBuffers(1, &vbo);
//
//	if(color.w != 0.0f)
//		writeUniform(shaderId, "color", glm::vec4(0,0,0,0));
//
//	glUseProgram(currShader);
//}
//
//
//Renderer::ThreadParameters::ThreadParameters(int threadId, glm::vec3 &maxPos, glm::vec3 &dirSample, const Mesh &cubeMesh, std::vector<Edge> &transformedEdges, int first, int last)
//	:threadId(threadId), maxPos(maxPos), dirSample(dirSample), cubeMesh(cubeMesh), transformedEdges(transformedEdges), first(first), last(last)
//{
//
//}

#endif // Renderer_h__
