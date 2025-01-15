#define MyAppName "Ecofloc"
#define MyAppVersion "0.85"
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

;OutputBaseFilename=ecofloc-installer

LicenseFile=LICENSE
InfoBeforeFile=README.md

Compression=lzma
SolidCompression=yes

Output=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "ecofloc4win.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "EcoflocConfigurator.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "vite-server.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: ui
Source: "ecofloc-UI\*"; DestDir: "{app}\ecofloc-UI\"; Flags: ignoreversion recursesubdirs createallsubdirs ; Components: ui

; Additional individual files
Source: "EcoflocConfigurator.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Ijwhost.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "LibreHardwareMonitorLib.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "System.Collections.Immutable.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "System.Reflection.Metadata.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "System.Text.Encoding.CodePages.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "System.Text.Encodings.Web.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "System.Threading.Channels.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Wrapper.deps.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "Wrapper.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Wrapper.exp"; DestDir: "{app}"; Flags: ignoreversion
Source: "Wrapper.lib"; DestDir: "{app}"; Flags: ignoreversion
Source: "Wrapper.runtimeconfig.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "ecofloc4win.sys"; DestDir: "{app}"; Flags: ignoreversion


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
//suppression de ecofloc dans la path
const
  EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';
  RequiredDotNetVersion = '8.0.10';
  DotNetInstallerURL64 = 'https://download.visualstudio.microsoft.com/download/pr/f398d462-9d4e-4b9c-abd3-86c54262869a/4a8e3a10ca0a9903a989578140ef0499/windowsdesktop-runtime-8.0.10-win-x64.exe'; // Replace with the actual URL
  DotNetInstallerURL86 = 'https://download.visualstudio.microsoft.com/download/pr/9836a475-66af-47eb-a726-8046c47ce6d5/ccb7d60db407a6d022a856852ef9e763/windowsdesktop-runtime-8.0.10-win-x86.exe';
  DotNetInstallerURLArm64 = 'https://download.visualstudio.microsoft.com/download/pr/c1387fab-1960-4cdc-8653-1e0333f6385a/3bd819d5f2aecff94803006a9e2c945a/windowsdesktop-runtime-8.0.10-win-arm64.exe';

// Function to check if a specific version of the .NET runtime is installed
function IsDotNetRuntimeInstalled(Version: string): Boolean;
var
  Key: string;
  InstallPath: string;
begin
  Result := False;
  Key := 'SOFTWARE\dotnet\Setup\InstalledVersions\x86\sharedfx\Microsoft.WindowsDesktop.App\' + Version;
  if RegQueryStringValue(HKLM, Key, '', InstallPath) then
    Result := True;
end;

function ProcArchi(): String;
begin
  case ProcessorArchitecture of
    paX86: Result := 'x86';
    paX64: Result := 'x64';
    paArm64: Result := 'arm64';
  else
    Result := 'Unrecognized';
  end;
end;

// Function to download a file using PowerShell
function DownloadFile(URL, DestPath: string): Boolean;
var
  PSCommand: string;
  ExecResult: Integer;
begin
  Result := False;
  PSCommand := Format(
    'powershell -Command "& { $client = New-Object System.Net.WebClient; ' +
    '$client.DownloadFile(''%s'', ''%s'') }"', [URL, DestPath]);
    
  if Exec(ExpandConstant('{cmd}'), '/C ' + PSCommand, '', SW_HIDE, ewWaitUntilTerminated, ExecResult) then
    Result := ExecResult = 0;
end;

// Download and install .NET runtime if not found
// Download and install .NET runtime if not found

function OnDownloadProgress(const Url, Filename: String; const Progress, ProgressMax: Int64): Boolean;
begin
  if ProgressMax <> 0 then
    Log(Format('  %d of %d bytes done.', [Progress, ProgressMax]))
  else
    Log(Format('  %d bytes done.', [Progress]));
  Result := True;
end;

function DownloadAndInstallDotNet: Boolean;
var
  DownloadURL: string;
  DotNetInstallerPath: string;
  TempFilePath: string;
  ExecResult: Integer;
  Proc: String;
begin
  Result := False;
  Proc := ProcArchi;
  case ProcessorArchitecture of
    paX86: DownloadURL := DotNetInstallerURL86;
    paX64: DownloadURL := DotNetInstallerURL64;
    paArm64: DownloadURL := DotNetInstallerURLArm64;
  else
    Result := False;
  end;
  //DotNetInstallerPath := ExpandConstant('{tmp}\windowsdesktop-runtime-' + RequiredDotNetVersion + '-win-' + Proc + '.exe');
  DownloadTemporaryFile(DownloadURL, 'desktop-runtime.exe', '', @OnDownloadProgress);
  DotNetInstallerPath := ExpandConstant('{tmp}\desktop-runtime.exe');
  MsgBox(DotNetInstallerPath, mbError, MB_OK);
  if not DotNetInstallerPath then
  begin
    MsgBox('Failed to download .NET Desktop Runtime (' + Proc + ').', mbError, MB_OK);
    Exit;
  end;
end;



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
function IsNodeInstalled(): Boolean;
var
  ErrorCode: Integer;
begin
  Result := False;
  Exec('cmd.exe', '/C node -v', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
  if ErrorCode = 0 then
    Result := True;
end;

//installation de npm et node si l'utilisateur sélectionne ui et que npm et node ne sont pas installer sur son ordinateur
function IsNpmInstalled(): Boolean;
var
  ErrorCode: Integer;
begin
  Result := False;
  Exec('cmd.exe', '/C npm -v', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
  if ErrorCode = 0 then
    Result := True;
end;

procedure InstallNodeAndNpm();
var
  ResultCode: Integer;
begin
  MsgBox('Node and npm are not installed. Installation of both will now start', mbInformation, MB_OK);
  if not Exec('msiexec.exe', '/i https://nodejs.org/dist/v18.17.1/node-v18.17.1-x64.msi /quiet /norestart', '', SW_HIDE, ewWaitUntilTerminated, ResultCode) then
  begin
    MsgBox('Unable to Node and npm. Try again or install them manually', mbError, MB_OK);
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    //OnDownloadProgress();
    DownloadAndInstallDotNet();
    if IsComponentSelected('ui') then
    begin
      if not IsNodeInstalled() or not IsNpmInstalled() then
      begin
        InstallNodeAndNpm();
      end
      else
      begin
        MsgBox('Node and npm are already installed on your system', mbInformation, MB_OK);
      end;
    end;
  end;
end;