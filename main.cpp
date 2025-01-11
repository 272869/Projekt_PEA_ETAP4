#include "ConfigFile.h"


int main() {
    ConfigFile configHandler("config.txt");
    configHandler.parseConfig();
    return 0;
}