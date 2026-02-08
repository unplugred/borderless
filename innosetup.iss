[Setup]
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
PrivilegesRequired=admin
AppName=Borderless
AppPublisherURL=https://unplug.red/
AppSupportURL=https://unplug.red/
AppUpdatesURL=https://unplug.red/
AppVerName=Borderless
AppVersion=1.0
Compression=lzma2/ultra64
DefaultDirName={pf}\borderless
DefaultGroupName=UnplugRed
DisableReadyPage=false
DisableWelcomePage=no
LanguageDetectionMethod=uilanguage
OutputBaseFilename=Borderless Installer
SetupIconFile=dist\install.ico
ShowLanguageDialog=no
VersionInfoCompany=UnplugRed
VersionInfoProductName=Borderless
VersionInfoProductVersion=1.0
VersionInfoVersion=1.0
WizardImageStretch=false
;WizardImageFile=assets\image\Borderless.bmp
;WizardSmallImageFile=assets\smallimage\Borderless.bmp
ChangesAssociations=yes

[Files]
Source: "artifact\*"; DestDir: {app}; Flags: recursesubdirs ignoreversion
[Icons]
Name: "{userdesktop}\Borderless"; Filename: "{app}\borderless.exe"; Tasks: desktopicon
Name: "{group}\Borderless"; Filename: "{group}\borderless.exe"
Name: "{group}\Uninstall Borderless"; Filename: {uninstallexe}
[Registry]
Root: HKCR; Subkey: ".png" ; Tasks: "associations/png" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".jpg" ; Tasks: "associations/jpg" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".jpeg"; Tasks: "associations/jpg" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".gif" ; Tasks: "associations/gif" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".webp"; Tasks: "associations/webp"; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".tiff"; Tasks: "associations/tiff"; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".svg" ; Tasks: "associations/svg" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".svgz"; Tasks: "associations/svgz"; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".bmp" ; Tasks: "associations/bmp" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".tga" ; Tasks: "associations/tga" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".cur" ; Tasks: "associations/cur" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".ico" ; Tasks: "associations/ico" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".jfif"; Tasks: "associations/jfif"; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".pbm" ; Tasks: "associations/pbm" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".pgm" ; Tasks: "associations/pgm" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".ppm" ; Tasks: "associations/ppm" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".xbm" ; Tasks: "associations/xbm" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".xpm" ; Tasks: "associations/xpm" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".icns"; Tasks: "associations/icns"; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".jp2" ; Tasks: "associations/jp2" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".mng" ; Tasks: "associations/mng" ; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".wbmp"; Tasks: "associations/wbmp"; ValueType: string; ValueName: ""; ValueData: "Borderless"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "Borderless";                        ValueType: string; ValueName: ""; ValueData: "Program Borderless"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Borderless\DefaultIcon";            ValueType: string; ValueName: ""; ValueData: "{app}\borderless.exe,0"
Root: HKCR; Subkey: "Borderless\shell\open\command";     ValueType: string; ValueName: ""; ValueData: """{app}\borderless.exe"" ""%1"""
[Tasks]
Name: "desktopicon"; Description: "Create a desktop icon"; GroupDescription: "Additional icons"; Flags: unchecked
Name: "associations"; Description: "Associate with:"; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/png" ; Description: ".PNG" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/jpg" ; Description: ".JPG" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/gif" ; Description: ".GIF" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/webp"; Description: ".WEBP"; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/tiff"; Description: ".TIFF"; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/svg" ; Description: ".SVG" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/svgz"; Description: ".SVGZ"; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/bmp" ; Description: ".BMP" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/tga" ; Description: ".TGA" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/cur" ; Description: ".CUR" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/ico" ; Description: ".ICO" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/jfif"; Description: ".JFIF"; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/pbm" ; Description: ".PBM" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/pgm" ; Description: ".PGM" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/ppm" ; Description: ".PPM" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/xbm" ; Description: ".XBM" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/xpm" ; Description: ".XPM" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/icns"; Description: ".ICNS"; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/jp2" ; Description: ".JP2" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/mng" ; Description: ".MNG" ; GroupDescription: "File associations"; Flags: unchecked
Name: "associations/wbmp"; Description: ".WBMP"; GroupDescription: "File associations"; Flags: unchecked
;[Run]
;Filename: "{app}\borderless.exe"; Description: "Launch application"; Flags: nowait postinstall skipifsilent