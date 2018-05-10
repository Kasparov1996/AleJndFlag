#ifndef SERVICES_H
#define SERVICES_H

/*
 * uid - to match with the sqlite services uid
 * score - prevent conflicts with other services, and for pwn_deduct
 */
struct services_info
{
    int uid;
    int score;
};

struct services_info service[] =
{
    {1000, 200},
    {1001, 300}
};

#endif
