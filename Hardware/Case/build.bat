@echo off
SET scad="C:\Program Files\OpenSCAD\openscad.exe"
SET folder=outputs

for %%i in (art_trimmed,bottom_plate,bottom_ring,top_plate,top_ring) DO (
    %scad% -o "%folder%\%%i.dxf" "%%i.scad"
)