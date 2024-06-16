#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

const char* TARGET_DIR = "downloads";
bool download_finished = false;

void* download_from_url(void* arg);
int downloaded_so_far(void* arg);
int get_download_size(char* url);

int downloader_main()
{
    pthread_t download_t;
    int download_ret;

    char* url = "https://artix.unixpeople.org/iso/artix-base-dinit-20230814-x86_64.iso";
    // char* url = "https://artix.unixpeople.org/iso/artix-base-dinit-20230814-x86_64.iso.sig";
    char* last_slash = strrchr(url, '/');
    char* filename = last_slash ? last_slash + 1 : url;

    int download_size = get_download_size(url);

    download_ret = pthread_create(&download_t, NULL, download_from_url, url);
    
    if (download_ret != 0) {
        printf("Failed to create thread.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("\nDownloading %s\n", filename);
    while (! download_finished) {
        printf("Downloading: %d/%d \\\r", downloaded_so_far(filename), download_size);
        usleep(1000);
        printf("Downloading: %d/%d |\r", downloaded_so_far(filename), download_size);
        usleep(1000);
        printf("Downloading: %d/%d /\r", downloaded_so_far(filename), download_size);
        usleep(1000);
        printf("Downloading: %d/%d -\r", downloaded_so_far(filename), download_size);
    }
    
    printf("\nDownloader online.");
    return 0;
}

void* download_from_url(void* arg) {
    char *url = (char*) arg;

    char* cmd_options = "--continue --quiet --directory-prefix=";
    char* fmt = "wget %s%s/ %s";

    char *cmd = NULL;
    size_t n = snprintf(NULL, 0, fmt, cmd_options, TARGET_DIR, url);

    cmd = malloc(sizeof(char) * (n + 1));

    snprintf(cmd, n+1, fmt, cmd_options, TARGET_DIR, url);
    // todo: error handling
    // printf("cmd: [%s]\n", cmd);

    system(cmd);

    download_finished = true;

    return NULL;
}

int downloaded_so_far(void* arg) {
    char *filename = (char*) arg;

    char* cmd_options = "-b";
    char* cmd_processing = "awk '{print $1}'";
    char* fmt = "du %s %s/%s | %s";

    char *cmd = NULL;
    size_t n = snprintf(NULL, 0, fmt, cmd_options, TARGET_DIR, filename, cmd_processing);

    cmd = malloc(sizeof(char) * (n + 1));

    snprintf(cmd, n+1, fmt, cmd_options, TARGET_DIR, filename, cmd_processing);
    // todo: error handling
    // printf("cmd: [%s]\n", cmd);

    FILE *pf;
    char data[50];
    pf = popen(cmd, "r");

    if (!pf) {
        printf("Could not open pipe for output.\n");
        exit(EXIT_FAILURE);
    }

    // Grab data from process execution
    fgets(data, 50, pf);
 
    int size = atoi(data);
 
    if (pclose(pf) != 0)
        fprintf(stderr," Error: Failed to close command stream \n");

    return size;
}

int get_download_size(char* url) {
    char* cmd_options = "--spider";
    char* cmd_processing = "awk '/Length/ {print $2}'";
    char* fmt = "wget %s %s 2>&1 | %s";

    char *cmd = NULL;
    size_t n = snprintf(NULL, 0, fmt, cmd_options, url, cmd_processing);

    cmd = malloc(sizeof(char) * (n + 1));

    snprintf(cmd, n+1, fmt, cmd_options, url, cmd_processing);
    // todo: error handling
    // printf("cmd: [%s]\n", cmd);

    FILE *pf;
    char data[50];
    pf = popen(cmd, "r");

    if (!pf) {
        printf("Could not open pipe for output.\n");
        exit(EXIT_FAILURE);
    }

    // Grab data from process execution
    fgets(data, 50, pf);
 
    int size = atoi(data);
 
    if (pclose(pf) != 0)
        fprintf(stderr," Error: Failed to close command stream \n");

    return size;
}
