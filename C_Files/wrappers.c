#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <netdb.h>
#include "wrappers.h"

pid_t Fork(void) {
    pid_t pid;
    if ((pid = fork()) == -1) {
        perror("Fork failure.");
        exit(-1);
    }
    return pid;
}

int Pipe(int pipefd[2]) {
    int p = pipe(pipefd);
    if (p == -1) {
        perror("Pipe failure.");
        exit(-1);
    } else {
        return p;
    }
}

int Read(int fd, void *buf, size_t count) {
    int r = read(fd, buf, count);
    if (r == -1) {
        perror("Read failure.");
        exit(-1);
    } else {
        return r;
    }
}

int Write(int fd, const void *buf, size_t count) {
    int w = write(fd, buf, count);
    if (w == -1) {
        perror("Write failure.");
        exit(-1);
    } else {
        return w;
    }
}

pid_t Wait(int *status) {
    pid_t w = wait(status);
    if (w == -1) {
        perror("Wait error.");
        exit(-1);
    } else {
        return w;
    }
}

pid_t Waitpid(pid_t pid, int *status, int options) {
    pid_t w = waitpid(pid, status, options);
    if (w == -1) {
        perror("Waitpid error.");
        exit(-1);
    } else {
        return w;
    }
}

int Open(const char *pathname, int flags, mode_t mode) {
    int o = open(pathname, flags, mode);
    if (o == -1) {
        perror("Open failure.");
        exit(-1);
    } else {
        return o;
    }
}

int Close(int fd) {
    int c = close(fd);
    if (c == -1) {
        perror("Close failure.");
        exit(-1);
    } else {
        return c;
    }
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int c = connect(sockfd, addr, addrlen);
    if (c == -1) {
        perror("Connect failure.");
        exit(-1);
    } else {
        return c;
    }
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int b = bind(sockfd, addr, addrlen);
    if (b == -1) {
        perror("Bind failure.");
        exit(-1);
    } else {
        return b;
    }
}

int Listen(int sockfd, int backlog) {
    int l = listen(sockfd, backlog);
    if (l == -1) {
        perror("Listen failure.");
        exit(-1);
    } else {
        return l;
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int a = accept(sockfd, addr, addrlen);
    if (a == -1) {
        perror("Accept failure.");
        exit(-1);
    } else {
        return a;
    }
}

ssize_t Send(int sockfd, const void *buf, size_t len, int flags) {
    ssize_t s = send(sockfd, buf, len, flags);
    if (s == -1) {
        perror("send failure.");
        exit(-1);
    } else {
        return s;
    }
}

ssize_t Recv(int sockfd, void *buf, size_t len, int flags) {
    ssize_t r = recv(sockfd, buf, len, flags);
    if (r == -1) {
        perror("receive failure.");
        exit(-1);
    } else {
        return r;
    }
}

int Socket(int domain, int type, int protocol) {
    int s = socket(domain, type, protocol);
    if (s == -1) {
        perror("socket failure.");
        exit(-1);
    } else {
        return s;
    }
}
