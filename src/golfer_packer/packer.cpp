#include <stdio.h>
#include <string.h>
#include <deque>

using std::deque;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

char line[1024];

int last_ptr;

FILE *rom, *out, *out_text;

#include "disk.cpp"
#include "decode.cpp"
#include "encode.cpp"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

int main( int argc, char** argv )
{
	if( argc<3 ) { fprintf( stderr, "ERROR - no ROM args\n" ); getc(stdin); return -1; }

	// ============================================================
	// ************************************************************
	// ************************************************************
	// ============================================================

	if( strcmp(argv[1],"UNPACK")==0 )
	{
		int start;

		rom = fopen( argv[2], "rb" );
		if( !rom ) { fprintf( stderr, "ERROR - no ROM %s\n", argv[2] ); getc(stdin); return -1; }

		FILE *dump_log = fopen( "file/unpack_list.txt", "r" );
		if( !dump_log ) { fprintf( stderr, "ERROR - no LIST %s\n", "file/unpack_list.txt" ); getc(stdin); return -1; }

		Cache_Buffer_Full( 0,rom );

		// ------------------------------------------------------------
		// ############################################################
		// ------------------------------------------------------------
		
		// parse list
		while(1)
		{
			char line[512];

			// read line
			line[0] = 0;
			fgets( line, 512, dump_log );

			if( line[0]==0 && feof(dump_log) ) break;
			if(line[0]==';') continue;
			if(line[0]==0x0d) continue;
			if(line[0]==0x0a) continue;
			if(line[0]==' ') continue;
			if(line[0]=='/' && line[1]=='/') continue;

			// -------------------------------------------------------------------
			// -------------------------------------------------------------------
			// -------------------------------------------------------------------

			char mode[64];

			sscanf( line, "%X %s", &start, mode );
			out = fopen( mode, "wb" );
			if( !out ) { fprintf( stderr, "ERROR - no OUTPUT %s\n", mode ); getc(stdin); return -1; }

			Decode_Start( start, mode );

			if(out) fclose(out);
		}
	}

	// ==============================================================
	// **************************************************************
	// ==============================================================

	if( strcmp(argv[1],"REPACK_ONLY")==0 )
	{
		int start;

		FILE *dump_log = fopen( "file/repack_list.txt", "r" );
		if( !dump_log ) { fprintf( stderr, "ERROR - no LIST %s\n", "file/repack_list.txt" ); getc(stdin); return -1; }

		// ------------------------------------------------------------
		// ############################################################
		// ------------------------------------------------------------
		
		// parse list
		while(1)
		{
			char line[512];

			// read line
			line[0] = 0;
			fgets( line, 512, dump_log );

			if( line[0]==0 && feof(dump_log) ) break;
			if(line[0]==';') continue;
			if(line[0]==0x0d) continue;
			if(line[0]==0x0a) continue;
			if(line[0]==' ') continue;
			if(line[0]=='/' && line[1]=='/') continue;

			// -------------------------------------------------------------------
			// -------------------------------------------------------------------
			// -------------------------------------------------------------------

			char mode[64], mode2[64];

			sscanf( line, "%X %s %s", &start, mode, mode2 );

			rom = fopen( mode, "rb" );
			if( !rom ) { fprintf( stderr, "ERROR - no INPUT %s\n", mode ); getc(stdin); return -1; }

			out = fopen( mode2, "wb" );
			if( !out ) { fprintf( stderr, "ERROR - no OUTPUT %s\n", mode2 ); getc(stdin); return -1; }

			fprintf( stderr, "\n%s", mode2 );
			Start_Encode( rom,out );

			printf( "\n[%s] %06X-%06X", mode2, start, start+ftell(out) );

			if(out) fclose(out);
			if(rom) fclose(rom);
		}
	}

	// ==============================================================
	// **************************************************************
	// ==============================================================

	if(out) fclose(out);
	if(rom) fclose(rom);

	return 0;
}