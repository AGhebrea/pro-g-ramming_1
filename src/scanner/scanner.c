#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<sys/inotify.h>
#include<errno.h>
#include<sys/ioctl.h>
#include"scanner.h"

char *evstr(uint32_t);

// TODO: 
//  - error handling 
//  - daemonize
//  - custom rules + targets
//  - sort files ONLY when all files in folder were CLOSE_WRITEd

int scanner_main()
{
    struct inotify_event buff[12];
    int inotify_fd;
    int watch_fd;
    int count;
    inotify_fd = inotify_init();
    if(inotify_fd < 0)
        perror("inotify_init: ");
    watch_fd = inotify_add_watch(inotify_fd, "./testdir/", IN_ALL_EVENTS);
    if(watch_fd < 0)
        perror("inotify_add_watch: ");
    fflush(stdout);
    while(1){
        errno = 0;
        count = read(inotify_fd, &buff, sizeof(struct inotify_event) * 12);
        if(errno)
            perror("\nread");
        for(int i = 0; i < 12; ++i){
            // this was an empirical find, TODO:
            // check out what it actually means and if
            // checking wd = fd2 is reliable
            if(buff[i].wd == watch_fd){
                printf("\n%s: %s", evstr(buff[i].mask), buff[i].name);
            }
        }
        fflush(stdout);
        }
}

char* evstr(uint32_t mask)
{
    switch(mask){
    case IN_ACCESS:
        return "ACCESS";
    case IN_MODIFY:
        return "MODIFY";
    case IN_ATTRIB:
        return "ATTRIB";
    case IN_CLOSE_WRITE:
        return "CLOSE_WRITE";
    case IN_CLOSE_NOWRITE: 
        return "CLOSE_NOWRITE";
    case IN_OPEN: 
        return "OPEN";
    case IN_MOVED_FROM:
        return "MOVED_FROM";
    case IN_MOVED_TO: 
        return "MOVED_TO";
    case IN_CREATE: 
        return "CREATE";
    case IN_DELETE:
        return "DELETE";
    case IN_DELETE_SELF:
        return "DELETE_SELF";
    case IN_MOVE_SELF:
        return "MOVE_SELF";
    default:
        return "WTF";
    }
}
