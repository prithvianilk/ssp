FROM debian:latest AS builder
RUN apt update -y
RUN apt install -y gcc
COPY src/lat_ctx_docker/docker_proc.c docker_proc.c
RUN gcc -o docker_proc docker_proc.c

FROM debian:latest
RUN mkdir /pipes
WORKDIR /bin
COPY --from=builder docker_proc .
ENTRYPOINT ["docker_proc"]