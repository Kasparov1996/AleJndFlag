AleJndFlag
==========

`AleJndFlag` is a "flag puller" used to get the flag from the scoreboard server. The flag returns is differ based on the entrypoints used by the attacker.

Example:
![alt text](get_flag.png)

#### Usage

First you have to define the vulnerable service SUID and the score inside the `service_info` structures.


```C++
struct services_info service[] =
{
    {1000, 200},
    {1001, 300}
};
```

The update the `flagsubmit_url` which can be found inside the `include/util.hpp` file according to your scoreboard server IP address.