//
// Created by pitko on 11.01.2025.
//

#ifndef PROJEKT_4_LOADFROMFILE_H
#define PROJEKT_4_LOADFROMFILE_H
#include <fstream>

class LoadFromFile {
public:
    bool openFile();

    int getDataFromFile();

    ~LoadFromFile() {
        file.close();
    }
    bool openFileName(const std::string& nameOfFile);

private:

    std::fstream file;
};


#endif //PROJEKT_4_LOADFROMFILE_H
