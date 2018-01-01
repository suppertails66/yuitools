*******************************************************************************
* Battle Golfer Yui English translation hacks
*******************************************************************************

*******************************************************************************
* Resize adventure mode dialogue choice tilemaps from 7x2 to 9x2
*******************************************************************************

  org $99D2
  mulu.w    #18,d1      * total byte size of each map is now 9 * 2 = 18
  
  org $99E4
  moveq     #9,d1       * width: 9
  moveq     #2,d2       * height: 2
  
  * Shift the target VRAM position for each option to compensate for
  * the increased width.
  * Only the last four options matter, since the game sorts options
  * into two columns of four options each.
  org $640BC
  dc.w      $C90A      * Option 1 VRAM target (same)
  dc.w      $CA0A      * Option 2 VRAM target (same)
  dc.w      $CB0A      * Option 3 VRAM target (same)
  dc.w      $CC0A      * Option 4 VRAM target (same)
  dc.w      $C922      * Option 5 VRAM target (shift right 2 tiles)
  dc.w      $CA22      * Option 6 VRAM target (shift right 2 tiles)
  dc.w      $CB22      * Option 7 VRAM target (shift right 2 tiles)
  dc.w      $CC22      * Option 8 VRAM target (shift right 2 tiles)

*******************************************************************************
* Resize adventure mode password explanation's individual tilemaps
* to uniform 22x2 tiles
*******************************************************************************

  * Line 1
  org $70AC
  move.w    #$15,d1
  
  * Line 2
  org $70CA
  move.w    #$15,d1
  
  * Line 3
  org $70E8
  move.w    #$15,d1
  
  * Line 4
  org $7106
  move.w    #$15,d1

*******************************************************************************
* Resize credits tilemaps to 36 tiles wide
*******************************************************************************
  
  *******************
  * Upper area
  *******************

  org $5006
  lea       $ba84,a2    * destination VRAM address (in window plane at B000)
  moveq     #36,d1      * width in tiles
  
  org $5022
  lea       $ba84,a2
  moveq     #36,d1
  
  org $503E
  lea       $ba84,a2
  moveq     #36,d1
  
  org $505A
  lea       $ba84,a2
  moveq     #36,d1
  
  org $5076
  lea       $ba84,a2
  moveq     #36,d1
  
  org $5092
  lea       $ba84,a2
  moveq     #36,d1
  
  org $50AE
  lea       $ba84,a2
  moveq     #36,d1
  
  org $50CA
  lea       $ba84,a2
  moveq     #36,d1
  
  org $50E6
  lea       $ba84,a2
  moveq     #36,d1
  
  org $5102
  lea       $ba84,a2
  moveq     #36,d1
  
  org $511E
  lea       $ba84,a2
  moveq     #36,d1
  
  org $513A
  lea       $ba84,a2
  moveq     #36,d1
  
  org $5156
  lea       $ba84,a2
  moveq     #36,d1
  
  org $5172
  lea       $ba84,a2
  moveq     #36,d1
  
  org $518E
  lea       $ba84,a2
  moveq     #36,d1
  
  org $51AA
  lea       $ba84,a2
  moveq     #36,d1
  
  org $51C6
  lea       $ba84,a2
  moveq     #36,d1
  
  org $51E2
  lea       $ba84,a2
  moveq     #36,d1
  
  org $51FE
  lea       $ba84,a2
  moveq     #36,d1
  
  org $521A
  lea       $ba84,a2
  moveq     #36,d1
  
  org $5256
  lea       $ba84,a2
  moveq     #36,d1
  
  org $5276
  lea       $ba84,a2
  moveq     #36,d1
  
  *******************
  * Lower area
  *******************
  
  org $52D6
  lea       $ba04,a2
  moveq     #36,d1
  
  org $52EA
  lea       $ba04,a2
  moveq     #36,d1
  
  org $52FE
  lea       $ba04,a2
  moveq     #36,d1
  
  org $5312
  lea       $ba04,a2
  moveq     #36,d1
  
  org $5326
  lea       $ba04,a2
  moveq     #36,d1
  
  org $533A
  lea       $ba04,a2
  moveq     #36,d1
  
  org $534E
  lea       $ba04,a2
  moveq     #36,d1

*******************************************************************************
* Resize intro tilemaps to 36x2 tiles
*******************************************************************************

  org $65752
  dc.b $BB,$84,$00,$24,$00,$02

  org $6575c
  dc.b $BB,$84,$00,$24,$00,$02

  org $65766
  dc.b $BB,$84,$00,$24,$00,$02

  org $65770
  dc.b $BB,$84,$00,$24,$00,$02

  org $6577a
  dc.b $BB,$84,$00,$24,$00,$02

  org $65784
  dc.b $BB,$84,$00,$24,$00,$02

  org $6578e
  dc.b $BB,$84,$00,$24,$00,$02

  org $65798
  dc.b $BB,$84,$00,$24,$00,$02

  org $657a2
  dc.b $BB,$84,$00,$24,$00,$02

  org $657ac
  dc.b $BB,$84,$00,$24,$00,$02

  org $657b6
  dc.b $BB,$84,$00,$24,$00,$02

  org $657c0
  dc.b $BB,$84,$00,$24,$00,$02

  org $657ca
  dc.b $BB,$84,$00,$24,$00,$02

  org $657d4
  dc.b $BB,$84,$00,$24,$00,$02

  org $657de
  dc.b $BB,$84,$00,$24,$00,$02

  org $657e8
  dc.b $BB,$84,$00,$24,$00,$02

  org $657f2
  dc.b $BB,$84,$00,$24,$00,$02

  org $657fc
  dc.b $BB,$84,$00,$24,$00,$02

  org $65806
  dc.b $BB,$84,$00,$24,$00,$02

  org $65810
  dc.b $BB,$84,$00,$24,$00,$02

  org $6581a
  dc.b $BB,$84,$00,$24,$00,$02

  org $65824
  dc.b $BB,$84,$00,$24,$00,$02

  org $6582e
  dc.b $BB,$84,$00,$24,$00,$02

  org $65838
  dc.b $BB,$84,$00,$24,$00,$02

  org $65842
  dc.b $BB,$84,$00,$24,$00,$02

  org $6584c
  dc.b $BB,$84,$00,$24,$00,$02

  org $65856
  dc.b $BB,$84,$00,$24,$00,$02

  org $65860
  dc.b $BB,$84,$00,$24,$00,$02

  org $6586a
  dc.b $BB,$84,$00,$24,$00,$02

  org $65874
  dc.b $BB,$84,$00,$24,$00,$02

  org $6587e
  dc.b $BB,$84,$00,$24,$00,$02

  org $65888
  dc.b $BB,$84,$00,$24,$00,$02

  org $65892
  dc.b $BB,$84,$00,$24,$00,$02

  org $6589c
  dc.b $BB,$84,$00,$24,$00,$02

  org $658a6
  dc.b $BB,$84,$00,$24,$00,$02

  org $658b0
  dc.b $BB,$84,$00,$24,$00,$02

*******************************************************************************
* Resize good ending part 1 tilemaps to 36x2 tiles
*******************************************************************************

  * This cutscene displays its text in multiple lines, each with its
  * own tilemap. VRAM addresses must advance 2 lines for each
  * successive entry in a text "block" (add $100 to previous address).
  
  ************************
  * Block 1
  ************************
  
  org $5f96
  * format:
  *   - 16-bit VRAM destination
  *   - 16-bit width
  *   - 16-bit height
  dc.b $E3,$84,$00,$24,$00,$04
  
  ************************
  * Block 2
  ************************

  org $5fa0
  dc.b $E3,$84,$00,$24,$00,$04

  org $5faa
  dc.b $E5,$84,$00,$24,$00,$04

  org $5fb4
  dc.b $E7,$84,$00,$24,$00,$04
  
  ************************
  * Block 3
  ************************

  org $5fbe
  dc.b $E3,$84,$00,$24,$00,$04

  org $5fc8
  dc.b $E5,$84,$00,$24,$00,$04

  org $5fd2
  dc.b $E7,$84,$00,$24,$00,$04

  org $5fdc
  dc.b $E9,$84,$00,$24,$00,$02

  org $5fe6
  dc.b $EA,$84,$00,$24,$00,$02
  
  ************************
  * Block 4
  ************************

  org $5ff0
  dc.b $E3,$84,$00,$24,$00,$04

  org $5ffa
  dc.b $E5,$84,$00,$24,$00,$04

  org $6004
  dc.b $E7,$84,$00,$24,$00,$02
  
  ************************
  * Block 5
  ************************

  org $600e
  * start this two lines lower than usual
  dc.b $E4,$84,$00,$24,$00,$04

*******************************************************************************
* Resize good ending part 2 tilemaps to 36x2 tiles
*******************************************************************************

  org $61f8
  dc.b $EB,$84,$00,$24,$00,$02

  org $6202
  dc.b $EB,$84,$00,$24,$00,$02

  org $620c
  dc.b $EB,$84,$00,$24,$00,$02

  org $6216
  dc.b $EB,$84,$00,$24,$00,$02

  org $6220
  dc.b $EB,$84,$00,$24,$00,$02

  org $622a
  dc.b $EB,$84,$00,$24,$00,$02

  org $6234
  dc.b $EB,$84,$00,$24,$00,$02

  org $623e
  dc.b $EB,$84,$00,$24,$00,$02

  org $6248
  dc.b $EB,$84,$00,$24,$00,$02

*******************************************************************************
* Resize incorrect password message
*******************************************************************************
  
  * message parameters
  org $7AEA
  lea         $E894,A2        * VRAM target
  move.w      #$13,D1         * width
  move.w      #$2,D2          * height
  
  * message clear parameters
  org $7892
  move.l      #$68940003,D0   * VDP target
  moveq       #$12,D1         * width - 1
  moveq       #$1,D2          * height - 1

*******************************************************************************
* Resize and rearrange intro profile tilemaps as needed
*******************************************************************************

  **********************
  * Yui name 1
  **********************
  org $2B80
  move.l      #$47880003,d0   * yui name VDP transfer command
                              * original was 468E0003; we need to move this
                              * down a row and 2 tiles to the left, to match
                              * the alignment of "physique"
  move.w      #$b,d1          * yui name width - 1
  move.w      #$0,d2          * yui name height - 1
  
  **********************
  * Yui age 1
  **********************
  org $2B9E
  move.l      #$48880003,d0   * yui age ("16") VDP transfer
                              * orig: 480E0003
                              * move to match new name alignment
  
  **********************
  * Yui physique 1
  **********************
  org $2BBC
  move.l      #$49880003,d0
  move.w      #$d,d1          * width - 1
  move.w      #$1,d2          * height - 1
  
  **********************
  * Yui name 2
  **********************
  org $2DDA
  move.l      #$67880003,d0   * yui's profile is transferred a second time
                              * as part of the "cut-in" effect
                              * orig: 668E0003
  move.w      #$b,d1          * yui name width - 1
  move.w      #$0,d2          * yui name height - 1
  
  **********************
  * Yui age 2
  **********************
  org $2DF8
  move.l      #$68880003,d0   * yui age ("16") VDP transfer duplicate
                              * orig: 680E0003
                              * move to match new name alignment
  
  **********************
  * Yui physique 2
  **********************
  org $2E16
  move.l      #$69880003,d0
  move.w      #$d,d1          * width - 1
  move.w      #$1,d2          * height - 1
  
  **********************
  * Ran name
  **********************
  org $2CC2
  move.l      #$67B00003,d0   * ran name VDP transfer command
                              * original was 66B60003; we need to move this
                              * down a row and 2 tiles to the left, to match
                              * the alignment of "physique"
  move.w      #$b,d1          * ran name width - 1
  move.w      #$0,d2          * ran name height - 1
  
  **********************
  * Ran age
  **********************
  org $2CE0
  move.l      #$68B00003,d0   * ran age ("17") VDP transfer
                              * orig: 68360003
                              * move to match new name alignment
  
  **********************
  * Ran physique
  **********************
  org $2CFE
  move.l      #$69B00003,d0
  move.w      #$d,d1          * width - 1
  move.w      #$1,d2          * height - 1
  
*******************************************************************************
* Modify password entry screen to use English characters
*******************************************************************************

  *****************************************************************************
  * New screen layout:
  *
  *   A B C D E  F G H I J # #
  *   K L M N O  P Q R S T # #
  *   U V W X Y  Z a b c d # #
  *   e f g h i  j k l m n # #
  *   o p q r s  t u v w x # #
  *   y z ! ? -  # # # # < > $
  *
  * Each space in the grid is assigned an index -- ascending order, left to
  * right, top to bottom, starting from 0. "#" indices are not valid, but are
  * assigned an index anyways.
  *
  * 7DC8: Table mapping indices to VRAM address at which to place the selection
  *       cursor when that each index is chosen. Also determines which indices
  *       are considered valid targets.
  * 7F0E: Table mapping indices to the "password data" that's written to
  *       memory when that index is used as input.
  * 7E7E: Table mapping indices to the pattern index of the character used to
  *       display them. This isn't used for the actual character selection
  *       area, but is used to print the currently entered characters at the
  *       bottom of the screen.
  * 7294: Mapping of "password data" (raw bytes 0x00-0x36) to the pattern
  *       indices used to show them. This is used to display passwords
  *       after the game generates them.
  * 
  *****************************************************************************

  *****************************************************************************
  * Table of VRAM addresses indicating where to place the cursor when each 
  * index is highlighted. Entries of FFFF indicate invalid indices; this is
  * used to detect which spaces to skip over when the player moves the cursor.
  *****************************************************************************
  org $7DC8
  dc.w        $E210,$E214,$E218,$E21C,$E220,$E226,$E22A,$E22E,$E232,$E236,$FFFF,$FFFF
  dc.w        $E310,$E314,$E318,$E31C,$E320,$E326,$E32A,$E32E,$E332,$E336,$FFFF,$FFFF
  dc.w        $E410,$E414,$E418,$E41C,$E420,$E426,$E42A,$E42E,$E432,$E436,$FFFF,$FFFF
  dc.w        $E510,$E514,$E518,$E51C,$E520,$E526,$E52A,$E52E,$E532,$E536,$FFFF,$FFFF
  dc.w        $E610,$E614,$E618,$E61C,$E620,$E626,$E62A,$E62E,$E632,$E636,$FFFF,$FFFF
  dc.w        $E710,$E714,$E718,$E71C,$E720,$FFFF,$FFFF,$FFFF,$FFFF,$E736,$E73A,$E73E
  
  *****************************************************************************
  * Mapping of indices to their corresponding "password data".
  * Invalid indices are filled with 00FE (but never checked under normal
  * circumstances).
  * It's critical that the "password data" be in exactly this order, or the
  * game will no longer generate valid passwords.
  *****************************************************************************
  org $7F0E
  dc.w        $0000,$0001,$0002,$0003,$0004,$0005,$0006,$0007,$0008,$0009,$00FE,$00FE * 0
  dc.w        $000A,$000B,$000C,$000D,$000E,$000F,$0010,$0011,$0012,$0013,$00FE,$00FE * C
  dc.w        $0014,$0015,$0016,$0017,$0018,$0019,$001A,$001B,$001C,$001D,$00FE,$00FE * 18
  dc.w        $001E,$001F,$0020,$0021,$0022,$0023,$0024,$0025,$0026,$0027,$00FE,$00FE * 24
  dc.w        $0028,$0029,$002A,$002B,$002C,$002D,$002E,$002F,$0030,$0031,$00FE,$00FE * 30
  dc.w        $0032,$0033,$0034,$0035,$0036,$00FE,$00FE,$00FE,$00FE,$00FE,$00FE,$00FE * 3C
  
  *****************************************************************************
  * Mapping of indices to the pattern index used to display them.
  *****************************************************************************
  org $7E7E
  dc.w        $005D,$005E,$005F,$0060,$0061,$0062,$0063,$0064,$0065,$0066,$00FE,$00FE
  dc.w        $0067,$0068,$0069,$006A,$006B,$006C,$006D,$006E,$006F,$0070,$00FE,$00FE
  dc.w        $0071,$0072,$0073,$0074,$0075,$0076,$0043,$0044,$0045,$0046,$00FE,$00FE
  dc.w        $0047,$0048,$0049,$004A,$004B,$004C,$004D,$004E,$004F,$0050,$00FE,$00FE
  dc.w        $0051,$0052,$0053,$0054,$0055,$0056,$0057,$0058,$0059,$005A,$00FE,$00FE
  dc.w        $005B,$005C,$0081,$0082,$0084,$00FE,$00FE,$00FE,$00FE,$00AC,$00AD,$00AE
  
  *****************************************************************************
  * Table mapping "password data" (bytes 0x00-0x36) to the character used to
  * display them on the password get screen.
  *****************************************************************************
  org $7288
  dc.w        $005D,$005E,$005F,$0060,$0061,$0062,$0063,$0064,$0065,$0066
  dc.w        $0067,$0068,$0069,$006A,$006B,$006C,$006D,$006E,$006F,$0070
  dc.w        $0071,$0072,$0073,$0074,$0075,$0076,$0043,$0044,$0045,$0046
  dc.w        $0047,$0048,$0049,$004A,$004B,$004C,$004D,$004E,$004F,$0050
  dc.w        $0051,$0052,$0053,$0054,$0055,$0056,$0057,$0058,$0059,$005A
  dc.w        $005B,$005C,$0081,$0082,$0084
  
  *****************************************************************************
  * Update special passwords.
  * The original game has three hardcoded cheats:
  *
  * 
  * ふるいけや
  * かはすとひこむ
  * みすのおと
  * 
  * 古池や蛙飛びこむ水の音
  * 
  * "Ah! The ancient pond, as a frog takes the plunge, sound of the water"
  * (famous haiku by Matsuo Basho)
  * 
  * This plays the credits.
  * 
  *
  * 
  * なつくさや
  * ゆわものともか
  * ゆめのあと
  *
  * 夏草や兵どもが夢の跡
  *
  * "Mounds of summer grass - the place where noble soldiers one time dreamed a dream"
  * (another haiku by the same author)
  * 
  * This starts adventure mode at level 99, with 980 SP.
  *
  * 
  *
  * すとろおく
  * わさかあったら
  * かんたんた
  *
  * すとろおく　わざがあったら　かんたんだ
  *
  * Stroke
  * Since there are skills
  * it is easy
  *
  * This allows skills to be used in Stroke Play mode.
  *
  *
  *
  * The Japanese passwords are obviously gibberish with the English font, so 
  * this changes them to some loose 17-character approximations. These are
  * encoded using raw password indices:
  * 
  *   A=00,B=01,C=02,D=03,E=04,F=05,G=06,H=07,I=08,J=09,
  *   K=0A,L=0B,M=0C,N=0D,O=0E,P=0F,Q=10,R=11,S=12,T=13,
  *   U=14,V=15,W=16,X=17,Y=18,Z=19,a=1A,b=1B,c=1C,d=1D,
  *   e=1E,f=1F,g=20,h=21,i=22,j=23,k=24,l=25,m=26,n=27,
  *   o=28,p=29,q=2A,r=2B,s=2C,t=2D,u=2E,v=2F,w=30,x=31,
  *   y=32,z=33,!=34,?=35,-=36
  *
  *****************************************************************************
  
  * Credits
  * Original: ふるいけやかはすとひこむみすのおと
  * New: TheRavenNevermore
  org $8050
  dc.w        $0013,$0021,$001E,$0011,$001A
  dc.w        $002F,$001E,$0027,$000D,$001E,$002F,$001E
  dc.w        $002B,$0026,$0028,$002B,$001E
  
  * Max level
  * Original: なつくさやゆわものともかゆめのあと
  * New: OneLessTraveledBy
  org $8072
  dc.w        $000E,$0027,$001E,$000B,$001E
  dc.w        $002C,$002C,$0013,$002B,$001A,$002F,$001E
  dc.w        $0025,$001E,$001D,$0001,$0032
  
  * Use skills in stroke mode
  * Original: すとろおくわさかあったらかんたんた
  * New: GimmeStrokeSkills
  org $8094
  dc.w        $0006,$0022,$0026,$0026,$001E
  dc.w        $0012,$002D,$002B,$0028,$0024,$001E,$0012
  dc.w        $0024,$0022,$0025,$0025,$002C

*****************************************************************************
* Resize and/or move menus
*****************************************************************************
  
  *****************************************************************************
  * Title screen "MESSAGE" menu
  *****************************************************************************
  
  org $4436
  move.w      #$C,D1        * width
  move.w      #$8,D2        * height
  
  *****************************************************************************
  * Stroke Play menu
  *****************************************************************************
  
  org $1A71E
  moveq       #$10,D1       * width
  moveq       #$6,D2        * height
  
  *****************************************************************************
  * Skill menu -- resize from 0xf * 0x14 to 0xf * 0x15 and move name printing
  * down a row so we can use two lines for each skill
  *****************************************************************************

    ***************************************
    * Two-line version (old)
    ***************************************
    
    org $C72E
*    move.w      #$13A,D7      * total bytes in background tilemap, minus one
    
    * size when opening menu
*    org $C80C
*    move.w      #$14,$FFB89E  * height, minus one
*    move.w      #$E,$FFB8A0   * width, minus one
*    lea         $C29C,A0      * base position in VRAM
    
    * size when closing menu
*    org $DA20
*    move.w      #$14,$FFB89E  * height, minus one
*    move.w      #$E,$FFB8A0   * width, minus one
*    move.w      #$FFE2,$FFB8C2  * backpedal amount per row
*    lea         $CC9C,A1      * "one past" last tile in VRAM
    
    * start drawing options one line lower than normal
*    org $C744
*    lea         $FF0422,A1    * target position in RAM buffer

    ***************************************
    * One-line version (new)
    ***************************************
    
    org $C72E
    move.w      #$371,D7      * total bytes in background tilemap, minus one
    
    * size when opening menu
    org $C80C
    move.w      #$13,$FFB89E  * height, minus one
    move.w      #$14,$FFB8A0  * width, minus one
    lea         $C292,A0      * base position in VRAM
    
    * size when closing menu
    org $DA20
    move.w      #$13,$FFB89E    * height, minus one
    move.w      #$14,$FFB8A0    * width, minus one
    move.w      #$FFD6,$FFB8C2  * backpedal amount per row (-(width * 2))
    lea         $CC12,A1        * "one past" last tile in VRAM
    
    org $DA4A
    lea         -$2A(A0),A0     * starting offset from end of data -(width * 2)
    
    * cursor VRAM position
    org $D902
    lea         $C314,A6      * when blanking old cursor
    org $D97A
    lea         $C314,A6      * when drawing new cursor
    
    * position to redraw first digit of remaining sp
    org $D8D4
    move.l      #$432E0003,D0
    
    * start drawing options at correct position
    org $C744
    lea         $FF0404,A1    * target position in RAM buffer
    
    * draw skill names/costs in correct places
    
    * advance width per row (width * 2)
    org $C756
    lea         $2A(A1),A1     
    
    * ?
    org $C768
    lea         $FF044C,A0
    
    * move "SP" option
    org $C744
    lea         $FF0404,A1
    
    * spacing of rows
    org $C7A4
    lea         $54(A0),A0
    org $C7E6
    lea         $54(A0),A0
    
    * number of tile ids to copy
    org $C74E
    moveq       #$F,D6
    
    org $C7B8
    lea         $FF042C,A0     * "not enough sp" marker
    org $C800
    lea         $FF0448,A1     * remaining SP
  
  *****************************************************************************
  * Score box
  *****************************************************************************

  * Fully copy opponent's name (7 characters)
  
  org $1B83C
  moveq       #$6,D7
  
  *****************************************************************************
  * Inter-match results screen
  *****************************************************************************
  
    * we need additional space for this one: widen box by a tile
  
    ***********************************
    * the box is copied to RAM so the names and score can be inserted into it;
    * this line determines the number of tile IDs copied, minus one due to
    * being used as an iteration counter.
    * original dimensions are 14*9 = 0x7E; we need 15*9 = 0x87
    ***********************************
    
    org $12578
    move.w      #$86,D7
    
    ***********************************
    * due to widening the box, the hardcoded addresses where the game inserts
    * data are now wrong, so we must adjust them to compensate
    ***********************************
    
    * Winner's name (lines 1-2)
    
    org $125EC
    lea         $FF042E,A3
    lea         $FF044C,A4
    
    * Player's name (lines 4-5)
    
    org $12620
    lea         $FF047A,A3
    lea         $FF0498,A4
    
    * Opponent's name (lines 6-7)
    
    org $1263C
    lea         $FF04B6,A3
    lea         $FF04D4,A4
    
    * Player/opponent's down/up status (lines 5/7)
    
    org $12680
    lea         $FF04AA,A3      * player
    lea         $FF04E6,A4      * opponent
    
    * Player/opponent's score (lines 5/7)
    
    org $126A4
    lea         $FF04A6,A3      * player
    lea         $FF04E2,A4      * opponent
    
    * "EVEN" text that overwrites "WINNER" area if match was a draw (lines 1-2)
    org $126C8
    lea         $1E(A1),A2      * length of a line in bytes
    lea         $FF041E,A3      * line 1 dst
    lea         $FF043C,A4      * line 2 dst
    moveq       #$E,D7          * box width

    ***********************************
    * set new box dimensions for VRAM transfer
    ***********************************
    org $126F2
    moveq       #$E,D1          * width
    moveq       #$8,D2          * height

    ***********************************
    * finally, increase the number of characters copied to each name to 6
    ***********************************
    
    org $125EA
    moveq       #$5,D7      * winner's name
    org $1261E
    moveq       #$5,D7      * player's name
    org $1263A
    moveq       #$5,D7      * opponent's name
  
  *****************************************************************************
  * Opponent select menu
  *****************************************************************************
  
  * Size when opening menu
  org $1A95E
  move.w      #$B,$FFB89E     * height
  move.w      #$8,$FFB8A0     * width
  
*  org $1A986
*  move.w      #$2A,$FF8A0E
  
  * Size when closing menu
  org $1AA54
  move.w      #$B,$FFB89E     * height
  move.w      #$8,$FFB8A0     * width
  move.w      #$FFEE,$FFB8C2  * backpedal amount per row
*  lea         $CA92,A1        * VRAM address of "one past last tile"
  
  * Hardcoded starting backpedal amount
  org $1AA7E
  lea         -$12(A0),A0
  
  *****************************************************************************
  * Golf action menu
  *****************************************************************************
  
  * First, move the score and hole boxes right a tile so we have more room
  
  * values used when opening boxes
  org $C5B0
  lea         $C0AC,A0        * hole infobox tilemap base VRAM address
  org $C5F8
  lea         $C098,A0        * score box tilemap base VRAM address
  org $CA38
  lea         $C098,A0        * match play score box tilemap base VRAM address
  
  * values used when closing boxes
  org $EC5C
  lea         $C418,A0        * "one past last" tile of score boxes (both)
  org $EC8A
  lea         $C32C,A0        * "one past last" tile of hole infobox
  
  * Adjust action menu open width
  org $C62C
  move.w      #$B,$FFB89E     * height
  move.w      #$9,$FFB8A0     * width
  
  * Adjust action menu close width
  org $EC1E
  move.w      #$B,$FFB89E     * height
  move.w      #$9,$FFB8A0     * width
  
*****************************************************************************
* Move dynamically inserted numbers to their new positions within the
* translated text
*****************************************************************************

  *****************************************************************************
  * Caddie message
  * Loaded to FF0400; 0x36 bytes per row
  *****************************************************************************
  
  * Position of course number
  org $19DBC
  lea         $FF044C,A1

  * Position of yard count
  org $19DC8
  lea         $FF0452,A1

  * Position of par number
  org $19DD4
  lea         $FF0466,A1
  
  * "x yards to the pin"
  org $19DE0
  lea         $FF04B8,A1      * buffer position of digits
  
  * Move second part of message down a line
  org $19E1E
  lea         $FF0522,A1
  lea         $FF0558,A2
  lea         $FF058E,A3
  lea         $FF05C4,A4
  
  *****************************************************************************
  * Level up box
  *****************************************************************************
  
  * resize from 19*11 to 20*11
  org $127BC
  move.w        #$DC,D7       * tile count
  org $12822
  moveq         #$13,D1       * width - 1
  moveq         #$A,D2        * height - 1
  
  * move level up number
  org $127D2
  lea           $FF04D6,A1
  
  * move SP gain number to correct position for English text
  org $127EE
  lea           $FF0530,A1
  
  * move SP total
  org $12806
  lea           $FF0570,A1
  
  *****************************************************************************
  * Scorebox on green
  *****************************************************************************
  
  * Use an actual period for the decimal point instead of a handakuten
  org $D674
  move.w        #$A042,(A3)+
  
  * Adjust position of course number
  org $D696
  lea           $FF002A,A1
  
  * Adjust position of yardage count
  org $D6AE
  lea           $FF0030,A1
  
  * Adjust position of yard count
  org $D6AE
  lea           $FF0030,A1
  
  *****************************************************************************
  * Hole infobox
  *****************************************************************************
  
  * Course number
  org $C57C
  lea           $FF0418,A1

*****************************************************************************
* Patch up intro
*****************************************************************************
  
  *********************************
  * Fix bugs
  *********************************
  
  * original game fails to clear text box between "take over the world" and
  * "yui"
  org $308C
  jsr           intrClrfx
  
  *********************************
  * Adjust timing
  *********************************
  
  * "from this day"
  org $28B2
  dc.l          $C60
  * "then you will"
  org $28BA
  dc.l          $D7C
  * "nooo"
  org $28C2
  dc.l          $E6C
  
  * "the exit is over there"
  org $291A
  dc.l          $11D0
  
  * "there's no time"
  org $292A
  dc.l          $12E0
  * "what about ran?"
  org $2932
  dc.l          $1358
  * "what about ran? there was"
  org $293A
  dc.l          $13D4
  * "yes, i know"
  org $2942
  dc.l          $149C
  * "grr-! what are you doing"
  org $294A
  dc.l          $1580
  * gunshots + fadeout
  org $2952
  dc.l          $1644
  * ?
  org $295A
  dc.l          $14AA
  * ?
  org $2962
  dc.l          $14CA
  * ?
  org $296A
  dc.l          $1600
  
  * "and just who was"
  org $2992
  dc.l          $17F0
  * "... a match play"
  org $299A
  dc.l          $18BC
  * "at ghyll"
  org $29A2
  dc.l          $1960
  * "Hazzard!"
  org $29AA
  dc.l          $19E0
  * border swipe
  org $29B2
  dc.l          $1A40
  * "if i enter this"
  org $29BA
  dc.l          $1A78
  * "ghyll country club"
  org $29C2
  dc.l          $1B28
  * "yui, on her own"
  org $29CA
  dc.l          $1BD4
  * end?
  org $29D2
  dc.l          $1C80
  

*****************************************************************************
* Expand flames on title screen
*****************************************************************************

  * move "yui" tilemap left a tile and expand its dimensions
  org $1A94
  move.l        #$412E0003,D0   * VDP target
  move.w        #$9,D1          * width - 1
  move.w        #$6,D2          * height - 1

  * load more tiles (this is the byte count / 2)
  org $1988
  move.w        #$14FF,D7

  * Change load address of title screen tiles so we have more space
  org $1978
  move.l        #$76000000,$C00004   * target = VRAM 3600
  
  * change targets for half-width tilemaps
  org $1AA2
  move.w        #$4240,D3
  org $1AC0
  move.w        #$4240,D3
  org $1B70
  move.w        #$4240,D3
  org $1A2A
  move.w        #$21B0,D3
  org $1A48
  move.w        #$21B0,D3
  org $1A66
  move.w        #$21B0,D3
  org $1A84
  move.w        #$21B0,D3
  
*  org $2BE8
*  move.w        #$21B0,D3
*  org $2E42
*  move.w        #$21B0,D3
*  org $2F88
*  move.w        #$21B0,D3
*  org $2FDC
*  move.w        #$21B0,D3
*  org $3000
*  move.w        #$21B0,D3
*  org $3116
*  move.w        #$21B0,D3
*  org $329C
*  move.w        #$21B0,D3
*  org $3330
*  move.w        #$21B0,D3
*  org $3358
*  move.w        #$21B0,D3
*  org $3380
*  move.w        #$21B0,D3
*  org $33A8
*  move.w        #$21B0,D3
*  org $33D0
*  move.w        #$21B0,D3
*  org $340E
*  move.w        #$21B0,D3
*  org $3814
*  move.w        #$21B0,D3
*  org $3B7A
*  move.w        #$21B0,D3
*  org $3CF8
*  move.w        #$21B0,D3
*  org $5C5E
*  move.w        #$21B0,D3
*  org $5F58
*  move.w        #$21B0,D3

  * Initial load
  org $1AB2
  move.l        #$622E0003,D0   * VDP target
  move.w        #$9,D1          * width - 1
  move.w        #$4,D2          * height - 1

  * Animation
  org $1B64
  move.w        #$E22E,A2       * VRAM target
  move.w        #$A,D1          * width
  move.w        #$5,D2          * height

*****************************************************************************
* Modify keypad minigame tilemap dimensions/positions
*****************************************************************************

  * line 1
  org $8EBA
  move.l        #$4A0A0003,D0   ; VDP target
  move.w        #$12,D1         ; width - 1
  move.w        #$1,D2          ; height - 1
  
  * line 2
  org $8ED8
  move.l        #$4B0A0003,D0   ; VDP target
  move.w        #$12,D1         ; width - 1
  move.w        #$1,D2          ; height - 1
  
  * line 3
  org $8EF6
  move.l        #$4C0A0003,D0   ; VDP target
  move.w        #$12,D1         ; width - 1
  move.w        #$1,D2          ; height - 1
  
  * "wrong keyword" message
  org $92E4
  move.l        #$490C0003,D0   ; VDP target
  move.w        #$10,D1         ; width - 1
  move.w        #$1,D2          ; height - 1

*****************************************************************************
* Fix probable script bug: Using "Check" on Doctor T after he reveals his
* identity plays message 0x22F, which is actually dialogue from a later
* conversation with Professor G. The intended message was probably 0x21F.
*****************************************************************************
  
  org $65348
  dc.w          $021F

*****************************************************************************
* Inject splash screen
*****************************************************************************

splashShown   equ   $FF80FF   * flag set after showing splash screen
                              * so we know not to show it on soft reset

  * Jump to extra init code for Sega logo screen
  org $1140
  jmp           splashScreenInit
  
  * Jump to alternate code for splash screen on Sega logo screen
  org $1176
  jmp splashScreenUpdateCheck

*****************************************************************************
* Extra code
*****************************************************************************

  org $FF000
  
* Offsets to various stuff. These are filled in when the ROM is built.
splashTilemapAddr:  ds.l  $1

* Existing assets
splashFontAddr      equ   $229A2

* Jump targets/routines
splashNoInitRet         equ   $1146
decmp1bppGrp            equ   $A0A
printHalfWidthTilemap   equ   $726
  
* Clear text during part of the intro
intrClrfx:
  jsr           $3EDA     * clear text area
  jsr           $B5FC     * make up work
  rts
  
splashScreenInit:

  tst.b         splashShown   * branch if splash screen already shown
  bne           splashScreenInitDone
  
    * set palette (same as Sega logo)
    lea         $18D0,A0
    jsr         $D48
    
    * Load game font to VRAM 0000 (background color = 0 = shows as black)
    moveq       #$0,D0
    lea         $229A2,A1
    move.l      #$40000000,$C00004
    move.w      #$507,D6
    jsr         decmp1bppGrp
    
    * Load splash screen tilemap
    move.l      (splashTilemapAddr),A1  * source address
    move.l      #$40000003,D0   * target = VRAM C000
    move.l      #39,D1          * width - 1
    move.l      #27,D2           * height - 1
    move.l      #$8000,D3       * base tile identifier
    jsr         printHalfWidthTilemap
    
    * finish up
    jsr         $806
    jsr         $850
    move        #$2300,SR
    rts
  
  splashScreenInitDone:
  * do makeup work and resume normal init sequence
  lea           $18D0,A0
  jmp           splashNoInitRet

splashScreenUpdateCheck:

noSplashUpdateJmp   equ   $117C
splashScreenTimer   equ   $FF8036
fadeToScene         equ   $10AE
pad1_pressed        equ   $FF8024
kSplashScreenLength equ   900
  
  tst.b         splashShown   * branch if splash screen not yet shown
  beq           splashScreenUpdate
  
  * splash screen already shown: do the regular Sega logo update
  addq.w        #1,$FF8036    * do makeup work
  jmp           noSplashUpdateJmp

splashScreenUpdate:
  
  addq.w        #1,splashScreenTimer      * increment timer
  move.w        splashScreenTimer,D3
  
  cmpi.w        #kSplashScreenLength,D3   * branch if screen has timed out
  bcc           endSplashScreen
  
  btst          #$7,pad1_pressed          * branch if start button pressed
  bne           endSplashScreen
  
  splashScreenUpdateDone:
  * do nothing while we wait for the screen to time out
  rts
  
  endSplashScreen:
  move.b        #$FF,splashShown  * mark splash screen as shown
  moveq         #0,D0             * fade out and start scene 0 (Sega logo)
  jmp           fadeToScene

