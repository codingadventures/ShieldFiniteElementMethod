/******************************************************************************

*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *

*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *

*                                                                             *

* This library is free software; you can redistribute it and/or modify it     *

* under the terms of the GNU Lesser General Public License as published by    *

* the Free Software Foundation; either version 2.1 of the License, or (at     *

* your option) any later version.                                             *

*                                                                             *

* This library is distributed in the hope that it will be useful, but WITHOUT *

* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *

* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *

* for more details.                                                           *

*                                                                             *

* You should have received a copy of the GNU Lesser General Public License    *

* along with this library; if not, write to the Free Software Foundation,     *

* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *

*******************************************************************************

*                               SOFA :: Modules                               *

*                                                                             *

* Authors: The SOFA Team and external contributors (see Authors.txt)          *

*                                                                             *

* Contact information: contact@sofa-framework.org                             *

******************************************************************************/

#ifndef MYCUDA_H

#define MYCUDA_H



#include "gpucuda.h"

#include <string.h>
 

//#include "../3rdparty/cudaHelper/helper_cuda_gl.h"
 

// #if defined(__cplusplus)

// namespace sofa

// {

// namespace gpu

// {

// namespace cuda

// {

// #endif

enum MycudaVerboseLevel

{

	LOG_NONE = 0,

	LOG_ERR = 1,

	LOG_INFO = 2,

	LOG_TRACE = 3

};



extern "C" {



	extern SOFA_GPU_CUDA_API int  mycudaGetnumDevices();

	extern SOFA_GPU_CUDA_API int  mycudaGetBufferDevice();  

	extern SOFA_GPU_CUDA_API const char* mycudaGetDeviceName();



	extern int SOFA_GPU_CUDA_API mycudaInit(int device=-1);

	extern void SOFA_GPU_CUDA_API mycudaMalloc(void **devPtr, size_t size,int d = mycudaGetBufferDevice());

	extern void SOFA_GPU_CUDA_API mycudaMallocPitch(void **devPtr, size_t* pitch, size_t width, size_t height);

	extern void SOFA_GPU_CUDA_API mycudaFree(void *devPtr,int d = mycudaGetBufferDevice());

	extern void SOFA_GPU_CUDA_API mycudaMallocHost(void **hostPtr, size_t size);

	extern void SOFA_GPU_CUDA_API mycudaFreeHost(void *hostPtr);

	//extern void SOFA_GPU_CUDA_API mycudaMemcpy(void *dst, const void *src, size_t count, enum cudaMemcpyKind kind);

	extern void SOFA_GPU_CUDA_API mycudaMemcpyHostToDevice(void *dst, const void *src, size_t count,int d = mycudaGetBufferDevice());

	extern void SOFA_GPU_CUDA_API mycudaMemcpyDeviceToDevice(void *dst, const void *src, size_t count,int d = mycudaGetBufferDevice());

	extern void SOFA_GPU_CUDA_API mycudaMemcpyDeviceToHost(void *dst, const void *src, size_t count,int d = mycudaGetBufferDevice());

	extern void SOFA_GPU_CUDA_API mycudaMemcpyHostToDevice2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height);

	extern void SOFA_GPU_CUDA_API mycudaMemcpyDeviceToDevice2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height);

	extern void SOFA_GPU_CUDA_API mycudaMemcpyDeviceToHost2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height);



	extern void SOFA_GPU_CUDA_API mycudaGLRegisterBufferObject(int id);

	extern void SOFA_GPU_CUDA_API mycudaGLUnregisterBufferObject(int id);



	extern void SOFA_GPU_CUDA_API mycudaGLMapBufferObject(void** ptr, int id);

	extern void SOFA_GPU_CUDA_API mycudaGLUnmapBufferObject(int id);



	extern void SOFA_GPU_CUDA_API mycudaMemset(void * devPtr, int val , size_t size,int d = mycudaGetBufferDevice());

	extern void SOFA_GPU_CUDA_API mycudaThreadSynchronize();



	//extern void SOFA_GPU_CUDA_API mycudaLogError(const char* err, const char* src);

	//extern int myprintf(const char* fmt, ...);

	extern int mycudaGetMultiProcessorCount();

	//extern void mycudaPrivateInit(int device=-1);



	extern void cuda_void_kernel();



	extern void CudaTetraMapper3f_apply(unsigned int size, const void* map_i, const void* map_f, void* out, const void* in);



	extern void CudaFixedConstraint3f_projectResponseIndexed(unsigned int size, const void* indices, void* dx);



#ifdef SOFA_GPU_CUDA_DOUBLE

	extern void CudaFixedConstraint3d_projectResponseIndexed(unsigned int size, const void* indices, void* dx);

#endif // SOFA_GPU_CUDA_DOUBLE



	extern void CudaMechanicalObject3f_vClear(unsigned int size, void* res);

	extern void CudaMechanicalObject3f_vEqBF(unsigned int size, void* res, const void* b, float f);

	extern void CudaMechanicalObject3f_vPEqBF(unsigned int size, void* res, const void* b, float f);

	extern void CudaMechanicalObject3f_vOp(unsigned int size, void* res, const void* a, const void* b, float f);

	extern void CudaMechanicalObject3f_vIntegrate(unsigned int size, const void* a, void* v, void* x, float h);

	extern void CudaMechanicalObject3f_vPEq1(unsigned int size, void* res, int index, const float* val);

	extern int CudaMechanicalObject3f_vDotTmpSize(unsigned int size);

	extern void CudaMechanicalObject3f_vDot(unsigned int size, float* res, const void* a, const void* b, void* tmp, float* cputmp);





	//extern const char* mygetenv(const char* name);





	extern MycudaVerboseLevel SOFA_GPU_CUDA_API mycudaVerboseLevel;

}



// #if defined(__cplusplus)

// } // namespace cuda

// } // namespace gpu

// } // namespace sofa

// #endif



#endif

