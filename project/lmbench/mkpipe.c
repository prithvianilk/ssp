#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    mkfifo("pipes/1", 0666);
    mkfifo("pipes/2", 0666);
    return 0;
}
