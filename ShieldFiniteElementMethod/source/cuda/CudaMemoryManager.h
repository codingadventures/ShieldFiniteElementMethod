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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GPU_CUDA_CUDAMEMORYMANAGER_H
#define SOFA_GPU_CUDA_CUDAMEMORYMANAGER_H

#include <sofa/helper/MemoryManager.h>
#include <cstring>
#include "mycuda.h"
#include "CudaCommon.h"

namespace sofa
{

namespace gpu
{
  
namespace cuda
{

/// CUDA-based GPU MemoryManager
template <class T >
class CudaMemoryManager : public sofa::helper::MemoryManager<T>
{

public :
	typedef T* host_pointer;
	typedef /*mutable*/ void* device_pointer;
	typedef GLuint gl_buffer;

	enum { MAX_DEVICES = 8 };
	enum { BSIZE = ::BSIZE };
	enum { SUPPORT_GL_BUFFER = 1 };

	static int numDevices() {
	    return mycudaGetnumDevices();
	}
	
	static void hostAlloc(void ** hPointer,int n) { 
	    mycudaMallocHost(hPointer,n);
	}
	
	static void memsetHost(host_pointer hPointer, int value,size_t n) { 
	    memset((void*) hPointer, value, n); 
	}
	
	static void hostFree(const host_pointer hSrcPointer) {
	    mycudaFreeHost(hSrcPointer);
	}
	
	static void deviceAlloc(int d,void ** dPointer, int n) {
	    mycudaMalloc(dPointer,n,d);
	}
	
	static void deviceFree(int d,const device_pointer dSrcPointer) {
	    mycudaFree(dSrcPointer,d);
	}
	
	static void memcpyHostToDevice(int d, device_pointer dDestPointer, const host_pointer hSrcPointer, size_t n) {
        //if (mycudaVerboseLevel>=LOG_TRACE) std::cout << "CUDA: CPU->GPU copy of "<< typeid ( *hSrcPointer ).name() <<": "<<n*sizeof(T) <<" B"<<std::endl;
	    mycudaMemcpyHostToDevice(dDestPointer,hSrcPointer,n,d);
	}
	
	static void memcpyDeviceToHost(int d, host_pointer hDestPointer, const void * dSrcPointer , size_t n) {
        //if (mycudaVerboseLevel>=LOG_TRACE) std::cout << "CUDA: GPU->CPU copy of "<< typeid ( *hDestPointer ).name() <<": "<<n*sizeof(T) <<" B"<<std::endl;
	    mycudaMemcpyDeviceToHost(hDestPointer,dSrcPointer,n,d);
	}
	
	static void memcpyDeviceToDevice(int d, device_pointer dDestPointer, const device_pointer dSrcPointer , size_t n) {
	    mycudaMemcpyDeviceToDevice(dDestPointer,dSrcPointer,n,d);
	}
	
	static void memsetDevice(int d, device_pointer dDestPointer, int value,size_t n) {
	    mycudaMemset(dDestPointer,value,n,d);
	}
	
	static int getBufferDevice() {
	    return mycudaGetBufferDevice();
	}
	
	static bool bufferAlloc(gl_buffer* bId, int n) { 
	    if (n > 0) {
	      glGenBuffers(1, bId);
	      glBindBuffer( GL_ARRAY_BUFFER, *bId);
	      glBufferData( GL_ARRAY_BUFFER, n, 0, GL_DYNAMIC_DRAW);
	      glBindBuffer( GL_ARRAY_BUFFER, 0);
	      return true;
	    }  
	    return false;
	}
	
	static void bufferFree(const gl_buffer bId) {
	    glDeleteBuffers( 1, &bId);
	}
	
	static bool bufferRegister(const gl_buffer bId) {
	    mycudaGLRegisterBufferObject(bId);
	    return true; 
	}
	
	static void bufferUnregister(const gl_buffer bId) {
	    mycudaGLUnregisterBufferObject(bId);
	}
	
        static bool bufferMapToDevice(device_pointer * dDestPointer, const gl_buffer bSrcId) {
                mycudaGLMapBufferObject(dDestPointer, bSrcId);
	    return true; 
	}
	
        static void bufferUnmapToDevice(device_pointer * /*dDestPointer*/, const gl_buffer bSrcId) {
            mycudaGLUnmapBufferObject(bSrcId);
	}

	static device_pointer deviceOffset(device_pointer dPointer,size_t offset)
	{
		return (T*)dPointer+offset;
	}

	static device_pointer null(){return NULL;}
	static bool isNull(device_pointer p){return p==NULL;}
};

}

}

}

#endif //SOFA_GPU_CUDA_CUDAMEMORYMANAGER_H
