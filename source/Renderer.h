#ifndef RENDERER_H
#define RENDERER_H 

#include "common.h" 
#include "Mesh.h" 
#include "Camera.h"
#include "Shader.h" 

#ifdef CUDA_ENABLED
#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../3rdparty/cudaHelper/helper_cuda.h"
#include "../3rdparty/cudaHelper/helper_math.h"
#endif
#include "Timer.h"
 
using namespace Shaders;
using namespace Rendering;

class Renderer
{
public:
	glm::vec4 m_clearColor;
	GLbitfield m_clearMask;
	int m_screenWidth, m_screenHeight;
	float m_aspectRatio;
#ifdef CUDA_ENABLED
	dim3 m_gridSize, m_blockSize;
#endif
	 
	unsigned int m_currRenderType;
	unsigned int m_currTransferFnType;
	unsigned int m_currVolume;
	Shader *m_debugShader;
	static Camera m_camera;
	Timer m_timer;
	bool m_constructIntersectRay;
	glm::vec3 m_rayStart, m_rayEnd;

	//struct ThreadParameters
	//{
	//	ThreadParameters(int threadId, glm::vec3 &maxPos,  glm::vec3 &dirSample,  const Mesh &cubeMesh,  std::vector<Edge> &transformedEdges, int first, int last);
	//	int threadId;
	//	glm::vec3 &maxPos;
	//	glm::vec3 &dirSample;
	//	//const Mesh &cubeMesh;
	//	//std::vector<Edge> &transformedEdges;
	//	int first;
	//	int last;
	//};

	//PTVEC3 m_crosshairPts;
#ifdef CUDA_ENABLED
	GLuint m_cudaPBO;
	GLuint m_cudaTex;
	struct cudaGraphicsResource *cuda_pbo_resource; // CUDA Graphics Resource (to transfer PBO)
	void loadCudaVolume(const Volume &volume, const TransferFunction &transferFunction);
#endif
	virtual void mainLoop() = 0;
	virtual void setUpdateCallback(void(*updateCallback)(unsigned int,unsigned int, unsigned int));
	virtual void handleInput() = 0;
	void loadDebugShader();
	void renderBasic( Shader *shader, Mesh &mesh, const glm::mat4 &MVP, bool renderWireframe) const;
	//void renderRaycastVR(const Shader *shader, const Mesh &cubeMesh, const Volume &volume, float maxRaySteps, float rayStepSize, float gradientStepSize, const glm::vec3 &lightPosWorld, const TransferFunction &transferFn) const;
	//void renderTextureBasedVR(const Shader *shader, const Mesh &cubeMesh, const Volume &volume, const TransferFunction &transferFn);
	//void renderTextureBasedVRMT(const Shader *shader, const Mesh &cubeMesh, const Volume &volume, const TransferFunction &transferFn);
#ifdef CUDA_ENABLED
	void renderRaycastVRCUDA(const Shader *shader, const Mesh &cubeMesh, const Volume &volume, float maxRaySteps, float rayStepSize, float gradientStepSize);
#endif
	/*void calculateProxyPlanes(ThreadParameters params,std::vector<PTVEC3> &sortedProxyPlanes);
	void sortPolygonClockwise(const PTVEC3 &proxyPlane, glm::vec3 centerPt, PTVEC3 &sortedProxyPlane) const;
	void getClosestPtsOnEdges(const glm::vec3 &maxPos, const glm::vec3 &dirSample, int currSample, const Mesh &cubeMesh, const std::vector<Edge> &transformedEdges, PTVEC3 &proxyPlane, glm::vec3 &centerPt) const;*/

	void drawCrosshair(const glm::vec4 &color) const;
	void incrementRenderType();
	void incrementTransferFnType();
	void incrementVolumeNumber();
	//void renderRaycastVRmobile1(const Shader *shader, const Mesh &cubeMesh, const Camera &camera, const Volume &volume, float maxRaySteps, float rayStepSize, float gradientStepSize, const glm::vec3 &lightPosWorld, const TransferFunction &transferFn) const
	void(*updateCallback)(unsigned int,unsigned int, unsigned int);
	void drawObject(const glm::mat4 &transformMatrix, const std::vector<glm::vec3> &points, GLenum mode, const glm::vec4 &color = glm::vec4(0)) const;
protected:
	//Renderer(float screenWidth, float screenHeight);
	//Renderer();
	void init(float screenWidth, float screenHeight);
	~Renderer();
#ifdef CUDA_ENABLED
	void initPixelBufferCuda();
#endif
	//void getMinMaxPointsVertices(const std::vector<glm::vec3> positions, const Mesh &cubeMesh, glm::vec3 &minPos, glm::vec3 &maxPos) const;
	//void getMinMaxPointsCube(const std::vector<glm::vec3> positions, const Mesh &cubeMesh, glm::vec3 &minPos, glm::vec3 &maxPos) const;
	//void getMinMaxPointsView(const std::vector<glm::vec3> positions, const Mesh &cubeMesh, glm::vec3 &minPos, glm::vec3 &maxPos) const;

	//void writeUniform(GLuint shaderId, const char *uniformName, float val) const;
	//void writeUniform(GLuint shaderId, const char *uniformName, int val) const;
	//void writeUniform(GLuint shaderId, const char *uniformName, glm::vec3 val) const;
	//void writeUniform(GLuint shaderId, const char *uniformName, glm::vec4 val) const;
	//void writeUniform(GLuint shaderId, const char *uniformName, glm::mat3 val) const;
	//void writeUniform(GLuint shaderId, const char *uniformName, glm::mat4 val) const;
	//void writeUniform2DTex(GLuint shaderId, const char *uniformName, unsigned int texUnit, GLuint texId) const;
	//void writeUniform3DTex(GLuint shaderId, const char *uniformName, unsigned int texUnit, GLuint texId) const;

};
#ifdef CUDA_ENABLED
extern "C" void initCuda();
extern "C" void writeTransferFunction(float *transferFunction, int transferFunctionSize);
extern "C" void writeVolume(void *volume, cudaExtent volumeSize);
extern "C" void initCudaVolume(void *volume, cudaExtent volumeSize, GLfloat *transferFunction, int transferFunctionSize);
extern "C" void exitCuda();
extern "C" void render_kernel(dim3 gridSize, dim3 blockSize, uint *d_output, uint imageW, uint imageH, float *invViewMatrix, float aspectRatio, float maxRaySteps, float rayStepSize);
extern "C" void copyInvViewMatrix(float *invViewMatrix, size_t sizeofMatrix);
extern "C" void copyRay(float *rayOrigin, float *rayDirection);
#endif

#endif


