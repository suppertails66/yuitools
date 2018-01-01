#define MSB
//#define SJIS
#define READ_BUFFER_CACHE
#define DEBUGGING

//#define CACHE_BUFFER Cache_Buffer_HIROM
//#define CACHE_BUFFER Cache_Buffer_LOROM
#define CACHE_BUFFER Cache_Buffer_Genesis
//#define CACHE_BUFFER Cache_Buffer_Gameboy

//#define ROM_TO_LINEAR LOROM_to_LINEAR
//#define ROM_TO_LINEAR HIROM_to_LINEAR
#define ROM_TO_LINEAR 


int BUFFER_SIZE;

#define BUFFER_MAX 0x10000*0x40			// 4MB
unsigned char *buffer;
unsigned char buffer_full[BUFFER_MAX];
int buffer_ptr;

#define BANK_SIZE 0x10000

int bank_low[0x100];
int bank_high[0x100];

int file_ptr, base, base_ptr;

// ===================================================================
// ###################################################################
// ===================================================================

unsigned int GetMSBAddress(unsigned int Offset, int size)
{
	// 16-bit MSB
	if( size == 16 )
	{
		return
			( ( Offset&0xff00 ) >> 8 ) |
			( ( Offset&0x00ff ) << 8 );
	}

	// 24-bit MSB
	if( size == 24 )
	{
		return
			( ( Offset&0xff0000 ) >> 16 ) |
			( ( Offset&0x00ff00 ) >> 0 ) |
			( ( Offset&0x0000ff ) << 16 );
	}

	// 32-bit MSB
	return
		( ( Offset&0xff000000 ) >> 24 ) |
		( ( Offset&0x00ff0000 ) >> 8 ) |
		( ( Offset&0x0000ff00 ) << 8 ) |
		( ( Offset&0x000000ff ) << 24 );
}


int LOROM_to_LINEAR( int offset )
{
	int addr;

	// LoROM $80 to ROM
	if( offset>=0x800000 )
	{
		offset -= 0x800000;
	}
	if( offset>=0x8000 )
		offset -= 0x8000;

	addr = ((offset>>16)/2) * 0x10000;
	addr += offset&0xffff;
	if( ((offset>>16)%2)==1 ) addr += 0x8000;

	return addr;
}


int HIROM_to_LINEAR( int offset )
{
	int addr;

	if( offset>=0xC00000 ) offset -= 0xC00000;
	if( offset>=0x800000 ) offset -= 0x800000;
	if( offset>=0x400000 ) offset -= 0x400000;

	addr = offset&0x3fffff;
	return addr;
}

// ==================================================================
// ##################################################################
// ==================================================================

unsigned char *save_buffer;
int save_ptr, save_base;

void PUSH_CACHE()
{
	save_buffer = buffer;
	save_ptr = buffer_ptr;
	save_base = base;
}


void POP_CACHE()
{
	buffer = save_buffer;
	buffer_ptr = save_ptr;
	base = save_base;
}


int Tell_Cache_Linear()
{
	return (buffer+buffer_ptr) - buffer_full;
}


int Tell_Cache()
{
	return base+buffer_ptr;
}


void Cache_Buffer( int offset, FILE *fp = rom )
{
	buffer = buffer_full + offset;
	buffer_ptr = 0;
}


void Cache_Buffer_HIROM( int offset, FILE *fp = rom )
{
	base = offset & ~0xffff;
	offset = HIROM_to_LINEAR(offset);

	if( offset>=0x10000 )
	{
		buffer = buffer_full + (offset&~0xffff) - 0x0000;
		buffer_ptr = (offset & 0xffff) + 0x0000;
	}
	else
	{
		buffer = buffer_full + (offset&~0xffff);
		buffer_ptr = (offset & 0xffff);
	}
}


void Cache_Buffer_LOROM( int offset, FILE *fp = rom )
{
	base = offset & ~0xffff;
	offset = LOROM_to_LINEAR(offset);

	if( offset>=0x8000 )
	{
		buffer = buffer_full + (offset&~0x7fff) - 0x8000;
		buffer_ptr = (offset & 0x7fff) + 0x8000;
	}
	else
	{
		buffer = buffer_full + (offset&~0x7fff);
		buffer_ptr = (offset & 0x7fff);
	}
}


void Cache_Buffer_Gameboy( int offset, FILE *fp = rom )
{
	if( offset>=0x4000 )
	{
		buffer = buffer_full + (offset&~0x3fff) - 0x4000;
		base = buffer-buffer_full;

		buffer_ptr = (offset & 0x3fff) + 0x4000;
	}
	else
	{
		buffer = buffer_full + (offset&~0x3fff);
		base = buffer-buffer_full;

		buffer_ptr = (offset & 0x3fff);
	}
}


void Cache_Buffer_Genesis( int offset, FILE *fp = rom )
{
	buffer = buffer_full + (offset&~0xffff) - 0x0000;
	base = buffer-buffer_full;

	buffer_ptr = (offset & 0xffff) + 0x0000;
}


void Cache_Buffer_Full( int offset, FILE *fp )
{
	fseek( fp,offset,SEEK_SET );
	
	BUFFER_SIZE = fread( buffer_full, 1, BUFFER_MAX, fp );

	buffer = buffer_full;
	buffer_ptr = 0;
}


void Bank_Wrap( bool memory_log = false )
{
#if 1
	if( buffer_ptr>=0x10000 )
	{
		// Memory - Genesis
		if(memory_log)
			bank_high[ base/BANK_SIZE ] = base + 0x10000;

		// Genesis
		buffer_ptr -= 0x10000;
		base += 0x10000;
		buffer += 0x10000;

		// Memory - Genesis
		if(memory_log)
			bank_low[ base/BANK_SIZE ] = base + 0x0000;

		fprintf( stderr, "\n[WRAP] %06X", base );
	}

#elif 0
	if( buffer_ptr>=0x10000 )
	{
		// Memory - LoROM
		if(memory_log)
			bank_high[ base/BANK_SIZE ] = base + 0x10000;

		// LoROM
		buffer_ptr -= 0x8000;
		base += 0x10000;
		buffer += 0x8000;

		// Memory - LoROM
		if(memory_log)
			bank_low[ base/BANK_SIZE ] = base + 0x8000;

		fprintf( stderr, "\n[WRAP] %06X", base );
	}

#elif 0
	if( buffer_ptr>=0x10000 )
	{
		// Memory - HiROM
		if(memory_log)
			bank_high[ base/BANK_SIZE ] = base + 0x10000;

		// HiROM
		buffer_ptr -= 0x10000;
		base += 0x10000;
		buffer += 0x10000;

		// Memory - HiROM
		if(memory_log)
			bank_low[ base/BANK_SIZE ] = base + 0x10000;

		fprintf( stderr, "\n[WRAP] %06X", base );
	}

#elif 0
	if( buffer_ptr>=0x8000 )
	{
		// Memory - Gameboy
		if(memory_log)
			bank_high[ (base+buffer_ptr)/BANK_SIZE ] = base + 0x8000;

		// Gameboy
		buffer_ptr -= 0x4000;
		base += 0x4000;
		buffer += 0x4000;

		// Memory - Gameboy
		if(memory_log)
		{
			bank_low[ (base+buffer_ptr)/BANK_SIZE ] = base + 0x4000;

			fprintf( stderr, "\n[WRAP] %06X", base );
		}
	}
#endif
}

// ==================================================================
// ##################################################################
// ==================================================================

unsigned char Peek_Byte( int num, int skip_byte=0 )
{
	int value, ptr;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	ptr = buffer_ptr;
	buffer_ptr += skip_byte;

	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = buffer[ buffer_ptr++ ];
	}

	buffer_ptr = ptr;
#else
	ptr = ftell(rom);
	while( skip_byte-- ) fgetc(rom);

	while( num-- )
	{
		value = fgetc(rom);
	}

	fseek( rom,ptr,SEEK_SET );
#endif

	return value;
}


unsigned char Read_Byte( int num, bool memory_log=true )
{
	int value;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = buffer[ buffer_ptr++ ];
	}
#else
	while( num-- )
	{
		value = fgetc(rom);
	}
#endif

	Bank_Wrap(memory_log);

	return value;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------

unsigned short Read_Word( int num, bool memory_log=true )
{
	int value;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = (buffer[ buffer_ptr++ ])<<0;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<8;
	}
#else
	while( num-- )
	{
		value = fgetc(rom)<<0;
		value |= fgetc(rom)<<8;
	}
#endif

#ifdef MSB
	value = GetMSBAddress( value,16 );
#endif

	Bank_Wrap(memory_log);

	return value;
}


unsigned short Peek_Word( int num, int skip_byte=0 )
{
	int value, ptr;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	ptr = buffer_ptr;
	buffer_ptr += skip_byte;
	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = (buffer[ buffer_ptr++ ])<<0;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<8;
	}
	buffer_ptr = ptr;
#else
	ptr = ftell(rom);
	while( skip_byte-- ) fgetc(rom);
	while( num-- )
	{
		value = fgetc(rom)<<0;
		value |= fgetc(rom)<<8;
	}
	fseek( rom,ptr,SEEK_SET );
#endif

#ifdef MSB
	value = GetMSBAddress( value,16 );
#endif

	return value;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------

unsigned int Read_Tribyte( int num, bool memory_log=true )
{
	int value;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = (buffer[ buffer_ptr++ ])<<0;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<8;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<16;
	}
#else
	while( num-- )
	{
		value = fgetc(rom)<<0;
		value |= fgetc(rom)<<8;
		value |= fgetc(rom)<<16;
	}
#endif

#ifdef MSB
	value = GetMSBAddress( value,24 );
#endif

	Bank_Wrap(memory_log);

	return value;
}


unsigned int Peek_Tribyte( int num, int skip_byte=0 )
{
	int value, ptr;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	ptr = buffer_ptr;
	buffer_ptr += skip_byte;
	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = (buffer[ buffer_ptr++ ])<<0;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<8;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<16;
	}
	buffer_ptr = ptr;
#else
	ptr = ftell(rom);
	while( skip_byte-- ) fgetc(rom);
	while( num-- )
	{
		value = fgetc(rom)<<0;
		value |= fgetc(rom)<<8;
		value |= fgetc(rom)<<16;
	}
	fseek( rom,ptr,SEEK_SET );
#endif

#ifdef MSB
	value = GetMSBAddress( value,24 );
#endif

	return value;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------

unsigned int Read_Dword( int num, bool memory_log=true )
{
	int value;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = (buffer[ buffer_ptr++ ])<<0;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<8;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<16;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<24;
	}
#else
	while( num-- )
	{
		value = fgetc(rom)<<0;
		value |= fgetc(rom)<<8;
		value |= fgetc(rom)<<16;
		value |= fgetc(rom)<<24;
	}
#endif

#ifdef MSB
	value = GetMSBAddress( value,32 );
#endif

	Bank_Wrap(memory_log);

	return value;
}


unsigned int Peek_Dword( int num )
{
	int value, ptr;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	ptr = buffer_ptr;
	while( num-- )
	{
		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value = (buffer[ buffer_ptr++ ])<<0;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<8;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<16;

		if(Tell_Cache_Linear()>=BUFFER_SIZE) return -1;
		value |= (buffer[ buffer_ptr++ ])<<24;
	}
	buffer_ptr = ptr;
#else
	ptr = ftell(rom);
	while( num-- )
	{
		value = fgetc(rom)<<0;
		value |= fgetc(rom)<<8;
		value |= fgetc(rom)<<16;
		value |= fgetc(rom)<<24;
	}
	fseek( rom,ptr,SEEK_SET );
#endif

#ifdef MSB
	value = GetMSBAddress( value,32 );
#endif

	return value;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------

unsigned char Print_Byte( int num, bool memory_log=true )
{
	int value;

	if(num<=0) return -1;

#ifdef READ_BUFFER_CACHE
	while( num-- )
	{
		value = buffer[ buffer_ptr++ ];
		fprintf( out_text, "<$%02X>", value );
	}
#else
	while( num-- )
	{
		value = fgetc(rom);
		fprintf( out_text, "<$%02X>", value );
	}
#endif

	Bank_Wrap(memory_log);

	return value;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------

void Write_MSB8( unsigned int value, FILE *out )
{
	fputc( (value>>0)&0xff, out );
}


void Write_MSB16( unsigned int value, FILE *out )
{
	fputc( (value>>8)&0xff, out );
	fputc( (value>>0)&0xff, out );
}


void Write_MSB24( unsigned int value, FILE *out )
{
	fputc( (value>>16)&0xff, out );
	fputc( (value>>8)&0xff, out );
	fputc( (value>>0)&0xff, out );
}


void Write_MSB32( unsigned int value, FILE *out )
{
	fputc( (value>>24)&0xff, out );
	fputc( (value>>16)&0xff, out );
	fputc( (value>>8)&0xff, out );
	fputc( (value>>0)&0xff, out );
}