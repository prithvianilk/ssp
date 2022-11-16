#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


const char* sudo_path = "/usr/bin/sudo";
const char* docker_path = "/usr/bin/docker";


void get_container_name(char* name, int id) {
        sprintf(name, "container_%d", id);
}

void run_container(int container_id) {
        char read_pipe_path[10], write_pipe_path[10]; // path is of pattern "/pipes/1"
        sprintf(read_pipe_path, "/pipes/%d", container_id - 1);
        sprintf(write_pipe_path, "/pipes/%d", container_id);

        char container_name[12]; // name is of pattern "container_1"
        get_container_name(container_name, container_id);

        char *args[] = {sudo_path, docker_path, "run", "--rm", "-d", "--name", container_name, "docker_proc", read_pipe_path, write_pipe_path, NULL};
        execv(sudo_path, args);
}

void run_containers(int container_count) {
        for (int i = 0; i < container_count; ++i) {
		int container_id = i + 1;
                pid_t pid = fork();
                switch (pid) {
                        case -1:
                                fprintf(stderr, "Failed to create docker container %d, error during fork\nExiting.\n", container_id);
                                break;
                        case 0:
                                run_container(container_id);
                                break;
			default:
				wait(NULL);
                }
        }
}

void kill_container(int container_id) {
        char container_name[12]; // name is of pattern "container_1"
        get_container_name(container_name, container_id);

        char *args[] = {sudo_path, docker_path, "kill", container_name, NULL};
        execv(sudo_path, args);
}

void kill_containers(int container_count) {
	for (int i = 0; i < container_count; ++i) {
		int container_id = i + 1;
                pid_t pid = fork();
                switch (pid) {
                        case -1:
                                fprintf(stderr, "Failed to kill docker container %d, error during fork\nExiting.\n", container_id);
                                break;
                        case 0:
                                kill_container(container_id);
                                break;
			default:
				wait(NULL);
                }
	}
}

