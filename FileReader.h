
#include <string>

using namespace std;

class FileReader{
public:
    int** tab;
    int size;
    FileReader() {
        this->size = 0;
        tab = NULL;
    };
    int** loadFromFile(std::string path);
    void alocate(int N);
    void dealocate();
    void showTab();
};

