#include <iostream>
#include "GeneticAlgorithm.h"

void menu() {
    int a;
    do {
        GeneticAlgorithm *gens;
        std::cout << "\n1. Algorytm Genetyczny  \n"
                  << "Opcja:";
        std::cin >> a;
        switch (a - 1) {
            case 0:
                gens = new GeneticAlgorithm();
                double p;
                int  populationSize, populationCopyNumber, generationNumber, selectionType, crossoverType;
                std::cout << "Podaj prawdopodobienstwo \n";
                cin >> p;
                std::cout << "Podaj rozmiar populacji \n";
                cin >> populationSize;
                std::cout << "Podaj liczbe kopiowanych osobnikow \n";
                cin >> populationCopyNumber;
                std::cout << "Podaj liczbe generacji \n";
                cin >> generationNumber;
                std::cout << "Podaj rodzaj selekcji \n"
                             "0 - selekcja kola ruletki\n"
                             "1 - selekcja turniejowa\n"
                             "2 - selekcja rankingowa\n";
                cin >> selectionType;
                std::cout << "Podaj typ krzyzowania \n"
                             "0 - Partially Mapped Crossover\n"
                             "1 - Order Crossover Operator\n";
                cin >> crossoverType;
                gens->startAlgorithm(p,
                                     populationSize,
                                     populationCopyNumber,
                                     generationNumber,
                                     selectionType,
                                     crossoverType);
                delete gens;
                break;

        }

    } while (a != 2);

}

int main() {
    menu();

    return 0;
}