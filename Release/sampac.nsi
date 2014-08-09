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
  Name "SA:MP AC-dev-beta1"
  OutFile "sampac.exe"

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

Section "Main Components" SecMain

	SectionIn 1 2 RO

  SetOutPath "$INSTDIR"
  
  File "ACv2_Client.asi"
  File "samp_elevator.exe"
  
  ;Store installation folder
  WriteRegStr HKCU "Software\sampac" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall_ac.exe"

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

  Delete "$INSTDIR\Uninstall_ac.exe"

  DeleteRegKey /ifempty HKCU "Software\sampac"

SectionEnd