/*
 * msleep.c
 *
 * A routine to let you sleep with millisecond precision, instead of seconds
 * like sleep(3).
 *
 * Jon Howell
 * Thu Apr  9 23:18:35 EDT 1998
 *
 * $Id: msleep.c,v 1.1 1998/04/10 03:19:34 jonh Exp $
 * $Log: msleep.c,v $
 * Revision 1.1  1998/04/10 03:19:34  jonh
 * Initial revision
 *
 */

#include <sys/types.h>
#include <sys/time.h>

void msleep(int milliseconds)
{
	struct timeval tv;

	if (milliseconds > 1000)
		milliseconds = 999;
	if (milliseconds < 1)
		milliseconds = 1;

	tv.tv_sec = 0;
	tv.tv_usec = milliseconds*1000;

	select(0, NULL, NULL, NULL, &tv);
}
