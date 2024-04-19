
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

void handle_event(struct inotify_event *event) {
    if (event->mask & IN_CREATE) {
        printf("Fichier créé : %s\n", event->name);
    } else if (event->mask & IN_DELETE) {
        printf("Fichier supprimé : %s\n", event->name);
    } else if (event->mask & IN_MODIFY) {
        printf("Fichier modifié : %s\n", event->name);
    }
}

int main() {
    int fd, wd;
    char buffer[BUF_LEN];

    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    wd = inotify_add_watch(fd, "./data", IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd < 0) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Surveillance des modifications...\n");

    while (1) {
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        struct inotify_event *event;
        for (char *ptr = buffer; ptr < buffer + length; ptr += EVENT_SIZE + event->len) {
            event = (struct inotify_event *)ptr;

                handle_event(event);
            
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}