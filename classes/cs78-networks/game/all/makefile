CC = g++
CFLAGS = -Wall

all: tester gameserv gameclient

gameserv: gameserv.o list.o String.o msleep.o sockaddr.o sockserver.o sockserverp.o dbase1.o dctrl1.o ac.o
	${CC} -o $@  gameserv.o list.o String.o msleep.o sockaddr.o sockserver.o sockserverp.o dbase1.o dctrl1.o ac.o

gameclient: menu.o sockcom.o String.o sockaddr.o gamec.o ac.o
	${CC} -o $@  menu.o sockcom.o String.o sockaddr.o gamec.o ac.o

tester: datatest.o dbase1.o String.o dctrl1.o ac.o
	${CC} -o $@ datatest.o dbase1.o String.o dctrl1.o ac.o

menu.o: menu.cc game.h gamec.cc player.h monster.h globals.h
	${CC} ${CFLAGS} -c menu.cc

game.o: menu.cc game.cc gamec.cc player.h monster.h globals.h
	${CC} ${CFLAGS} -lcurses -c game.cc

gamec.o: menu.cc game.cc gamec.cc player.h monster.h globals.h
	${CC} ${CFLAGS} -c gamec.cc

sockcom.o: menu.cc game.cc gamec.cc player.h monster.h globals.h sockcom.cc sockcom.h
	${CC} ${CFLAGS} -c sockcom.cc

datatest.o: datatest.cc String.cc dbase1.cc dctrl1.cc String.h dbase1.h dctrl1.h
	${CC} ${CFLAGS} -c datatest.cc

dbase1.o: dbase1.cc dbase1.h 
	${CC} ${CFLAGS} -c dbase1.cc

dctrl1.o: dctrl1.cc dctrl1.h dbase1.cc dbase1.h
	${CC} ${CFLAGS} -c dctrl1.cc

ac.o: ac.cc dctrl1.cc dctrl1.h dbase1.cc dbase1.h
	${CC} ${CFLAGS} -c ac.cc

String.o: String.cc String.h
	${CC} ${CFLAGS} -c String.cc

sockserver.o: sockserver.cc sockserver.h
	${CC} ${CFLAGS} -c sockserver.cc

sockserverp.o: sockserverp.cc sockserver.h
	${CC} ${CFLAGS} -c sockserverp.cc

gameserv.o: gameserv.cc String.cc dbase1.cc dctrl1.cc String.h dbase1.h dctrl1.h ac.cc list.cc list.h
	${CC} ${CFLAGS} -c gameserv.cc

list.o: list.cc list.h
	${CC} ${CFLAGS} -c list.cc

msleep.o: msleep.c msleep.h
	${CC} ${CFLAGS} -c msleep.c

sockaddr.o: sockaddr.c sockaddr.h
	${CC} ${CFLAGS} -c sockaddr.c

clean:
	rm -f *.o tester gameserv gameclient