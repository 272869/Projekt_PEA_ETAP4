#include "GeneticAlgorithm.h"
#include "Timer.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;
// Metoda inicjująca algorytm genetyczny
void GeneticAlgorithm::startAlgorithm(double probability, int populationCopyNumber, double maxTime, int crossoverType, double crossoverCoefficient) {
    random_device rd; // Generowanie losowego źródła
    mt19937 gen(rd()); // Inicjalizacja silnika RNG
    generateParents(); // Generowanie początkowej populacji
    mainLoop(gen, probability,populationCopyNumber, maxTime, crossoverType, crossoverCoefficient); // Rozpoczęcie głównej pętli algorytmu
}

// Główna pętla algorytmu genetycznego
void GeneticAlgorithm::mainLoop(mt19937 &engine, double probability, int populationCopyNumber,double maxTime, int crossoverType, double crossoverCoefficient) {
    pair<int, int> parents;
    auto pointer1 = population.begin(); // Wskaźnik na pierwszego rodzica
    auto pointer2 = population.begin(); // Wskaźnik na drugiego rodzica
    uniform_real_distribution<double> crossoverChance(0.0, 1.0); // Rozkład do określania szansy na krzyżowanie
    Timer timer;
    timer.start(); // Uruchomienie zegara do mierzenia czasu
    uint64_t elapsedTime = 0;

    int iter = 0; // Licznik iteracji
    while (elapsedTime < static_cast<uint64_t>(maxTime * 1000000)) { // Pętla wykonuje się, dopóki czas trwania nie przekroczy maksymalnego czasu
        int populationSize = matrixWeights->size * 2;
        for (int j = 0; j < populationSize; j++) {
            vector<unsigned> child1(matrixWeights->size, 0); // Inicjalizacja pierwszego dziecka
            vector<unsigned> child2(matrixWeights->size, 0); // Inicjalizacja drugiego dziecka

            // Selekcja metodą koła ruletki
            sort(population.begin(), population.end(), cmp); // Sortowanie populacji według dopasowania
            vector<float> fitnessValueTable(population.size(), 0);
            countFitnessValue(fitnessValueTable); // Obliczanie wartości dopasowania
            parents = rouletteWheelSelection(engine, fitnessValueTable); // Wybór rodziców

            pointer1 = population.begin() + parents.first; // Ustawienie wskaźnika na pierwszego rodzica
            pointer2 = population.begin() + parents.second; // Ustawienie wskaźnika na drugiego rodzica

            // Krzyżowanie
            if (crossoverChance(engine) <= crossoverCoefficient) { // Decyzja o przeprowadzeniu krzyżowania
                switch (crossoverType) {
                    case 0:
                        makePartiallyMappedCrossover(pointer1->second, pointer2->second, child1, child2, engine); // PMX
                        break;
                    case 1:
                        makeOrderCrossoverOperator(pointer1->second, pointer2->second, child1, child2, engine); // OX
                        break;
                }
            } else {
                child1 = pointer1->second; // Przekazanie genotypu rodzica bez zmian
                child2 = pointer2->second;
            }

            checkMutation(engine, child1, probability, iter * populationSize + j); // Mutacja pierwszego dziecka
            checkMutation(engine, child2, probability, iter * populationSize + j); // Mutacja drugiego dziecka
        }
        sort(population.begin(), population.end(), cmp); // Sortowanie populacji po zakończeniu iteracji
        copyPopulation(populationSize); // Zachowanie najlepszych osobników
        iter++;
        timer.stop();
        elapsedTime = timer.timeperiod(); // Aktualizacja czasu trwania algorytmu
    }
    showPath(globalPath); // Wyświetlenie najlepszej ścieżki
    showPRD(elapsedTime); // Wyświetlenie odchylenia PRD
}

// Funkcja obliczania wartości zdatności (dopasowania)
void GeneticAlgorithm::countFitnessValue(vector<float> &fitness) {
    for (int i = 0; i < population.size(); i++) {
        fitness.at(i) = ((float) finalCost / (population.begin() + i)->first); // Obliczanie wartości fitness dla każdego osobnika
    }
}

// Selekcja metodą koła ruletki
pair<int, int> GeneticAlgorithm::rouletteWheelSelection(mt19937 &engine, vector<float> &fitness) {
    float sum = accumulate(fitness.begin(), fitness.end(), 0.0f); // Obliczanie sumy wartości fitness
    uniform_real_distribution<float> randomNumber(0, sum); // Rozkład losowy do wyboru rodziców
    float r1 = randomNumber(engine);
    float r2 = randomNumber(engine);
    if (r2 < r1) swap(r1, r2); // Zamiana losowych liczb w celu uporządkowania

    pair<int, int> parents(-1, 0); // Indeksy rodziców
    float sum2 = 0;

    for (int i = 0; i < population.size(); i++) {
        sum2 += fitness.at(i);
        if (r1 <= sum2 && parents.first == -1)
            parents.first = i; // Wybranie pierwszego rodzica
        else if (r2 <= sum2) {
            parents.second = i; // Wybranie drugiego rodzica
            return parents;
        }
    }
    return parents;
}

// Funkcja sprawdzająca mutację
void GeneticAlgorithm::checkMutation(mt19937 engine, vector<unsigned int>& child, double probability, uint64_t time) {
    population.emplace_back(calculateCost(child), child); // Dodanie dziecka do populacji z obliczeniem kosztu
    pair<int, vector<unsigned int>> pointerLast = population.back(); // Ostatni dodany osobnik

    if (finalCost > pointerLast.first) { // Aktualizacja najlepszego wyniku, jeśli jest lepszy
        finalCost = pointerLast.first;
        globalPath = pointerLast.second;
        showPRD(time);
    }

    uniform_real_distribution<double> generateProbability(0.0, 1.0);
    double genProb = generateProbability(engine);

    if (genProb <= probability) {
        makeMutationBest(&pointerLast); // Wykonanie mutacji
    }

    population.push_back(pointerLast); // Dodanie dziecka po mutacji

    if (finalCost > pointerLast.first) { // Ponowna aktualizacja najlepszego wyniku, jeśli mutacja poprawiła wynik
        finalCost = pointerLast.first;
        globalPath = pointerLast.second;
        showPRD(time);
    }
}

// Generowanie początkowej populacji
void GeneticAlgorithm::generateParents() {
    vector<unsigned int> vertexes;
    int populationSize = matrixWeights->size * 2;
    for (unsigned int i = 0; i < matrixWeights->size; i++) vertexes.push_back(i); // Tworzenie listy wierzchołków

    for (unsigned int i = 0; i < populationSize; i++) {
        vector<unsigned int> path;
        vector<bool> visited(matrixWeights->size, false); // Tablica odwiedzonych wierzchołków
        int current = rand() % matrixWeights->size; // Losowy wierzchołek początkowy
        path.push_back(current);
        visited[current] = true;

        for (unsigned int j = 1; j < matrixWeights->size; j++) { // Algorytm najbliższego sąsiada
            int nearest = -1;
            int minDistance = INT_MAX;

            for (unsigned int k = 0; k < matrixWeights->size; k++) {
                if (!visited[k] && matrix[current][k] < minDistance) {
                    minDistance = matrix[current][k];
                    nearest = k;
                }
            }

            path.push_back(nearest);
            visited[nearest] = true;
            current = nearest;
        }

        pair<int, vector<unsigned int>> p = {this->calculateCost(path), path};
        if (p.first < finalCost) {
            finalCost = p.first;
            globalPath = p.second;
            showPRD(i); // Pokazanie PRD
        }
        population.push_back(p); // Dodanie ścieżki do populacji
    }
}

// Funkcja wykonująca krzyżowanie metodą PMX
void GeneticAlgorithm::makePartiallyMappedCrossover(const vector<unsigned int> &parent1, const vector<unsigned int> &parent2, std::vector<unsigned int> &child1, std::vector<unsigned int> &child2, std::mt19937 engine) {
    uniform_int_distribution<> cutPoints(1, matrixWeights->size - 1);
    int a = 0, b = 0;

    while (a == b || a > b) { // Losowanie punktów przecięcia
        a = cutPoints(engine);
        b = cutPoints(engine);
    }

    for (int i = a; i <= b; i++) { // Kopiowanie obszarów przecięcia
        child1.at(i) = parent2.at(i);
        child2.at(i) = parent1.at(i);
    }

    unsigned int firstNoCopied1 = a;
    unsigned int firstNoCopied2 = a;
    for (int i = 0; i < parent1.size(); i++) {
        if (i < a || i > b) {
            copyCutPoints(parent1, &child1, a, b, i, firstNoCopied1);
            copyCutPoints(parent2, &child2, a, b, i, firstNoCopied2);
        }
    }
    completeRestPoints(firstNoCopied1, parent2, &child1, a, b); // Uzupełnienie brakujących punktów
    completeRestPoints(firstNoCopied2, parent1, &child2, a, b);
}

// Funkcja wykonująca krzyżowanie metodą OX
void GeneticAlgorithm::makeOrderCrossoverOperator(const std::vector<unsigned int> &parent1, const std::vector<unsigned int> &parent2, std::vector<unsigned int> &child1, std::vector<unsigned int> &child2, std::mt19937 engine) {
    uniform_int_distribution<> cutPoints(1, static_cast<int>(parent1.size()) - 2);
    int a = 0, b = 0;

    while (a == b) { // Losowanie punktów przecięcia
        a = cutPoints(engine);
        b = cutPoints(engine);
    }
    if (a > b)
        swap(a, b);

    for (int i = a; i <= b; i++) { // Kopiowanie obszarów przecięcia
        child1.at(i) = parent1.at(i);
        child2.at(i) = parent2.at(i);
    }
    copySequenceFromOrderCrossoverOperator(parent2, child1, a, b); // Uzupełnienie brakujących punktów w dziecku 1
    copySequenceFromOrderCrossoverOperator(parent1, child2, a, b); // Uzupełnienie brakujących punktów w dziecku 2
}

// Funkcja kopiująca pozostałe punkty sekwencji poza obszarem przecięcia
void GeneticAlgorithm::copySequenceFromOrderCrossoverOperator(const vector<unsigned int> &parent, vector<unsigned int> &child, const int &a, const int &b) {
    int j = b + 1;
    auto v = parent.begin() + b + 1;
    do {
        if (find(child.begin() + a, child.begin() + b + 1, *v) == child.begin() + b + 1) { // Dodanie punktów nieobecnych w dziecku
            child.at(j) = *v;
            if (v != parent.end() - 1)
                v++;
            else
                v = parent.begin();
        } else {
            if (v != parent.end() - 1)
                v++;
            else
                v = parent.begin();
            continue;
        }
        if (j < child.size() - 1)
            j++;
        else
            j = 0;
    } while (j != a);
}

// kopiowanie punktów cięcia
void GeneticAlgorithm::copyCutPoints(const vector<unsigned int> &parent, vector<unsigned int> *child, int a, int b, int i, unsigned int &firstNoCopied) {
    unsigned int vertex = (parent.begin() + i).operator*();
    if (find(child->begin() + a, child->begin() + b + 1, vertex) == child->begin() + b + 1)
        (child->begin() + i).operator*() = vertex;
    else if (firstNoCopied == a)
        firstNoCopied = i;
}

// uzupełnianie wektora o pozostałe wierzchołki, które nie zostały przekopiowane w obszarze kopiowania
void GeneticAlgorithm::completeRestPoints(const unsigned int firstNoCopied, const vector<unsigned int> &parent, vector<unsigned int> *child, int a, int b) {
    for (unsigned int i = firstNoCopied; i < child->size(); i++) {
        if (i < a || i > b) {
            for (int j = 0; j < parent.size(); j++) {
                if (j < a || j > b) {
                    if (find(child->begin(), child->end(), parent.at(j)) == child->end()) {
                        (child->begin() + i).operator*() = parent.at(j);
                        break;
                    }
                }
            }
        }
    }
}

// wyznaczanie najlepszej mutacji
void GeneticAlgorithm::makeMutationBest(pair<int, vector<unsigned int>> *path) {
    int bestI, bestJ, bestSwap = INT_MAX;
    int swapValue;
    for (int i = 1; i < path->second.size() - 3; i++) {
        for (int j = i + 1; j < path->second.size() - 2; j++) {
            swapValue = swapNeighbors(&path->second, i, j);
            if (swapValue < bestSwap) {
                bestI = i;
                bestJ = j;
                bestSwap = swapValue;
            }
        }
    }
    swap(path->second.at(bestI), path->second.at(bestJ));
    path->first += bestSwap;
}


// podliczanie zmiany kosztów po zamianie wierzchołków
int GeneticAlgorithm::swapNeighbors(vector<unsigned int> *path, int i, int j) {
    int subtractOldEdges = 0;
    int addNewEdges = 0;
    if (j - i == 1) {
        subtractOldEdges += matrix[path->at(i - 1)][path->at(i)];
        subtractOldEdges += matrix[path->at(i)][path->at(j)];
        subtractOldEdges += matrix[path->at(j)][path->at(j + 1)];

        addNewEdges += matrix[path->at(i - 1)][path->at(j)];
        addNewEdges += matrix[path->at(j)][path->at(i)];
        addNewEdges += matrix[path->at(i)][path->at(j + 1)];
    } else {
        subtractOldEdges += matrix[path->at(i - 1)][path->at(i)];
        subtractOldEdges += matrix[path->at(i)][path->at(i + 1)];
        subtractOldEdges += matrix[path->at(j - 1)][path->at(j)];
        subtractOldEdges += matrix[path->at(j)][path->at(j + 1)];

        addNewEdges += matrix[path->at(i - 1)][path->at(j)];
        addNewEdges += matrix[path->at(j)][path->at(i + 1)];
        addNewEdges += matrix[path->at(j - 1)][path->at(i)];
        addNewEdges += matrix[path->at(i)][path->at(j + 1)];
    }
    return addNewEdges - subtractOldEdges;
}

void GeneticAlgorithm::showPRD(uint64_t elapsedTimeMicro) {
    double bestcost403 = 2465;
    double bestcost170 = 2755;
    double bestcost47 = 1776;

    // Wyświetlanie w mikrosekundach zamiast iteracji
    std::cout << elapsedTimeMicro << " us " << finalCost << "   "
              << 100 * (finalCost - bestcost47) / bestcost47 << "% \n";
}


// wyświetlanie ścieżki
void GeneticAlgorithm::showPath(vector<unsigned int> path) {
    for (unsigned int i: path)
        cout << i << "->";
    cout << path.front() << "\n";
}

// obliczanie kosztu ścieżki
int GeneticAlgorithm::calculateCost(vector<unsigned int> path) {
    int cost = 0;
    auto i = path.begin();
    while (i != (path.end() - 1)) {
        cost += matrix[i.operator*()][(i + 1).operator*()];
        i.operator++();
    }
    cost += matrix[path.back()][path.front()];
    return cost;
}

void GeneticAlgorithm::copyPopulation(int number) {
    sort(population.begin(), population.end(), cmp); // Sortowanie według wartości dopasowania
    population.resize(number); // Ograniczenie rozmiaru populacji do 'number'
}
