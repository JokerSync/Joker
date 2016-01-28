#define MyAppName "Joker"

#include "..\..\common\common.iss"

#define PortaudioPath GetEnv('PORTAUDIO_PATH')

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E892D5E9-1FA6-4BFC-A749-3C942E0A5C56}
LicenseFile="..\..\LICENSE.TXT"

[Files]
; images
Source: "{#PWD}\release\*.png"; DestDir: "{app}"; Flags: ignoreversion
; fonts
Source: "{#PWD}\release\*.ttf"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
