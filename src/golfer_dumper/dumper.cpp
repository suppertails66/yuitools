#include <stdio.h>
#include <string.h>
#include <deque>

using namespace std;

// =========================================================================
// #########################################################################
// =========================================================================

char line[1024];

int last_ptr;

FILE *out_text;
FILE *rom;

#include "table.cpp"
#include "disk.cpp"
#include "scan.cpp"
#include "find.cpp"

// =========================================================================
// #########################################################################
// =========================================================================

int main( int argc, char** argv )
{
	if( argc<2 ) { fprintf( stderr, "ERROR - no ROM args\n" ); getc(stdin); return -1; }

	// ============================================================
	// ************************************************************
	// ************************************************************
	// ============================================================

	if( strcmp(argv[1],"DUMP_TEXT") == 0 )
	{
		char out_name[1024];

		rom = fopen( argv[2], "rb" );
		if( !rom ) { fprintf( stderr, "ERROR - no ROM %s\n", argv[2] ); getc(stdin); return -1; }

		// ============================================================
		// ############################################################
		// ============================================================

		Load_Table( 0x00, "table//table_dump.txt" );

		sprintf( out_name, "text//script.txt" );
		out_text = fopen( out_name, "w" );
		if( !out_text ) { fprintf( stderr, "ERROR - no TEXT %s\n", out_name ); getc(stdin); return -1; }

		// Atlas header
		fprintf( out_text, "#VAR(Table, TABLE)\n" );
		fprintf( out_text, "#ADDTBL(\"table//golfer_table.txt\", Table)\n" );
		fprintf( out_text, "#ACTIVETBL(Table)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#SMA(\"MSB32\")\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#EMBTYPE(\"MSB32\",32,$0)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#VAR(MyPtr, CUSTOMPOINTER)\n" );
		fprintf( out_text, "#VAR(PtrTable, POINTERTABLE)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#CREATEPTR(MyPtr, \"MSB16\", $-80000, 16)\n" );
		//fprintf( out_text, "#PTRTBL(PtrTable, $80000, 2, MyPtr)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "//---------------------------------------------------------\n" );
		fprintf( out_text, "//---------------------------------------------------------\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#SETINDEX($0000,2)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#JMP($FFFFF)\n" );
		fprintf( out_text, "<$00>\n" );
		fprintf( out_text, "\n" );
		//fprintf( out_text, "#JMP($8034C)\n" );
		//fprintf( out_text, "\n" );

		// Gather ptrs
		Cache_Buffer_Full( 0,rom );
		Init_Dumper();

		//Find_Text_1();
		//Find_Text_2();

		Dump_Layer1();
	}

	// ============================================================
	// ************************************************************
	// ************************************************************
	// ============================================================

	if( strcmp(argv[1],"DUMP_STRING") == 0 )
	{
		/*
		char out_name[1024];

		// -----------------------------------------------------------

		Load_Table( 0x00, "table//table_demon.txt" );

		sprintf( out_name, argv[3] );
		out_text = fopen( out_name, "w" );
		if( !out_text ) { fprintf( stderr, "ERROR - no TEXT dir\n" ); return -1; }

		// Atlas header
		fprintf( out_text, "#VAR(Table, TABLE)\n" );
		fprintf( out_text, "#ADDTBL(\"bible3_demon.txt\", Table)\n" );
		fprintf( out_text, "#ACTIVETBL(Table)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#SMA(\"HIROM\")\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#VAR(MyPtr, CUSTOMPOINTER)\n" );
		fprintf( out_text, "#VAR(PtrTable, POINTERTABLE)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "#CREATEPTR(MyPtr, \"HIROM\", $0, 24)\n" );
		fprintf( out_text, "#PTRTBL(PtrTable, $300000, 3, MyPtr)\n" );
		fprintf( out_text, "\n" );
		fprintf( out_text, "//---------------------------------------------------------\n" );
		fprintf( out_text, "//---------------------------------------------------------\n" );
		fprintf( out_text, "\n" );

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		int bank, offset;
		char *table;

		sscanf( argv[4], "%x", &bank );
		sscanf( argv[5], "%x", &offset );
		table = argv[6];

		// Use INDEX table
		Demon_Layer2( bank, offset, -1, -1, table );

		if(out_text) fclose(out_text);
		*/
	}

	// ============================================================
	// ************************************************************
	// ************************************************************
	// ============================================================

	if(rom) fclose(rom);
	if(out_text) fclose(out_text);

	return 0;
}