# Use the official GCC image with the specified version
FROM gcc:13.3.0

# Set the working directory in the container
WORKDIR /usr/src/app

# Copy the source code into the container
COPY main.cpp main.cpp

# Compile the C++ program
RUN g++ -o myapp main.cpp
