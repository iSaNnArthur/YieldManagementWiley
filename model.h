#pragma once

#include <ilcplex/ilocplex.h>

void buildModel(
    IloEnv& env,
    IloModel& model,
    IloIntVar& planes
);