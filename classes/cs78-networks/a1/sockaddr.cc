/*
 * sockaddr.c
 *
 * A suite of routines to insulate students from the hairy sockaddr_in
 * structure.
 * The struct sockaddr_in is a "subclass" of sockaddr;
 * conceptually, that is, but not syntactically, since this is C and not C++.
 * That means you'll need to explicitly cast when passing it to system
 * functions that expect struct sockaddrs) of sockaddr.
 *
 * Jon Howell
 * Thu Apr  2 13:01:31 EST 1998
 *
 * $Id: sockaddr.c,v 1.5 1998/04/04 21:31:50 jonh Exp $
 * $Log: sockaddr.c,v $
 * Revision 1.5  1998/04/04 21:31:50  jonh
 * #included stdio.h
 * added missing return statement at end of getHostnameFromAddress()
 *
 * Revision 1.4  1998/04/03 01:41:29  jonh
 * *** empty log message ***
 *
 * Revision 1.3  1998/04/02 21:20:26  jonh
 * added decent comments
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include "sockaddr.h"

int getAddressForHost(struct sockaddr_in *sa, char *hostname, int port)
/* Purpose: fills in a sockaddr_in with a specific host and port
 * Input Parameters: the hostname and port number of the machine you
 *		want to connect to.
 * Return value: The struct sockaddr_in passed as a pointer is
 *		filled in with the appropriate values.
 *		Returns 0 if successful, or -1 if the hostname couldn't be resolved
 *		to an IP address.
 */
{
	struct hostent *he;
		/* structure returned by gethostbyname(), contains IP address. */

	he = gethostbyname(hostname);	/* lookup IP address for hostname */
	if (he==NULL) {
		return -1;
	}

	sa->sin_family = AF_INET;
	sa->sin_port = htons(port);
		/* the htonl(), htons(), ntohl(), ntohs() family of functions	*/
		/* handles byte-order transformations (little- and big-endian)	*/
		/* transparently. The Internet (IP) is "big-endian." Alphas are	*/
		/* little-endian. So each function reorders the bytes in a		*/
		/* short- or long-word to match what IP expects. On a big-endian*/
		/* host, they do nothing (since the byte order is already		*/
		/* correct), and on some really bizarre old processors they do	*/
		/* really bizarre things.										*/

	memcpy(&sa->sin_addr.s_addr,	/* extract just one IP address from */
			he->h_addr_list[0],		/* struct hostent retrieved above	*/
			sizeof(sa->sin_addr.s_addr));
	return 0;
}

void getAddressForAnyHost(struct sockaddr_in *sa, int port)
/* Purpose: fills in a sockaddr_in with just a port number, and
 *		a wildcard address. This lets you listen to a given port number
 *		regardless of the IP address(es) of the machine you're running on.
 * Input Parameters: the port number you want to listen on.
 * Return value: The struct sockaddr_in passed as a pointer is
 *		filled in with the appropriate values.
 */
{
	sa->sin_family = AF_INET;
	sa->sin_port = htons(port);
	sa->sin_addr.s_addr = htonl(INADDR_ANY);
}

char *getHostnameFromAddress(struct sockaddr_in *sa)
/* Purpose: takes an existing sockaddr_in, and looks up the
 *		hostname of the address therein.
 * Parameters: A struct sockaddr_in * describing the address of
 *		the host you want to get a name for.
 * Return value: A pointer to a C string (char*).
 *		You should copy the name to another string right away, as it may
 *		get overwritten on future calls.
 *		(This means it's also not multithread-ready.)
 */
{
	struct hostent *he;
		/* structure returned by gethostbyaddr(), contains hostname. */

	/* perform a "DNS reverse lookup" to turn an IP address back */
	/* into a hostname. */
	he = gethostbyaddr((char *) &sa->sin_addr,
						sizeof(sa->sin_addr),
						AF_INET);

	if (he!=NULL) {
		/* if the lookup succeeded, return the resolved name */
		return he->h_name;
	} else {
		/* if the lookup failed, we format a string (sprintf) that	*/
		/* contains a dotted-decimal representation of the IP addr	*/

		int addrbuf;	/* IP address is copied here, using ntohl() to	*/
						/* fix byte order.								*/
		char *addrp;	/* points to addrbuf to give us byte-by-byte	*/
						/* access to the address components.			*/
		static char dummy[1024];
						/* the string we'll end up returning. Notice	*/
						/* it's allocated 'static' so that it doesn't	*/
						/* disappear when the function returns. (though	*/
						/* it does get overwritten when the function is	*/
						/* called again.								*/

		addrbuf = ntohl(sa->sin_addr.s_addr);
		addrp = (char *) &addrbuf;
		sprintf(dummy, "%u.%u.%u.%u",
			addrp[0],
			addrp[1],
			addrp[2],
			addrp[3]);

		return dummy;
	}
}
