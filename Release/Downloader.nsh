; NSIS Nightly Builds Downloader header


;--------------------------------
; Variables

var date2file
var header
var subheader


;--------------------------------
; Include Headers

!include TextFunc.nsh
!insertmacro LineFind

!include StrFunc.nsh
${StrStr}
${StrRep}
${StrLoc}
${StrTrimNewLines}

# LogicLib is already included in StrFunc header, no need to include it again.


;--------------------------------
; Script Macros

!macro HeaderDisplayText _Txt2Header _Txt2SubHeader
    StrCpy $header "${_Txt2Header}"
    StrCpy $subheader "${_Txt2SubHeader}"
!macroend


!macro InetcDownload _PluginFunc _DownLink _LocalFile
  Push $0

    ClearErrors
    
    inetc::${_PluginFunc} /NOPROXY /TRANSLATE ${LANG_STR} "${_DownLink}" "${_LocalFile}" /END
    Pop $0 # return value = exit code, "OK" if OK

   ${Unless} $0 == "OK"
    SetErrors
    MessageBox MB_ICONEXCLAMATION|MB_OK "$(ErrorMsg) $0"
   ${EndUnless}

  Pop $0
!macroend


!macro MakeDownloadIni _InputStr _Str2Find _Str2Replace
  Push $0
  Push $1
  Push $2
  Push $3

    ${StrTrimNewLines} $3 "${_InputStr}"
    ${StrStr} $0 "$3" "${_Str2Find}"
    ${StrRep} $0 "$0" "${_Str2Find}" "${_Str2Replace}"
    ${StrLoc} $1 "$0" "=" ">"
     StrCpy $1 "$0" "$1"
    ${StrLoc} $2 "$0" "=" "<"
     Intop $2 $2 - 1
     StrCpy $2 "$0" "" "-$2"
    ${If} $2 != ""
     WriteINIStr "${TEMP_INI}" "SETTINGS" "$1 " "$2"
    ${EndIf}

  Pop $3
  Pop $2
  Pop $1
  Pop $0
!macroend


!macro ReplaceIni
    Delete "${DOWN_INI}"
    Rename "${TEMP_INI}" "${DOWN_INI}"
!macroend


!macro CompareIniValues _Value
  Push $0
  Push $1

    ClearErrors
    
    ReadINIStr $0 "${DOWN_INI}" "SETTINGS" "${_Value}"
    ReadINIStr $1 "${TEMP_INI}" "SETTINGS" "${_Value}"

   ${Unless} "$0" == "$1"
    SetErrors
   ${EndUnless}

  Pop $1
  Pop $0
!macroend


!macro _Date2FileName_ _STRING _RETURN
  Push $0
  Push $1
  Push $2
  Push $3

   StrCpy ${_RETURN} "${_STRING}"
    ${StrLoc} $0 "${_RETURN}" " " ">"
    Intop $0 $0 + 1

   StrCpy ${_RETURN} "${_RETURN}" "" "$0"
    ${StrLoc} $0 "${_RETURN}" " " ">"
    StrCpy $1 "${_RETURN}" "$0" ""
    Intop $0 $0 + 1

   StrCpy ${_RETURN} "${_RETURN}" "" "$0"
    ${StrLoc} $0 "${_RETURN}" " " ">"
    StrCpy $2 "${_RETURN}" "$0" ""
    Intop $0 $0 + 1

   StrCpy ${_RETURN} "${_RETURN}" "" "$0"
    ${StrLoc} $0 "${_RETURN}" " " ">"
    StrCpy $3 "${_RETURN}" "$0" ""
    StrCpy ${_RETURN} "$1-$2-$3"

  Pop $3
  Pop $2
  Pop $1
  Pop $0
!macroend


!macro Date2FileName
  Push $0

   ReadINIStr $0 "${TEMP_INI}" "SETTINGS" "${LAST_MOD}"
   !insertmacro _Date2FileName_ "$0" "$date2file"

  Pop $0
!macroend


!define BuildString "!insertmacro _BUILD_STRINGS"

!macro _BUILD_STRINGS _MSG_ _ID_ _STR_
   LangString ${_MSG_}  "$${LANG_${_ID_}" "${_STR_}" 
!macroend


!macro BUILD_LANGSTRINGS

   ${BuildString} "ReDowMsg" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_ReDowMsg}"

   ${BuildString} "FoundMsg" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_FoundMsg}"

   ${BuildString} "ErrorMsg" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_ErrorMsg}"

   ${BuildString} "VerHeader" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_VerHeader}"

   ${BuildString} "NewHeader" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_NewHeader}"

   ${BuildString} "DlHeader" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_DlHeader}"

   ${BuildString} "HeaderOk" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_HeaderOk}"

   ${BuildString} "HeaderEr" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_HeaderEr}"

   ${BuildString} "VerSubHead" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_VerSubHead}"

   ${BuildString} "NewSubHead" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_NewSubHead}"

   ${BuildString} "DlSubHead" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_DlSubHead}"

   ${BuildString} "SubHeadOk" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_SubHeadOk}"

   ${BuildString} "SubHeadEr" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_SubHeadEr}"

   ${BuildString} "DownPrint" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_DownPrint}"

   ${BuildString} "InetcStr1" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr1}"

   ${BuildString} "InetcStr2" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr2}"

   ${BuildString} "InetcStr3" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr3}"

   ${BuildString} "InetcStr4" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr4}"

   ${BuildString} "InetcStr5" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr5}"

   ${BuildString} "InetcStr6" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr6}"

   ${BuildString} "InetcStr7" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr7}"

   ${BuildString} "InetcStr8" "${DL_LANGUAGE}" "${${DL_LANGUAGE}_InetcStr8}"

!macroend


!macro DOWN_LANGUAGE DL_LANGUAGE

   !insertmacro MUI_LANGUAGE "${DL_LANGUAGE}"
   !include "DL_${DL_LANGUAGE}.nsh"
   !insertmacro BUILD_LANGSTRINGS

!macroend
