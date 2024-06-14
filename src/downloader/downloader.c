#include<stdio.h>
#include <stdlib.h>
#include <string.h>

const char* TARGET_DIR = "~/Downloads/";

int downloader_main()
{
    char* url = "https://artix.unixpeople.org/iso/artix-base-dinit-20230814-x86_64.iso";

    char* cmd = malloc(sizeof(char) * 100 + 1);
    strcat(cmd, "wget --continue --quiet ");
    strcat(cmd, url);

    system(cmd);

    printf("\nDownloader online.");
    return 0;
}
