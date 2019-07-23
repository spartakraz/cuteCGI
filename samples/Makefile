# Makefile for CGI library's samples

CC = gcc
CC_OPTIONS = -g -Wall -Wfatal-errors
CC_INCLUDE = -I/usr/include/gdbm
CC_REFS = -lccgi -lgdbm_compat -lgdbm

all: cc_all

cc_all: cc_guestbook cc_visitcount
	@echo "Done!"
	
cc_guestbook: guestbook.c
	$(CC) $(CC_OPTIONS) $(CC_INCLUDE) guestbook.c -o guestbook.cgi $(CC_REFS)
cc_visitcount: visitcount.c
	$(CC) $(CC_OPTIONS) $(CC_INCLUDE) visitcount.c -o visitcount.cgi $(CC_REFS)	
	
clean:
	rm -v guestbook.cgi visitcount.cgi		

