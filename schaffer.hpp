#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[ ";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1) {
            os << ", ";
        }
    }
    os << " ]";
    return os;
}

struct Point {
  double x;
  double y;

  Point(double x, double y ) : x(x), y(y) {}

  // Permite que Point seja impresso via std::cout
  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
      return os << "(" << p.x << ", " << p.y << ")";
  }
};

Point newRandomPoint() {
  // std::random_device gera uma semente baseada no hardware
  static std::random_device rd;
  // std::mt19937 é o algoritmo Mersenne Twister para números pseudoaleatórios
  static std::mt19937 gen(rd());
  // Define a distribuição de ponto flutuante no intervalo [-10.0, 10.0]
  std::uniform_real_distribution<double> dis(-10.0, 10.0);
  return Point(dis(gen), dis(gen));
}

double schaffer(Point p) {
    double x = p.x, y = p.y;
    double r2 = x * x + y * y;
    double sin_term = sin(sqrt(r2));
    double num = (sin_term * sin_term) - 0.5;
    double den = 1.0 + 0.001 * r2;
    return 0.5 - (num / (den * den));
}

double avg(vector<Point> vec){
  double acc = 0.0;
  for(Point p : vec) {
    acc+=schaffer(p);
  }
  return acc/vec.size();
}

// Configuração estática do gerador aleatório
static std::mt19937 gen(std::random_device{}());

// 1. Torneio (k = 3)
Point torneio(const std::vector<Point>& pop) {
    std::uniform_int_distribution<> dist(0, pop.size() - 1);
    
    Point melhor = pop[dist(gen)];
    
    // Sorteia apenas mais 1 competidor (k=2) em vez de 2
    for(int i = 0; i < 1; i++) { 
        Point competidor = pop[dist(gen)];
        if(schaffer(competidor) > schaffer(melhor)) {
            melhor = competidor;
        }
    }
    return melhor;
}

// 2. Cruzamento BLX-0.5 (Blend Crossover)
Point cruzamento(const Point& pai1, const Point& pai2) {
    // Permite extrapolação (-0.5 a 1.5) para evitar o encolhimento da população
    std::uniform_real_distribution<double> dist(-0.5, 1.5);
    
    double betaX = dist(gen);
    double betaY = dist(gen);
    
    return Point(
        betaX * pai1.x + (1.0 - betaX) * pai2.x,
        betaY * pai1.y + (1.0 - betaY) * pai2.y
    );
}

// 3. Mutação Híbrida
void mutacao(Point& p) {
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    
    // 75% das mutações serão saltos grandes (para escapar das armadilhas)
    // 25% serão saltos finos (para acertar a precisão -0.09 a 0.09)
    bool saltoGrande = prob(gen) < 0.75;
    std::normal_distribution<double> gauss(0.0, saltoGrande ? 3.0 : 0.3); 
    
    // Taxa agressiva de exploração
    double taxaMutacao = 0.80; 
    
    if (prob(gen) < taxaMutacao) p.x += gauss(gen);
    if (prob(gen) < taxaMutacao) p.y += gauss(gen);
    
    // Trava nos limites
    p.x = std::clamp(p.x, -10.0, 10.0);
    p.y = std::clamp(p.y, -10.0, 10.0);
}

Point simulaSchaffer(int maxGen , int maxPoints){
    vector<vector <Point>> generations;
  
    //Criação população inicial
    vector<Point> initialGeneration;
    for (int i = 0; i < maxPoints; i++) {
      initialGeneration.push_back(newRandomPoint());
    }
  
    //Criando os dados do fitness a serem avaliados (Melhor, Pior e Médio)
    vector<Point> best;
    vector<Point> worst;
    vector<double> fitness_avg;
    
    //Variável que guardará a geração no loop for
    vector<Point> currentGen = initialGeneration;
    
    for(int gen = 1; gen <= maxGen; gen++){
  
      //Ordena a geração pelo fitness
      std::sort(currentGen.begin(), currentGen.end(), [](const Point& a, const Point& b) {
            return schaffer(a) > schaffer(b);
      });
  
      //Adiciona a geração ao vetor global de gerações
      generations.push_back(currentGen);
  
      best.push_back(currentGen.front());
      worst.push_back(currentGen.back());
      fitness_avg.push_back(avg(currentGen));
  
      //cout << best.front() << "," << worst.back() << "," << avg(currentGen) << endl;
  
      //Gerando a nova geração
      vector<Point> nextGen;
  
      //Elitismo: Adicionamos os 2 melhores a próxima geração
      nextGen.push_back(currentGen[0]);
      nextGen.push_back(currentGen[1]);
  
      //Seleção por torneio, cruzamento e mutação para os 48 restantes
      while (nextGen.size() < maxPoints) {
          // Torneio
          Point pai1 = torneio(currentGen);
          Point pai2 = torneio(currentGen);
  
          // Recombinação
          Point filho = cruzamento(pai1, pai2);
  
          // Mutação
          mutacao(filho);
  
          nextGen.push_back(filho);
      }
  
      //Nova geração vira a geração atual
      currentGen = nextGen;
      
    }
    
    //Mandando os dados para saída em forma de texto
    for (int i = 0; i < maxGen; i++) {
      cout << "\n<------Geração " << i+1 << "------>\n\n"<< generations[i] << endl
      << "Média: "<< fitness_avg[i] << endl
      << "Melhor ("<< schaffer(best[i])<< "): " << best[i] << endl
      << "Pior ("<< schaffer(worst[i])<<"): " << worst[i] << endl;
    }

    return best[maxGen -1];
}
