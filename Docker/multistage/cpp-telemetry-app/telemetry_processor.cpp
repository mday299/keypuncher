#include "telemetry_processor.h" 
#include <sstream> 
#include <iomanip> 

std::string formatTelemetry(const Telemetry &data) {
    std::ostringstream oss; 
    oss << std::fixed << std::setprecision(2); 
    oss << "Telemetry Data:\n"; 
    oss << "Position: (" << data.latitude << ", " << data.longitude << ", " << data.altitude << " m)\n";
    oss << "Airspeed: " << data.airspeed << " m/s\n"; 
    oss << "Attitude: Pitch=" << data.pitch << "°, Roll=" << data.roll << "°, Yaw=" << data.yaw << "°\n";
    oss << "Engine RPM: " << data.engineRPM << " RPM\n"; 
    oss << "Temperature: " << data.temperature << " °C\n"; 
    oss << "Vertical Speed: " << data.verticalSpeed << " m/s\n"; 
    
    return oss.str(); 
}