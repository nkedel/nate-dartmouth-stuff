#ifndef __SOCKADDR_H
#define __SOCKADDR_H

#include <netinet/in.h>		/* sockaddr_in definition */
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int getAddressForHost(struct sockaddr_in *sa, char *hostname, int port);
void getAddressForAnyHost(struct sockaddr_in *sa, int port);
char *getHostnameFromAddress(struct sockaddr_in *sa);

#endif /* __SOCKADDR_H */
