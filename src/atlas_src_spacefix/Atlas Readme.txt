Atlas v1.06
By Klarth (stevemonaco@hotmail.com)
http://rpgd.emulationworld.com/klarth/

I.    Overview
II.   Command Line Usage
III.  Function Reference
IV.   Table Reference
V.    Pascal Strings
VI.   Pointer List Files
VII.  Extensions
VIII. Thanks Section



I. Overview
     Atlas was created originally as a script insertion tool to deal with complicated, but
uncompressed videogame scripts.  Prior, there was not a utility that could handle this.  With
the release of Atlas 1.0 this has changed.  Now there is finally a dedicated, extensible tool
that can insert scripts of nearly any complexity.



II. Command Line Usage

     Atlas [switches] TargetFile.ext ScriptFile.txt
     
     Valid switches
         -d filename - Activates debugging output to filename.  If "stdout" is used, then the
                       output is displayed in the console



III. Function Reference

     To see examples of how to use these functions, check Atlas How-To.txt

-----------------------------------------------------------------------------------------
|
|    Common Functions

     1. JMP(unsigned int Address)
          Changes the current position of the TargetFile
          
             Address - New file position

     ------------------------------------------------------------------------------------

     2. JMP(unsigned int Address, unsigned int MaxAddress)
          Modifies the current position of the TargetFile and sets a text bound
          
             Address    - New file position
             MaxAddress - Uppermost address for script insertion

     ------------------------------------------------------------------------------------

     3. HDR(unsigned int HeaderSize)
          Modifies the header size for the TargetFile.  This does not affect JMP nor
          pointer positions for the pointer writing functions.
          
             HeaderSize - New size for the header

     ------------------------------------------------------------------------------------

     4. ADDTBL(String TableName, TABLE TblId)
          Opens, parses, and loads a table file.  Activate the newly loaded table with
          ACTIVETBL.

             TableName - The filename of the table
             TblId     - Variable to hold the table

     ------------------------------------------------------------------------------------

     5. ACTIVETBL(TABLE TblId)
          Switches the current table being used to table TblId.

             TblId - TABLE variable initialized with ADDTBL

     ------------------------------------------------------------------------------------

     6. SMA(String Type)
          Changes the Machine Addressing type for pointers.
          
             Type - New type
               Values:  "LINEAR", "LOROM00", "LOROM80", "HIROM", "GB"

     ------------------------------------------------------------------------------------

     7. STRTYPE(String StringType)
          Modifies the type of string used in the insertion
          
             StringType - New string type.
               Values:  "ENDTERM", "PASCAL"

     ------------------------------------------------------------------------------------

     8. PASCALLEN(unsigned int NewLength)
          Modifies the length code (in bytes) for Pascal strings, when applicable.
          
             NewLength - The new length.
               Values:  1, 2, 3, 4

     ------------------------------------------------------------------------------------

     9. VAR(Variable VarName, VariableType Type)
          Creates a new variable.
          
             VarName - The name of the variable.
             Type    - Type of the variable.
               Values:  TABLE, POINTERTABLE, POINTERLIST, CUSTOMPOINTER, EXTENSION
          
          Example: Declaring a new TABLE variable
          #VAR(MyTable, TABLE)


-----------------------------------------------------------------------------------------
|
|    Low-Level Pointer Functions

     1.  WUB(unsigned int Address)
           Write Uppermost Byte of current text position to Address

              Address - Address where the pointer will be written

     ------------------------------------------------------------------------------------

     2.  WBB(unsigned int Address)
           Write Bank Byte of current text position to Address

              Address - Address where the pointer will be written

     ------------------------------------------------------------------------------------

     3.  WHB(unsigned int Address)
           Write High Byte of current text position to Address

              Address - Address where the pointer will be written

     ------------------------------------------------------------------------------------

     4.  WLB(unsigned int Address)
           Write Low Byte of current text position to Address

              Address - Address where the pointer will be written

     ------------------------------------------------------------------------------------

     5.  W16(unsigned int Address)
           Writes a 16bit pointer calculated from the current text position,
             Addressing type, and header, to Address
          
              Address - Address where the pointer will be written

     ------------------------------------------------------------------------------------

     6.  W24(unsigned int Address)
           Writes a 24bit pointer calculated from the current text position,
             Addressing type, and header, to Address
          
              Address - Address where the pointer will be written

     ------------------------------------------------------------------------------------

     7.  W32(unsigned int Address)
           Writes a 32bit pointer calculated from the current text position,
             Addressing type, and header, to Address
          
              Address - Address where the pointer will be written

     ------------------------------------------------------------------------------------

     8.  EMBTYPE(String Type, unsigned int Size, int Offsetting)
           Sets embedded pointers to be of Size bits and affected by Offsetting bytes.
 
             Type - New type
               Values:  "LINEAR", "LOROM00", "LOROM80", "HIROM", "GB"
              Size - Size of pointer, in bits.
                Values:  8, 16, 24, 32
              Offsetting - Number of bytes the pointer will be offsetted.
                           Ex. $1000 subtracts $1000 bytes, $-1000 adds $1000 bytes.

     ------------------------------------------------------------------------------------

     9.  EMBSET(unsigned int PointerNumber)
           Sets embedded pointer PointerNumber's physical position to the current text
           position.
          
              PointerNumber - The pointer number modified

     ------------------------------------------------------------------------------------

     10. EMBWRITE(unsigned int PointerNumber)
           Sets the embedded pointer PointerNumber's point-to position to the current
             text position.
           
              PointerNumber - The pointer number modified.


------------------------------------------------------------------------------------------
|
|    High-Level Pointer Functions
     
     1.  CREATEPTR(CUSTOMPOINTER Name, String AddressType, int Offsetting, unsigned int PointerSize)
            Creates a CUSTOMPOINTER
           
               Name        - Name of the CUSTOMPOINTER
               AddressType - Address type of the pointer, same as SMA.
               Offsetting  - How the pointer is offsetted.  Positive amounts subtract, negative add.
               PointerSize - Size of the pointer
                 Values: 8, 16, 24, 32

     ------------------------------------------------------------------------------------

     2.  WRITE(CUSTOMPOINTER Ptr, unsigned int Address)
            Writes a CUSTOMPOINTER to Address.

               Ptr     - Name of the CUSTOMPOINTER
               Address - Physical location to write the pointer at

     ------------------------------------------------------------------------------------
  
     3.  PTRLIST(POINTERLIST List, String Filename, CUSTOMPOINTER Ptr)
            Creates a POINTERLIST from file Filename and Ptr.

               List  - Name of the POINTERLIST
               Filename - Name of the file containing the offsets
               Ptr      - Name of the custom pointer to use

     ------------------------------------------------------------------------------------

     4.  WRITE(POINTERLIST List)
            Writes the current position pointer to the next entry in List

               List - Name of the POINTERLIST

     ------------------------------------------------------------------------------------

     5.  PTRTBL(POINTERTABLE Tbl, unsigned int Start, unsigned int Offsetting, CUSTOMPOINTER Ptr)
            Creates a POINTERTABLE starting at Start, increasing Offsetting bytes each write,
            using Ptr.

               Tbl        - Name of the POINTERTABLE
               Start      - Beginning of the pointer table
               Offsetting - Number of bytes to advance after each write
               Ptr        - Name of the CUSTOMPOINTER

     ------------------------------------------------------------------------------------

     6.  WRITE(POINTERTABLE Tbl)
            Writes the next pointer to POINTERTABLE.

               Tbl - Name of the POINTERTABLE to write from

     ------------------------------------------------------------------------------------

     7.  AUTOWRITE(POINTERLIST List, String EndTag)
            Writes List everytime token EndTag is encountered.  EndTag must be defined in the table
            as a string end tag for this to work.

               List   - Name of the POINTERLIST to use.  Only one POINTERLIST per unique EndTag supported
               EndTag - Name of an end token used in the table.  Triggers the write.

     ------------------------------------------------------------------------------------

     8.  AUTOWRITE(POINTERTABLE Tbl, String EndTag)
            Writes Tbl everytime token EndTag is encountered.  EndTag must be defined in the table
            as a string end tag for this to work.

               Tbl    - Name of the POINTERTABLE to use.  Only one POINTERTABLE per unique EndTag supported
               EndTag - Name of an end token used in the table.  Triggers the write.

     ------------------------------------------------------------------------------------

     9.  DISABLE(POINTERLIST List, String EndTag)
            Disables AUTOWRITE for List and EndTag.

               List   - Name of the POINTERLIST
               EndTag - Name of the end token in the table.

     ------------------------------------------------------------------------------------

     10. DISABLE (POINTERTABLE Tbl, String EndTag)
            Disables AUTOWRITE for Tbl and EndTag.

               List   - Name of the POINTERLIST
               EndTag - Name of the end token in the table

------------------------------------------------------------------------------------------
|
|    Extension Functions

     1. EXTLOAD(EXTENSION Ext, String filename)
           Loads an extension from file filename.  Only DLLs supported.

               Ext      - Name of the EXTENSION
               filename - Filename of the extension to load, must be a DLL.

     ------------------------------------------------------------------------------------

     2. EXTEXEC(EXTENSION Ext, String function)
           Executes function from Ext.

               Ext      - The EXTENSION to use
               function - The name of the function to execute
            
     ------------------------------------------------------------------------------------

     3. AUTOEXEC(EXTENSION Ext, String function, String EndTag)
           Executes function every time EndTag is encountered.  EndTag must be defined in the table
           as a string end tag for this to work.  Only one function per EndTag supported

               Ext      - The EXTENSION to use
               function - The name of the function to execute
               EndTag   - Name of the end toke in the table

     ------------------------------------------------------------------------------------

     4. DISABLE(String function, String EndTag)
           Disables AUTOEXEC for function and EndTag.

               function - The function name associated with the AUTOWRITE
               EndTag   - Name of the end token in the table
 
 
 
 IV. Table Reference
 
      Atlas supports run-of-the-mill Thingy tables with one slight extension for end string tags.
      The original syntax is "/FF", making FF an end value.  Atlas is like this:
      /FF=<end>
      Or, for blank strings:
      /<end>
      You must define end strings like this or the AUTOWRITE command will not function.



  V. Pascal Strings

      Ever see strings like this: 05Sword
      where 05 is the count of characters of the string?  If so, you have just seen a pascal string.

      Currently, pascal string support has not been tested but should work.  To enable support you
      must add an end string tag to the table.  Then you must call the Atlas commands:

      #STRTYPE(PASCAL)
      #PASCALLEN(1)

      That's pretty much all there is to it.



 VI. Pointer List Files

      Pointer list files are simple .txt files with one integer on each line, like so:

        $40000
        $40002
        $40006
        $41000

VII. Extensions

      First, create a barebone dll with your compiler.  Then #include "Atlas.h" and the function
      declaration is like so:

      __declspec(dllexport) unsigned int MyFunction(AtlasContext** Context)

      Return codes are as follows:
      NO_ACTION    : Does nothing
      REPLACE_TEXT : Replace's Atlas's internal text buffer with .StringTable
      WRITEPOINTER : Writes a pointer of .PointerSize bytes at location .PointerPosition with .PointerValue

      Unfortunately, it's difficult to get Atlas to do block-type compression...if you do, then you need to
      manage the text buffer yourself (clear it) and manage your own pointers.  This will be fixed if I
      release a feature update to Atlas.



VIII. Thanks Section

       Well, what document is complete without a thanks section?  Not one of mine!

       Gideon Zhi - For coming up with the basic idea and testing.  Oh, and not killing me everytime I sent
                    him a quick fix that didn't work.

       Flobbster/Yousei - Helped me numerous times in the past and even now with coding.

       Inverse and Neil_ - For romhacking.com