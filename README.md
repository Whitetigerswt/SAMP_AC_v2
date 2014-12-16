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
* ~~Pirate~~ Acquire VMProtect Ultimate v2.12.3.
* Move ACv2_Client.map from "acv2" to "Release".
* Open ACv2_Client.asi - Max FPS.vmp with VMProtect.
* Press the green play button.
* Rename ACv2_Client.vmp.asi to "ACv2_Client.asi"
* Download nullsoft scriptable install system v2.46. - http://nsis.sourceforge.net/Download
* Run sampac.nsi

Linux
----------
```
1) Use Ubuntu 14.04 LTS Through VMWare (Although any form of linux will work, you'll just have to accomodate the steps with yum or whatever package system)
2) sudo apt-get update
3) sudo apt-get upgrade
4) sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev cmake g++4.4 make
5) You cannot directly use git as this is a private repo, clone the repo from the GitHub app on Windows
6) Create a new folder on Virtual machine, title it whatever
7) Paste "ACv2_Server" and "Shared" into the new folder.
8) cd to "ACv2_Server"
9) "make"
10) sampac.so should be located in ACv2_Server/bin/
```
