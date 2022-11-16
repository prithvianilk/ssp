#include "container.h"

int main() {
	int container_count = 2, proc_size = 1;
	run_containers(container_count, proc_size);
	printf("Containers started...\n");
	sleep(5);
	kill_containers(container_count);
	printf("Containers killed...\n");
}
