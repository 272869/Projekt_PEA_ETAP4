#include <string>
#include <chrono>

class ConfigFile {
public:
    void parseConfig();
    explicit ConfigFile(const std::string &filename) : filename(filename) {}
private:
    void handleGenerateMode();
    void handleTestMode(const std::string& fileName, double probability, int populationCopyNumber, int generationNumber, double crossoverCoefficient,int crossoverType);
    std::string filename;
};

