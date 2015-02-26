samp-ac.com
==========

This is the code that powers samp-ac.com!

Adding a new cheat
----------
* Navigate to API/badexecutablefiles.html
* Get the MD5 of the new cheat - (http://www.nullriver.com/downloads/Install-winMd5Sum.exe)
* Add a new line, and the md5 at the bottom of the current list.

Updating Client
----------
* Obtain the following files from compiling: sampac-client.exe and ACv2_Client.asi (AFTER VMPROTECT!)
* Place them in the Releases folder
* Double check to make sure you changed the version numbers in CClientUpdater.h, and if not, increment them by 0.01 and recompile
* Navigate to API/SAMP_AC_v2_Client_Updater.html
* Increment the version number by 0.01.
* Commit & Sync

Updating Server
-----------
* Obtain the following files from compiling: sampac.dll and sampac.so
* Place them in the Releases folder
* Open samp-ac-server.zip
* Replace the sampac.dll and sampac.so files in the plugin folder
* Update any example scripts in the filterscript folder, or the readme if needed.
* Double check to make sure you changed the version numbers in CServerUpdater.h, and if not, increment them by 0.01 and recompile
* Navigate to API/Server_Windows_Updater.html
* Increment the version number by 0.01.
* Navigate to API/Server_Linux_Updater.html
* Increment the version number by 0.01.
* Commit & Sync
