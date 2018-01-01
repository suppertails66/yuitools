/*
Sylvan Tale: LZ compression
*/

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <vector>
#include <deque>

using namespace std;

typedef struct
{
	int pos;
	int ptr;
	int len;
	int type;
} lz_find;


//#define DEBUG_LZ

short buf_ptr;
unsigned char out_buffer[0x10000];

///////////////////////////////////////////////////////

vector<int> table[256];
deque<lz_find> lz_table;

int ptr = 0;
int size = 0;

int max_runs = 0;
int max_delta = 0;

int RLE_min_match1 = 3+1;
int RLE_max_match1 = 9+1;					// x-bits


void Find_RLE1( int start, unsigned char byte, int &length, int &pos )
{
	int longest_length = 0;
	int longest_ptr = 0;

	// straight RLE
	int match_length = 1;

	// search for longest identical substring
	for( int lcv2 = start+1; ( lcv2 < start+RLE_max_match1 ) && ( lcv2<size ); lcv2++ )
	{
		// look for a mismatch
		if( buffer[ lcv2 ] != buffer[ lcv2-1 ] )
			break;

		// keep looking
		match_length++;
	}

	// output findings
	length = match_length;
	pos = start;
}


void Start_Encode( FILE *in, FILE *out )
{
	lz_find lz;
	int start;
	int lcv;
	int counter = 0;
	int real_size;

	buffer = buffer_full;

	// Step 0: Check file size
	fread( buffer, 1, 0x10000, in );
	size = ftell( in );
	fseek( in, 0, SEEK_SET );

	
	// SPECIAL-CASE: Battle Golfer Yui 4-bpp
	for( lcv=0; lcv<size; lcv++ )
	{
		int code = fgetc(in);

		buffer[ lcv*2+0 ] = (code&0xf0) >> 4;
		buffer[ lcv*2+1 ] = (code&0x0f) >> 0;
	}
	fseek( in, 0, SEEK_SET );
	size <<= 1;


	// init
	for( lcv=0; lcv<256; lcv++ )
		table[lcv].clear();
	lz_table.clear();

	// ============================================================
	// ############################################################
	// ============================================================

	// Step 1: Find all LZ matches
	start = 0;
	while( start < size )
	{
		int length1,length2,length3;
		int pos1,pos2,pos3;
		int length, pos, type;

		// Prepare for LZ
		table[ buffer[ start ] ].push_back( start );

		// Go find the longest substring match (and future ones)
		Find_RLE1( start, buffer[ start ], length, pos );

		// Sort weights
		type = 0;
		if( length>=RLE_min_match1 ) { type=1; }

		// ------------------------------------------------------------
		// ------------------------------------------------------------

		if( type>0 )
		{
			// Found substring match; record and re-do
			lz.pos = start;
			lz.ptr = start - pos;
			lz.len = length;
			lz.type = type;

			lz_table.push_back( lz );

			// Need to add to LZ table
			for( int lcv = 1; lcv < length; lcv++ )
				table[ buffer[ start + lcv ] ].push_back( start + lcv );

			// Fast update
			start += length;
		}
		else
		{
			// No substrings found; try again
			start++;
		}
	}

	// insert dummy entry
	lz.pos = size;
	lz_table.push_back( lz );

	// ============================================================
	// ############################################################
	// ============================================================

	int lz_ptr;
	int out_byte;
	int out_bits;
	int out_ptr;
	int out_size;

	// init
	lz_ptr = 0;
	start = 0;
	out_ptr = 0;
	out_byte = 0;
	out_bits = 8;
	out_size = 0;

	// Step 2: Prepare encoding methods
	while( start < size )
	{
		lz = lz_table[ lz_ptr ];

		if( lz.pos == start )
		{
			if( lz.type==1 )
			{
				// RLE 4-bpp
				//out_buffer[ out_ptr++ ] = buffer[start];
				//out_buffer[ out_ptr++ ] = 0xff-buffer[start];
				//out_buffer[ out_ptr++ ] = ((lz.len-0)&0xff) + 0x00;

				Write_Nybble( lz.len-1-2 );
				Write_Nybble( buffer[start] );
			}

			start += lz.len;
			lz_ptr++;
		}
		else
		{
			// Free byte 4-bpp
			while( start < lz.pos )
			{
				int len;

				// saturation
				len = lz.pos-start;
				if( len>8 ) len=8;

				Write_Nybble( len-1+8 );
				while( len-- )
				{
					//out_buffer[ out_ptr++ ] = buffer[start++];
					Write_Nybble( buffer[ start++ ] );
				}
			}
		}

		// flush out data
		//fwrite( out_buffer, 1, out_ptr, out );
		//out_size += out_ptr;
		//out_ptr = 0;
	} // end method check

	// --------------------------------------------------------------------
	// --------------------------------------------------------------------

	int value;

	// flush out data
	//fwrite( out_buffer, 1, out_ptr, out );
	//out_size += out_ptr;


	// STOP code
	Write_Nybble(0);


	// Step 2.5:
	// - WORD alignment
	//if( (ftell(out)%2)==1 ) fputc( 0,out );
	if( nybble_count_out<8 ) Write_Nybble(0);
}
