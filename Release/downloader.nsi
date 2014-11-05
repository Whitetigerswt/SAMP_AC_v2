;--------------------------------
; Script Definitions

!define APP_NAME   "SA-MP Anti-Cheat Client Installer"
!define OUT_DIR    "Output"
!define TEMP_INI   "$PLUGINSDIR\download.ini"
!define DOWN_INI   "$EXEDIR\download.ini"
!define DOWN_LNK   "http://whitetigerswt.github.io/SAMP_AC_v2/Releases/sampac-client.exe"
!define HEAD_GET   "$PLUGINSDIR\head.txt"
!define FILE_GET   "$EXEDIR\sampac-$date2file.exe"
!define C_LENGTH   "Content-Length"
!define LAST_MOD   "Last-Modified"
!define LANG_STR  `"$(InetcStr1)" "$(InetcStr2)" \
                   "$(InetcStr3)" "$(InetcStr4)" \
                   "$(InetcStr5)" "$(InetcStr6)" \
                   "$(InetcStr7)" "$(InetcStr8)"`


!system "md ${OUT_DIR}"

;--------------------------------
; General Attributes

Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${OUT_DIR}\${APP_NAME}.exe"
BrandingText "${APP_NAME}"
ShowInstDetails nevershow


;--------------------------------
; Include Headers

!include MUI.nsh
!include Downloader.nsh


;--------------------------------
; MUI Interface & Page Settings

  !define MUI_PAGE_HEADER_TEXT "$(DlHeader)"
  !define MUI_PAGE_HEADER_SUBTEXT "$(DlSubHead)"
  !define MUI_INSTFILESPAGE_FINISHHEADER_TEXT "$header"
  !define MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT "$subheader"
  !insertmacro MUI_PAGE_INSTFILES


;--------------------------------
; Languages
  ; Instead of MUI_LANGUAGE use DOWN_LAGUAGE for languages already translated
  ; see the header dl_english.nsh for details
  
  !insertmacro DOWN_LANGUAGE "English"


;--------------------------------
; Installer Sections

Section "${APP_NAME}" Sec_01

    DetailPrint "$(DownPrint)"

    !insertmacro InetcDownload "head /silent" "${DOWN_LNK}" "${HEAD_GET}"

  ${Unless} ${Errors}

    SetDetailsPrint none

   ${LineFind} "$PLUGINSDIR\head.txt" "" "" LineFindCallBack

    !insertmacro Date2FileName

    SetDetailsPrint both

   ${If} ${FileExists} "${DOWN_INI}"
    !insertmacro CompareIniValues "${C_LENGTH}"
   ${AndUnless} ${Errors}
    !insertmacro CompareIniValues "${LAST_MOD}"
   ${AndUnless} ${Errors}
    MessageBox MB_ICONQUESTION|MB_YESNO "$(ReDowMsg)" IDYES _redown
    !insertmacro HeaderDisplayText "$(VerHeader)" "$(VerSubHead)"
    Goto _end

  _redown:
    Rename "${FILE_GET}" "${FILE_GET}.bak"
    !insertmacro InetcDownload "get" "${DOWN_LNK}" "${FILE_GET}"
   ${If} ${Errors}
    !insertmacro HeaderDisplayText "$(HeaderEr)" "$(SubHeadEr)"
   ${Else}
    !insertmacro HeaderDisplayText "$(HeaderOk)" "$(SubHeadOk)"
    !insertmacro ReplaceIni
   ${EndIf}
   Exec "${FILE_GET}"

  _end:
   ${Else}
    !insertmacro HeaderDisplayText "$(NewHeader)" "$(NewSubHead)"
    !insertmacro InetcDownload "get" "${DOWN_LNK}" "${FILE_GET}"
   ${If} ${Errors}
    !insertmacro HeaderDisplayText "$(HeaderEr)" "$(SubHeadEr)"
   ${Else}
    !insertmacro HeaderDisplayText "$(HeaderOk)" "$(SubHeadOk)"
    !insertmacro ReplaceIni
   ${EndIf}
   Exec "${FILE_GET}"
   
  _exit:
   ${EndIf}

   ${If} ${FileExists} "${FILE_GET}"
    Delete "${FILE_GET}.bak"
   ${Else}
    Rename "${FILE_GET}.bak" "${FILE_GET}"
   ${EndIf}
   
  ${Else}
    !insertmacro HeaderDisplayText "$(HeaderEr)" "$(SubHeadEr)"
  ${EndUnless}
  
SectionEnd


;--------------------------------
; Installer Functions

Function LineFindCallBack

   !insertmacro MakeDownloadIni "$R9" "${C_LENGTH}:" "${C_LENGTH} ="

   !insertmacro MakeDownloadIni "$R9" "${LAST_MOD}:" "${LAST_MOD} ="

  Push $0
    
FunctionEnd


Function .onInit

    StrCpy $LANGUAGE ${LANG_ENGLISH}

FunctionEnd