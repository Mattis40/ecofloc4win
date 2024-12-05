#define MyAppName "Ecofloc"
#define MyAppVersion "alpha"
#define MyAppPublisher "Ecofloc4win team"
#define Installer "Ecofloc4win team"
[Setup]
;Info de base
AppName={#MyAppName}
AppVersion={#MyAppVersion}
DefaultDirName={autopf}\ecofloc

WizardStyle=modern
UninstallDisplayIcon=images\ecoflocV1.ico
SetupIconFile=images\ecoflocV1.ico
DisableWelcomePage=no
WizardImageFile=images\ecoflocV1.bmp

OutputBaseFilename=ecofloc-installer

LicenseFile=LICENSE
InfoBeforeFile=readme.md

Compression=lzma
SolidCompression=yes

Output=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "ecofloc.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "config.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "ecofloc-UI\*"; DestDir: "{app}\ecofloc-UI\"; Flags: ignoreversion recursesubdirs createallsubdirs ; Components: ui

[Components]
Name: "main"; Description: "Ecofloc"; Types: full compact custom; Flags: fixed
Name: "ui"; Description: "Web UI"; Types: full

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"; Flags: createvalueifdoesntexist ;Components: main
Root: HKA; Subkey: "Software\Classes\ecofloc"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc"; ValueType: string; ValueName: ""; ValueData: "URL:Ecofloc Protocol"; Flags: uninsdeletevalue; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc"; ValueType: string; ValueName: "URL Protocol"; ValueData: ""; Flags: uninsdeletevalue; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell\open"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell\open\command"; Flags: uninsdeletekey; Components: ui
Root: HKA; Subkey: "Software\Classes\ecofloc\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "\""{app}"""; Flags: uninsdeletevalue; Components: ui
Root: HKLM; Subkey: "SOFTWARE\Ecofloc"; Flags: uninsdeletekey; Components: ui
Root: HKLM; Subkey: "SOFTWARE\Ecofloc"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletevalue; Components: ui

[Code]
const
  EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';

procedure RemovePath(Path: string);
var
  Paths: string;
  P: Integer;
begin
  if not RegQueryStringValue(HKLM, EnvironmentKey, 'Path', Paths) then
  begin
    Log('PATH not found');
  end
    else
  begin
    Log(Format('PATH is [%s]', [Paths]));

    P := Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';');
    if P = 0 then
    begin
      Log(Format('Path [%s] not found in PATH', [Path]));
    end
      else
    begin
      if P > 1 then P := P - 1;
      Delete(Paths, P, Length(Path) + 1);
      Log(Format('Path [%s] removed from PATH => [%s]', [Path, Paths]));

      if RegWriteStringValue(HKLM, EnvironmentKey, 'Path', Paths) then
      begin
        Log('PATH written');
      end
        else
      begin
        Log('Error writing PATH');
      end;
    end;
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then
  begin
    RemovePath(ExpandConstant('{app}'));
  end;
end;