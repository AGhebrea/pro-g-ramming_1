#include<stdio.h>
#include <stdlib.h>
#include <string.h>

const char* TARGET_DIR = "~/Downloads/";

int downloader_main()
{
    char* url = "https://artix.unixpeople.org/iso/artix-base-dinit-20230814-x86_64.iso";
    char* wget_options = "--continue --quiet";
    char* fmt = "wget %s %s";

    char *cmd = NULL;
    size_t n = snprintf(NULL, 0, fmt, wget_options, url);
    
    cmd = malloc(sizeof(char) * (n + 1));

    snprintf(cmd, n+1, fmt, wget_options, url);
    // todo: error handling

    printf("cmd: [%s]", cmd);

    system(cmd);

    printf("\nDownloader online.");
    return 0;
}
