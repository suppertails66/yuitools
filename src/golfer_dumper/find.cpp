void Init_Dumper()
{
	// init
	memset( mark_rom, 0, sizeof(mark_rom) );
	dump_list.clear();

	string_total = 0;
	string_count = 0;

	// ============================================================
	// ############################################################
	// ============================================================

	int lcv;

	// init memory check
	for( lcv=0; lcv<0x100; lcv++ )
	{
		bank_low[lcv] = 0x7fffffff;
		bank_high[lcv] = 0;
	}
}


void Find_Text_1()
{
	CACHE_BUFFER( 0x1b9e4 );

	// Atlas header
	printf( "\n//*****************************************" );
	printf( "\n//SCRIPT (auto-scan)" );
	printf( "\n//*****************************************" );
	printf( "\n" );

	// file information
	printf( "\nJMP %X", 0x80000 );
	printf( "\nTABLE-16 %X %X", 0x1B9E4, 0x80000 );
	printf( "\nW32 %X", 0xB400 );
	printf( "\nW32 %X", 0xB406 );
	printf( "\nLINE" );
	printf( "\n" );

	printf( "\nJMP %X", 0x8034C );
	printf( "\nLINE" );
	printf( "\nLINE" );
	printf( "\n" );

	while( Tell_Cache() < 0x1bd30 )
	{
		int base_ptr;

		// read 16-bit table ptr
		base_ptr = Read_Word(1);
		if( base_ptr==0 ) continue;

		// ==================================================================
		// ==================================================================

		// file information
		printf( "\nRAW %X %X", Tell_Cache()-2, base_ptr+0x1b9e4 );
	}

	// final
	printf( "\nDUMP script" );
	printf( "\n" );
}


void Find_Text_2()
{
	Cache_Buffer_Genesis(0x239e8);

	// Atlas header
	printf( "\n\n" );
	printf( "\n//*****************************************" );
	printf( "\n//SYSTEM TEXT" );
	printf( "\n//*****************************************" );
	printf( "\n\n" );

	while( base+buffer_ptr < 0x23ab8 )
	{
		unsigned int ptr;

		//if( Tell_Cache()>=0x1c5370 )
			//save_pc=Tell_Cache();

		ptr = Read_Dword(1);
		if( ptr<0x20000 ) continue;
		if( ptr>=0x30000 ) continue;

		// ============================================================
		// ############################################################
		// ============================================================

		// FOUND
		printf( "\nRAW %X %X", base+buffer_ptr-4, ptr );

		continue;
	} // end rom
}
