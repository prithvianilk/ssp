#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>

const char* USAGE =
    "Usage: docker_proc <read-pipe-path> <write-pipe-path> <process size>";

void check_invalid_usage(int argc) {
  int is_valid_usage = (argc == 3 || argc == 4);
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
  fflush(stdout);
}

void write_token(int write_pipe_fd, char* token) {
  int bytes_written = write(write_pipe_fd, token, 1);
  if (bytes_written != 1) {
    fprintf(stderr, "Error while writing token: %d\n", bytes_written);
    exit(1);
  }
}

long bread(void* buf, long nbytes) {
  long sum = 0;
  register long *p, *next;
  register char* end;

  p = (long*)buf;
  end = (char*)buf + nbytes;
  for (next = p + 128; (void*)next <= (void*)end; p = next, next += 128) {
    sum +=
        p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8] + p[9] +
        p[10] + p[11] + p[12] + p[13] + p[14] + p[15] + p[16] + p[17] + p[18] +
        p[19] + p[20] + p[21] + p[22] + p[23] + p[24] + p[25] + p[26] + p[27] +
        p[28] + p[29] + p[30] + p[31] + p[32] + p[33] + p[34] + p[35] + p[36] +
        p[37] + p[38] + p[39] + p[40] + p[41] + p[42] + p[43] + p[44] + p[45] +
        p[46] + p[47] + p[48] + p[49] + p[50] + p[51] + p[52] + p[53] + p[54] +
        p[55] + p[56] + p[57] + p[58] + p[59] + p[60] + p[61] + p[62] + p[63] +
        p[64] + p[65] + p[66] + p[67] + p[68] + p[69] + p[70] + p[71] + p[72] +
        p[73] + p[74] + p[75] + p[76] + p[77] + p[78] + p[79] + p[80] + p[81] +
        p[82] + p[83] + p[84] + p[85] + p[86] + p[87] + p[88] + p[89] + p[90] +
        p[91] + p[92] + p[93] + p[94] + p[95] + p[96] + p[97] + p[98] + p[99] +
        p[100] + p[101] + p[102] + p[103] + p[104] + p[105] + p[106] + p[107] +
        p[108] + p[109] + p[110] + p[111] + p[112] + p[113] + p[114] + p[115] +
        p[116] + p[117] + p[118] + p[119] + p[120] + p[121] + p[122] + p[123] +
        p[124] + p[125] + p[126] + p[127];
  }
  for (next = p + 16; (void*)next <= (void*)end; p = next, next += 16) {
    sum += p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8] + p[9] +
           p[10] + p[11] + p[12] + p[13] + p[14] + p[15];
  }
  for (next = p + 1; (void*)next <= (void*)end; p = next, next++) {
    sum += *p;
  }
  return sum;
}

int main(int argc, char* argv[]) {
  check_invalid_usage(argc);

  char *read_pipe_path = argv[1], *write_pipe_path = argv[2];

  void* data = NULL;
  int process_size = 0;

  if (argc == 4) {
    process_size = atoi(argv[3]) * 1024;
    if (process_size > 0) {
      data = malloc(process_size);
      if (data) bzero(data, process_size);
    }
  }

  int read_pipe_fd = open(read_pipe_path, O_RDONLY);
  int write_pipe_fd = open(write_pipe_path, O_WRONLY);

  for (;;) {
    char token[2];
    read_token(read_pipe_fd, token);
    bread(data, process_size);
    write_token(write_pipe_fd, token);
  }
  return 0;
}
