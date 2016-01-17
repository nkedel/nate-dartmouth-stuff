/*
 * sockaddr.h
 *
 * Interface to a suite of routines to insulate students from the
 * hairy sockaddr_in structure.
 *
 * Jon Howell
 * Thu Apr  2 16:17:35 EST 1998
 *
 * $Id: sockaddr.h,v 1.3 1998/04/02 21:20:52 jonh Exp $
 * $Log: sockaddr.h,v $
 * Revision 1.3  1998/04/02 21:20:52  jonh
 * added decent comments
 *
 *
 */
#ifndef __SOCKADDR_H
#define __SOCKADDR_H

#include <netinet/in.h>		/* sockaddr_in definition */

int getAddressForHost(struct sockaddr_in *sa, char *hostname, int port);
	/* fills in a sockaddr_in with a specific host and port		*/

void getAddressForAnyHost(struct sockaddr_in *sa, int port);
	/* fills in a sockaddr_in with just a port number,			*/
	/* letting the host part be treated as a wildcard			*/

char *getHostnameFromAddress(struct sockaddr_in *sa);
	/* takes an existing sockaddr_in, and looks up the			*/
	/* hostname of the address therein.							*/

#endif /* __SOCKADDR_H */
