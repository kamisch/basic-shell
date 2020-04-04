#Usage: make [all|HW5server|HW5client]
#Use warning/wHW5server/wHW5client to show all compilation errors
#Use warning to print out all warnings from gcc
#Use all to compile both HW5server and HW5client
#Use HW5x to compile HW5server or HW5client

CC=gcc
CFLAGS=
LDLIBS=
#Put HW5server/HW5client dependencies here
HW5serverDEPS=
HW5clientDEPS=

all: HW5server HW5client

HW5server: HW5server.c libsocket.o $(HW5serverDEPS)
		$(CC) $(LDLIBS) -o $@.out $^ $(CFLAGS)

HW5client: HW5client.c libsocket.o $(HW5clientDEPS)
		$(CC) $(LDLIBS) -o $@.out $^ $(CFLAGS)

warning: CFLAGS+=-Wall -Werror
warning: all

wHW5server: CFLAGS+=-Wall -Werror
wHW5server: HW5server

wHW5client: CFLAGS+=-Wall -Werror
wHW5client: HW5client
