#TODO specify version of ubuntu more
FROM ubuntu:24.04

ARG BUILD_GUI=false

# System dependencies
RUN apt-get update && apt-get install -y \
    python3 python3-pip python3-venv python3-dev \
    git build-essential libtool libxml2-dev \
    pkg-config g++ wget curl

# GUI dependencies (conditionally installed)
RUN if [ "$BUILD_GUI" = "true" ]; then \
    apt-get install -y \
        python3-pyqt5 \
        python3-matplotlib \
        python3-wxgtk4.0 \
        libglu1-mesa; \
    fi && \
    rm -rf /var/lib/apt/lists/*

# Set up Python virtual environment
ENV VIRTUAL_ENV=/opt/venv
RUN python3 -m venv $VIRTUAL_ENV
ENV PATH="$VIRTUAL_ENV/bin:$PATH"

# Clone ArduPilot at the specified branch
RUN git clone --branch Plane-4.6.0 --recursive https://github.com/ArduPilot/ardupilot.git

#set working directory to ArduPilot
WORKDIR /ardupilot

# -------------------------------
#  Install Python dependencies needed for build + SITL
# -------------------------------
RUN pip install --no-cache-dir \
    setuptools \
    dronecan \
    pymavlink \
    MAVProxy \
    empy==3.3.4 \
    pexpect

# -------------------------------
# Build ArduPlane for SITL
# -------------------------------
RUN ./waf configure --board sitl && ./waf plane -j$(nproc --ignore=1)

# -------------------------------
# Expose SITL default MAVLink port
# -------------------------------
EXPOSE 5760

# -------------------------------
#  Default command to run ArduPlane SITL
# -------------------------------
CMD ["./Tools/autotest/sim_vehicle.py", "-v", "ArduPlane", "--console", "--map"]

