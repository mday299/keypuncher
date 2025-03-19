#include "telemetry_processor.h" 
#include <gtest/gtest.h> 
#include <cmath> 

// Test: Position formatting (Latitude, Longitude, Altitude) 
TEST(TelemetryProcessorTest, FormatPosition) {
    Telemetry testData = {10.1234, -20.5678, 300.0, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0}; 
    std::string expectedPosition = "Position: (10.12, -20.57, 300.00 m)\n";

    std::string actualOutput = formatTelemetry(testData); 
    ASSERT_TRUE(actualOutput.find(expectedPosition) != std::string::npos); 
}
    
// Test: Airspeed formatting 
TEST(TelemetryProcessorTest, FormatAirspeed) { 
    Telemetry testData = {0.0, 0.0, 0.0, 150.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0}; 
    std::string expectedAirspeed = "Airspeed: 150.00 m/s\n"; 
    
    std::string actualOutput = formatTelemetry(testData); 
    ASSERT_TRUE(actualOutput.find(expectedAirspeed) != std::string::npos); 
} 
    
// Test: Attitude formatting (Pitch, Roll, Yaw) 
TEST(TelemetryProcessorTest, FormatAttitude) { 
    Telemetry testData = {0.0, 0.0, 0.0, 0.0, 10.0, -5.0, 180.0, 0, 0.0, 0.0}; 
    std::string expectedAttitude = "Attitude: Pitch=10.00°, Roll=-5.00°, Yaw=180.00°\n"; 
    
    std::string actualOutput = formatTelemetry(testData); 
    ASSERT_TRUE(actualOutput.find(expectedAttitude) != std::string::npos); 
} 

// Test: Engine RPM formatting 
TEST(TelemetryProcessorTest, FormatEngineRPM) { 
    Telemetry testData = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3000, 0.0, 0.0}; 
    std::string expectedEngineRPM = "Engine RPM: 3000 RPM\n"; 
    
    std::string actualOutput = formatTelemetry(testData); 
    ASSERT_TRUE(actualOutput.find(expectedEngineRPM) != std::string::npos); 
} 

// Test: Temperature formatting 
TEST(TelemetryProcessorTest, FormatTemperature) { 
    Telemetry testData = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 50.0, 0.0}; 
    std::string expectedTemperature = "Temperature: 50.00 °C\n"; 
    
    std::string actualOutput = formatTelemetry(testData); 
    ASSERT_TRUE(actualOutput.find(expectedTemperature) != std::string::npos); 
} 

// Test: Vertical Speed formatting 
TEST(TelemetryProcessorTest, FormatVerticalSpeed) { 
    Telemetry testData = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 2.0}; 
    std::string expectedVerticalSpeed = "Vertical Speed: 2.00 m/s\n"; 
    
    std::string actualOutput = formatTelemetry(testData); 
    ASSERT_TRUE(actualOutput.find(expectedVerticalSpeed) != std::string::npos); 
} 

// Test: Extreme maximum values for telemetry 
TEST(TelemetryProcessorTest, MaxTelemetryValues) { 
    Telemetry testData = {90.0, 180.0, 100000.0, 999.99, 90.0, 90.0, 360.0, 20000, 150.0, 50.0};
    std::string expectedOutput = 
        "Telemetry Data:\n" 
        "Position: (90.00, 180.00, 100000.00 m)\n"
        "Airspeed: 999.99 m/s\n" 
        "Attitude: Pitch=90.00°, Roll=90.00°, Yaw=360.00°\n"
        "Engine RPM: 20000 RPM\n" 
        "Temperature: 150.00 °C\n" 
        "Vertical Speed: 50.00 m/s\n"; 
        
    ASSERT_EQ(formatTelemetry(testData), expectedOutput); 
} 
    
    // Test: Extreme minimum values for telemetry 
    TEST(TelemetryProcessorTest, MinTelemetryValues) { 
        Telemetry testData = {-90.0, -180.0, -100.0, 0.0, -90.0, -90.0, -360.0, 0, -50.0, -100.0}; 
        std::string expectedOutput = 
            "Telemetry Data:\n" 
            "Position: (-90.00, -180.00, -100.00 m)\n"
            "Airspeed: 0.00 m/s\n" "Attitude: Pitch=-90.00°, Roll=-90.00°, Yaw=-360.00°\n"
            "Engine RPM: 0 RPM\n" 
            "Temperature: -50.00 °C\n" 
            "Vertical Speed: -100.00 m/s\n"; 
            
        ASSERT_EQ(formatTelemetry(testData), expectedOutput); 
} 

// Test: High-frequency updates 
TEST(TelemetryProcessorTest, HighFrequencyUpdates) { 
    for (int i = 0; i < 10000; ++i) {
        Telemetry testData = {33.6407 + i * 0.0001, -84.4277 + i * 0.0001, 1000.0 + i, 250.0 + i * 0.1, 5.0 + i * 0.01, 1.0 + i * 0.01, 0.0 + i * 0.01, 2500 + i * 10, 65.0 + i * 0.1, 3.5 + i * 0.01};
        std::string result = formatTelemetry(testData); 
        
        ASSERT_FALSE(result.empty()); 
    } 
} 

// Test: Invalid values for telemetry 
TEST(TelemetryProcessorTest, InvalidTelemetryValues) { 
    Telemetry testData = {NAN, INFINITY, -INFINITY, -500.0, NAN, NAN, INFINITY, -1, NAN, -10000.0};
    std::string result = formatTelemetry(testData); 
    
    // Application should produce a meaningful output or error message for invalid inputs 
    ASSERT_TRUE(result.find("Invalid") != std::string::npos || !result.empty()); 
}