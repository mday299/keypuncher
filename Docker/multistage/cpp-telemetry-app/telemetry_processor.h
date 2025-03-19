#ifndef TELEMETRY_PROCESSOR_H 
#define TELEMETRY_PROCESSOR_H

#include <string>

struct Telemetry { 
    double latitude; 
    double longitude; 
    double altitude; 
    double airspeed; 
    double pitch; 
    double roll; 
    double yaw; 
    int engineRPM; 
    double temperature; 
    double verticalSpeed; 
}; 

std::string formatTelemetry(const Telemetry &data); 

#endif // TELEMETRY_PROCESSOR_H