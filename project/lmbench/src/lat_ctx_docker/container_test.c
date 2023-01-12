#include "container.h"

int main() {
	int container_count = 2, proc_size = 1;
	run_containers(container_count, proc_size, "docker_proc", "3", "outputs/container_test");
	printf("Containers started...\n");
	sleep(5);
	kill_containers(container_count);
	printf("Containers killed...\n");
}
