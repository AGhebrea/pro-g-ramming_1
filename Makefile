
SRC=./src
BUILDDIR=./bin

all:
	gcc $(SRC)/main.c $(SRC)/downloader/downloader.c $(SRC)/scanner/scanner.c -o $(BUILDDIR)/dlman
