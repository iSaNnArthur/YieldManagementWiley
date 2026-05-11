#include <ilcplex/ilocplex.h>

#include "model.h"

#include <iostream>

ILOSTLBEGIN

int main()
{
    IloEnv env;

    try {

        std::cout << "Before buildModel\n";

        // =========================================
        // MODEL
        // =========================================

        IloModel model(env);

        // =========================================
        // ZMIENNE GŁÓWNE
        // =========================================

        IloIntVar planes(env, 0, 6, "planes");

        // =========================================
        // BUDOWA MODELU
        // =========================================

        buildModel(
            env,
            model,
            planes
        );

        std::cout << "After buildModel\n";

        // =========================================
        // CPLEX
        // =========================================

        IloCplex cplex(model);

        std::cout << "After IloCplex\n";

        // =========================================
        // EXPORT DEBUG
        // =========================================

        cplex.exportModel("debug.lp");

        std::cout << "After export\n";

        // opcjonalnie:
        // cplex.setOut(env.getNullStream());

        // =========================================
        // SOLVE
        // =========================================

        if (cplex.solve()) {

            std::cout << "\n";
            std::cout << "====================================\n";
            std::cout << "YIELD MANAGEMENT - CONCERT API\n";
            std::cout << "====================================\n";

            std::cout << "Status = "
                << cplex.getStatus()
                << std::endl;

            std::cout << "Planes = "
                << cplex.getValue(planes)
                << std::endl;

            std::cout << "Objective = "
                << cplex.getObjValue()
                << std::endl;

            std::cout << "====================================\n";
        }
        else {

            std::cout << "No solution found.\n";
        }

    }
    catch (IloException& e) {

        std::cout << "CPLEX Exception:\n";
        std::cout << e << std::endl;
    }
    catch (...) {

        std::cout << "Unknown exception.\n";
    }

    env.end();

    return 0;
}