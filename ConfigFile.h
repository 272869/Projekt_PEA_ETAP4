#include <string>
#include <chrono>

class ConfigFile {
public:
    void parseConfig();
    explicit ConfigFile(const std::string &filename) : filename(filename) {}
private:
    void handleGenerateMode();
    void handleTestMode(const std::string& fileName, double probability, int populationSize, int populationCopyNumber, int generationNumber, int selectionType,int crossoverType);
    std::string filename;
};

