#include <iostream>

#include <ilcplex/ilocplex.h>
#include <ilopl/iloopl.h>

ILOSTLBEGIN

int main()
{
    IloEnv env;

    try {

        IloOplErrorHandler errHandler(env);

        IloOplModelSource modelSource(env, "YieldMenag.mod");

        IloOplSettings settings(env, errHandler);

        IloOplModelDefinition def(modelSource, settings);

        IloCplex cplex(env);

        IloOplModel opl(def, cplex);

        IloOplDataSource dataSource(env, "YieldMenag.dat");

        opl.addDataSource(dataSource);

        opl.generate();

        if (cplex.solve()) {

            opl.postProcess();

            std::cout << "OBJ = "
                << cplex.getObjValue()
                << std::endl;
        }
        else {
            std::cout << "No solution\n";
        }

    }
    catch (IloException& e) {
        std::cerr << e << std::endl;
    }

    env.end();

    return 0;
}