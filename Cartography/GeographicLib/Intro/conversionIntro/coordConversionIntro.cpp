//#include <GeographicLib/GeoConvert.hpp>
#include <iostream>
#include <string>

//using namespace GeographicLib;

int main() {

    // Example: Convert DMS to DD (Decimal Degrees)
    std::string dmsLat = "40° 26' 46.02\" N";  // Latitude DMS
    std::string dmsLon = "79° 58' 56.17\" W"; // Longitude DMS

    double latDD, lonDD;

    // Convert Latitude and Longitude from DMS to Decimal Degrees
    latDD = GeographicLib::GeoConvert::DMS2DD(dmsLat);
    lonDD = GeoConvert::DMS2DD(dmsLon);

    std::cout << "Latitude in Decimal Degrees: " << latDD << std::endl;
    std::cout << "Longitude in Decimal Degrees: " << lonDD << std::endl;

    return 0;
}
