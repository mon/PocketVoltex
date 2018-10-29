@echo off
SET scad="C:\Program Files\OpenSCAD\openscad.exe"
SET inkscape="C:\Program Files\Inkscape\inkscape.exe"
SET folder=outputs

for %%i in (black,white,grey,orange,blue,pink,cuts,dimensions) DO (
    @echo on
    %scad% -o "%folder%\art_%%i.svg" "artwork.scad" -D enable_bleed=1 -D "build=\"%%i\""
)

python stack_art.py
%inkscape% --export-png=outputs/artwork.png --export-dpi=1200 --without-gui outputs/artwork.svg

@echo off

for %%i in (black,white,grey,orange,blue,pink,cuts,dimensions) DO (
    del "%folder%\art_%%i.svg"
)

REM pause