@echo off
set ALLEGRO_PATH=..\allegro

if not exist output mkdir output

gcc -Wall -Wextra -g3 ^
main.c avl-tree.c allegro-functions.c ^
-o output/main.exe ^
-I"include" ^
-I"%ALLEGRO_PATH%/include" ^
-L"%ALLEGRO_PATH%/lib" ^
-lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_main

if %ERRORLEVEL% EQU 0 (
    echo Build successful
    output\main.exe
) else (
    echo Build failed
)