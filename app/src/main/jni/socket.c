#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>

#include <time.h>

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define COUNT (100000)
#define PACK_SIZE (64)

#define THREAD_HOPPING 0
#define CENTER_DRIVER 1

static double g_result = 0.0;

inline void select_fd(int fd) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  fprintf(stderr, "EEE %s begin\n", __func__);
  int r = select (fd + 1, &fds, NULL, NULL, NULL);
  fprintf(stderr, "EEE %s r=%d\n", __func__, r);
}

int recv_and_send(int fd1, int fd2) {
  char r[PACK_SIZE];
  if (recv(fd1, r, sizeof(r), 0) != sizeof(r)) {
    perror("recvmsg failed");
    return -1;
  }

  if (send(fd2, r, sizeof(r), 0) != sizeof(r)) {
    perror("sendmsg failed");
    return -1;
  }
}

int send_and_recv(int fd1, int fd2) {
  char i[PACK_SIZE];
  if (send(fd2, i, sizeof(i), 0) != sizeof(i)) {
    perror("sendmsg failed");
    return -1;
  }

  char r[PACK_SIZE];
  if (recv(fd1, r, sizeof(r), 0) != sizeof(r)) {
    perror("recvmsg failed");
    return -1;
  }
}

void* work_thread(void *p) {
  int fd = *(int*)(p);
  while(1) {
    if (recv_and_send(fd, fd) < 0)
      return NULL;
  }
  return NULL;
}


// Server
int server_process(int fd) {
#if THREAD_HOPPING == 1
  int sv[2];
  int error = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
  if (error != 0) {
    fprintf(stderr, "Create socket pair failed!\n");
    return EXIT_FAILURE;
  }

  pthread_t t;
  pthread_create(&t, NULL, work_thread, (void*)&sv[1]);

  while(1) {
    if (recv_and_send(fd, sv[0]) < 0)
      return -1;
    if (recv_and_send(sv[0], fd) < 0)
      return -1;
  }
#else
  work_thread((void*)&fd);
#endif
  return 0;
}

// Driver
int driver_process(int fd1, int fd2) {
  int max_fd = (fd1 > fd2 ? fd1 : fd2) + 1;
  while (1) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd1, &fds);
    FD_SET(fd2, &fds);
    int r = select(max_fd, &fds, NULL, NULL, NULL);
    if (FD_ISSET(fd1, &fds)) {
      if (recv_and_send(fd1, fd2) < 0)
        return -1;
    }
    if (FD_ISSET(fd2, &fds)) {
      if (recv_and_send(fd2, fd1) < 0)
        return -1;
    }
  }
}

double clock_now() {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    return 0;
  }
  return ts.tv_sec + (((double)ts.tv_nsec) / 1000000000);
}

void* work_client_thread(void* p) {

  int fd = *(int*)p;

  double begin = clock_now();
  int r = 0;
  int i;
  for (i = 0; i < COUNT; i++) {
    if (send_and_recv(fd, fd) < 0)
      return NULL;
  }

  g_result = clock_now() - begin;
  fprintf(stderr, "EEE r=%d time=%lf\n", r, g_result);
  shutdown(fd, SHUT_RDWR);
  return NULL;
}
// client
int client_process(int fd) {
#if THREAD_HOPPING == 1
  int sv[2];
  int error = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
  if (error != 0) {
    fprintf(stderr, "Create socket pair failed!\n");
    return EXIT_FAILURE;
  }

  pthread_t t;
  pthread_create(&t, NULL, work_client_thread, (void*)&sv[1]);

  while(1) {
    if (recv_and_send(sv[0], fd) < 0)
      return -1;
    if (recv_and_send(fd, sv[0]) < 0)
      return -1;
  }
#else
  work_client_thread((void*)&fd);
#endif
}


double socket_main() {
  int sv_client[2];
  int error = socketpair(AF_UNIX, SOCK_STREAM, 0, sv_client);
  if (error != 0) {
    fprintf(stderr, "Create socket pair failed!\n");
    return EXIT_FAILURE;
  }


  pid_t pid = fork();
  if (pid != 0) {
    // client process
    close(sv_client[0]);
    client_process(sv_client[1]);
    return g_result;
  } else {
    close(sv_client[1]);
    int sv_server[2];
    error = socketpair(AF_UNIX, SOCK_STREAM, 0, sv_server);
    if (error != 0) {
      fprintf(stderr, "Create socket pair failed!\n");
      close(sv_client[0]);
      exit(EXIT_FAILURE);
    }

#if CENTER_DRIVER == 1
    if (fork() != 0) {
      // driver process
      close(sv_server[0]);
      int r = driver_process(sv_client[0], sv_server[1]);
      exit(r);
    } else {
      // server process
      close(sv_server[1]);
      close(sv_client[0]);
      int r = server_process(sv_server[0]);
      exit(r);
    }
#else
      int r = server_process(sv_client[0]);
      exit(r);
#endif
  }
}

