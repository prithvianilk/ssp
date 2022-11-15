#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const char* USAGE = "Usage: docker_proc <read-pipe-path> <write-pipe-path>";

void check_invalid_usage(int argc) {
  int is_valid_usage = (argc == 3);
  if (!is_valid_usage) {
    fprintf(stderr, "Invalid usage\n%s\n", USAGE);
    exit(1);
  }
}

char* read_token(int read_pipe_fd, char* token) {
  int bytes_read = read(read_pipe_fd, token, 1);
  if (bytes_read != 1) {
    fprintf(stderr, "Error while reading token: %d\n", bytes_read);
    exit(1);
  }
  printf("Read: %s\n", token);
}

void write_token(int write_pipe_fd, char* token) {
  int bytes_written = write(write_pipe_fd, token, 1);
  if (bytes_written != 1) {
    fprintf(stderr, "Error while writing token: %d\n", bytes_written);
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  check_invalid_usage(argc);

  char *read_pipe_path = argv[1], *write_pipe_path = argv[2];

  printf("Pipes are yet to be created\n");
  fflush(stdout);
  mkfifo(read_pipe_path, 0666);
  mkfifo(write_pipe_path, 0666);
  printf("Pipes are created\n");
  fflush(stdout);

  int read_pipe_fd = open(read_pipe_path, O_RDONLY);
  int write_pipe_fd = open(write_pipe_path, O_WRONLY);
  printf("Pipes are opened\n");
  fflush(stdout);

  for (;;) {
    char token[2];
    read_token(read_pipe_fd, token);
    write_token(write_pipe_fd, token);
  }
  return 0;
}
