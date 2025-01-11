#include "Matrix.h"
#include <iostream>
#include <random>

///tworzenie tablicy wag
void Matrix::createTables() {

    matrixWeights = new int *[size];

    for (int i = 0; i < size; i++) {
        matrixWeights[i] = new int[size];
    }

}

///wczytanie pliku
void Matrix::loadData() {
    auto *loadFromFile = new LoadFromFile();

    ///wczytujemy podstawowe dane z pliku
    if (loadFromFile->openFile()) {
        size = loadFromFile->getDataFromFile();
        createTables();
        createMatrix(loadFromFile);
        std::cout << "Wczytano dane\n";

    }
    delete loadFromFile;
}

///wczytanie pliku
void Matrix::loadDataName(std::string fileName) {
    auto *loadFromFile = new LoadFromFile();

    ///wczytujemy podstawowe dane z pliku
    if (loadFromFile->openFileName(fileName)) {
        size = loadFromFile->getDataFromFile();
        createTables();
        createMatrix(loadFromFile);
        std::cout << "Wczytano dane\n";

    }
    delete loadFromFile;
}

///tworzenie macierzy wag
void Matrix::createMatrix(LoadFromFile *loadFromFile) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrixWeights[i][j] = loadFromFile->getDataFromFile();
            if (i == j)
                matrixWeights[i][j] = INT_MAX;
        }
    }
    optimum = loadFromFile->getDataFromFile();

}

///wyswietlanie macierz wag
void Matrix::showMatrixWages() {

    std::cout << "  ";
    for (int i = 0; i < size + 1; i++) {

        for (int j = 0; j < size; j++) {
            if (i == 0)
                std::cout << j << " ";
            else {
                if (matrixWeights[i - 1][j] != INT32_MAX)
                    std::cout << matrixWeights[i - 1][j];
                else
                    std::cout << "-";

                std::cout << " ";
            }
        }

        if (i != size)
            std::cout << std::endl << i << " ";

    }
    std::cout << std::endl;
    std::cout << std::endl;

}

///tworzenie nowego grafu na podstawie liczby wierzchołków
void Matrix::createNewMatrix(bool directed, int vNumber) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, vNumber - 1);
    std::uniform_int_distribution<> dist2(0, 100000);

    size = vNumber;

    createTables();


    if (!directed) {
        for (int i = 0; i < size; i++) {
            matrixWeights[i][i] = INT_MAX;
            for (int j = i + 1; j < size; j++) {
                matrixWeights[i][j] = dist2(gen);
                matrixWeights[j][i] = matrixWeights[i][j];
            }
        }

    } else {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == j)
                    matrixWeights[i][i] = INT_MAX;
                else
                    matrixWeights[i][j] = dist2(gen);
            }
        }
    }
    //showMatrixWages();

}

///usuwanie macierzy wag
void Matrix::deleteMatrixTable(int **matrixW) {
    for (int i = 0; i < size; i++) {
        delete[] matrixW[i];
    }
    delete[]matrixW;
}

int Matrix::getSize() const {
    return size;
}

int **Matrix::getMatrixWeights() const {
    return matrixWeights;
}

int Matrix::getOptimum() const {
    return optimum;
}