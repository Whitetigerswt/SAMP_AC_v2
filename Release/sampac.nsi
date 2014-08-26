;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include 'nsdialogs.nsh'
  
!define MUI_ICON		"ac.ico"
!define MUI_UNICON		"ac.ico"

!define MUI_ABORTWARNING_TEXT "Are you sure you wish to abort installation?"

SetCompressor /SOLID lzma

;--------------------------------
;General

  ;Name and file
  !define VERSION "pub-beta3"
  !define AUTHOR "Whitetiger"
  Name "SA:MP AC-${VERSION}"
  OutFile "sampac-${VERSION}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Rockstar Games\GTA San Andreas"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\sampac" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !define MUI_PAGE_CUSTOMFUNCTION_LEAVE MyFuncDir
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

Function MyFuncDir

IfFileExists "$INSTDIR\gta_sa.exe" Exists DoesntExist

DoesntExist:
messagebox mb_ok|mb_iconstop "It doesn't look like GTA:SA is installed in this folder." idok
abort

Exists:

FunctionEnd

;--------------------------------
;Installer Sections

!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
!define PRODUCT_WEBSITE "http://whitetigerswt.github.io/SAMP_AC_v2/"

Section "Main Components" SecMain

	SectionIn 1 2 RO

  SetOutPath "$INSTDIR"
  
  File "ACv2_Client.asi"
  File "samp_elevator.exe"
  File "crashrpt_lang.ini"
  File "CrashRpt1402.dll"
  File "dbghelp.dll"
  File "CrashSender1402.exe"
  
  Delete "$INSTDIR\d3d9.dll"
  
  ;Store installation folder
  WriteRegStr HKCU "Software\sampac" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall_ac.exe"
  
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\Uninstall_ac.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\uninstall_ac.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEBSITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${AUTHOR}"

SectionEnd

Section "ASI Loader" SecASILoader

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  File "vorbisHooked.dll"
  File "vorbisFile.dll"


SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecMain ${LANG_ENGLISH} "All the required files."
  LangString DESC_SecASILoader ${LANG_ENGLISH} "ASI Loader."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecASILoader} $(DESC_SecASILoader)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
  

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\ACv2_Client.asi"
  Delete "$INSTDIR\samp_elevator.exe"
  Delete "$INSTDIR\crashrpt_lang.ini"
  Delete "$INSTDIR\CrashRpt1402.dll"
  Delete "$INSTDIR\dbghelp.dll"
  Delete "$INSTDIR\CrashSender1402.exe"

  Delete "$INSTDIR\Uninstall_ac.exe"
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"

  DeleteRegKey /ifempty HKCU "Software\sampac"

SectionEnd