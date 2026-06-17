/*
Exception Handling
Write a program that throws and catches an integer exception. 
Handle the exception and print its value

Now create a USEFUL example where a config file is parsed through an input strm.
*/

#include <iostream>
#include <string>
#include <fstream>

struct Config {
    int port = 0;
    std::string logLevel;
    bool enableCache = false;
    int timeoutMs = 0;
};

static inline std::string trim(const std::string& s) {
    const auto start = s.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

void parseConfigLine(const std::string& line, Config& cfg) {
    std::string trimmed = trim(line);
    if (trimmed.empty() || trimmed[0] == '#')
        return;

    auto eq = trimmed.find('=');
    if (eq == std::string::npos)
        throw std::runtime_error("Invalid config line: " + trimmed);

    std::string key = trim(trimmed.substr(0, eq));
    std::string val = trim(trimmed.substr(eq + 1));

    if (key == "port")
        cfg.port = std::stoi(val);
    else if (key == "log_level")
        cfg.logLevel = val;
    else if (key == "enable_cache")
        cfg.enableCache = (val == "true");
    else if (key == "timeout_ms")
        cfg.timeoutMs = std::stoi(val);
    else
        throw std::runtime_error("Unknown config key: " + key);
}

Config loadConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Could not open config file: " + path);

    Config cfg;
    std::string line;

    while (std::getline(file, line)) {
        parseConfigLine(line, cfg);
    }

    return cfg;
}


int main() {
    //part 1
    std::cout << "Exception handling test" << std::endl;
    try {
        int x = 6;
        throw x;
    } catch (int ex) {
        std::cout << "Exception value: " << ex << " was caught and handled" << std::endl;
    }

    //part 2
    try {
        Config cfg = loadConfig("config.txt");

        std::cout << "Config loaded:\n";
        std::cout << "  port        = " << cfg.port << "\n";
        std::cout << "  log_level   = " << cfg.logLevel << "\n";
        std::cout << "  enable_cache= " << std::boolalpha << cfg.enableCache << "\n";
        std::cout << "  timeout_ms  = " << cfg.timeoutMs << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << "\n";
        return 1;
    }

    return 0;
}