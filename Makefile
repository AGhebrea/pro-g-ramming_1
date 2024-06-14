
SRC=./src
BUILDDIR=./bin

all:
	mkdir --parents $(BUILDDIR)
	gcc $(SRC)/main.c $(SRC)/downloader/downloader.c $(SRC)/scanner/scanner.c -o $(BUILDDIR)/dlman
