#ifndef INITHEADER_H
#define INITHEADER_H

#include "common.h" 

#include "simulation.h"
#include "Shader.h"
#include <sofa/helper/system/thread/CTime.h>
#include "Timer.h" 
 
//
Renderer *renderer; 

float currTime, lastTime;
Timer timer;


int main_load()
{
	vector<string> render_filenames;

	render_filenames.push_back("raptor-skin.obj");
	render_filenames.push_back("raptor-misc.obj");


	LOGI("main_load - Load meshes  File: %s Line: %d", __FILE__, __LINE__);

	simulation_load_fem_mesh("raptor-8418.mesh");

	for (unsigned int a = 0; a < render_filenames.size(); ++a)
		simulation_load_render_mesh(render_filenames[a].c_str());

	/*if (reorder)
	simulation_reorder_fem_mesh();*/
	LOGI("main_load - Init simulation  File: %s Line: %d", __FILE__, __LINE__);

	if (!simulation_init())
		return 1;
	return 0;
}

//everything that must access the opengl state must come after the window has been initialized (which come as an event in the command queue in the case of android)
void initAppParams()
{

	auto vertexShaderChar = ArrayConversion<const char *>(2, "vertex.vert" , "common.vert" );
	auto fragmentShader = ArrayConversion<const char *>(2, "fragment.frag" , "common.frag");


	if (!simulation_preload())
		return;

	std::vector<std::string> vertexShaderString(vertexShaderChar.begin(), vertexShaderChar.end());
	std::vector<std::string> fragmentShaderString(fragmentShader.begin(), fragmentShader.end());


	if (main_load()) return;

	
	shader = new Shader(vertexShaderString,fragmentShaderString);

	//currVolume = 0;
	//volumePtr = &volume1;
	//volumetfsPtr = &volume1tfs;

	//currTransferFn = LOW;
	////Volume volume;
	////volume.parseMHD(File("","CT-Knee.mhd"));
	//File basicVS("","basicVS.glsl");
	//File basicFS("","basicFS.glsl");
	//basicShader = ShaderManager::get().getShader(basicVS, basicFS);
	//File raycastVRShaderVS("", "raycastVRShaderVS.glsl");
	//File raycastVRShaderFS("", "raycastVRShaderFS.glsl");
	//raycastVRShader = ShaderManager::get().getShader(raycastVRShaderVS, raycastVRShaderFS);
	//File textureBasedVRShaderVS("", "textureBasedVRShaderVS.glsl");
	//File textureBasedVRShaderFS("", "textureBasedVRShaderFS.glsl");
	//textureBasedVRShader = ShaderManager::get().getShader(textureBasedVRShaderVS, textureBasedVRShaderFS);
	//File planeShaderVS("", "planeVS.glsl");
	//File planeShaderFS("", "planeFS.glsl");
	//planeShader = ShaderManager::get().getShader(planeShaderVS, planeShaderFS);
	//cubeMesh.generateCube(raycastVRShader);
	//planeMesh.generatePlane(planeShader);
	//cubeMesh.transform.scaleUniform(MESH_SCALE);
	//planeMesh.transform.scaleUniform(MESH_SCALE);


	//File vf1("",VOLUME_NAME1);
	//volume1.parseMHD(vf1);
	//volume1.printProperties();
	//volume1.generate();

	//File vf2("",VOLUME_NAME2);
	//volume2.parseMHD(vf2);
	//volume2.printProperties();
	//volume2.generate();

	//File tf11("",TRANSFER_FN_NAME1_LOW);
	//volume1tfs.resize(3);
	//volume1tfs[LOW].parseVoreenXML(tf11);
	//volume1tfs[LOW].generate();
	//File tf12("",TRANSFER_FN_NAME1_MEDIUM);
	//volume1tfs[MEDIUM].parseVoreenXML(tf12);
	//volume1tfs[MEDIUM].generate();
	//File tf13("",TRANSFER_FN_NAME1_HIGH);
	//volume1tfs[HIGH].parseVoreenXML(tf13);
	//volume1tfs[HIGH].generate();

	//File tf21("",TRANSFER_FN_NAME2_LOW);
	//volume2tfs.resize(3);
	//volume2tfs[LOW].parseVoreenXML(tf21);
	//volume2tfs[LOW].generate();
	//File tf22("",TRANSFER_FN_NAME2_MEDIUM);
	//volume2tfs[MEDIUM].parseVoreenXML(tf22);
	//volume2tfs[MEDIUM].generate();
	//File tf23("",TRANSFER_FN_NAME2_HIGH);
	//volume2tfs[HIGH].parseVoreenXML(tf23);
	//volume2tfs[HIGH].generate();

	//cubeMesh.transform.pivotOnLocalAxis(90,0, 0);
	//planeMesh.transform.pivotOnLocalAxis(90,0, 0);

#ifdef CUDA_ENABLED
	renderer->loadCudaVolume(volume1, volume1tfs[LOW]);
#endif
	//renderer->loadDebugShader();

	sdkCreateTimer(&timer.m_timer);
	sdkStartTimer(&timer.m_timer);
	lastTime = currTime = timer.m_timer->getTime();

}

void updateCallback(unsigned int currRenderType, unsigned int newTransferFn, unsigned int newVolume)
{
	/*if(newVolume != currVolume)
	{
	currVolume = newVolume;
	switch(currVolume)
	{
	case 0: volumePtr = &volume1; volumetfsPtr = &volume1tfs; break;
	case 1: volumePtr = &volume2; volumetfsPtr = &volume2tfs; break;
	}
	#if CUDA_ENABLED
	renderer->loadCudaVolume(*volumePtr, volumetfsPtr->at(currTransferFn));
	#endif
	}

	if(newTransferFn != currTransferFn)
	{
	currTransferFn = newTransferFn;
	#if CUDA_ENABLED
	renderer->loadCudaVolume(*volumePtr, volumetfsPtr->at(currTransferFn));
	#endif
	}

	float currTime = timer.m_timer->getTime();
	float delta = currTime - lastTime;
	lastTime = currTime;

	cubeMesh.transform.pivotOnLocalAxis(0,0.00001 * delta, 0);
	planeMesh.transform.pivotOnLocalAxis(0,0.00001 * delta, 0);

	int maxRaySteps = MAX_RAY_STEPS;
	float step_size = RAY_STEP_SIZE_MODEL_SPACE;

	switch (currRenderType)
	{
	case RenderType::RAYTRACE_SHADER:
	renderer->renderRaycastVR(raycastVRShader, cubeMesh, *volumePtr, MAX_RAY_STEPS, RAY_STEP_SIZE_MODEL_SPACE, GRADIENT_STEP_SIZE, LIGHT_POS, (volumetfsPtr->at(currTransferFn)));
	break;
	case RenderType::TEXTURE_BASED:
	renderer->renderTextureBasedVR(textureBasedVRShader, cubeMesh, *volumePtr, (volumetfsPtr->at(currTransferFn)));
	break;
	case RenderType::TEXTURE_BASED_MT:
	renderer->renderTextureBasedVRMT(textureBasedVRShader, cubeMesh, *volumePtr, (volumetfsPtr->at(currTransferFn)));
	break;
	#ifdef CUDA_ENABLED
	case RenderType::RAYTRACE_CUDA:
	renderer->renderRaycastVRCUDA(planeShader, planeMesh, *volumePtr, MAX_RAY_STEPS, RAY_STEP_SIZE_MODEL_SPACE, GRADIENT_STEP_SIZE);
	break;
	#endif*/
	//}
	//renderer->renderBasic(basicShader, cubeMesh, renderer->m_camera.GetProjectionMatrix() * renderer->m_camera.GetViewMatrix() * cubeMesh.transform.getMatrix(), true);
	//renderer->drawCrosshair(glm::vec4(0,0,1,1));
	//renderer->renderBasic(shader,);
}

#endif
