#include "ConfigFile.h"
#include "GeneticAlgorithm.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

void ConfigFile::parseConfig() {
    // Otwiera plik konfiguracyjny do odczytu
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku konfiguracyjnego!" << std::endl;
        return; // Jeśli nie udało się otworzyć pliku, kończy działanie funkcji
    }

    std::unordered_map<std::string, std::string> config; // Słownik do przechowywania konfiguracji
    std::string line; // Do przechowywania kolejnych linii z pliku

    while (std::getline(file, line)) {
        // Usuwa komentarze i białe znaki na początku i końcu linii
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos); // Obcięcie komentarza
        line.erase(0, line.find_first_not_of(" \t")); // Usunięcie białych znaków z przodu
        line.erase(line.find_last_not_of(" \t") + 1); // Usunięcie białych znaków z tyłu
        if (line.empty()) continue; // Pominięcie pustych linii

        std::istringstream stream(line); // Strumień do podzielenia linii na klucz i wartość
        std::string key, value;
        if (std::getline(stream, key, ':') && std::getline(stream, value)) {
            // Usunięcie białych znaków wokół klucza i wartości
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            config[key] = value; // Dodanie pary klucz-wartość do mapy
        }
    }
    file.close(); // Zamknięcie pliku

    // Pobiera tryb działania z konfiguracji i wywołuje odpowiednią funkcję
    const auto& mode = config["mode"];
    if (mode == "generate")
        handleGenerateMode();
    else if (mode == "test")
        handleTestMode(config["fileName"], std::stod(config["probability"]),std::stoi(config["generationNumber"]),std::stod(config["crossoverCoefficient"]),std::stoi(config["crossoverType"]) );
}

void ConfigFile::handleGenerateMode() {
    std::cout << "Tryb testowania danych\n";
}

void ConfigFile::handleTestMode(const std::string& fileName, double probability, int generationNumber, double crossoverCoefficient,int crossoverType) {
    // Wczytanie macierzy z pliku
    GeneticAlgorithm *gens;
    gens = new GeneticAlgorithm(fileName);
    gens->startAlgorithm(probability,  generationNumber, crossoverType, crossoverCoefficient);
    delete gens;
}