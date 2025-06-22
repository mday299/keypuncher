import time
import socket
import argparse
from testcontainers.core.container import DockerContainer
from pymavlink import mavutil

ARDUPILOT_IMAGE = "arduplane-sitl-test:latest"


def wait_for_port(host, port, timeout=15):
    """Waits for MAVLink port to become reachable."""
    start = time.time()
    while time.time() - start < timeout:
        try:
            with socket.create_connection((host, int(port)), timeout=2):
                return True
        except OSError:
            time.sleep(1)
    raise TimeoutError("MAVLink port not reachable")


def run_sitl_test(debug=False):
    with DockerContainer(ARDUPILOT_IMAGE) as container:
        container.with_exposed_ports(5760).start()
        host = container.get_container_host_ip()
        port = container.get_exposed_port(5760)

        wait_for_port(host, port)

        if debug:
            print(f"Connecting to MAVLink: tcp:{host}:{port}")

        mav = mavutil.mavlink_connection(f"tcp:{host}:{port}")
        mav.wait_heartbeat(timeout=15)
        if debug:
            print(f"Heartbeat received from system {mav.target_system}, component {mav.target_component}")

        # Arm the vehicle
        mav.mav.command_long_send(
            mav.target_system, mav.target_component,
            mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM,
            0, 1, 0, 0, 0, 0, 0, 0
        )

        ack = mav.recv_match(type='COMMAND_ACK', blocking=True, timeout=5)
        assert ack.result == mavutil.mavlink.MAV_RESULT_ACCEPTED
        if debug:
            print("Vehicle armed successfully.")

        # Validate altitude and GPS
        pos = mav.recv_match(type='GLOBAL_POSITION_INT', blocking=True, timeout=10)
        assert pos.alt > 0
        if debug:
            print(f"Altitude reported: {pos.alt} cm")

        gps = mav.recv_match(type='GPS_RAW_INT', blocking=True, timeout=5)
        assert gps.fix_type >= 3
        if debug:
            print(f"GPS Fix Type: {gps.fix_type}")


# Pytest-compatible function name
def test_sitl_telemetry():
    run_sitl_test(debug=False)


# CLI entry point
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run ArduPlane SITL container test.")
    parser.add_argument("--debug", action="store_true", help="Enable verbose MAVLink logging.")
    args = parser.parse_args()

    run_sitl_test(debug=args.debug)