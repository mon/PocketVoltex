@echo off
SET scad="C:\Program Files\OpenSCAD\openscad.exe"
SET folder=outputs

@echo on
%scad% -o "%folder%\5mm_box_bottom.dxf" "box.scad" -D produce=1 
%scad% -o "%folder%\42mm_box_middle.dxf" "box.scad" -D produce=2
%scad% -o "%folder%\14mm_box_insert.dxf" "box.scad" -D produce=3

%scad% -o "%folder%\render_box.png" "box.scad" -D produce=0 --colorscheme=Tomorrow --imgsize=3840,4320 --camera=32.3082,9.69532,33.9842,45.9,0,314.3,839.47 --projection=p
bash -c "convert '%folder%/render_box.png' -resize 960x1080 '%folder%/render_box.png'"

REM pause