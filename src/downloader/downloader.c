#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

const char* TARGET_DIR = "downloads";

struct shared_data_t {
    bool finished;
    pthread_mutex_t mutex;
};

struct shared_data_t shared_data;

void init_shared_data();
void* download_from_url(void* arg);
int get_download_size(char* url);

int downloader_main()
{
    pthread_t thread_t;
    int ret;
    bool exit_flag = false;

    char* url = "https://artix.unixpeople.org/iso/artix-base-dinit-20230814-x86_64.iso";
    // char* url = "https://artix.unixpeople.org/iso/artix-base-dinit-20230814-x86_64.iso.sig";
    char* last_slash = strrchr(url, '/');
    char* filename = last_slash ? last_slash + 1 : url;

    int size = get_download_size(url);

    ret = pthread_create(&thread_t, NULL, download_from_url, url);
    
    if (ret != 0) {
        printf("Failed to create thread.\n");
        exit(EXIT_FAILURE);
    }

    struct stat sb;

    char *path = NULL;
    char* fmt = "%s/%s";
    size_t n = snprintf(NULL, 0, fmt, TARGET_DIR, filename);

    path = malloc(sizeof(char) * (n + 1));

    snprintf(path, n+1, fmt, TARGET_DIR, filename);
    // todo: error handling

    while (stat(path, &sb) && errno == ENOENT) {
        usleep(100000);
    }
    char *output_str = NULL;
    char* fmt_output = "Downloading: %ld/%d\r";

    printf("\nDownloading %s\n", filename);
    pthread_mutex_lock(&shared_data.mutex);
    if(shared_data.finished == true)
        exit_flag = true;
    pthread_mutex_unlock(&shared_data.mutex);
    while (! exit_flag) {
        // todo: make shared_data.finished write only 
        // by downloader thread ???
        pthread_mutex_lock(&shared_data.mutex);
        if(shared_data.finished == true)
            exit_flag = true;
        pthread_mutex_unlock(&shared_data.mutex);
        stat(path, &sb);

        n = snprintf(NULL, 0, fmt_output, sb.st_size, size);
        output_str = malloc(sizeof(char) * (n + 1));
        snprintf(output_str, n+1, fmt_output, sb.st_size, size);

        write(STDOUT_FILENO, output_str, n + 1);
        usleep(300000);
        free(output_str);
    }
    
    printf("\nDownloader online.");
    free(path);
    return 0;
}

void init_shared_data()
{
    shared_data.finished = false;
    pthread_mutex_init(&shared_data.mutex, NULL);
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

    system(cmd);

    pthread_mutex_lock(&shared_data.mutex);
    shared_data.finished = true;
    pthread_mutex_unlock(&shared_data.mutex);

    free(cmd);
    return NULL;
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

    free(cmd);
    return size;
}
