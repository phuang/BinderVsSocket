#include <sys/types.h>
#include <sys/socket.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int binder_client_main(int fd);
int binder_server_main();

extern "C"
double binder_main() {
    double result = -9;
    do {
        int sv[2];
        int error = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
        if (error != 0) {
            fprintf(stderr, "Create socket pair failed!\n");
            break;
        }
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Fork failed!\n");
            break;
        }

        if (pid != 0) {
            // Parent process
            close(sv[1]);
            binder_server_main();
            if (read(sv[0], &result, sizeof(result)) != sizeof(result))
                result = -8;
            return result;
        } else {
            // Child process
            close(sv[0]);
            sleep(1);
            binder_client_main(sv[1]);
            exit(0);
        }
    } while (false);
    return result;
}