#include <iostream>

#include "test.pb.h"

void printPosition(const example::Position& position) {
    std::cout << "Aircraft ID: " << position.aircraft_id() << std::endl;
    std::cout << "Aircraft Name: " << position.aircraft_name() << std::endl;
    std::cout << "Latitude: " << position.lat_deg() << std::endl;
    std::cout << "Longitude: " << position.lon_deg() << std::endl;
    std::cout << "Altitude: " << position.alt_msl() << std::endl;
    std::cout << "Altitude Units: " << (position.alt_units() == example::Position::METERS ? "Meters" : "Feet") << std::endl;
}

int main(int argc, char* argv[]) {
    GOOGLE_PRTOBUF_VERIFY_VERSION;

    std::cout << "Howdy" << std:endl;

    printPostition();
}