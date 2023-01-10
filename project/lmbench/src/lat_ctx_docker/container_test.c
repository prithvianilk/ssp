#include "container.h"

int main() {
	int container_count = 2;
	run_containers(container_count, 1, "docker_proc", "2", "outputs/test", "perf");
	printf("Containers started...\n");
	sleep(5);
	kill_containers(container_count);
	printf("Containers killed...\n");
}
