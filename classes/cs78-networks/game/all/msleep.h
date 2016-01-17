/*
 * msleep.h
 *
 * A routine to let you sleep with millisecond precision, instead of seconds
 * like sleep(3).
 *
 * Jon Howell
 * Thu Apr  9 23:18:35 EDT 1998
 *
 * $Id: msleep.h,v 1.1 1998/04/10 03:19:34 jonh Exp $
 * $Log: msleep.h,v $
 * Revision 1.1  1998/04/10 03:19:34  jonh
 * Initial revision
 *
 */

#ifndef __MSLEEP_H
#define __MSLEEP_H

void msleep(int milliseconds);

#endif /* __MSLEEP_H */
