#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


#define MAX_PATH_LEN 200
#define MAX_PIPE_PATH_LEN 10 // path is of pattern "/pipes/1"
#define MAX_CONTAINER_NAME_LEN 15 // name is of pattern "container_1"
#define MAX_PROC_SIZE_LEN 5

const char* sudo_path = "/usr/bin/sudo";
const char* docker_path = "/usr/bin/docker";


void get_container_name(char* name, int id) {
        sprintf(name, "container_%d", id);
}

void disable_print() {
        int fd = open("/dev/null", O_WRONLY);

        dup2(fd, 1);    /* make stdout a copy of fd (> /dev/null) */
        dup2(fd, 2);    /* ...and same with stderr */
        close(fd);      /* close fd */
}

void get_volume_arg(char* volume_arg) {
        char cwd[MAX_PATH_LEN];
        getcwd(cwd, sizeof(cwd));
        sprintf(volume_arg, "%s/pipes:/pipes/", cwd);
}

void run_container(int container_id, int proc_size, char* image_name) {
        char read_pipe_path[MAX_PIPE_PATH_LEN], write_pipe_path[MAX_PIPE_PATH_LEN];
        sprintf(read_pipe_path, "/pipes/%d", container_id - 1);
        sprintf(write_pipe_path, "/pipes/%d", container_id);

        char container_name[MAX_CONTAINER_NAME_LEN]; 
        get_container_name(container_name, container_id);

        char volume_arg[MAX_PATH_LEN];
        get_volume_arg(volume_arg);

        char proc_size_str[MAX_PROC_SIZE_LEN];
        sprintf(proc_size_str, "%d", proc_size);

        disable_print();

        char *args[] = {sudo_path, docker_path, "run", "--rm", "-d", "-v", volume_arg, "--name", container_name, image_name, read_pipe_path, write_pipe_path, proc_size_str, NULL};
        execv(sudo_path, args);
}

void run_containers(int container_count, int proc_size, char* image_name) {
        for (int i = 0; i < container_count; ++i) {
                int container_id = i + 1;
                // fprintf(stderr, "containerId: %d\n", container_id);
                pid_t pid = fork();
                switch (pid) {
                        case -1:
                                fprintf(stderr, "Failed to create docker container %d, error during fork\nExiting.\n", container_id);
                                exit(1);
                                break;
                        case 0:
                                run_container(container_id, proc_size, image_name);
                                break;
                        default:
                                wait(NULL);
                }
        }
}

void kill_container(int container_id) {
        char container_name[MAX_CONTAINER_NAME_LEN];
        get_container_name(container_name, container_id);

        disable_print();

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

