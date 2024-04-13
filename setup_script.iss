[Setup]
AppName=Mti680_Data_receiver
AppVersion=1.0
DefaultDirName={pf}\Mti680_Data_receiver
OutputDir=Output
OutputBaseFilename=Setup
Compression=lzma2
SolidCompression=yes

[Files]
Source: "Mti680_Data_receiver.exe"; DestDir: "{app}"
Source: "libxl.dll"; DestDir: "{app}"
Source: "libiomp5md.dll"; DestDir: "{app}"
Source: "minizip.dll"; DestDir: "{app}"
Source: "xlsxwriter.dll"; DestDir: "{app}"
Source: "xsensdeviceapi64.dll"; DestDir: "{app}"
Source: "xstypes64.dll"; DestDir: "{app}"
; Add more DLLs or libraries as needed
; Source: "path_to_your_additional_dll.dll"; DestDir: "{app}"

[Icons]
Name: "{commondesktop}\Mti680_Data_receiver"; Filename: "{app}\Mti680_Data_receiver.exe"; IconFilename: "{app}\Mti680_Data_receiver.exe"

[Run]
Filename: "{app}\Mti680_Data_receiver.exe"; Description: "Launch Mti680_Data_receiver"; Flags: nowait postinstall
