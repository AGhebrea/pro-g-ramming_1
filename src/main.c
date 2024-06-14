#include<stdio.h>

#include"./downloader/downloader.h"
#include"./scanner/scanner.h"

int main()
{
    printf("\nMain");

    scanner_main();
    downloader_main();

    printf("\n\n");

    return 0;
}
