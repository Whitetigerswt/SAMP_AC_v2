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
1) Download Visual Studio 2015 Community Edition - https://www.visualstudio.com/en-us/visual-studio-homepage-vs.aspx
3) Open acv2.sln with Visual Studio 2015 (Make sure you installed the VC++ 2015 tools with XP support)
4) Switch to release mode
4) Compile
```

Client Specific
----------
* Ask Whitetiger to run the Enigma protector on the compiled file (Or ask for the key for the enigma protector v5.00 - http://www.enigmaprotector.com/ - Yes I did purchase this. only v4.4 is avilable to the public, but I use v5.0 as it's better with anti-viruses.)
* Download nullsoft scriptable install system v2.46. - http://nsis.sourceforge.net/Download
* Run sampac.nsi

Linux
----------
```
1) Use _32_-bit Ubuntu 14.04.3 LTS Through VMWare (Or whatever distro you want, Ubuntu is the easiest)
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
