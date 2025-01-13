#include <fstream>
#include <iostream>
#include "FileReader.h"
#include <iomanip>
#include <sstream>


int** FileReader::loadFromFile(std::string path) {
    std::ifstream file(path);
    if (!file.good()) {
        std::cout << "Blednie podana nazwa pliku\n";
        return NULL;
    }
    std::string line;
    int N = 0;
    // Wczytywanie danych
    while (std::getline(file, line)) {
        if (line.find("DIMENSION:") != std::string::npos) {
            std::istringstream iss(line);
            std::string dimension;
            iss >> dimension >> N;
        }
        if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
            break; // Przejście do sekcji z macierzą
        }
    }

    if (N == 0) {
        std::cout << "Nie mozna odczytac wymiaru macierzy.\n";
        return NULL;
    }

    alocate(N);

    // Wczytywanie macierzy kosztów
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            file >> tab[i][j];
        }
    }

    return tab;
}

void FileReader::alocate(int N) {
    if (tab != NULL) dealocate();
    this->size = N;
    tab = new int* [N];
    for (int i = 0; i < N; ++i) {
        tab[i] = new int[N];
    }
}

void FileReader::dealocate() {
    if (tab) {
        for (int i = 0; i < size; ++i) {
            delete[] tab[i];
        }
        delete[] tab;
        tab = NULL;
    }
}

void FileReader::showTab() {
    if (tab != NULL) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (tab[i][j] != -1)
                    std::cout << std::setw(3) << tab[i][j] << " ";
                else std::cout << "   " << " ";
            }
            std::cout << std::endl;
        }
    }
}

