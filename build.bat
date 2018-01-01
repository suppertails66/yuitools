@echo off

REM Macro-defined command used to terminate script on abnormal conditions.
REM Disabled because this apparently doesn't do what I want it to.
REM 
REM TODO: turn windows into a competently designed operating system in which
REM utilities function in a remotely sane fashion
REM 
REM set errmac=if %errorlevel% neq 0 goto onError
set errmac=

set inrom=Battle Golfer Yui (J) [c][!].bin
set bakrom=Copy of Battle Golfer Yui (J) [c][!].bin
set buildrom=Battle Golfer Yui (J) [c][!].bin
set outrom=Battle Golfer Yui (J) [c][!] (EN).bin

if not exist "%inrom%" (
  echo Error: input ROM "%inrom%" not found
  exit /B 1
)

echo -------------------------------------------------------------------------
echo Copying clean ROM to "tools\%bakrom%"...
echo -------------------------------------------------------------------------
copy /y "%inrom%" "tools\%bakrom%"

REM Use Guest's tools to insert the script

cd tools

  echo -------------------------------------------------------------------------
  echo Setting up build files with "copy IMAGE.bat"...
  echo -------------------------------------------------------------------------
  call "copy IMAGE.bat"


  echo -------------------------------------------------------------------------
  echo Inserting script...
  echo -------------------------------------------------------------------------
  call "insert TEXT.bat"

cd ..

REM Use new tools to do everything else

echo -------------------------------------------------------------------------
echo Moving WIP ROM to "new_tools\yui\%buildrom%"...
echo -------------------------------------------------------------------------
copy /y "tools\%buildrom%" "new_tools\%buildrom%"

cd new_tools

REM  rmdir \s \q out
  mkdir out

  echo -------------------------------------------------------------------------
  echo Generating new fonts...
  echo -------------------------------------------------------------------------
  mkdir out\font
  golfer_fontbuild.exe ..\resources\font\trans\font_0x229a2.png 0xA1 out\font\font_0x229a2.bin
  %errmac%
  golfer_fontbuild.exe ..\resources\font\trans\font_0x4d78e.png 0xA1 out\font\font_0x4d78e.bin
  %errmac%
  tileundmp ..\resources\font\trans\font_0x4dcae.png 36 out\font\font_0x4dcae.bin -p ..\resources\pal\yui_pal_golfmenu_line0.bin -r 16
  %errmac%

  echo -------------------------------------------------------------------------
  echo Patching fonts to ROM...
  echo -------------------------------------------------------------------------
  datpatch.exe "%buildrom%" "%buildrom%" out\font\font_0x229a2.bin 0x229a2
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\font\font_0x4d78e.bin 0x4d78e
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\font\font_0x4dcae.bin 0x4dcae
  %errmac%

  echo -------------------------------------------------------------------------
  echo Generating new uncompressed graphics...
  echo -------------------------------------------------------------------------
  mkdir out\grp_uncmp
  tileundmp ..\resources\grp_uncmp\trans\0x4804e.png 4 out\grp_uncmp\0x4804e.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 4
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x480ce.png 4 out\grp_uncmp\0x480ce.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 4
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x4814e.png 4 out\grp_uncmp\0x4814e.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 4
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x482ce.png 2 out\grp_uncmp\0x482ce.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 2
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x4868e.png 1 out\grp_uncmp\0x4868e.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 1
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x48d0e.png 4 out\grp_uncmp\0x48d0e.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 2
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x48e0e.png 4 out\grp_uncmp\0x48e0e.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 2
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x48e8e.png 4 out\grp_uncmp\0x48e8e.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 2
  %errmac%
  tileundmp ..\resources\grp_uncmp\trans\0x4976e.png 4 out\grp_uncmp\0x4976e.bin -p ..\resources\pal\yui_pal_golf_line1.bin -r 2
  %errmac%

  echo -------------------------------------------------------------------------
  echo Patching uncompressed graphics to ROM...
  echo -------------------------------------------------------------------------
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x4804e.bin 0x4804e
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x480ce.bin 0x480ce
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x4814e.bin 0x4814e
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x482ce.bin 0x482ce
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x4868e.bin 0x4868e
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x48d0e.bin 0x48d0e
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x48e0e.bin 0x48e0e
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x48e8e.bin 0x48e8e
  %errmac%
  datpatch.exe "%buildrom%" "%buildrom%" out\grp_uncmp\0x4976e.bin 0x4976e
  %errmac%
  
  echo -------------------------------------------------------------------------
  echo Generating tiles and tilemaps from tilemapper scripts...
  echo -------------------------------------------------------------------------
  mkdir out\precmp
  mkdir out\maps_raw
  tilemapper.exe ..\resources\buildscripts\tilemappers\title.txt
  %errmac%
  tilemapper.exe ..\resources\buildscripts\tilemappers\title_yui.txt
  %errmac%
  tilemapper.exe ..\resources\buildscripts\tilemappers\sidebar_logo.txt
  %errmac%

  echo -------------------------------------------------------------------------
  echo Converting raw tilemaps to half-width format...
  echo -------------------------------------------------------------------------
  mkdir out\maps_conv
  golfer_tilemapbuild out\maps_raw\title_map_0.bin 0xe 0x1 0x41B0 out\maps_conv\title_map_0.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_1.bin 0xf 0x2 0x41B0 out\maps_conv\title_map_1.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_2.bin 0x10 0x3 0x41B0 out\maps_conv\title_map_2.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_3.bin 0xb 0x1 0x41B0 out\maps_conv\title_map_3.bin
  %errmac%
REM  golfer_tilemapbuild out\maps_raw\title_map_4.bin 0x8 0x7 0x41B0 out\maps_conv\title_map_4.bin
REM  %errmac%
REM  golfer_tilemapbuild out\maps_raw\title_map_5.bin 0x8 0x5 0x40a2 out\maps_conv\title_map_5.bin
REM  %errmac%
REM  golfer_tilemapbuild out\maps_raw\title_map_6.bin 0x8 0x5 0x40a2 out\maps_conv\title_map_6.bin
REM  %errmac%
REM  golfer_tilemapbuild out\maps_raw\title_map_7.bin 0x8 0x5 0x40a2 out\maps_conv\title_map_7.bin
REM  %errmac%
REM  golfer_tilemapbuild out\maps_raw\title_map_8.bin 0x8 0x5 0x40a2 out\maps_conv\title_map_8.bin
REM  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_4.bin 0xA 0x7 0x4240 out\maps_conv\title_map_4.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_5.bin 0xA 0x5 0x4240 out\maps_conv\title_map_5.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_6.bin 0xA 0x5 0x4240 out\maps_conv\title_map_6.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_7.bin 0xA 0x5 0x4240 out\maps_conv\title_map_7.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\title_map_8.bin 0xA 0x5 0x4240 out\maps_conv\title_map_8.bin
  %errmac%
  golfer_tilemapbuild out\maps_raw\sidebar_logo_map.bin 0xa 0x5 0x20ba out\maps_conv\sidebar_logo_map.bin
  %errmac%

  echo -------------------------------------------------------------------------
  echo Compressing RLE4-encoded graphics...
  echo -------------------------------------------------------------------------
  del /q "out\precmp\title_grp.bin"
  copy /b "out\precmp\title_grp_*" "out\precmp\title_grp.bin"
  mkdir out\rle4
  cd out\precmp
    for /f %%G in ('dir /b') do (
      "..\..\golfer_rle4cmp" %%G ..\rle4\%%~nG%%~xG
      %errmac%
    )
  cd ..\..

  echo -------------------------------------------------------------------------
  echo Patching tilemaps to ROM...
  echo -------------------------------------------------------------------------

  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_0.bin 0x43f9e
  %errmac%
  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_1.bin 0x43f80
  %errmac%
  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_2.bin 0x43fac
  %errmac%
  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_3.bin 0x43fdc
  %errmac%
REM  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_4.bin 0x43fe8
REM  %errmac%
REM  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_5.bin 0x44020
REM  %errmac%
REM  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_6.bin 0x44048
REM  %errmac%
REM  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_7.bin 0x44070
REM  %errmac%
REM  datpatch "%buildrom%" "%buildrom%" out\maps_conv\title_map_8.bin 0x44098
REM  %errmac%
  datpatch "%buildrom%" "%buildrom%" out\maps_conv\sidebar_logo_map.bin 0x440c0

  echo -------------------------------------------------------------------------
  echo Assembling hacks...
  echo -------------------------------------------------------------------------
  68kasm yui_hacks.asm
  %errmac%

  echo -------------------------------------------------------------------------
  echo Patching hacks to ROM...
  echo -------------------------------------------------------------------------
  srecpatch "%buildrom%" "%buildrom%" < yui_hacks.h68
  
  echo -------------------------------------------------------------------------
  echo Injecting variable-size data...
  echo -------------------------------------------------------------------------
  datinjct ..\resources\buildscripts\yui_freespace.txt ..\resources\buildscripts\yui_inject_script.txt "%buildrom%" "%buildrom%"
  %errmac%

  echo -------------------------------------------------------------------------
  echo Updating ROM header...
  echo -------------------------------------------------------------------------
  romprep "%buildrom%" "%buildrom%"
  %errmac%

cd ..

echo -------------------------------------------------------------------------
echo Moving finished ROM to "%outrom%"...
echo -------------------------------------------------------------------------
copy /y "new_tools\%buildrom%" "%outrom%"
  

echo -------------------------------------------------------------------------
echo Build complete!
echo Output ROM: "%outrom%"
echo -------------------------------------------------------------------------
exit /B 0

:onError
echo ERROR: Build failed
exit /B 1
