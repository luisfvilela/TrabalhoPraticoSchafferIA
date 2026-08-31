#include <cstdlib>
#include <iostream>
#include "schaffer.hpp"

using namespace std;

int main (int argc, char *argv[]) {
  int maxPoints = 10, maxGen = 300;
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

  Point result = simulaSchaffer(maxGen, maxPoints);
  cout << "\nMelhor" << result << endl;
  
  return 0;
}

