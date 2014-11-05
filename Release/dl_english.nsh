; dl_english.nsh
; English Language Strings
 ; Make a copy of this header and save it as dl_[NewLanguage].nsh
 ; where NewLanguage is the name of a language supported by NSIS e.g. dl_spanish.nsh
 ; Translate all strings below to the selected language

   !define ${DL_LANGUAGE}_ReDowMsg "There is not new version on server.$\r$\n\
   Do you want to re-download the current version?"

   !define ${DL_LANGUAGE}_FoundMsg "A new version of the program found on server.$\r$\n\
   Do you want to download the new version now?"

   !define ${DL_LANGUAGE}_ErrorMsg   "Download Error:"

   !define ${DL_LANGUAGE}_VerHeader  "New Version Check Out"

   !define ${DL_LANGUAGE}_NewHeader  "Found New Version"

   !define ${DL_LANGUAGE}_DlHeader   "Downloading..."

   !define ${DL_LANGUAGE}_HeaderOk   "Download Complete"

   !define ${DL_LANGUAGE}_HeaderEr   "Download Failed"

   !define ${DL_LANGUAGE}_VerSubHead "There is not new version on server."

   !define ${DL_LANGUAGE}_NewSubHead "A new version of the program found on server."

   !define ${DL_LANGUAGE}_DlSubHead  "Please wait while SA:MP Anti-Cheat Client is being downloaded."

   !define ${DL_LANGUAGE}_SubHeadOk  "$(^NameDA) was completed successfully."

   !define ${DL_LANGUAGE}_SubHeadEr  "$(^NameDA) was failed."

   !define ${DL_LANGUAGE}_DownPrint  "New version check out (internet connection)"

   !define ${DL_LANGUAGE}_InetcStr1  "Downloading: %s"

   !define ${DL_LANGUAGE}_InetcStr2  "Connecting ..."

   !define ${DL_LANGUAGE}_InetcStr3  "second"

   !define ${DL_LANGUAGE}_InetcStr4  "minute"

   !define ${DL_LANGUAGE}_InetcStr5  "hour"

   !define ${DL_LANGUAGE}_InetcStr6  "s"

   !define ${DL_LANGUAGE}_InetcStr7  "%d kB (%d%%) of %d kB @ %d.%01d kB/s "

   !define ${DL_LANGUAGE}_InetcStr8  " (%d %s%s remaining)"
