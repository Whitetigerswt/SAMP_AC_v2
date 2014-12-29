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
1) Use 32-bit Ubuntu 14.04.1 LTS Through VMWare (http://releases.ubuntu.com/14.04.1/ubuntu-14.04.1-desktop-i386.iso) (Although any form of linux will work, you'll just have to accomodate the steps with yum or whatever package system)
2) sudo apt-get update
3) sudo apt-get upgrade
4) sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev cmake g++4.4 make git geomview
5) Follow steps here to generate an ssh key and add to github: https://help.github.com/articles/generating-ssh-keys/
6) navigate to Shared and rename "Boost" to "boost" - case matters.
7) git clone git@github.com:Whitetigerswt/SAMP_AC_v2.git
8) cd to "ACv2_Server"
9) "make"
10) sampac.so should be located in ACv2_Server/bin/
```
