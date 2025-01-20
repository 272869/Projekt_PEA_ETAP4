
#include <vector>
#include <random>
#include <ctime>
#include "FileReader.h"

using namespace std;
class GeneticAlgorithm {
private:
    FileReader *matrixWeights;
    std::vector<std::pair<int, std::vector<unsigned int>>> population;

    static bool cmp(std::pair<int, std::vector<unsigned int>> &a, std::pair<int, std::vector<unsigned int>> &b) {
        return a.first < b.first;
    }
    int **matrix;
    int finalCost = INT_MAX;
    std::vector<unsigned int> globalPath;
    void mainLoop(mt19937 &engine, double probability,int populationSize,double maxTime, int crossoverType, double crossoverCoefficient);
    void generateParents(int populationSize);

    void countFitnessValue(vector<float> &fitness);
    pair<int, int> rouletteWheelSelection(mt19937 &engine, vector<float> &fitness);

    void checkMutation(std::mt19937 engine, vector<unsigned int> &child, double probability, uint64_t time);
    void makeMutationSwap(std::pair<int, std::vector<unsigned int>> *path);

    int swapNeighbors(std::vector<unsigned int> *path, int i, int j);

    void makePartiallyMappedCrossover(const vector<unsigned int> &parent1, const vector<unsigned int> &parent2,std::vector<unsigned int> &child1, std::vector<unsigned int> &child2,std::mt19937 engine);
    static void copyCutPoints(const std::vector<unsigned int> &parent, std::vector<unsigned int> *child, int a, int b, int i,unsigned int &firstNoCopied);

    static void completeRestPoints(unsigned int firstNoCopied, const std::vector<unsigned int> &parent, std::vector<unsigned int> *child, int a, int b);
    static void makeOrderCrossoverOperator(const vector<unsigned int>& parent1, const vector<unsigned int>& parent2, vector<unsigned int> &child1, vector<unsigned int> &child2, mt19937 engine);
    static void copySequenceFromOrderCrossoverOperator(const vector<unsigned int> &parent, vector<unsigned int> &child, const int &a, const int &b);

    void copyPopulation(int number);
    void showPRD(uint64_t time);
    int calculateCost(std::vector<unsigned int> path);
    static void showPath(std::vector<unsigned int> path);

public:
    explicit GeneticAlgorithm(const std::string& fileName) {
        matrixWeights = new FileReader();
        matrix = matrixWeights->loadFromFile(fileName);
        //matrixWeights->showTab();
    }
    void startAlgorithm(double probability,int populationSize, double maxTime, int crossoverType, double crossoverCoefficient);
};

