
SET scad="C:\Program Files\OpenSCAD\openscad.exe"
SET folder=outputs

REM fiducials are for stacking layers - my software centers the piece,
REM which results in imperfect alignment. Delete them afterwards
REM set to 0 if your software can handle coords properly
SET fiducials=0

REM intitial render
for %%i in (bottom_plate,bottom_spacer,top_spacer,top_spacer_engrave,top_reflector,top_plate,art_cutout) DO (
    @echo on
    %scad% -o "%folder%\%%i.dxf" "case.scad" -D enable_fiducials=%fiducials% -D "build=\"%%i\""
)

REM clean polylines for single layers
for %%i in (bottom_plate,bottom_spacer,top_plate,top_reflector) DO (
    @echo on
    py -2 odmt.py --input "%folder%\%%i.dxf" --output "%folder%\%%i.dxf"
)

REM combine DXF for multiple layers
py -2 odmt.py --input "%folder%\top_spacer.dxf" "%folder%\top_spacer_engrave.dxf" --output "%folder%\top_spacer.dxf"

REM delete the multi-layer leftovers
DEL "%folder%\top_spacer_engrave.dxf"

REM openscad has no anti-aliasing so let's just do it ourselves
REM main render
%scad% -o "%folder%\render.png" "case.scad" -D "build=\"render\"" --colorscheme=Tomorrow --imgsize=5600,4320 --camera=7.40041,-3.99594,-17.4439,44.5,0,327.6,401.516 --projection=p
bash -c "convert '%folder%/render.png' -resize 1400x1080 '%folder%/render.png'"

REM exploded render
%scad% -o "%folder%\render_exploded.png" "case.scad" -D "build=\"render\"" -D $t=1 --colorscheme=Tomorrow --imgsize=3440,4320 --camera=49.3894,45.0844,40.101,66.2,0,320.6,679.971 --projection=p
bash -c "convert '%folder%/render_exploded.png' -resize 860x1080 '%folder%/render_exploded.png'"

REM pause