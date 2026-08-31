#include <cstdlib>
#include <iostream>
#include "schaffer.hpp"

using namespace std;

int main (int argc, char *argv[]) {
  int maxPoints = 50, maxGen = 300;
  switch (argc) {
      case 2: maxPoints = atoi(argv[1]);
          break;
      case 3: {
          maxPoints = atoi(argv[1]);
          maxGen = atoi(argv[2]);
          break;
      }
      default: cerr << "Utilizando População de tamanho 10 e 300 gerações" << endl;
          break;
  }

  int contadorConvergiu = 0;
  for(int i = 0; i < 100; i++){
      Point result = simulaSchaffer(maxGen,maxPoints);
      if(abs(result.x) <= 0.09 && abs(result.y) <= 0.09)
          contadorConvergiu++;
  }

  cerr << "Total de vezes que convergiu em 100 execuções: " << contadorConvergiu << endl;
  
  return 0;
}

