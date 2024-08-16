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
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    example::Position pos;
    pos.set_aircraft_id(1);
    pos.set_aircraft_name("ac 1");
    pos.set_lat_deg(32.5);
    pos.set_lon_deg(-33.4);
    pos.set_alt_msl(100.1);
    pos.set_alt_units(example::Position::METERS); 

    printPosition(pos);

    return 0;
}