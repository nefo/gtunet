SOURCES = gtunet.c \
	callbacks.c \
	actions.c \
	utils.c \
	log.c \
	mytunet/des.c \
	mytunet/dot1x.c \
	mytunet/ethcard_bpf.c \
	mytunet/ethcard_eth.c \
	mytunet/ethcard_win.c \
	mytunet/logs.c \
	mytunet/md5.c \
	mytunet/mytunet.c \
	mytunet/mytunetsvc.c \
	mytunet/os.c \
	mytunet/os_posix.c \
	mytunet/os_win32.c \
	mytunet/setting.c \
	mytunet/tunet.c \
	mytunet/userconfig.c \
	mytunet/util.c

OBJS = $(subst .c,.o,$(SOURCES))

CC = gcc
CPP = g++

CFLAGS = -g -O -Wall -export-dynamic -D_POSIX -DPOSIX -D_LINUX
CFLAGS += `pkg-config --cflags glib-2.0 gtk+-2.0 gmodule-export-2.0`
LDLIBS = `pkg-config --libs glib-2.0 gtk+-2.0 gmodule-export-2.0`
LDFLAGS = -lglib-2.0 -lm -pthread

all: gtunet

gtunet: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f gtunet $(OBJS) *.o *~
.PHONY: all clean
