#ifndef GPU_h__
#define GPU_h__

#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/helper/vector.h>

#if defined(SOFA_DEVICE_CPU)
#define SOFA_DEVICE "CPU"
#elif defined(SOFA_DEVICE_CUDA)
#define SOFA_DEVICE "CUDA"
#else
#error Please define SOFA_DEVICE_CPU or SOFA_DEVICE_CUDA
#endif

#if defined(SOFA_DEVICE_CUDA)
#include "cuda/CudaMemoryManager.h"
using namespace sofa::gpu::cuda;
#define MyMemoryManager sofa::gpu::cuda::CudaMemoryManager
#else
enum { BSIZE=1 };
#define MyMemoryManager sofa::helper::CPUMemoryManager
#endif
// we can't use templated typedefs yet...
#define MyVector(T) sofa::helper::vector<T,MyMemoryManager< T > >

// Flag to use 2x3 values instead of 3x3 for co-rotational FEM rotation matrices
#define USE_ROT6

#if defined(SOFA_DEVICE_CUDA)
#define PARALLEL_REDUCTION
#define PARALLEL_GATHER
#define USE_VEC4
#endif

typedef float TReal;
typedef sofa::defaulttype::Vec<3,TReal> TCoord;
typedef sofa::defaulttype::Vec<3,TReal> TDeriv;
typedef sofa::defaulttype::Vec<4,TReal> TCoord4;
typedef MyVector(TReal) TVecReal;
typedef MyVector(TCoord) TVecCoord;
typedef MyVector(TDeriv) TVecDeriv;

typedef sofa::helper::fixed_array<unsigned int, 3> TTriangle;
typedef sofa::helper::fixed_array<unsigned int, 4> TTetra;
typedef MyVector(TTriangle) TVecTriangle;
typedef MyVector(TTetra) TVecTetra;

typedef sofa::defaulttype::Vec<2,float> TTexCoord;
typedef sofa::defaulttype::Vec<4,float> TColor;
typedef MyVector(TTexCoord) TVecTexCoord;

typedef sofa::defaulttype::Vec<3,float> Vec3f;
typedef sofa::defaulttype::Vec<3,double> Vec3d;
typedef sofa::defaulttype::Vec<4,float> Vec4f;
typedef sofa::defaulttype::Vec<4,double> Vec4d;
typedef sofa::defaulttype::Vec<4,int> Vec4i;
typedef sofa::defaulttype::Mat<3,3,float> Mat3x3f;
typedef sofa::defaulttype::Mat<3,3,double> Mat3x3d;

template<class real>
struct GPUElement
{
	/// index of the 4 connected vertices
	//Vec<4,int> tetra;
	int ia[BSIZE];
	int ib[BSIZE];
	int ic[BSIZE];
	int id[BSIZE];
	/// material stiffness matrix
	//Mat<6,6,Real> K;
	real gamma_bx2[BSIZE], mu2_bx2[BSIZE];
	/// initial position of the vertices in the local (rotated) coordinate system
	//Vec3f initpos[4];
	real bx[BSIZE],cx[BSIZE];
	real cy[BSIZE],dx[BSIZE],dy[BSIZE],dz[BSIZE];
	/// strain-displacement matrix
	//Mat<12,6,Real> J;
	real Jbx_bx[BSIZE],Jby_bx[BSIZE],Jbz_bx[BSIZE];
};

template<class real>
struct GPUElementRotation
{
#ifdef USE_ROT6
	real rx[3][BSIZE];
	real ry[3][BSIZE];
#else
	real r[9][BSIZE];
#endif
};

template<class real>
struct GPUElementForce
{
	sofa::defaulttype::Vec<4,real> fA,fB,fC,fD;
};


template<class real>
struct GPUPlane
{
	real normal_x, normal_y, normal_z;
	real d;
	real stiffness;
	real damping;
};

template<class real>
struct GPUSphere
{
	real center_x, center_y, center_z;
	real velocity_x, velocity_y, velocity_z;
	real radius;
	real stiffness;
	real damping;
};
#endif // GPU_h__