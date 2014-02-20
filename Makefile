# Makefile for the CS:APP Shell Lab


VERSION = 1
ipc = ./ipc
CC = gcc
CXX = g++
CFLAGS = -Wall -O
FILES = $(ipc)
 

all: $(FILES)

ipc: ipc.o helper-routines.o
	$(CXX) -o ipc ipc.o helper-routines.o
	
pipes:
	./ipc -p

signals:
	./ipc -s

clean:
	rm -f $(FILES) *.o *~
