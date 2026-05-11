#include "model.h"
#include "data.h"

#include <iostream>
#include <climits>

ILOSTLBEGIN

void buildModel(
    IloEnv& env,
    IloModel& model,
    IloIntVar& planes
)
{
    // =====================================================
    // PLANES
    // =====================================================

    planes = IloIntVar(env, 0, 6, "planes");

    model.add(planes);

    // =====================================================
    // x1[C][P]
    // =====================================================

    IloArray<IloBoolVarArray> x1(env, C);

    for (int c = 0; c < C; c++) {

        x1[c] = IloBoolVarArray(env, P);

        for (int p = 0; p < P; p++) {

            std::string name =
                "x1_" +
                std::to_string(c) + "_" +
                std::to_string(p);

            x1[c][p].setName(name.c_str());
        }
    }

    // =====================================================
    // x2[S][C][P]
    // =====================================================

    using Bool3D =
        IloArray<IloArray<IloBoolVarArray>>;

    Bool3D x2(env, S);

    for (int s1 = 0; s1 < S; s1++) {

        x2[s1] =
            IloArray<IloBoolVarArray>(env, C);

        for (int c = 0; c < C; c++) {

            x2[s1][c] =
                IloBoolVarArray(env, P);
        }
    }

    // =====================================================
    // x3[S][S][C][P]
    // =====================================================

    using Bool4D =
        IloArray<IloArray<IloArray<IloBoolVarArray>>>;

    Bool4D x3(env, S);

    for (int s1 = 0; s1 < S; s1++) {

        x3[s1] =
            IloArray<IloArray<IloBoolVarArray>>(env, S);

        for (int s2 = 0; s2 < S; s2++) {

            x3[s1][s2] =
                IloArray<IloBoolVarArray>(env, C);

            for (int c = 0; c < C; c++) {

                x3[s1][s2][c] =
                    IloBoolVarArray(env, P);
            }
        }
    }

    // =====================================================
    // sell1[C][S][P]
    // =====================================================

    using Int3D =
        IloArray<IloArray<IloIntVarArray>>;

    Int3D sell1(env, C);

    for (int c = 0; c < C; c++) {

        sell1[c] =
            IloArray<IloIntVarArray>(env, S);

        for (int s = 0; s < S; s++) {

            sell1[c][s] =
                IloIntVarArray(env, P);

            for (int p = 0; p < P; p++) {

                sell1[c][s][p] =
                    IloIntVar(env, 0, INT_MAX);
            }
        }
    }

    // =====================================================
    // sell2[S][C][S][P]
    // =====================================================

    using Int4D =
        IloArray<IloArray<IloArray<IloIntVarArray>>>;

    Int4D sell2(env, S);

    for (int s1 = 0; s1 < S; s1++) {

        sell2[s1] =
            IloArray<IloArray<IloIntVarArray>>(env, C);

        for (int c = 0; c < C; c++) {

            sell2[s1][c] =
                IloArray<IloIntVarArray>(env, S);

            for (int s2 = 0; s2 < S; s2++) {

                sell2[s1][c][s2] =
                    IloIntVarArray(env, P);

                for (int p = 0; p < P; p++) {

                    sell2[s1][c][s2][p] =
                        IloIntVar(env, 0, INT_MAX);
                }
            }
        }
    }

    // =====================================================
    // sell3[S][S][C][S][P]
    // =====================================================

    using Int5D =
        IloArray<IloArray<IloArray<IloArray<IloIntVarArray>>>>;

    Int5D sell3(env, S);

    for (int s1 = 0; s1 < S; s1++) {

        sell3[s1] =
            IloArray<IloArray<IloArray<IloIntVarArray>>>(env, S);

        for (int s2 = 0; s2 < S; s2++) {

            sell3[s1][s2] =
                IloArray<IloArray<IloIntVarArray>>(env, C);

            for (int c = 0; c < C; c++) {

                sell3[s1][s2][c] =
                    IloArray<IloIntVarArray>(env, S);

                for (int s3 = 0; s3 < S; s3++) {

                    sell3[s1][s2][c][s3] =
                        IloIntVarArray(env, P);

                    for (int p = 0; p < P; p++) {

                        sell3[s1][s2][c][s3][p] =
                            IloIntVar(env, 0, INT_MAX);
                    }
                }
            }
        }
    }

    // =====================================================
    // rem1[C][S]
    // =====================================================

    IloArray<IloIntVarArray> rem1(env, C);

    for (int c = 0; c < C; c++) {

        rem1[c] =
            IloIntVarArray(env, S);

        for (int s = 0; s < S; s++) {

            rem1[c][s] =
                IloIntVar(env, 0, INT_MAX);
        }
    }

    // =====================================================
    // rem2[S][S][C]
    // =====================================================

    using Rem3D =
        IloArray<IloArray<IloIntVarArray>>;

    Rem3D rem2(env, S);

    for (int s1 = 0; s1 < S; s1++) {

        rem2[s1] =
            IloArray<IloIntVarArray>(env, S);

        for (int s2 = 0; s2 < S; s2++) {

            rem2[s1][s2] =
                IloIntVarArray(env, C);

            for (int c = 0; c < C; c++) {

                rem2[s1][s2][c] =
                    IloIntVar(env, 0, INT_MAX);
            }
        }
    }

    // =====================================================
    // FUNKCJA CELU
    // =====================================================

    IloExpr obj(env);

    for (int c = 0; c < C; c++) {
        for (int s = 0; s < S; s++) {
            for (int p = 0; p < P; p++) {

                obj +=
                    prob[s]
                    * price[0][c][p]
                    * sell1[c][s][p];
            }
        }
    }

    for (int s1 = 0; s1 < S; s1++) {
        for (int c = 0; c < C; c++) {
            for (int s2 = 0; s2 < S; s2++) {
                for (int p = 0; p < P; p++) {

                    obj +=
                        prob[s1]
                        * prob[s2]
                        * price[1][c][p]
                        * sell2[s1][c][s2][p];
                }
            }
        }
    }

    for (int s1 = 0; s1 < S; s1++) {
        for (int s2 = 0; s2 < S; s2++) {
            for (int c = 0; c < C; c++) {
                for (int s3 = 0; s3 < S; s3++) {
                    for (int p = 0; p < P; p++) {

                        obj +=
                            prob[s1]
                            * prob[s2]
                            * prob[s3]
                            * price[2][c][p]
                            * sell3[s1][s2][c][s3][p];
                    }
                }
            }
        }
    }

    obj -= planeCost * planes;

    model.add(IloMaximize(env, obj));

    obj.end();

    // =====================================================
    // ONE PRICE
    // =====================================================

    for (int c = 0; c < C; c++) {

        IloExpr expr(env);

        for (int p = 0; p < P; p++) {
            expr += x1[c][p];
        }

        model.add(expr == 1);

        expr.end();
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int c = 0; c < C; c++) {

            IloExpr expr(env);

            for (int p = 0; p < P; p++) {
                expr += x2[s1][c][p];
            }

            model.add(expr == 1);

            expr.end();
        }
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int s2 = 0; s2 < S; s2++) {

            for (int c = 0; c < C; c++) {

                IloExpr expr(env);

                for (int p = 0; p < P; p++) {
                    expr += x3[s1][s2][c][p];
                }

                model.add(expr == 1);

                expr.end();
            }
        }
    }

    // =====================================================
    // SELL <= DEMAND
    // =====================================================

    for (int c = 0; c < C; c++) {

        for (int s = 0; s < S; s++) {

            for (int p = 0; p < P; p++) {

                model.add(
                    sell1[c][s][p]
                    <=
                    demand[0][s][c][p] * x1[c][p]
                );
            }
        }
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int c = 0; c < C; c++) {

            for (int s2 = 0; s2 < S; s2++) {

                for (int p = 0; p < P; p++) {

                    model.add(
                        sell2[s1][c][s2][p]
                        <=
                        demand[1][s2][c][p]
                        * x2[s1][c][p]
                    );
                }
            }
        }
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int s2 = 0; s2 < S; s2++) {

            for (int c = 0; c < C; c++) {

                for (int s3 = 0; s3 < S; s3++) {

                    for (int p = 0; p < P; p++) {

                        model.add(
                            sell3[s1][s2][c][s3][p]
                            <=
                            demand[2][s3][c][p]
                            * x3[s1][s2][c][p]
                        );
                    }
                }
            }
        }
    }

    // =====================================================
    // CAPACITY
    // =====================================================

    for (int c = 0; c < C; c++) {

        for (int s = 0; s < S; s++) {

            IloExpr expr(env);

            for (int p = 0; p < P; p++) {
                expr += sell1[c][s][p];
            }

            model.add(expr <= seats[c] * planes);

            model.add(
                rem1[c][s]
                ==
                seats[c] * planes - expr
            );

            expr.end();
        }
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int c = 0; c < C; c++) {

            for (int s2 = 0; s2 < S; s2++) {

                IloExpr expr(env);

                for (int p = 0; p < P; p++) {
                    expr += sell2[s1][c][s2][p];
                }

                model.add(expr <= rem1[c][s1]);

                model.add(
                    rem2[s1][s2][c]
                    ==
                    rem1[c][s1] - expr
                );

                expr.end();
            }
        }
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int s2 = 0; s2 < S; s2++) {

            for (int c = 0; c < C; c++) {

                for (int s3 = 0; s3 < S; s3++) {

                    IloExpr expr(env);

                    for (int p = 0; p < P; p++) {
                        expr += sell3[s1][s2][c][s3][p];
                    }

                    model.add(
                        expr <= rem2[s1][s2][c]
                    );

                    expr.end();
                }
            }
        }
    }

    // =====================================================
    // GLOBAL CAPACITY
    // =====================================================

    int totalSeats = 0;

    for (int c = 0; c < C; c++) {
        totalSeats += seats[c];
    }

    for (int s = 0; s < S; s++) {

        IloExpr expr(env);

        for (int c = 0; c < C; c++) {

            for (int p = 0; p < P; p++) {

                expr += sell1[c][s][p];
            }
        }

        model.add(expr <= planes * totalSeats);

        expr.end();
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int s2 = 0; s2 < S; s2++) {

            IloExpr expr(env);

            for (int c = 0; c < C; c++) {

                for (int p = 0; p < P; p++) {

                    expr += sell2[s1][c][s2][p];
                }
            }

            model.add(expr <= planes * totalSeats);

            expr.end();
        }
    }

    for (int s1 = 0; s1 < S; s1++) {

        for (int s2 = 0; s2 < S; s2++) {

            for (int s3 = 0; s3 < S; s3++) {

                IloExpr expr(env);

                for (int c = 0; c < C; c++) {

                    for (int p = 0; p < P; p++) {

                        expr += sell3[s1][s2][c][s3][p];
                    }
                }

                model.add(expr <= planes * totalSeats);

                expr.end();
            }
        }
    }
}