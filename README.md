SAMP_AC_v2
==========
Version 2.0 of SAMP Anti-Cheat. This is a full re-code from the ground up.

Documentation
========== 
This documentation is for my personal use only, if I quit for a while on this project, and come back, I need to know how to work with this code. I don't intend on this going public until the program is out of use.

Compiling
==========

Windows
----------
Open acv2.sln with Visual Studio 2013
Compile
That should be all that's required.

Linux
----------
```
1) Use Ubuntu 14.04 LTS
2) sudo apt-get install g++ && sudo apt-get install make
3) sudo apt-get update
4) sudo apt-get upgrade
5) make sure all software updates have been completed.
6) The Shared/libs folder should contain 5 files "libsampgdk.a, libsubhook.a, libsampgdk.so, libsampgdk.so.4, libsampgdk.so.4.0.0-beta2" where libsampgdk.so.4 and libsamp.gdk.so are symbolically linked to libsampgdk.so.4.0.0-beta2. These files can be downloaded from the samp GDK respository (releases) and the .a files can be downloaded from SA-MP+ Repository
7) cd into ACv2_Server folder && run "make"
```