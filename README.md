AleJndFlag
==========

`AleJndFlag` is a "flag puller" used to get the flag from the scoreboard server. The flag returns is differ based on the entrypoints used by the attacker.

Example:

![alt text](get_flag.png)

#### Usage

Install the dependencies

```Bash
$ sudo apt-get install libjsoncpp-dev libcurl4-openssl-dev
```

Define the vulnerable service SUID and the score inside the `service_info` structures.


```C++
struct services_info service[] =
{
    {1000},
    {1001}
};
```

Then update the `flagsubmit_url` which can be found inside the `include/util.hpp` file according to your scoreboard server IP address.
