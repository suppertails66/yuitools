#define FONT_MSB


// UTF-8 file
int utf_8;

// normal font
unsigned char table_00[256*256][40];


void Load_Table( int table_num, char *table_file )
{
	FILE *table;

	table = fopen( table_file, "r" );
	if( !table )
	{
		printf( "\nTable file not found: %s", table_file );
		return;
	}

	// init
	memset( table_00, 0, sizeof(table_00) );

	// check UTF-8
	if( fgetc(table) == 0xEF )
	{
		if( fgetc(table) == 0xBB )
		{
			if( fgetc(table) == 0xBF )
			{
				utf_8 = 1;
			}
			else
			{
				fseek( table, ftell(table)-3, SEEK_SET );
			}
		}
		else
		{
			fseek( table, ftell(table)-2, SEEK_SET );
		}
	}
	else
	{
		fseek( table, ftell(table)-1, SEEK_SET );
	}

	// -------------------------------------------------------------

	int byte;

	while(1)
	{
		char entry[256];
		int lcv, w_lcv;

		byte = fgetc( table );
		fseek( table, ftell(table)-1, SEEK_SET );

		if( byte == EOF ) break;

		// valid hex byte for line
		if( (byte>='0' && byte<='9') || (byte>='a' && byte<='z') || (byte>='A' && byte<='Z') )
		{
			fgets( entry, 256, table );
			w_lcv = 0;

			// select table format
			if( entry[2] == '=' )
			{
				sscanf( entry, "%02X=", &byte );
				lcv = 2+1;
			}
			else if( entry[4] == '=' )
			{
				sscanf( entry, "%04X=", &byte );
				lcv = 4+1;
			}
			else if( entry[6] == '=' )
			{
				sscanf( entry, "%06X=", &byte );
				lcv = 6+1;
			}
			else if( entry[8] == '=' )
			{
				sscanf( entry, "%08X=", &byte );
				lcv = 8+1;
			}

			switch( table_num )
			{
#define READ_TABLE(num) \
	while( entry[lcv]!=0x0d && entry[lcv]!=0x0a ) \
	{ \
		table_##num##[ byte ][w_lcv] = entry[lcv]; \
		lcv++; \
		w_lcv++; \
	} \
	table_##num##[ byte ][w_lcv] = 0;

			case -1:
				break;

			// all
			default:
				READ_TABLE(00);
				break;
			}
		}
		else
		{
			// eat line
			fgets( entry, 256, table );
		}
	}

	// -------------------------------------------------------------

	fclose( table );
}


void Print_Font( int table_num, int tile, int code )
{
	tile &= 0xffff;
	
	switch( table_num )
	{
	// font
	case 0x00:
		if( table_00[tile][0] == 0 )
		{
			fprintf( out_text,"<$%02X>", (code&0xff) );
			return;
		}
		break;

	// ----------------------------------------------------------

	// kanji
	case 0x100:
		// FE17
		if( table_00[tile][0] != 0 )
		{
			fprintf( out_text,"%s", table_00[tile] );
			return;
		}

		// FExx
		/*
		if( table_00[tile>>8][0] != 0 )
		{
			fprintf( out_text,"%s<$%02X>", table_00[tile>>8], tile&0xff );
			return;
		}
		*/

#ifdef FONT_MSB
		fprintf( out_text,"<$%02X><$%02X>", (code>>8)&0xff, (code>>0)&0xff );
#else
		fprintf( out_text,"<$%02X><$%02X>", (code>>0)&0xff, (code>>8)&0xff );
#endif
		return;
	}

	fprintf( out_text,"%s", table_00[tile] );
}


void Print_Code( int table_num, int tile, int code )
{
	tile &= 0xffff;
	
	switch( table_num )
	{
	// single-byte codes
	case 0x00:
		if( table_00[tile][0] == 0 )
		{
			fprintf( out_text,"<CODE %02X>", (code&0xff) );
			return;
		}

		fprintf( out_text,"%s", table_00[tile] );
		break;

	// ----------------------------------------------------------

	// multi-byte codes
	case 0x100:
		// FE17
		if( table_00[tile][0] != 0 )
		{
			fprintf( out_text,"%s", table_00[tile] );
			break;
		}

		// FExx
		if( table_00[tile>>8][0] != 0 )
		{
			fprintf( out_text,"%s<$%02X>", table_00[tile>>8], tile&0xff );
			break;
		}

		fprintf( out_text,"<CODE $%02X><$%02X>", (code>>8)&0xff, (code>>0)&0xff );
		return;
	}
}


void Print_Data( int table_num, int tile, int code )
{
	tile &= 0xffff;
	
	switch( table_num )
	{
	// single-byte codes
	case 0x00:
		fprintf( out_text,"<$%02X>", (code&0xff) );
		break;

	// ----------------------------------------------------------
	// ----------------------------------------------------------

	// multi-byte codes
	case 0x100:
#ifdef FONT_MSB
		fprintf( out_text,"<$%02X><$%02X>", (code>>8)&0xff, (code>>0)&0xff );
#else
		fprintf( out_text,"<$%02X><$%02X>", (code>>0)&0xff, (code>>8)&0xff );
#endif
		break;

	// ----------------------------------------------------------
	// ----------------------------------------------------------

	// multi-byte codes
	case 0x10000:
#ifdef FONT_MSB
		fprintf( out_text,"<$%02X><$%02X><$%02X>", (code>>16)&0xff, (code>>8)&0xff, (code>>0)&0xff );
#else
		fprintf( out_text,"<$%02X><$%02X><$%02X>", (code>>0)&0xff, (code>>8)&0xff, (code>>16)&0xff );
#endif
		break;
	}
}


#undef FONT_MSB