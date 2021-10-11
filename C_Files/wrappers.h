#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

/**
 * Wrapper for fork().
 * 
 * @return pid of process.
 */
pid_t Fork(void);

/**
 * Wrapper for pipe().
 *
 * @return 0 upon success, -1 upon error.
 */
int Pipe(int pipefd[2]);

/**
 * Wrapper for read().
 *
 * @return number of bytes read.
 */
int Read(int fd, void *buf, size_t count);

/**
 * Wrapper for write().
 *
 * @return number of bytes written.
 */
int Write(int fd, const void *buf, size_t count);

pid_t Wait(int *status);

pid_t Waitpid(pid_t pid, int *status, int options);

int Open(const char *pathname, int flags, mode_t mode);

int Close(int fd);

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

ssize_t Send(int sockfd, const void *buf, size_t len, int flags);

ssize_t Recv(int sockfd, void *buf, size_t len, int flags);

int Socket(int domain, int type, int protocol);

#endif /* _WRAPPERS_H_ */
