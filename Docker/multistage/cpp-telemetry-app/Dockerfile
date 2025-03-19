# Stage 1: Build stage 
FROM gcc:14 AS builder 

# Set working directory 
WORKDIR /app 

# Install dependencies 
RUN apt-get update && apt-get install -y cmake git wget && \
  wget https://github.com/google/googletest/archive/release-1.12.1.tar.gz && \
  tar -xzf release-1.12.1.tar.gz && \
  cd googletest-release-1.12.1 && mkdir build && cd build && cmake .. && make && \ 
  make install 
  
# Copy source files 
COPY . . 

# Build the application 
RUN mkdir build && cd build && \
 cmake .. && \
 make 

# Stage 2: Test stage 
FROM builder AS tester 

# Set working directory to the build directory 
WORKDIR /app/build 

# Run tests 
CMD ["./telemetry_processor"] 

# Stage 3: Production stage 
FROM ubuntu:24.04 AS runner 

# Install required libraries 
RUN apt-get update && apt-get install -y libstdc++6 

# Set working directory 
WORKDIR /app 

# Copy the built application from the builder stage 
COPY --from=builder /app/build/test_application /app/test_application 
COPY --from=builder /app/build/telemetry_processor /app/telemetry_processor 

# Run the application 
CMD ["./test_application"]