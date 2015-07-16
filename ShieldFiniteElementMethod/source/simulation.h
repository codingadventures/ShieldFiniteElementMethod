#ifndef SIMULATION_H
#define SIMULATION_H
 

#include "FEMMesh.h"
#include "SimulationParameters.h"   
#include "MecanicalMatrix.h"
#include "kernels.h"
#include <mesh/read_mesh_netgen.h> 
#include "Mesh.h"
#include "GPU.h"


class Simulation
{
public:
	bool simulation_preload();
	bool simulation_load_fem_mesh(const char* filename);
	void simulation_reorder_fem_mesh();
	bool simulation_load_render_mesh(const char* filename);
	bool simulation_init();

	void simulation_animate();
	void timeIntegrator_EulerImplicit(const SimulationParameters* params, FEMMesh* mesh);
	void timeIntegrator_EulerExplicit(const SimulationParameters* params, FEMMesh* mesh);
	void computeForce(const SimulationParameters* params, FEMMesh* mesh, TVecDeriv& result);
	void applyConstraints(const SimulationParameters* /*params*/, FEMMesh* mesh, TVecDeriv& result);
	void accFromF(const SimulationParameters* params, FEMMesh* mesh, const TVecDeriv& f);
	void addKv(const SimulationParameters* params, FEMMesh* mesh, double kFactor);
	void mulMatrixVector(const SimulationParameters* params, FEMMesh* mesh, MechanicalMatrix matrix, TVecDeriv& result, const TVecDeriv& input);
	void linearSolver_ConjugateGradient(const SimulationParameters* params, FEMMesh* mesh, MechanicalMatrix matrix);
	template<class TVec> void showDebug(const TVec& v, const char* name);
	void simulation_mapping();
	void simulation_reset();
	void simulation_save();
	void simulation_load();

	FEMMesh* fem_mesh;
	SimulationParameters simulation_params;
	//std::vector<SurfaceMesh*> render_meshes;
	std::vector<Rendering::Mesh>* d_meshes;
	int d_verbose;
	Simulation(int verbose = 0);

	void SetMeshes(std::vector<Rendering::Mesh>* meshes);
};

#endif
