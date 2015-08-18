#ifndef FEMMesh_h__
#define FEMMesh_h__


#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/helper/vector.h>

#include "GPU.h"
#include <iostream>
#include "SimulationParameters.h"
#include <mesh/write_mesh_obj.h>
#include "common.h"
struct FEMMesh
{
	std::string filename;
	TVecCoord positions;
	TVecTetra tetrahedra;
	TVecTriangle triangles;
	TCoord bbox[2];
	TVecDeriv velocity;
	TVecCoord positions0; // rest positions

	// Description of external forces
	// In a real application, this could be extended to a different force for each particle
	struct ExternalForce
	{
		int index;
		TDeriv value;
	};
	ExternalForce externalForce;

	// Description of constraints
	int nbFixedParticles;
	MyVector(int) fixedParticles;
	MyVector(unsigned int) fixedMask;
	

	// Internal data and methods for simulation
	TVecDeriv f; // force vector when using Euler explicit
	TVecDeriv a,b; // solution and right-hand term when calling CG solver
	TVecDeriv r,d,q; // temporary vectors used by CG solver
#ifdef PARALLEL_REDUCTION
	TVecReal dottmp; // temporary buffer for dot product reductions
#endif

	// PlaneForceField
	GPUPlane<TReal> plane;
	TVecReal planePenetration;

	// SphereForceField
	GPUSphere<TReal> sphere;
	TVecReal spherePenetration;

	// TetrahedronFEMForceField
	MyVector(GPUElement<TReal>) femElem;
	MyVector(GPUElementRotation<TReal>) femElemRotation;
#ifdef PARALLEL_GATHER
	// data for parallel gather operation
	MyVector(GPUElementForce<TReal>) femElemForce;
	int nbElemPerVertex;
	MyVector(int) femVElems;
#endif

#ifdef USE_VEC4
	MyVector(TCoord4) x4,dx4;
#endif

	bool isFixedParticle(int index) const;
	void addFixedParticle(int index);
	void removeFixedParticle(int index);

	void reorder();

	void init(SimulationParameters* params);
	void update(SimulationParameters* params);

	void reset();
	void setPushForce(SimulationParameters* params);

	bool save(const std::string& filename);
	bool load(const std::string& filename);

	void setPushRandomForce(TDeriv pushForce);
	TReal tetraYoungModulus(int index, SimulationParameters* params)
	{
		const TReal youngModulusTop = (TReal)params->youngModulusTop;
		const TReal youngModulusBottom = (TReal)params->youngModulusBottom;
		TTetra t = tetrahedra[index];
		TReal y = (positions0[t[0]][1]+positions0[t[1]][1]+positions0[t[2]][1]+positions0[t[3]][1])*0.25f;
		y = (y - bbox[0][1])/(bbox[1][1]-bbox[0][1]);
		TReal youngModulus = youngModulusBottom + (youngModulusTop-youngModulusBottom) * y;
		return youngModulus;
	}
	TReal tetraPoissonRatio(int index, SimulationParameters* params)
	{
		return (TReal)params->poissonRatio;
	}

	void saveObj(const std::string& filename, const std::string& mtlfilename);

	FEMMesh()
	{
		nbFixedParticles = 0;
		externalForce.index = -1;
		plane.normal_x = 0;
		plane.normal_y = 1;
		plane.normal_z = 0;
		plane.d = 0;
		plane.stiffness = 0;
		plane.damping = 0;
		sphere.center_x = 0;
		sphere.center_y = 0;
		sphere.center_z = 0;
		sphere.velocity_x = 0;
		sphere.velocity_y = 0;
		sphere.velocity_z = 0;
		sphere.radius = 0;
		sphere.stiffness = 0;
		sphere.damping = 0;
#ifdef PARALLEL_GATHER
		nbElemPerVertex = 0;
#endif
	}

	private:
		double d_simulationSize;
};


template<class T1, class T2>
bool SortPairFirstFn(const std::pair<T1,T2>& a, const std::pair<T1,T2>& b) { return a.first < b.first; }


#endif // FEMMesh_h__
