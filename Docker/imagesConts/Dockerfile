# Use an Alpine-based image with C++ support
FROM alpine:latest

# Install necessary build tools and dependencies
RUN apk add --no-cache g++ make cmake

# Set the working directory
WORKDIR /usr/src/app

# Copy source files
COPY . .

# Compile the C++ application
RUN g++ -o app main.cpp

# Expose the necessary port (adjust based on your application)
EXPOSE 8080

# Run the compiled application
CMD ["./app"]
