
SRC=./src
BUILDDIR=./bin
LIBS=-lpthread
CFLAGS=-g3

all:
	mkdir --parents $(BUILDDIR)
	gcc $(SRC)/main.c $(SRC)/downloader/downloader.c $(SRC)/scanner/scanner.c $(CFLAGS) $(LIBS) -o $(BUILDDIR)/dlman
