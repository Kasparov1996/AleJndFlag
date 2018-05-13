AleJndFlag
==========

`AleJndFlag` is a "flag puller" used to get the flag from the scoreboard server. The flag returns is differ based on the entrypoints used by the attacker.

Example:

![alt text](get_flag.png)

#### Usage

Install the dependencies

*NOTE* : Make sure to set the `INTERFACE` in file `get_flag.cpp` to the configured interface name.

```Bash
$ sudo apt-get install libssl-dev
$ make
$ ./get_flag
```
