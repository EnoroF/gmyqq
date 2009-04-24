# Makefile for MyQQ
MyQQ_HOME=..
SRC=$(MyQQ_HOME)/src
BIN=$(MyQQ_HOME)/bin
OBJ=$(MyQQ_HOME)/obj
DATA=$(MyQQ_HOME)/data
EXEC=$(BIN)/myqq


CC=	gcc
CFLAGS=	-c -Wall -Werror
LDFLAGS=-lpthread -s
LD=	gcc

OBJS=qqsocket.o qqcrypt.o md5.o debug.o qqclient.o memory.o config.o packetmgr.o qqpacket.o \
		prot_login.o protocol.o prot_misc.o prot_im.o prot_user.o list.o buddy.o group.o qun.o \
		prot_group.o prot_qun.o prot_buddy.o loop.o utf8.o myqq.o util.o crc32.o

SRCS:= $(addsuffix .c, $(basename $(OBJS)))
OBJS:=$(addprefix $(OBJ)/, $(OBJS))
SRCS:=$(addprefix $(SRC)/,$(OBJS))

all: $(EXEC)
	@echo done.

$(EXEC): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) `pkg-config --cflags dbus-glib-1 libnm_glib` -c $< -o $@


.PHONY: clean cleanobj
clean:
	rm -f $(OBJ)/*.o
run:
	(xterm -e "cd $(BIN);./myqq"&)
update:
	@echo update.
install:
	cp -vr $(BIN) ~/bin/MyQQ
	cp -v $(DATA)/myqq ~/bin
release:
	tar jcvf ~/Dropbox/Public/myqq_3.13.tar.bz2 $(MyQQ_HOME)/../myqq3.13/
