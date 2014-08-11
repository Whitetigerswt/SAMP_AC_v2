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
Open acv2.sln with Visual Studio 2013 (Use 2013-XP to compile so it's compat with XP.)
Compile
```
That should be all that's required. SAMP GDK May need to be recompiled. CMake on windows can generate the visual studio solutions. be sure to generate the solutions in Visual studio 2013
CMake to generate cpp-netlib as well


Linux
----------
```
1) Use Ubuntu 14.04 LTS
2) sudo apt-get install g++4.4 && sudo apt-get install make
3) sudo apt-get update
4) sudo apt-get upgrade
5) sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev cmake
6) Install boost:
    - wget -O boost_1_56_0.tar.gz http://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.tar.gz/download
	- tar xzvf boost_1_56_0.tar.gz
	- cd boost_1_56_0/
	- ./bootstrap.sh --prefix=/usr/local
7) make sure all software updates have been completed.
8) Compile SAMP GDK as statically linked, the instructions for doing that are in the SampGDK repository.
9) cd into ACv2_Server folder && run "make"

cpp-netlib may need to be recompiled as well.
```