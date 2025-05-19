# Use an Alpine-based image with C++ support
FROM alpine:3.21.3

# Install necessary build tools and dependencies
RUN apk add --no-cache g++

# Set the working directory
WORKDIR /usr/src/app

# Copy source files
COPY . .

# Compile the C++ application
RUN g++ -o app main.cpp

# Run the compiled application
CMD ["./app"]
