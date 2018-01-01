int mode;

int nybble_count;
int nybble_barrel;

int Read_Nybble()
{
	// reload barrel
	if( nybble_count==0 )
	{
		nybble_barrel = Read_Byte(1);
		nybble_count = 8;
	}

	int code;
	code = (nybble_barrel&0xf0) >> 4;
	
	nybble_count -= 4;
	nybble_barrel <<= 4;

	return code;
}

int nybble_count_out;
int nybble_barrel_out;

void Write_Nybble( int data )
{
	nybble_barrel_out <<= 4;
	nybble_barrel_out |= (data & 0x0f);
	nybble_count_out += 4;
	
	// flush barrel
	if( nybble_count_out == 8 )
	{
		Write_MSB8( nybble_barrel_out, out );
		nybble_count_out = 0;
	}
}

// =====================================================================
// *********************************************************************
// *********************************************************************
// =====================================================================

void Decode_RLE()
{
	while(1)
	{
		int run, rle;
		int code;

		code = Read_Nybble();
		if( code == 0x00 ) break;

		// ---------------------------------------------------------------

		// RLE 4-bpp
		if( code < 0x08 )
		{
			run = code+2;
			rle = Read_Nybble();
			while( run >=0 )
			{
				Write_Nybble( rle );
				run--;
			}

			continue;
		}

		// ---------------------------------------------------------------

		// RAW 4-bpp
		run = code-8;
		while( run>=0 )
		{
			code = Read_Nybble();

			Write_Nybble( code );
			run--;
		}
		continue;
	}

	//fwrite( buffer,1,buffer_ptr,out );
}


void Decode_Start( int start, char *name )
{
	int tile_adjust;
	int input_length;
	int output_length;

	CACHE_BUFFER( start );

	// -----------------------------------------------------------------
	// -----------------------------------------------------------------

	// header data
	//tile_adjust = Read_Word(1);
	//input_length = Read_Word(1);
	//output_length = Read_Word(1);

	//Write_MSB16( tile_adjust,out );
	//Write_MSB16( input_length,out );
	//Write_MSB16( output_length,out );

	Decode_RLE();

	// WORD alignment (input)
	if( (Tell_Cache()%2)==1 )
	{
		//Read_Byte(1);
	}

	// -----------------------------------------------------------------
	// -----------------------------------------------------------------

	printf( "\n[%s] %06X-%06X = %X",
		name, start, Tell_Cache(), Tell_Cache()-start );
}
