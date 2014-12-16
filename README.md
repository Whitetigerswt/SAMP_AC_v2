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
1) Download Visual Studio 2013 Community Edition - http://www.visualstudio.com/products/visual-studio-community-vs (or any edition of VS2013 works)
2) Get all the updates for VS2013, through update 4 - http://support.microsoft.com/kb/2994375 (One of the updates should include the v120_xp platform toolset, which allows us to compile for windows XP on vs2013)
3) Open acv2.sln with Visual Studio 2013
4) Compile
```

Client Specific
----------
~~Pirate~~ Acquire VMProtect Ultimate v2.12.3.
Move ACv2_Client.map from "acv2" to "Release".
Open ACv2_Client.asi - Max FPS.vmp with VMProtect.
Press the green play button.
Rename ACv2_Client.vmp.asi to "ACv2_Client.asi"
Download nullsoft scriptable install system v2.46. - http://nsis.sourceforge.net/Download
Run sampac.nsi

Linux
----------
```
1) Use Ubuntu 14.04 LTS
2) sudo apt-get install g++4.4 && sudo apt-get install make
3) sudo apt-get update
4) sudo apt-get upgrade
5) sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev cmake
6) Install boost v1.56 (NOT 1.57 or newer!):
    - wget -O boost_1_56_0.tar.gz http://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.tar.gz/download
	- tar xzvf boost_1_56_0.tar.gz
	- cd boost_1_56_0/
	- ./bootstrap.sh --prefix=/usr/local
7) make sure all software updates have been completed.
8) cd into ACv2_Server folder && run "make"
```