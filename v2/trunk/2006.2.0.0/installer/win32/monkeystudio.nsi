!include "MUI.nsh"
!define ALL_USERS
!include writeenvStr.nsh
!include addtopath.nsh
!define EXE_VERSION "2.0"
!define MUI_VERSION ${EXE_VERSION}
!define QTDIR "C:\Qt\4.2.2\bin"
Var STARTMENU_FOLDER
!define VERSION_INFO ""

Name "MonkeyStudio"
BrandingText "MonkeyStudio"
!ifdef COVERAGE
!undef VERSION_INFO
!define VERSION_INFO "(Test Version)"
OutFile "MonkeyStudioSetup_${EXE_VERSION}_test.exe"
!else
OutFile "MonkeyStudioSetup_${EXE_VERSION}.exe"
!endif
Caption "MonkeyStudio Setup - Version ${MUI_VERSION}"

!define VISUALSTUDIO "c:/Programme/Microsoft Platform SDK for Windows Server 2003 R2/Bin/win64"

SetDatablockOptimize on ; 
SetCompressor /SOLID /FINAL lzma
CRCCheck on ; 
ShowInstDetails hide ;
SetDateSave on ;

InstallDir "$PROGRAMFILES\MonkeyStudio"
InstallDirRegKey HKEY_LOCAL_MACHINE "SOFTWARE\MonkeyStudio\MonkeyStudio" ""
DirText "Select the directory to install MonkeyStudio in:"
!define MUI_ICON "..\..\monkey\src\resources\Icons\icon.ico"
!define MUI_UNICON  "..\..\monkey\src\resources\Icons\icon.ico"

; License page
!define MUI_PAGE_HEADER_TEXT "MonkeyStudio - Version ${MUI_VERSION} ${VERSION_INFO}"
!define MUI_PAGE_HEADER_SUBTEXT "IDE for Qt4"
!define MUI_LICENSEPAGE_RADIOBUTTONS
!define MUI_LICENSEPAGE_TEXT_TOP "MonkeyStudio - License Agreement"
!define MUI_LICENSEPAGE_TEXT_BOTTOM "You must agree to this license before installing."
!insertmacro MUI_PAGE_LICENSE "..\..\LICENSE.gpl"

; Directory page
!define MUI_PAGE_HEADER_TEXT "MonkeyStudio - Installation Directory"
!define MUI_PAGE_HEADER_SUBTEXT "IDE for Qt4"
!insertmacro MUI_PAGE_DIRECTORY

;Start Menu Folder Page Configuration
!define MUI_PAGE_HEADER_TEXT "MonkeyStudio - Start Menu"
!define MUI_PAGE_HEADER_SUBTEXT "IDE for Qt4"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "SOFTWARE\MonkeyStudio\MonkeyStudio"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!define MUI_STARTMENUPAGE_NODISABLE 
!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

; progress dialog
!define MUI_PAGE_HEADER_TEXT "MonkeyStudio - Installing..."
!define MUI_PAGE_HEADER_SUBTEXT "IDE for Qt4"
!define MUI_INSTFILESPAGE_FINISHHEADER_TEXT "MonkeyStudio - Installation Done"
!define MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT "IDE for Qt4"
!define MUI_INSTFILESPAGE_ABORTHEADER_TEXT "MonkeyStudio - Installation Aborted"
!define MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT "IDE for Qt4"
!insertmacro MUI_PAGE_INSTFILES

; confirm uninstall dialog
!define MUI_PAGE_HEADER_TEXT "MonkeyStudio - Version ${MUI_VERSION}"
!define MUI_PAGE_HEADER_SUBTEXT "IDE for Qt4"
!insertmacro MUI_UNPAGE_CONFIRM

; Uninstall progress
!define MUI_PAGE_HEADER_TEXT "MonkeyStudio - Uninstalling..."
!define MUI_PAGE_HEADER_SUBTEXT "IDE for Qt4"
!define MUI_UNINSTFILESPAGE_FINISHHEADER_TEXT "MonkeyStudio - Uninstallation Done"
!define MUI_UNINSTFILESPAGE_FINISHHEADER_SUBTEXT "IDE for Qt4"
!define MUI_UNINSTFILESPAGE_ABORTHEADER_TEXT "MonkeyStudio - Uninstallation Aborted"
!define MUI_UNINSTFILESPAGE_ABORTHEADER_SUBTEXT "IDE for Qt4"
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

VIProductVersion "${MUI_VERSION}.0.0"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "MonkeyStudio"
VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "IDE for Qt4"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "MonkeyStudio"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" ""
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "© MonkeyStudio"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Installation program of MonkeyStudio"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${MUI_VERSION}"

Section "" ; (default section)
SetOutPath "$INSTDIR"
RMDir /r "$INSTDIR\monkeystudio_tests"
; add files / whatever that need to be installed here.
file ..\..\bin\monkey.exe
file ..\..\bin\monkey.lib
file ..\..\bin\monkey.exp
CreateDirectory "$INSTDIR\Plugins"
CreateDirectory "$INSTDIR\Plugins\bridges"
file /oname=$INSTDIR\Plugins\bridges\Console.dll "..\..\bin\Plugins\bridges\Console.dll"
file /oname=$INSTDIR\Plugins\bridges\Debugger.dll ..\..\bin\Plugins\bridges\Debugger.dll
CreateDirectory "$INSTDIR\Plugins\childs"
file /oname=$INSTDIR\Plugins\childs\Cpp.dll ..\..\bin\Plugins\childs\Cpp.dll
file /oname=$INSTDIR\Plugins\childs\Text.dll ..\..\bin\Plugins\childs\Text.dll
file /oname=$INSTDIR\Plugins\childs\Ui.dll ..\..\bin\Plugins\childs\Ui.dll
CreateDirectory "$INSTDIR\Plugins\compilers"
file /oname=$INSTDIR\Plugins\compilers\GNUMake.dll ..\..\bin\Plugins\compilers\GNUMake.dll
CreateDirectory "$INSTDIR\Plugins\projects"
file /oname=$INSTDIR\Plugins\projects\QMake.dll ..\..\bin\Plugins\projects\QMake.dll
CreateDirectory "$INSTDIR\Plugins\workspace"
file /oname=$INSTDIR\Plugins\workspace\QtAssistant.dll ..\..\bin\Plugins\workspace\QtAssistant.dll
file /oname=$INSTDIR\Plugins\workspace\AStyle.dll ..\..\bin\Plugins\workspace\AStyle.dll
file /oname=$INSTDIR\Plugins\workspace\MessageBox.dll ..\..\bin\Plugins\workspace\MessageBox.dll


!ifdef COVERAGE
CreateDirectory "$INSTDIR\CoverageMeter"
file /oname=$INSTDIR\CoverageMeter\monkey.exe.csmes ..\..\bin\monkey.exe.csmes
file /oname=$INSTDIR\CoverageMeter\Console.dll.csmes ..\..\bin\Plugins\bridges\Console.dll.csmes
file /oname=$INSTDIR\CoverageMeter\Debugger.dll.csmes ..\..\bin\Plugins\bridges\Debugger.dll.csmes
file /oname=$INSTDIR\CoverageMeter\Cpp.dll.csmes ..\..\bin\Plugins\childs\Cpp.dll.csmes
file /oname=$INSTDIR\CoverageMeter\Text.dll.csmes ..\..\bin\Plugins\childs\Text.dll.csmes
file /oname=$INSTDIR\CoverageMeter\Ui.dll.csmes ..\..\bin\Plugins\childs\Ui.dll.csmes
file /oname=$INSTDIR\CoverageMeter\GNUMake.dll.csmes ..\..\bin\Plugins\compilers\GNUMake.dll.csmes
file /oname=$INSTDIR\CoverageMeter\QMake.dll.csmes ..\..\bin\Plugins\projects\QMake.dll.csmes
file /oname=$INSTDIR\CoverageMeter\QtAssistant.dll.csmes ..\..\bin\Plugins\workspace\QtAssistant.dll.csmes
file /oname=$INSTDIR\CoverageMeter\AStyle.dll.csmes ..\..\bin\Plugins\workspace\AStyle.dll.csmes
file /oname=$INSTDIR\CoverageMeter\MessageBox.dll.csmes ..\..\bin\Plugins\workspace\MessageBox.dll.csmes
!endif
File "Microsoft.VC80.CRT"
File "${VISUALSTUDIO}\MSVCR80.dll"
File "${VISUALSTUDIO}\MSVCP80.dll"
File "${QTDIR}\QtGui4.dll"
File "${QTDIR}\QtCore4.dll"

; setting environment variable
DetailPrint "Setting environment variable %MONKEYSTUDIO% to $INSTDIR..."
Push MONKEYSTUDIO
Push $INSTDIR
Call WriteEnvStr

; File Association
DetailPrint "File association *.pro->MonkeyStudio ..."
 ; back up old value of .pro
!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".pro" ""
  StrCmp $1 "" "${Index}-NoBackup"
    StrCmp $1 "MonkeyStudio.CSMes" "${Index}-NoBackup"
    WriteRegStr HKCR ".pro" "backup_val" $1
"${Index}-NoBackup:"
  WriteRegStr HKCR ".pro" "" "MonkeyStudio.CSMes"
  ReadRegStr $0 HKCR "MonkeyStudio.CSMes" ""
  StrCmp $0 "" 0 "${Index}-Skip"
        WriteRegStr HKCR "MonkeyStudio.CSMes" "" "MonkeyStudio Instrumentation"
        WriteRegStr HKCR "MonkeyStudio.CSMes\shell" "" "open"
        WriteRegStr HKCR "MonkeyStudio.CSMes\DefaultIcon" "" "$INSTDIR\execute.exe,0"
"${Index}-Skip:"
  WriteRegStr HKCR "MonkeyStudio.CSMes\shell\open\command" "" \
    '$INSTDIR\ononkeyStudio.exe "%1"'
  ;WriteRegStr HKCR "MonkeyStudio.CSMes\shell\edit" "" "Edit Options File"
  ;WriteRegStr HKCR "MonkeyStudio.CSMes\shell\edit\command" "" \
  ;  '$INSTDIR\execute.exe "%1"'
 
  System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
!undef Index
  ; Rest of script

; start menu
SetShellVarContext all
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
DetailPrint "Creating shortcuts ($SMPROGRAMS\$STARTMENU_FOLDER)..."
CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\MonkeyStudio.lnk" "$INSTDIR\MonkeyStudio.exe"
!ifdef COVERAGE
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\CoverageMeter Instrumentation Files.lnk" "$INSTDIR\CoverageMeter"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\CoverageMeter Execution Files.lnk" "$INSTDIR\monkeystudio_tests"
!endif

!insertmacro MUI_STARTMENU_WRITE_END


WriteRegStr HKEY_LOCAL_MACHINE "SOFTWARE\MonkeyStudio\MonkeyStudio" "" "$INSTDIR"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio" "URLInfoAbout" "http://www.coveragemeter.com"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio" "URLUpdateInfo" "http://www.coveragemeter.com/download.html"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio" "DisplayVersion" "${MUI_VERSION}"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio" "DisplayName" "MonkeyStudio"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio" "UninstallString" '"$INSTDIR\uninst.exe"'
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio" "DisplayIcon" '"$INSTDIR\coveragemeter_logo.ico"'
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio" "Publisher" "MonkeyStudio"

; write out uninstaller
WriteUninstaller "$INSTDIR\uninst.exe"
SectionEnd ; end of default section


Section Uninstall
; add delete commands to delete whatever files/registry keys/etc you installed here.
SetShellVarContext all
Delete "$INSTDIR\uninst.exe"

DetailPrint "Removing $INSTDIR..."
RMDir /r "$INSTDIR"

!insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_FOLDER
DetailPrint "Removing shortcuts ($SMPROGRAMS\$STARTMENU_FOLDER)..."
RMDir /r "$SMPROGRAMS\$STARTMENU_FOLDER"

;start of restore script
; File Association
DetailPrint "File association *.pro->CoverageBrowser ..."
!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".pro" ""
  StrCmp $1 "MonkeyStudio.CSMes" 0 "${Index}-NoOwn" ; only do this if we own it
    ReadRegStr $1 HKCR ".pro" "backup_val"
    StrCmp $1 "" 0 "${Index}-Restore" ; if backup="" then delete the whole key
      DeleteRegKey HKCR ".pro"
    Goto "${Index}-NoOwn"
"${Index}-Restore:"
      WriteRegStr HKCR ".pro" "" $1
      DeleteRegValue HKCR ".pro" "backup_val"
   
    DeleteRegKey HKCR "MonkeyStudio.CSMes" ;Delete key with association settings
 
    System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
"${Index}-NoOwn:"
!undef Index
  ;rest of script

; environment variables
DetailPrint "Removing environment variable %MONKEYSTUDIO%..."
Push MONKEYSTUDIO
Call un.DeleteEnvStr

DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\MonkeyStudio\MonkeyStudio"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\MonkeyStudio"

SectionEnd ; end of uninstall section

; eof

