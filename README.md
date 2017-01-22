SAMP_AC_v2
==========
[![Linux Build Status](https://travis-ci.com/Whitetigerswt/SAMP_AC_v2.svg?token=ENM294yxGXvxxZbK9fRC&branch=master)](https://travis-ci.com/Whitetigerswt/SAMP_AC_v2)
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
2) Download Visual Studio 2013 SDK - https://www.microsoft.com/en-us/download/details.aspx?id=40758
3) Open acv2.sln with Visual Studio 2015
4) Switch to release mode
5) Compile
```

Client Specific
----------
* Ask Whitetiger to run the Enigma protector on the compiled file - you can use the demo for testing.
* Download nullsoft scriptable install system - http://nsis.sourceforge.net/Download
* Run sampac.nsi

Linux
----------
```
1) Use _32_-bit Ubuntu 14.04.3 LTS Through VMWare (Or whatever distro you want, Ubuntu is the easiest)
2) sudo apt-get update
3) sudo apt-get upgrade 
4) sudo apt-get install build-essential git g++-4.4
5) Follow steps here to generate an ssh key and add to github: https://help.github.com/articles/generating-ssh-keys/
6) git clone git@github.com:Whitetigerswt/SAMP_AC_v2.git
7) cd to "ACv2_Server"
8) "make"
9) sampac.so should be located in ACv2_Server/bin/
```
