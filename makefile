#
#	Makefile for squish interpreter
#

EXE	= squish
OBJS = \
	squish_main.o \
	squish_run.o \
	squish_tokenize.o

CFLAGS = -g

$(EXE) : $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

clean :
	- rm -f $(EXE)
	- rm -f $(OBJS)
	- rm -f tags


# ctags makes a database of source code tags for use with vim and co
tags ctags : dummy
	- ctags *.c

# a rule like this dummy rule can be use to make another rule always
# run - ctags depends on this, so therefore ctags will always be
# executed by "make ctags" as make is fooled into thinking that it
# has performed a dependency each time, and therefore re-does the work
dummy :
