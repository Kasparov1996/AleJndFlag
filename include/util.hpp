#ifndef UTIL_HPP
#define UTIL_HPP

namespace util
{
    extern "C"
    {
        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <ifaddrs.h>

        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        char *getLocalIP();
    };

    char *getLocalIP()
    {
        struct ifaddrs *ifaces,  *res;
        int ret;

        if ((ret = getifaddrs(&ifaces)) != 0)
        {
            fprintf(stderr, "Failed to get interface.\n");
            exit(EXIT_FAILURE);
        }

        for (res = ifaces; res != NULL; res = res->ifa_next)
        {
            if (res->ifa_name == NULL)
                continue;

            if (strncmp(res->ifa_name, INTERFACE, strlen(INTERFACE)) !=0)
                continue;

            if (res->ifa_addr->sa_family == AF_INET)
            {
                freeifaddrs(ifaces);
                return inet_ntoa(((struct sockaddr_in *)res->ifa_addr)->sin_addr);
            }
        }
    }
}

#endif
