#include "telemetry_processor.h" 
#include <iostream> 

int main() {
    Telemetry sampleData = {33.6407, -84.4277, 1000.0, 250.0, 5.0, 1.0, 0.0, 2500, 65.0, 3.5}; 
    std::cout << formatTelemetry(sampleData) << std::endl; 
    return 0; 
}