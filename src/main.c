#include<stdio.h>

#include"./downloader/downloader.h"
#include"./scanner/scanner.h"

int main(int argc, char **argv)
{
    printf("\nMain");

    scanner_main(argc, argv);
    downloader_main();

    printf("\n\n");

    return 0;
}
