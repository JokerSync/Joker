#if !Defined(MyAppName)
#error "You must define MyAppName"
#endif

#define MyAppPublisher "Phonations"
#define MyAppURL "http://www.phonations.com/"
#define MyAppExeName MyAppName + ".exe"

#if !Defined(PWD)
#define PWD "."
#endif

#define MyAppSrc PWD + "\release\" + MyAppExeName
#pragma message MyAppSrc

#if !FileExists(MyAppSrc)
#error "Unable to find MyAppSrc"
#endif

#define FileVerStr GetFileVersion(MyAppSrc)
#define StripBuild(str VerStr) Copy(VerStr, 1, RPos(".", VerStr)-1)
#define MyAppVerStr StripBuild(FileVerStr)
#define MyAppVerName MyAppName + " v" + MyAppVerStr

#define QtDir GetEnv('QTDIR')

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVerStr}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableDirPage=yes
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputDir={#PWD}
OutputBaseFilename={#MyAppName}_v{#MyAppVerStr}
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#MyAppSrc}"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#PWD}\release\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PWD}\release\bearer\*.dll"; DestDir: "{app}\bearer"; Flags: ignoreversion
Source: "{#PWD}\release\iconengines\*.dll"; DestDir: "{app}\iconengines"; Flags: ignoreversion
Source: "{#PWD}\release\imageformats\*.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#PWD}\release\platforms\*.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion

Source: "{#PWD}\release\*.qm"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
