
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <net/if.h>

#define INTERVAL 3 // Intervalle en secondes pour mesurer le débit

struct net_stat {
    char iface[IFNAMSIZ];
    unsigned long rx_bytes;
    unsigned long tx_bytes;
};

void get_net_stat(struct net_stat *stats, int *count) {
    FILE *fp;
    char line[256];
    int i = 0;

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/net/dev");
        exit(EXIT_FAILURE);
    }

    // Skip the first two lines
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, " %[^:]: %lu %*d %*d %*d %*d %*d %*d %*d %lu", stats[i].iface, &stats[i].rx_bytes, &stats[i].tx_bytes);
        i++;
    }

    *count = i;
    fclose(fp);
}

unsigned long get_tx_rate(const char *interface) {
    struct net_stat prev_stats[10], curr_stats[10];
    int count, i;
    unsigned long tx_rate = 0;

    // Initial stats
    get_net_stat(prev_stats, &count);
    sleep(INTERVAL);

    // Stats after interval
    get_net_stat(curr_stats, &count);

    // Calculate TX rate for the specified interface
    for (i = 0; i < count; i++) {
        if (strcmp(prev_stats[i].iface, interface) == 0) {
            tx_rate = (curr_stats[i].tx_bytes - prev_stats[i].tx_bytes) / INTERVAL;
            break;
        }
    }

    return tx_rate;
}

int main() {
    const char *interface = "wlp1s0";
    unsigned long tx_rate = get_tx_rate(interface);
    printf("TX Rate for interface %s: %lu bytes/sec\n", interface, tx_rate);
    return 0;
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>

#define INTERVAL 3 // Intervalle en secondes pour mesurer le débit

struct net_stat {
    char iface[IFNAMSIZ];
    unsigned long rx_bytes;
    unsigned long tx_bytes;
};

void get_net_stat(struct net_stat *stats, int *count) {
    FILE *fp;
    char line[256];
    int i = 0;

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/net/dev");
        exit(EXIT_FAILURE);
    }

    // Skip the first two lines
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, " %[^:]: %lu %*d %*d %*d %*d %*d %*d %*d %lu", stats[i].iface, &stats[i].rx_bytes, &stats[i].tx_bytes);
        i++;
    }

    *count = i;
    fclose(fp);
}

void calculate_net_speed(struct net_stat *prev, struct net_stat *curr, int count) {
    for (int i = 0; i < count; i++) {
        unsigned long rx_rate = (curr[i].rx_bytes - prev[i].rx_bytes) / INTERVAL;
        unsigned long tx_rate = (curr[i].tx_bytes - prev[i].tx_bytes) / INTERVAL;
        printf("%s: RX Rate: %lu bytes/sec, TX Rate: %lu bytes/sec\n", curr[i].iface, rx_rate, tx_rate);
    }
}

int main() {
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    struct net_stat prev_stats[10], curr_stats[10];
    int count;

    // Initial stats
    get_net_stat(prev_stats, &count);
    sleep(INTERVAL);

    // Stats after interval
    get_net_stat(curr_stats, &count);

    // Calculate and display the network speed
    calculate_net_speed(prev_stats, curr_stats, count);

    // Get interface addresses
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 1;
    }

    // Iterate through the interfaces and display information
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        // Display only IPv4 interfaces
        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                           host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                return 1;
            }

            printf("Interface: %s\n", ifa->ifa_name);
            printf("  IP Address: %s\n", host);
        }
    }

    freeifaddrs(ifaddr);
    return 0;
}


*/

