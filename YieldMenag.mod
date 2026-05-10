range T = 1..3;
range C = 1..3;   // 1=First, 2=Business, 3=Economy
range P = 1..3;   // opcje cen
range S = 1..3;   // scenariusze

float prob[S] = ...;
int seats[C] = ...;
int planeCost = ...;

float price[T][C][P] = ...;   // Twoje dane
int   demand[T][S][C][P] = ...; // Twoje dane

// ====================== ZMIENNE ======================
dvar int+ planes;

dvar boolean x1[C][P];                    // cena okres 1
dvar boolean x2[S][C][P];                 // cena okres 2 (zależna od s1)
dvar boolean x3[S][S][C][P];              // cena okres 3 (zależna od s1,s2)

dvar int+ sell1[C][S][P];
dvar int+ sell2[S][C][S][P];
dvar int+ sell3[S][S][C][S][P];

// Zmienne do kumulacyjnej pojemności
dvar int+ rem1[C][S];      // po okresie 1
dvar int+ rem2[S][S][C];   // po okresie 2

// ====================== CEL ======================
maximize
    sum(c in C, s in S, p in P)       prob[s]   * price[1][c][p] * sell1[c][s][p]
  + sum(s1 in S, c in C, s2 in S, p in P) prob[s1]*prob[s2] * price[2][c][p] * sell2[s1][c][s2][p]
  + sum(s1 in S, s2 in S, c in C, s3 in S, p in P) prob[s1]*prob[s2]*prob[s3] * price[3][c][p] * sell3[s1][s2][c][s3][p]
  - planeCost * planes;

// ====================== OGRANICZENIA ======================
subject to {

    // Wybór jednej ceny
    forall(c in C) sum(p in P) x1[c][p] == 1;
    forall(s1 in S, c in C) sum(p in P) x2[s1][c][p] == 1;
    forall(s1 in S, s2 in S, c in C) sum(p in P) x3[s1][s2][c][p] == 1;

    planes <= 6;

    // Sprzedaż <= popyt
    forall(c in C, s in S, p in P) sell1[c][s][p] <= demand[1][s][c][p] * x1[c][p];
    forall(s1 in S, c in C, s2 in S, p in P) sell2[s1][c][s2][p] <= demand[2][s2][c][p] * x2[s1][c][p];
    forall(s1 in S, s2 in S, c in C, s3 in S, p in P) sell3[s1][s2][c][s3][p] <= demand[3][s3][c][p] * x3[s1][s2][c][p];

    // Kumulacyjna pojemność
    forall(c in C, s in S) {
        sum(p in P) sell1[c][s][p] <= seats[c] * planes;
        rem1[c][s] == seats[c]*planes - sum(p in P) sell1[c][s][p];
    }

    forall(s1 in S, c in C, s2 in S) {
        sum(p in P) sell2[s1][c][s2][p] <= rem1[c][s1];
        rem2[s1][s2][c] == rem1[c][s1] - sum(p in P) sell2[s1][c][s2][p];
    }

    forall(s1 in S, s2 in S, c in C, s3 in S)
        sum(p in P) sell3[s1][s2][c][s3][p] <= rem2[s1][s2][c];

    // Całkowita pojemność na samolot
    forall(s in S)
        sum(c in C, p in P) sell1[c][s][p] <= planes * sum(c in C) seats[c];

    forall(s1 in S, s2 in S)
        sum(c in C, p in P) sell2[s1][c][s2][p] <= planes * sum(c in C) seats[c];

    forall(s1 in S, s2 in S, s3 in S)
        sum(c in C, p in P) sell3[s1][s2][c][s3][p] <= planes * sum(c in C) seats[c];
}

// ===== WYŚWIETLANIE WYNIKÓW =====
execute {
   writeln("=============================================================");
   writeln("     YIELD MANAGEMENT - WERSJA FINALNA");
   writeln("=============================================================");
   writeln("Liczba samolotow       = ", planes);
   writeln("Koszt samolotow        = ", planeCost*planes);
   writeln("Oczekiwany zysk netto  = ", cplex.getObjValue());
   writeln("");

   writeln("Ceny w Okresie 1:");
   for(var c in C) for(var p in P) if(x1[c][p]==1)
      writeln("  Klasa ",c," -> Opcja ",p," (",price[1][c][p],")");

   writeln("\nCeny w Okresie 2 (zalezne od Scen1):");
   for(var s1 in S) for(var c in C) for(var p in P) if(x2[s1][c][p]==1)
      writeln("  Scen1=",s1," | Klasa ",c," -> Opcja ",p," (",price[2][c][p],")");

   writeln("\nCeny w Okresie 3 (zalezne od Scen1+Scen2):");
   for(var s1 in S) for(var s2 in S) for(var c in C) for(var p in P) if(x3[s1][s2][c][p]==1)
      writeln("  Scen1=",s1," Scen2=",s2," | Klasa ",c," -> Opcja ",p," (",price[3][c][p],")");

   writeln("\nOczekiwany zysk netto = ", cplex.getObjValue());
   writeln("=============================================================");
}