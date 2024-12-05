#define MyAppName "Ecofloc"
#define MyAppVersion "alpha"
#define MyAppPublisher "Ecofloc4win team"
[Setup]
;Info de base
AppName=Ecofloc
AppVersion=1.0
DefaultDirName={autopf}\ecofloc
;Apparence
WizardStyle=modern
UninstallDisplayIcon=images\ecoflocV1.ico
SetupIconFile=images\ecoflocV1.ico
DisableWelcomePage=no
WizardImageFile=images\ecoflocV1.bmp


;
LicenseFile=LICENSE
InfoBeforeFile=readme.md

Compression=lzma
SolidCompression=yes

Output=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "ecofloc.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "config.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "ecofloc-UI\*"; DestDir: "{app}\ecofloc-UI\"; Flags: ignoreversion recursesubdirs createallsubdirs

[Components]
Name: "main"; Description: "Ecofloc"; Types: full compact custom; Flags: fixed
Name: "ui"; Description: "Web UI"; Types: full

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}";Components: main
Root: HKA; Subkey: "Software\Classes\ecofloc"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc"; ValueType: string; ValueName: ""; ValueData: "URL:Ecofloc Protocol"; Flags: uninsdeletevalue; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc"; ValueType: string; ValueName: "URL Protocol"; ValueData: ""; Flags: uninsdeletevalue; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell\open"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell\open\command"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "\""{app}"""; Flags: uninsdeletevalue; Components: ui
Root: HKLM; Subkey: "SOFTWARE\Ecofloc"; Flags: uninsdeletekey; Components: ui
Root: HKLM; Subkey: "SOFTWARE\Ecofloc"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletevalue; Components: ui