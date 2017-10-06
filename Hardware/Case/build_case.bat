@echo off
SET scad="C:\Program Files\OpenSCAD\openscad.exe"
SET folder=outputs

REM fiducials are for stacking layers - my software centers the piece,
REM which results in imperfect alignment. Delete them afterwards
REM set to 0 if your software can handle coords properly
SET fiducials=1

for %%i in (bottom_plate,bottom_spacer,top_spacer,top_spacer_engrave,top_plate,top_plate_engrave) DO (
    @echo on
    %scad% -o "%folder%\%%i.dxf" "case.scad" -D enable_fiducials=%fiducials% -D "build=\"%%i\""
)

%scad% -o "%folder%\render.png" "case.scad" -D "build=\"render\"" --imgsize=1920,1080 --camera=-9.47,-33.79,17.17,40.30,0.00,313.60,400 --projection=p

REM pause