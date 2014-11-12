RSHELL
======


Licensing Information: READ LICENSE
---
Project can be downloaded from https://github.com/jmaw001/rshell.git
----

Author and Contributors List
---------------
Jesse Maw

All other known bugs and fixes can be sent to jmaw001@ucr.edu

Reported bugs/fixes will be submitted to correction

File List
---------
```
.:

Makefile

LICENSE

README.md

./src

./tests
```
```
./src:

rshell.cpp
```
```
./tests:

exec.script
```


Problems in Rshell
-------
```

*Logic connectors do not work as of yet. Single commands on different lines will.

```

Problems in ls
--------
```
*-R flag can be taken in, but the code itself does not work. So in testing, it will do nothing as of
now.

*-l works, but breaks when it is combined with other things such as the -a flag or directories.(on the bright side, -a and file directories cooperate!)

*-l also randomly breaks with an error related to stat because the directory pathing for some files is slightly off.
```
