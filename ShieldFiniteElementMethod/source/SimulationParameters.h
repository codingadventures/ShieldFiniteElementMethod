#ifndef SimulationParameters_h__
#define SimulationParameters_h__

#include "GPU.h"
enum TimeIntegration
{
	ODE_EulerExplicit = 0,
	ODE_EulerImplicit,
};
struct SimulationParameters
{
	// Time integration
	double timeStep;
	double rayleighMass;
	double rayleighStiffness;
	// CG Solver
	double tolerance;
	// Material properties
	double youngModulusTop,youngModulusBottom;
	double poissonRatio;
	double massDensity;
	// External forces

	double planeRepulsion;
	double sphereRepulsion;
	// Constraints
	double fixedHeight;

	double simulation_time;

	double simulation_size;
	double sphere_radius;

	double plane_size;

	int maxIter;

	TCoord sphere_position0;
	TCoord sphere_position;
	TCoord simulation_bbox[2];
	TCoord simulation_center;
	TCoord plane_position;

	TDeriv sphere_velocity;
	TDeriv gravity;
	TDeriv pushForce;

	TimeIntegration odeSolver;
	bool simulation_mapping_needed;

	SimulationParameters();

	void Init(TCoord boundingBox[2]);

	void Reset();
};

#endif // SimulationParameters_h__