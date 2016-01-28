#define MyAppName "GraphicTest"

#include "..\..\common\common.iss"

[Setup]
AppId={{E25B6CDD-17F2-4437-9C05-E368E92FDAF1}

[Files]
Source: "{#PWD}\release\*.png"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PWD}\release\*.ttf"; DestDir: "{app}"; Flags: ignoreversion
