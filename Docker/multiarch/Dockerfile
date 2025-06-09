# Select the base image dynamically based on architecture
ARG TARGETARCH
FROM alpine:latest

# Install dependencies
RUN apk add --no-cache bash

# Print architecture info
CMD ["bash", "-c", "echo Running on $(uname -m)"]
