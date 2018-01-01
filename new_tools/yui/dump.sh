# terminate script on error
set -e

PATH=".:$PATH"
ROM="$1"

if [ "$1" == "" ]; then
  echo "Battle Golfer Yui resource dumper script"
  echo "Usage: $0 <rom>"
  exit 0
fi;

dump1bppFont() {
  golfer_fontrip $ROM $1 $2 rsrc/font/1bpp/$1.png
}

dumpUncmpGrp() {
  PALETTE=""
  if [ ! "$4" == "" ]; then
    PALETTE="-p $4"
  fi;
  
  TILESPERROW=""
  if [ ! "$5" == "" ]; then
    TILESPERROW="-r $5"
  fi;

  tiledmp $ROM $1 $2 $3$1.png $PALETTE $TILESPERROW
}

dumpRle4Grp() {
  golfer_rle4decmp $ROM $1 rsrc/grp_raw/rle4/$1.bin
}

dumpHwTilemap() {
  golfer_tilemaprip $ROM $1 $2 $3 $4 rsrc/tilemap/hw/$1.bin
  grpunmap $5 rsrc/tilemap/hw/$1.bin $2 $3 rsrc/grp_mapped/$8$1.png -p $6 -v $7
}

dumpTextMap() {
  PADDING=""
  if [ ! "$6" == "" ]; then
    PADDING="-p $6"
  fi;
  
  tiletext_dump $ROM $1 $2 $3 $4 $PADDING > $5$1.txt
}

prependToFile() {
  BSNAME=$(basename $1)
  DRNAME=$(dirname $1)
  printf "$2" | cat - "$1" > "$DRNAME/_$BSNAME"
  mv "$DRNAME/_$BSNAME" "$1"
}

#------------------------------------------------------------------------------
# Clear existing rips
#------------------------------------------------------------------------------
rm -r -f rsrc/*

#------------------------------------------------------------------------------
# Font
#------------------------------------------------------------------------------
mkdir -p rsrc/font/1bpp
dump1bppFont 0x229a2 0xa1
dump1bppFont 0x4d78e 0xa1

#------------------------------------------------------------------------------
# Special uncompressed replacement font for alphanumeric characters in golf
# mode
#------------------------------------------------------------------------------
mkdir -p rsrc/font/uncmp
dumpUncmpGrp 0x4dcae 36 rsrc/font/uncmp/ pal/yui_pal_golfmenu_line0.bin

#------------------------------------------------------------------------------
# Uncompressed graphics
#------------------------------------------------------------------------------
mkdir -p rsrc/grp_uncmp
# golf menu stuff
#dumpUncmpGrp 0x47f2e 0xea rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin
# golf menu components
  # CLUB
  dumpUncmpGrp 0x4804e 0x4 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 4
  # FACE
  dumpUncmpGrp 0x480ce 0x4 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 4
  # ワザ
  dumpUncmpGrp 0x4814e 0x4 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 4
  # GO
  dumpUncmpGrp 0x482ce 0x2 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 2
  # m
  dumpUncmpGrp 0x4868e 0x1 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 1
  # something
  dumpUncmpGrp 0x48d0e 0x4 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 2
  # 火
  dumpUncmpGrp 0x48e0e 0x4 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 2
  # ESP
  dumpUncmpGrp 0x48e8e 0x4 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 2
  # m
  dumpUncmpGrp 0x4976e 0x1 rsrc/grp_uncmp/ pal/yui_pal_golf_line1.bin 1

#------------------------------------------------------------------------------
# RLE4-compressed graphics
#------------------------------------------------------------------------------
mkdir -p rsrc/grp_raw/rle4
# title
dumpRle4Grp 0x2f0d2
# "battle golfer yui" logo displayed on sidebar
dumpRle4Grp 0x30574

# intro character profiles
#dumpRle4Grp 0x30a64

# new game stuff
#dumpRle4Grp 0x22eaa
#dumpRle4Grp 0x4a04

# yui face
#dumpRle4Grp 0x2365c
#dumpRle4Grp 0x2b5ca

# country club bg
#dumpRle4Grp 0x2c866
# caddie
#dumpRle4Grp 0x24958

#------------------------------------------------------------------------------
# Half-width tilemaps
#------------------------------------------------------------------------------
mkdir -p rsrc/tilemap/hw
mkdir -p rsrc/grp_mapped
# title
mkdir -p rsrc/grp_mapped/title
  # "battle golfer"
  dumpHwTilemap 0x43f80 0xf 0x2 0x20a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  dumpHwTilemap 0x43f9e 0xe 0x1 0x20a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  dumpHwTilemap 0x43fac 0x10 0x3 0x20a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  dumpHwTilemap 0x43fdc 0xb 0x1 0x20a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  # "yui" kanji
  dumpHwTilemap 0x43fe8 0x8 0x7 0x40a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  # background flame animation
  dumpHwTilemap 0x44020 0x8 0x5 0x40a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  dumpHwTilemap 0x44048 0x8 0x5 0x40a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  dumpHwTilemap 0x44070 0x8 0x5 0x40a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
  dumpHwTilemap 0x44098 0x8 0x5 0x40a2 rsrc/grp_raw/rle4/0x2f0d2.bin pal/yui_pal_title.bin 0xA2 title/
# sidebar logo
mkdir -p rsrc/grp_mapped/adv
dumpHwTilemap 0x440c0 0xa 0x5 0x20ba rsrc/grp_raw/rle4/0x30574.bin pal/yui_pal_adv.bin 0xBA adv/

#------------------------------------------------------------------------------
# Mapped graphics
#------------------------------------------------------------------------------

# title
#grpunmap rsrc/grp_raw/rle4/0x2f0d2.bin rsrc/tilemap/hw/0x43f80.bin 0xf 0x2 rsrc/grp_mapped/0x43f80.png -p pal/yui_pal_title.bin -v 0xA2
#grpunmap rsrc/grp_raw/rle4/0x2f0d2.bin rsrc/tilemap/hw/0x43f9e.bin 0xe 0x1 rsrc/grp_mapped/0x43f9e.png -p pal/yui_pal_title.bin -v 0xA2
# sidebar logo
#grpunmap rsrc/grp_raw/rle4/0x30574.bin rsrc/tilemap/hw/0x440c0.bin 0xa 0x5 rsrc/grp_mapped/0x440c0.png -p pal/yui_pal_adv.bin -v 0xBA

#------------------------------------------------------------------------------
# Text that isn't already in script
#------------------------------------------------------------------------------
mkdir -p rsrc/text

# Intro (re-rip with uniform width)
introrip $ROM 0x6574e golfer_table.txt 36 -p 36 > rsrc/text/intro.txt

# Cutscene 1
introrip $ROM 0x5f92 golfer_table.txt 13 -p 36 > rsrc/text/cutscene_1.txt

# Cutscene 2
introrip $ROM 0x61f4 golfer_table.txt 9 -p 36 > rsrc/text/cutscene_2.txt

# Dialogue choices
mkdir -p rsrc/text/dlog_choices_split
> rsrc/text/dlog_choices.txt
for i in `seq 0 52`; do
  POS=$((($i * 14) + 409804))
#  tiletext_dump "$ROM" $POS golfer_table.txt 7 2 | cat rsrc/text/dlog_choices.txt - > rsrc/text/_dlog_choices.txt
#  mv rsrc/text/_dlog_choices.txt rsrc/text/dlog_choices.txt
#  tiletext_dump $ROM $POS golfer_table.txt 7 2 -p 20 > rsrc/text/dlog_choices_split/$POS.txt
  tiletext_dump $ROM $POS golfer_table.txt 7 2 > rsrc/text/dlog_choices_split/$POS.txt
done;
#cat dump_dlog_choice_header.txt rsrc/text/dlog_choices.txt > rsrc/text/_dlog_choices.txt
#mv rsrc/text/_dlog_choices.txt rsrc/text/dlog_choices.txt
cat dump_dlog_choice_header.txt rsrc/text/dlog_choices_split/* > rsrc/text/dlog_choices.txt

# Adventure mode
> rsrc/text/adv_misc.txt
printf "//=============================================================================\n// Miscellaneous adventure mode text\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/adv_misc.txt
tiletext_dump $ROM 0x4cfe golfer_table.txt 0x9 0x7 -w 0x481a >> rsrc/text/adv_misc.txt
printf "// Keypad minigame text\n\n" >> rsrc/text/adv_misc.txt
tiletext_dump $ROM 0x9414 golfer_table.txt 0x10 0x2 -w 0x8eb6 >> rsrc/text/adv_misc.txt
tiletext_dump $ROM 0x9434 golfer_table.txt 0xe 0x2 -w 0x8ed4 >> rsrc/text/adv_misc.txt
tiletext_dump $ROM 0x9450 golfer_table.txt 0xe 0x2 -w 0x8ef2 >> rsrc/text/adv_misc.txt

# Title screen
> rsrc/text/title.txt
printf "//=============================================================================\n// Title screen (plus save game text)\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/title.txt
# "PUSH START BUTTON"
tiletext_dump $ROM 0x7370 golfer_table.txt 0x11 0x1 -w 0x430e -w 0x4338 -w 0x4362 -w 0x438c -w 0x711a -w 0x73ac -w 0x73d4 >> rsrc/text/title.txt
tiletext_dump $ROM 0x72f6 golfer_table.txt 0x16 0x2 -w 0x70a2 >> rsrc/text/title.txt
tiletext_dump $ROM 0x7322 golfer_table.txt 0x9 0x2 -w 0x70c0 >> rsrc/text/title.txt
tiletext_dump $ROM 0x7334 golfer_table.txt 0x14 0x2 -w 0x70de >> rsrc/text/title.txt
tiletext_dump $ROM 0x735c golfer_table.txt 0xa 0x2 -w 0x70fa >> rsrc/text/title.txt
tiletext_dump $ROM 0x7382 golfer_table.txt 0x8 0x1 -w 0x7138 >> rsrc/text/title.txt
printf "#ALIGN(2)\n\n" >> rsrc/text/title.txt
maptext_dump $ROM 0x1b04 golfer_table.txt 0xa 0x1 -w 0x1acc >> rsrc/text/title.txt
printf "#ALIGN(2)\n\n" >> rsrc/text/title.txt
maptext_dump $ROM 0x401c golfer_table.txt 0x13 0x5 -w 0x3fe4 >> rsrc/text/title.txt
printf "#ALIGN(2)\n\n" >> rsrc/text/title.txt
maptext_dump $ROM 0x40da golfer_table.txt 0xb 0x8 -w 0x442c >> rsrc/text/title.txt
printf "#ALIGN(2)\n\n" >> rsrc/text/title.txt
maptext_dump $ROM 0x418a golfer_table.txt 0xd 0x5 -w 0x448e >> rsrc/text/title.txt
printf "#ALIGN(2)\n\n" >> rsrc/text/title.txt
maptext_dump $ROM 0x420c golfer_table.txt 0x10 0x5 -w 0x4508 >> rsrc/text/title.txt

# Password
> rsrc/text/password.txt
printf "//=============================================================================\n// Password screen\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/password.txt
tiletext_dump $ROM 0x76ac golfer_table.txt 0x12 0x1 -w 0x74d8 >> rsrc/text/password.txt
tiletext_dump $ROM 0x76be golfer_table.txt 0xd 0x2 -w 0x7ae6 >> rsrc/text/password.txt
tiletext_dump $ROM 0x76d8 golfer_table.txt 0x14 0x1 -w 0x74f6 >> rsrc/text/password.txt
tiletext_dump $ROM 0x76ec golfer_table.txt 0x14 0x1 -w 0x7514 >> rsrc/text/password.txt
tiletext_dump $ROM 0x7700 golfer_table.txt 0x14 0x1 -w 0x7532 >> rsrc/text/password.txt
tiletext_dump $ROM 0x7714 golfer_table.txt 0x14 0x1 -w 0x7550 >> rsrc/text/password.txt
tiletext_dump $ROM 0x7728 golfer_table.txt 0x14 0x1 -w 0x756e >> rsrc/text/password.txt
tiletext_dump $ROM 0x773c golfer_table.txt 0x14 0x1 -w 0x758c >> rsrc/text/password.txt
#tiletext_dump $ROM 0x7750 golfer_table.txt 0x5 0x1 -w 0x75aa >> rsrc/text/password.txt
# gap?
#tiletext_dump $ROM 0x77c6 golfer_table.txt 0x11 0x8 -w 0x75c8 >> rsrc/text/password.txt
#tiletext_dump $ROM 0x7756 golfer_table.txt 0x12 0x2 -w 0x75e6 >> rsrc/text/password.txt

# Sound test
> rsrc/text/sound_test.txt
printf "//=============================================================================\n// Sound test\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/sound_test.txt
tiletext_dump $ROM 0x23f4 golfer_table.txt 0x9 0x1 -w 0x22a8 >> rsrc/text/sound_test.txt
tiletext_dump $ROM 0x23fe golfer_table.txt 0x18 0x1 -w 0x22c6 >> rsrc/text/sound_test.txt
# Track names
for i in `seq 0 52`; do
  POS=$((($i * 14) + 9238))
#  tiletext_dump "$ROM" $POS golfer_table.txt 7 2 | cat rsrc/text/dlog_choices.txt - > rsrc/text/_dlog_choices.txt
#  mv rsrc/text/_dlog_choices.txt rsrc/text/dlog_choices.txt
  tiletext_dump $ROM $POS golfer_table.txt 14 1 >> rsrc/text/sound_test.txt
done;

# Golf mode
> rsrc/text/golf.txt
printf "//=============================================================================\n// Golf mode\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/golf.txt
#tiletext_dump $ROM 0x17898 golfer_table.txt 0x11 0x1 -w 0x430e >> rsrc/text/golf.txt
maptext_dump $ROM 0x17898 golfer_table_menu.txt 0xf 0x7 -w 0x1a714 >> rsrc/text/golf.txt
maptext_dump $ROM 0x1796a golfer_table_menu.txt 0xf 0x7 -w 0x1a74c >> rsrc/text/golf.txt
maptext_dump $ROM 0x1adf2 golfer_table_menu.txt 0x8 0xc -w 0x1a954 >> rsrc/text/golf.txt

# Golf mode in-place stuff
> rsrc/text/golf_inplace.txt
printf "//=============================================================================\n// Golf mode in-place tilemaps\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/golf_inplace.txt
printf "#JMP(\$11072)\n\n" >> rsrc/text/golf_inplace.txt
printf "// Course hazard messages\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x11072 golfer_table_menu.txt 0xd 0x19 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$15AD8)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x15ad8 golfer_table_menu.txt 0x9 0x12 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$15E08)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x15e08 golfer_table_menu.txt 0xb 0x12 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$160CE)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x160ce golfer_table_menu.txt 0xa 0x10 >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x1620e golfer_table_menu.txt 0x8 0x6 >> rsrc/text/golf_inplace.txt
#maptext_dump $ROM 0x1626e golfer_table_menu.txt 0xf 0x1 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$1628C)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x1628c golfer_table_menu.txt 0xf 0x8 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$16456)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x16456 golfer_table_menu.txt 0xe 0xb >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x1658a golfer_table_menu.txt 0x13 0xb >> rsrc/text/golf_inplace.txt
#printf "#JMP(\$16456)\n\n" >> rsrc/text/golf_inplace.txt
printf "// Caddie message -- the mess on the left side is the character portrait.\n// The first line of the message is fixed ("x yards to go"), while the second\n// varies depending on the situation." >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x1672c golfer_table_menu.txt 0x1b 0xb >> rsrc/text/golf_inplace.txt
printf "#JMP(\$16B6A)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x16b6a golfer_table_menu.txt 0x24 0xa >> rsrc/text/golf_inplace.txt
printf "#JMP(\$16EDC)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x16edc golfer_table_menu.txt 0x9 0xa >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x16f90 golfer_table_menu.txt 0x1c 0x5 >> rsrc/text/golf_inplace.txt
#maptext_dump $ROM 0x170a8 golfer_table_menu.txt 0xa 0x11 >> rsrc/text/golf_inplace.txt
#maptext_dump $ROM 0x171fc golfer_table_menu.txt 0x7 0x7 >> rsrc/text/golf_inplace.txt
#maptext_dump $ROM 0x1725e golfer_table_menu.txt 0x7 0x51 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$17626)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x17626 golfer_table_menu.txt 0x8 0x12 >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x17746 golfer_table_menu.txt 0x4 0x6 >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x17776 golfer_table_menu.txt 0x17 0x5 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$17898)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x17898 golfer_table_menu.txt 0xf 0x16 >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x17b2c golfer_table_menu.txt 0x13 0xf >> rsrc/text/golf_inplace.txt
printf "#JMP(\$17DB4)\n\n" >> rsrc/text/golf_inplace.txt
printf "// Terrain infobox on putting screen\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x17db4 golfer_table_menu.txt 0x6 0x8 >> rsrc/text/golf_inplace.txt
printf "#JMP(\$17ED4)\n\n" >> rsrc/text/golf_inplace.txt
maptext_dump $ROM 0x17ed4 golfer_table_menu.txt 0xe 0xe >> rsrc/text/golf_inplace.txt

# Caddie messages
> rsrc/text/caddie.txt
printf "//=============================================================================\n// Caddie messages\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/caddie.txt
printf "#VAR(Table3,TABLE)\n#ADDTBL(\"table/golfer_table_caddie.txt\", Table3)\n#ACTIVETBL(Table3)\n\n" >> rsrc/text/caddie.txt
printf "#JMP(\$19EC2)\n\n" >> rsrc/text/caddie.txt
for i in `seq 0 5`; do
  POS=$((($i * 64) + 106178))
  tiletext_dump $ROM $POS golfer_table_caddie.txt 0x10 0x4 >> rsrc/text/caddie.txt
done;

# Intro profiles
> rsrc/text/intro_profile.txt
printf "//=============================================================================\n// Intro profiles\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/intro_profile.txt
# Common
tiletext_dump $ROM 0x3f4c golfer_table.txt 0x4 0x1 -w 0x2b22 -w 0x2C64 -w 0x2d7c >> rsrc/text/intro_profile.txt
tiletext_dump $ROM 0x3f50 golfer_table.txt 0x3 0x1 -w 0x2b40 -w 0x2C82 -w 0x2d9a >> rsrc/text/intro_profile.txt
tiletext_dump $ROM 0x3f54 golfer_table.txt 0x8 0x1 -w 0x2b5e -w 0x2CA0 -w 0x2db8 >> rsrc/text/intro_profile.txt
# Yui
tiletext_dump $ROM 0x3f5c golfer_table.txt 0x7 0x2 -p 10 -w 0x2b7c -w 0x2dd6 >> rsrc/text/intro_profile.txt
tiletext_dump $ROM 0x3f6a golfer_table.txt 0x2 0x1 -w 0x2b9a -w 0x2df4 >> rsrc/text/intro_profile.txt
tiletext_dump $ROM 0x3f6c golfer_table.txt 0xc 0x2 -w 0x2bb8 -w 0x2e12 >> rsrc/text/intro_profile.txt
#tiletext_dump $ROM 0x440f2 golfer_table.txt 0xa 0xa -w 0x2bd6 >> rsrc/text/intro_profile.txt
# Ran
tiletext_dump $ROM 0x3f84 golfer_table.txt 0x8 0x2 -p 10 -w 0x2cbe >> rsrc/text/intro_profile.txt
tiletext_dump $ROM 0x3f94 golfer_table.txt 0x2 0x1 -w 0x2cdc >> rsrc/text/intro_profile.txt
tiletext_dump $ROM 0x3f96 golfer_table.txt 0xc 0x2 -w 0x2cfa >> rsrc/text/intro_profile.txt

# Shop text
> rsrc/text/shop.txt
printf "//=============================================================================\n// Shop text\n//=============================================================================\n\n#ALIGN(4)\n\n" >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6c62 golfer_table.txt 0x16 0x2 -w 0x6726 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6c8e golfer_table.txt 0x12 0x1 -w 0x67b8 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6ca0 golfer_table.txt 0xc 0x2 -w 0x69ee >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6cb8 golfer_table.txt 0x15 0x2 -w 0x6b78 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6ce2 golfer_table.txt 0xf 0x2 -w 0x6bb8 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d00 golfer_table.txt 0x9 0x1 -w 0x67f2 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d0a golfer_table.txt 0xb 0x1 -w 0x6894 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d16 golfer_table.txt 0x6 0x2 -w 0x6806 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d22 golfer_table.txt 0xb 0x2 -w 0x681a >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d38 golfer_table.txt 0x12 0x2 -w 0x6a5e >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d5c golfer_table.txt 0xf 0x1 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d6c golfer_table.txt 0xe 0x2 -w 0x68cc >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6d88 golfer_table.txt 0xe 0x2 -w 0x68de >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6da4 golfer_table.txt 0xe 0x2 -w 0x68f0 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6dc0 golfer_table.txt 0x16 0x2 -w 0x695a >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6dec golfer_table.txt 0x17 0x2 -w 0x6972 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6e1a golfer_table.txt 0x16 0x2 -w 0x698a >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6e46 golfer_table.txt 0x2 0x1 -w 0x6a06 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6e48 golfer_table.txt 0x9 0x2 -w 0x6a1e >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6e5a golfer_table.txt 0x6 0x2 -w 0x6ae4 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6e66 golfer_table.txt 0x6 0x2 -w 0x6b02 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6e72 golfer_table.txt 0x7 0x2 -w 0x6b20 >> rsrc/text/shop.txt
tiletext_dump $ROM 0x6e80 golfer_table.txt 0x3 0x1 -w 0x6b3e >> rsrc/text/shop.txt

# Credits
mkdir -p rsrc/text/credits_split
> rsrc/text/credits.txt
dumpTextMap 0x578c golfer_table.txt 0x9 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x578c.txt "#W32(\$5002)\n"
dumpTextMap 0x57e6 golfer_table.txt 0xa 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x57e6.txt "#W32(\$501E)\n#W32(\$5056)\n"
dumpTextMap 0x57fa golfer_table.txt 0x7 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x57fa.txt "#W32(\$503A)\n"
dumpTextMap 0x5808 golfer_table.txt 0x6 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5808.txt "#W32(\$5072)\n"
dumpTextMap 0x5814 golfer_table.txt 0x6 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5814.txt "#W32(\$508E)\n#W32(\$50E2)\n"
dumpTextMap 0x5820 golfer_table.txt 0xa 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5820.txt "#W32(\$50AA)\n#W32(\$50C6)\n"
dumpTextMap 0x5834 golfer_table.txt 0x5 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5834.txt "#W32(\$50FE)\n"
dumpTextMap 0x583e golfer_table.txt 0x5 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x583e.txt "#W32(\$511A)\n"
dumpTextMap 0x5848 golfer_table.txt 0x3 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5848.txt "#W32(\$5136)\n"
dumpTextMap 0x584e golfer_table.txt 0x5 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x584e.txt "#W32(\$5152)\n"
dumpTextMap 0x5858 golfer_table.txt 0x6 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5858.txt "#W32(\$516E)\n"
dumpTextMap 0x5864 golfer_table.txt 0xb 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5864.txt "#W32(\$518A)\n"
dumpTextMap 0x587a golfer_table.txt 0x9 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x587a.txt "#W32(\$51A6)\n"
dumpTextMap 0x588c golfer_table.txt 0x9 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x588c.txt "#W32(\$51C2)\n"
dumpTextMap 0x589e golfer_table.txt 0x6 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x589e.txt "#W32(\$51DE)\n"
dumpTextMap 0x58aa golfer_table.txt 0x3 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x58aa.txt "#W32(\$51FA)\n"
dumpTextMap 0x58b0 golfer_table.txt 0x3 0x2 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x58b0.txt "#W32(\$5216)\n"
# non-text "finale"
#dumpTextMap 0x58c4 golfer_table.txt 0x9 0x3 rsrc/text/credits_split/ 20
dumpTextMap 0x58b6 golfer_table.txt 0x7 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x58b6.txt "#W32(\$5252)\n"
dumpTextMap 0x58be golfer_table.txt 0x5 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x58be.txt "#W32(\$5272)\n"
# PLANNER
dumpTextMap 0x5796 golfer_table.txt 0x7 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x5796.txt "#W32(\$52D2)\n"
dumpTextMap 0x579e golfer_table.txt 0x8 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x579e.txt "#W32(\$52E6)\n"
dumpTextMap 0x57a6 golfer_table.txt 0x12 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x57a6.txt "#W32(\$52FA)\n"
dumpTextMap 0x57b8 golfer_table.txt 0x10 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x57b8.txt "#W32(\$530E)\n"
dumpTextMap 0x57c8 golfer_table.txt 0xa 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x57c8.txt "#W32(\$5322)\n"
dumpTextMap 0x57d2 golfer_table.txt 0x5 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x57d2.txt "#W32(\$5336)\n"
dumpTextMap 0x57d8 golfer_table.txt 0xe 0x1 rsrc/text/credits_split/ 20
prependToFile rsrc/text/credits_split/0x57d8.txt "#W32(\$534A)\n"
cat dump_credits_header.txt rsrc/text/credits_split/* > rsrc/text/credits.txt

#------------------------------------------------------------------------------
# Autodump decompressed graphics
#------------------------------------------------------------------------------
mkdir -p rsrc/grp_autodump
for file in rsrc/grp_raw/rle4/*.bin; do
  grpdmp "$file" rsrc/grp_autodump/$(basename $file .bin).png
done;