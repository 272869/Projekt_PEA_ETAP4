#include <string>
#include <chrono>

class ConfigFile {
public:
    void parseConfig();
    explicit ConfigFile(const std::string &filename) : filename(filename) {}
private:
    void handleGenerateMode(const std::string& fileName, double probability,int populationSize, int time, double crossoverCoefficient,int crossoverType);
    void handleTestMode(const std::string& fileName, double probability,int populationSize,  int time, double crossoverCoefficient,int crossoverType);
    std::string filename;
};

