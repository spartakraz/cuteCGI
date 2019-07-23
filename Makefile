# Makefile for cuteCGI library

CC = gcc
CC_OPTIONS = -g -std=c99 -Wall -Wfatal-errors 
AR = ar
AR_OPTIONS = rcs
SRC_DIR = .
INC_DIR = .
BIN_DIR = ./bin
LIB_DIR = ./bin
INST_LIB_DIR = /usr/local/lib
INST_HDR_DIR = /usr/local/include/ccgi
SESSION_DIR = /var/tmp

API_HDR = ccgi.h
HELPER_HDR_1 = ccgi_cookie.h
HELPER_HDR_2 = ccgi_env.h
HELPER_HDR_3 = ccgi_header.h
HELPER_HDR_4 = ccgi_html.h
HELPER_HDR_5 = ccgi_request.h
HELPER_HDR_6 = ccgi_shared.h
HELPER_HDR_7 = ccgi_stdout.h
HELPER_HDR_8 = ccgi_session.h

LIB_NAME = libccgi.a

UNIT1=ccgi

all: build_lib

build_lib: $(UNIT1)
	$(AR) $(AR_OPTIONS) $(LIB_DIR)/$(LIB_NAME) \
	$(BIN_DIR)/$(UNIT1).o
	
$(UNIT1): $(SRC_DIR)/$(UNIT1).c $(SRC_DIR)/$(API_HDR)
	$(CC) $(CC_OPTIONS) -c -I$(INC_DIR) $(SRC_DIR)/$(UNIT1).c -o $(BIN_DIR)/$(UNIT1).o
clean:
	rm -v $(BIN_DIR)/*
install: build_lib
	@if [ -d $(INST_LIB_DIR) ]; then \
		cp -v $(LIB_DIR)/$(LIB_NAME) $(INST_LIB_DIR); \
		echo "Installed in $(INST_LIB_DIR)"; \
	else \
		echo "$(INST_LIB_DIR) not found!";\
	fi	
	@if [ ! -d $(INST_HDR_DIR) ]; then \
		mkdir $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(API_HDR) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_1) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_2) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_3) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_4) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_5) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_6) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_7) $(INST_HDR_DIR); \
		cp -v $(SRC_DIR)/$(HELPER_HDR_8) $(INST_HDR_DIR); \
		echo "Installed in $(INST_HDR_DIR)"; \
	fi	
	@if [ ! -d $(SESSION_DIR) ]; then \
		echo "$(SESSION_DIR) not found!"; \
		mkdir $(SESSION_DIR); \
	fi

uninstall:
	@if [ -e $(INST_LIB_DIR)/$(LIB_NAME) ]; then \
		rm -v $(INST_LIB_DIR)/$(LIB_NAME); \
		echo "Uninstalled from $(INST_LIB_DIR)"; \
	else \
		echo "$(INST_LIB_DIR)/$(LIB_NAME) not found!";\
	fi	
	@if [ -e $(INST_HDR_DIR)/$(API_HDR) ]; then \
		rm -v $(INST_HDR_DIR)/*.*; \
		rm -rvf $(INST_HDR_DIR); \
		echo "Uninstalled from $(INST_HDR_DIR)"; \
	else \
		echo "$(INST_HDR_DIR)/$(API_HDR) not found!";\
	fi	
