; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!


[Setup]
#define VERSION "2.49b"

; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{C45CB76D-AD5F-49CC-86DE-72B168A6A888}
AppName=Blender
AppVerName=Blender {#VERSION}
AppPublisher=Blender Foundation
AppPublisherURL=http://www.blender.org
AppSupportURL=http://www.blender.org
AppUpdatesURL=http://www.blender.org
DefaultDirName={pf}\Blender
DefaultGroupName=Blender Foundation
AllowNoIcons=true
LicenseFile=.\copyright.txt
OutputBaseFilename=blender-{#VERSION}
Compression=lzma
SolidCompression=true
ChangesAssociations=true
WizardImageFile=.\installer.bmp
WizardSmallImageFile=.\header.bmp
SetupIconFile=.\installer.ico
MinVersion=,5.01.2600sp1
PrivilegesRequired=none
AllowRootDirectory=true
ShowLanguageDialog=auto

[Dirs]
Name: {userdocs}\Blender; Flags: uninsneveruninstall; Tasks: ; Languages: 

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\..\..\..\build\bin\release\blender.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\blender.html; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\BlenderQuickStart.pdf; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\copyright.txt; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\GPL-license.txt; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\Python-license.txt; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\release_249.txt; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\*.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\..\..\..\build\bin\release\plugins\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\..\..\..\build\bin\release\.blender\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: {group}\Blender; Filename: {app}\blender.exe
Name: {group}\ReleaseNotes; Filename: {app}\release_249.txt
Name: {group}\Blender.org; Filename: {app}\blender.html
Name: {group}\Copyright; Filename: {app}\copyright.txt
Name: {group}\GPL; Filename: {app}\GPL-license.txt
Name: {group}\Uninstall; Filename: {uninstallexe}; Tasks: ; Languages: 
Name: {commondesktop}\Blender; Filename: {app}\blender.exe; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\Blender; Filename: {app}\blender.exe; Tasks: quicklaunchicon

[Registry]
Root: HKCR; Subkey: .blend; ValueType: string; ValueName: ; ValueData: BlenderFile; Flags: uninsdeletevalue
Root: HKCR; Subkey: BlenderFile; ValueType: string; ValueName: ; ValueData: Blender File; Flags: uninsdeletekey
Root: HKCR; Subkey: BlenderFile\DefaultIcon; ValueType: string; ValueName: ; ValueData: {app}\blender.exe,0
Root: HKCR; Subkey: BlenderFile\shell\open\command; ValueType: string; ValueName: ; ValueData: """{app}\blender.exe"" ""%1"""

[Run]
Filename: {app}\blender.exe; Description: {cm:LaunchProgram,Blender}; Flags: nowait postinstall skipifsilent


[_ISToolDownload]

[UninstallDelete]
Name: {app}\blender.exe; Type: files
Name: {app}\*.dll; Type: files
Name: {app}\blender.html; Type: files; Tasks: ; Languages: 
Name: {app}\BlenderQuickStart.pdf; Type: files
Name: {app}\copyright.txt; Type: files
Name: {app}\GPL-license.txt; Type: files
Name: {app}\Python-license.txt; Type: files
Name: {app}\release_249.txt; Type: files
Name: {app}\.blender\*; Type: filesandordirs
Name: {app}\plugins\*; Type: filesandordirs
Name: {app}\.blender; Type: dirifempty
Name: {app}\plugins; Type: dirifempty
Name: {app}; Type: dirifempty