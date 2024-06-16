
SRC=./src
BUILDDIR=./bin
LIBS=-lpthread

all:
	mkdir --parents $(BUILDDIR)
	gcc $(SRC)/main.c $(SRC)/downloader/downloader.c $(SRC)/scanner/scanner.c $(LIBS) -o $(BUILDDIR)/dlman
