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
```
Open acv2.sln with Visual Studio 2013 (Though you will need Visual studio 2010 to compile)
Compile
```
That should be all that's required. SAMP GDK May need to be recompiled. CMake on windows can generate the visual studio solutions. be sure to generate the solutions in Visual studio 2010



Linux
----------
```
1) Use Ubuntu 14.04 LTS
2) sudo apt-get install g++ && sudo apt-get install make
3) sudo apt-get update
4) sudo apt-get upgrade
5) make sure all software updates have been completed.
6) Compile SAMP GDK as statically linked, the instructions for doing that are in the SampGDK repository.
7) cd into ACv2_Server folder && run "make"
```