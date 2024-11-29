[Setup]
;Info de base
AppName=Ecofloc
AppVersion=1.0
DefaultDirName={autopf}\ecofloc
;Apparence
WizardStyle=modern
UninstallDisplayIcon={app}\images\ecoflocV1.ico

;
LicenseFile=LICENSE
InfoBeforeFile=readme.md
InfoAfterFile=readme.md

Output=no

[Files]
;peut ajouter autant de fichier qu'on veut
Source: "hello.exe"; DestDir: "{app}"

[Components]
Name: "component"; Description: "Ecofloc"; Types: full compact custom; Flags: fixed
Name: "ui"; Description: "Web UI"; Types: full



;je sais pas encore quoi faire mais ça change le path normalement
;[Registry]
;Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; \
;    ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};C:\foo"