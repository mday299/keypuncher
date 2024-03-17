//#include <wireshark/config.h>
//#note, may be different on your system!
#include </usr/include/wireshark/config.h>

#include <wireshark/epan/packet.h>
#include <wireshark/epan/dissectors/packet-tcp.h>
#include <wireshark/epan/prefs.h>
#include <epan/packet.h>
#include <iostream>

// Define protocol fields
static int proto_udp_simple = -1;
static int hf_udp_string = -1;
static int hf_udp_length = -1;
static int hf_udp_alt = -1;
static int hf_udp_bank_angle = -1;
static int hf_udp_lat = -1;
static int hf_udp_lon = -1;
static int hf_udp_speed = -1;

// Define dissector function
void dissect_udp_simple(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree) {
    proto_item *ti = nullptr;
    proto_tree *subtree = nullptr;
    guint16 length = tvb_get_ntohs(tvb, 0);
    guint offset = 2;
    
    ti = proto_tree_add_item(tree, proto_udp_simple, tvb, 0, -1, ENC_NA);
    subtree = proto_item_add_subtree(ti);

    proto_tree_add_uint(subtree, hf_udp_length, tvb, 0, 2, length);

    guint str_len = length - 24;
    char *str = tvb_get_string(tvb, offset, str_len);
    proto_tree_add_string_format(subtree, hf_udp_string, tvb, offset, str_len, str);
    offset += str_len;

    proto_tree_add_float(subtree, hf_udp_alt, tvb, offset, 4, tvb_get_ntohf(tvb, offset), "Altitude");
    offset += 4;
    proto_tree_add_float(subtree, hf_udp_bank_angle, tvb, offset, 4, tvb_get_ntohf(tvb, offset), "Bank Angle");
    offset += 4;
    proto_tree_add_float(subtree, hf_udp_lat, tvb, offset, 4, tvb_get_ntohf(tvb, offset), "Latitude");
    offset += 4;
    proto_tree_add_float(subtree, hf_udp_lon, tvb, offset, 4, tvb_get_ntohf(tvb, offset), "Longitude");
    offset += 4;
    proto_tree_add_float(subtree, hf_udp_speed, tvb, offset, 4, tvb_get_ntohf(tvb, offset), "Speed");
}

// Define the dissector table
static dissector_table_t udp_dissector_table;

// Register the dissector
void proto_register_udp_simple(void) {
    static hf_register_info hf[] = {
        { &hf_udp_string, {"String", "udp_simple.string", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
        { &hf_udp_length, {"Length", "udp_simple.length", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL }},
        { &hf_udp_alt, {"Altitude", "udp_simple.alt", FT_FLOAT, BASE_NONE, NULL, 0x0, NULL, HFILL }},
        { &hf_udp_bank_angle, {"Bank Angle", "udp_simple.bank_angle", FT_FLOAT, BASE_NONE, NULL, 0x0, NULL, HFILL }},
        { &hf_udp_lat, {"Latitude", "udp_simple.lat", FT_FLOAT, BASE_NONE, NULL, 0x0, NULL, HFILL }},
        { &hf_udp_lon, {"Longitude", "udp_simple.lon", FT_FLOAT, BASE_NONE, NULL, 0x0, NULL, HFILL }},
        { &hf_udp_speed, {"Speed", "udp_simple.speed", FT_FLOAT, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    };

    proto_udp_simple = proto_register_protocol("Simple UDP Protocol", "UDP_SIMPLE", "udp_simple");
    proto_register_field_array(proto_udp_simple, hf, array_length(hf));
}

// Dissector initialization
void plugin_register(void) {
    proto_register_udp_simple();
    udp_dissector_table = register_dissector("udp.port", dissect_udp_simple, proto_udp_simple);
}

// Cleanup
void plugin_reg_handoff(void) {
    dissector_add_uint("udp.port", 12345, udp_dissector_table);
}
