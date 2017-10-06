@echo off
SET scad="C:\Program Files\OpenSCAD\openscad.exe"
SET folder=outputs

@echo on
%scad% -o "%folder%\layer0_5mm_box_bottom.dxf" "box.scad" -D produce=1 
%scad% -o "%folder%\layer1_37.5mm_box_middle.dxf" "box.scad" -D produce=2
%scad% -o "%folder%\layer1.5_12mm_box_insert.dxf" "box.scad" -D produce=3
pause