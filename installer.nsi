!include "MUI2.nsh"

Name "PassVault"
OutFile "passvault_installer.exe"
InstallDir "$PROGRAMFILES64\PassVault"
InstallDirRegKey HKCU "Software\PassVault" ""
RequestExecutionLevel admin

!define MUI_ICON "resources\icon.ico"
!define MUI_UNICON "resources\icon.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\PassVault.exe"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Section "PassVault" SecCore
  SetOutPath "$INSTDIR"
  
  ; Include all files from deploy folder
  File /r "deploy\*"
  
  ; Write the installation path into the registry
  WriteRegStr HKCU "Software\PassVault" "" $INSTDIR
  
  ; Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  ; Create shortcuts
  CreateDirectory "$SMPROGRAMS\PassVault"
  CreateShortcut "$SMPROGRAMS\PassVault\PassVault.lnk" "$INSTDIR\PassVault.exe"
  CreateShortcut "$SMPROGRAMS\PassVault\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  CreateShortcut "$DESKTOP\PassVault.lnk" "$INSTDIR\PassVault.exe"

  ; Add/Remove Programs registry keys
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "NoRepair" 1
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "DisplayName" "PassVault"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "QuietUninstallString" "$\"$INSTDIR\Uninstall.exe$\" /S"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "InstallLocation" "$\"$INSTDIR$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "DisplayIcon" "$\"$INSTDIR\PassVault.exe$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault" "Publisher" "PassVault"
SectionEnd

Section "Uninstall"
  RMDir /r "$INSTDIR"
  
  Delete "$SMPROGRAMS\PassVault\PassVault.lnk"
  Delete "$SMPROGRAMS\PassVault\Uninstall.lnk"
  RMDir "$SMPROGRAMS\PassVault"
  Delete "$DESKTOP\PassVault.lnk"

  DeleteRegKey HKCU "Software\PassVault"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PassVault"
SectionEnd
