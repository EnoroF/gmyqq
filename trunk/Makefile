# Makefile for MyQQ
MyQQ_HOME=.
SRC=$(MyQQ_HOME)/src
BIN=$(MyQQ_HOME)/bin
OBJ=$(MyQQ_HOME)/obj
DATA=$(MyQQ_HOME)/data
EXEC=$(BIN)/gmyqq


CC=	g++

# Use for compile.
CFLAGS = `pkg-config --cflags dbus-glib-1 libglademm-2.4 gtkmm-2.4 libnotifymm-1.0 libnm_glib`
# Use for link.
CLIBS  = `pkg-config --libs dbus-glib-1 gtkmm-2.4 libglademm-2.4 libnotifymm-1.0 libnm_glib` -lpthread -lncursesw -lpanel -s

OBJS=qqsocket.o qqcrypt.o md5.o debug.o qqclient.o memory.o config.o packetmgr.o qqpacket.o \
	prot_login.o protocol.o prot_misc.o prot_im.o prot_user.o list.o buddy.o group.o qun.o \
	prot_group.o prot_qun.o prot_buddy.o loop.o utf8.o util.o crc32.o \
	tinyxml/tinystr.o tinyxml/tinyxml.o tinyxml/tinyxmlerror.o tinyxml/tinyxmlparser.o\
	gmyqq.o myqq.o checkAndSetConfig.o


SRCS:= $(addsuffix .c, $(basename $(OBJS)))
OBJS:=$(addprefix $(OBJ)/, $(OBJS))
SRCS:=$(addprefix $(SRC)/,$(OBJS))

all: $(EXEC)
	@echo done.

$(EXEC): $(OBJS)
	$(CC) $(CLIBS) $^ -g -o $@

$(OBJ)/%.o : $(SRC)/%.c
	gcc `pkg-config --cflags dbus-glib-1 libnm_glib` -c $< -o $@

$(OBJ)/%.o:$(SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) -c $< -o $@

.PHONY: clean cleanobj
clean:
	rm -f $(OBJ)/*.o
run:
	(cd $(BIN);./gmyqq)
runc:
	(cd $(BIN);./gmyqq --nogui)
runt:
	(cd $(BIN);./gmyqq --test)
runt1:
	(cd $(BIN);./gmyqq --test -u 1140048821)
runt2:
	(cd $(BIN);./gmyqq --test -u 1147071944)
update:
	@echo update.
install:
	cp -vr $(BIN) ~/bin/MyQQ
	cp -v $(DATA)/myqq ~/bin
release:
	$(MyQQ_HOME)/export
