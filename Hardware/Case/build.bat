@echo off
SET scad="C:\Program Files\OpenSCAD\openscad.exe"
SET folder=outputs

for %%i in (bottom_plate,bottom_ring,top_plate,top_plate_holes,top_plate_outline,top_ring,holes_internal,holes_surface) DO (
    %scad% -o "%folder%\%%i.dxf" "%%i.scad"
)