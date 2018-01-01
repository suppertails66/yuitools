#define EMBED_WRITE 1
#define EMBED_SET 2
typedef struct
{
	int type;
	int num;
	int set;
	int set_state;
	int set_base;
	int write;
	int write_state;
} EMBED_TYPE;
typedef deque<EMBED_TYPE>::iterator EMBED_ITER;

int embed_count;
deque<EMBED_TYPE> embed_list;
deque<EMBED_TYPE> embed_rewind;
deque<EMBED_TYPE> dump_list;


int last_stop;
int string_num, string_total, string_count;

FILE *rewind_log;


int args_table[] =
{
	3,2,3,3,		// F0-F3
	1,4,0,1,		// F4-F7
	1,1,0,2,		// F8-FB
	2,2,1,0,		// FC-FF
};


unsigned char mark_rom[0x100*0x10000];
int mark_index;

int text_stop;

#define TYPE_NONE 0x00
#define TYPE_CODE 0x01
#define TYPE_TEXT 0x02
#define TYPE_STOP 0x04
#define TYPE_PASS2 0x08
#define TYPE_DIRTY 0x80


int PRINT_STATE = TYPE_CODE;
int PRINT_RESET	= true;

// ============================================================
// ************************************************************
// ************************************************************
// ============================================================

void Dump_Layer3( bool RET_FAR = false )
{
	int TEXT_START = Tell_Cache();
	int START_PTR = Tell_Cache();
	int ATTR_STATE = 0;
	bool check_stop = false;

	// init
	PRINT_RESET = true;
	PRINT_STATE = TYPE_CODE;

	/*
	// SRW4 hack (!)
	if( Tell_Cache()==0x5C6C2 )
	{
		ATTR_STATE = 0x100;
		PRINT_STATE = TYPE_TEXT;
	}
	*/

	// dump string
	while(1)
	{
		int code;

		// DEBUGGING
		//if( Tell_Cache()>=0x3d5c6 && Tell_Cache()<=0x3d5c9 )
			//code=0;

		// safety check -- sub-strings
		if( Tell_Cache() > START_PTR )
		{
			if( (mark_rom[ Tell_Cache() ] & TYPE_PASS2) != 0 )
			{
				// normal abort
				if( check_stop ) break;

				printf( "\n[SUB-STRING] %X", Tell_Cache() );
				break;
			}
		}

		// ======================================================
		// ######################################################
		// ======================================================

		// check for REWIND WRITES

		EMBED_ITER iter = embed_rewind.begin();
		bool embed_hit = false;
		while( iter != embed_rewind.end() )
		{
			EMBED_TYPE embed_ptr;
			embed_ptr = *iter;

			// SPEED
			if( (*iter).write > Tell_Cache()-0 ) break;

			// no match
			if( (*iter).write < Tell_Cache()-0 ) { iter++; continue; }
			if( (*iter).type != EMBED_SET ) { iter++; continue; }

			// formatting
			if( !embed_hit && PRINT_RESET==false )
			{
				fprintf( out_text, "\n" );
				//fprintf( out_text, "\n//[%06X-%d]", Tell_Cache()-1, bit_count );
			}

			// embed HIT
			fprintf( out_text, "\n#EMBWRITE(%d)", embed_count );
			embed_hit = true;

			// MODIFY entry
			(*iter).num = embed_count;
			//if( (*iter).state != -1 ) ATTR_STATE = (*iter).state;

			// cleanup
			iter++;
			embed_count++;
		}

		// ======================================================
		// ######################################################
		// ======================================================

		// check for embed WRITES
		iter = embed_list.begin();
		while( iter != embed_list.end() )
		{
			EMBED_TYPE embed_ptr;
			embed_ptr = *iter;

			// SPEED
			if( (*iter).write > Tell_Cache()-0 ) break;

			// no match
			if( (*iter).write < Tell_Cache()-0 ) { iter++; continue; }
			if( (*iter).type != EMBED_WRITE ) { iter++; continue; }

			// formatting
			if( !embed_hit && PRINT_RESET==false )
			{
				fprintf( out_text, "\n" );
				//fprintf( out_text, "\n//[%06X-%d]", Tell_Cache()-1, bit_count );
			}

			// embed HIT
			fprintf( out_text, "\n#EMBWRITE(%d)", (*iter).num );
			embed_hit = true;

			// MODIFY state
			//if( (*iter).state != -1 ) ATTR_STATE = (*iter).state;

			// erase entry
			embed_list.erase(iter);
			iter = embed_list.begin();
		}

		// formatting
		if( embed_hit )
		{
			fprintf( out_text, "\n" );
			PRINT_RESET = true;
		}

		// ------------------------------------------------------
		// ######################################################
		// ------------------------------------------------------

		// FULL-STOP
		if( check_stop )
		{
			// log FILE boundaries
			//if( Tell_Cache()>last_stop ) last_stop = Tell_Cache();

			if( RET_FAR )
			{
				if( embed_hit ) { fprintf( out_text, "\nOH NO!" ); getc(stdin); }
				break;
			}

			if( !embed_hit )
			{
				break;
			}
			check_stop = false;
		}

		// ------------------------------------------------------
		// ######################################################
		// ------------------------------------------------------

		code = Read_Byte(1);

		/*
		if( code>0xE8 || (code>0x06 && code<0x0D) ||
			 (code>0x0D && code<0x13) || (code>0x16 && code<0x30) )
		{
			printf( "\n[FAIL-SAFE] BAD OPCODE %02X", code );
			break;
		}
		*/

		//if(Tell_Cache()>=0x5c66b)
			//code+=0;

		// raw tiles
		//if( ATTR_STATE==0x100 && code<0xF6 )
		{
			if( code>=0x00 && code<0xB0 )
			{
				// formatting
				if( PRINT_STATE == TYPE_CODE )
				{
					if( PRINT_RESET == false )
					{
						fprintf( out_text, "\n" );
					}

					PRINT_STATE = TYPE_TEXT;
					PRINT_RESET = true;
				}

				// sound marks
				if( code==0x88 || code==0x89 )
				{
					int tile = Read_Byte(1);
					Print_Font( 0, tile, tile );
				}

				Print_Font( 0, code, code );

				PRINT_RESET = false;
				continue;
			}

			/*
			// kanji
			if( code>=0xf0 && code<=0xf5 )
			{
				code <<= 8;
				code |= Read_Byte(1);

				// formatting
				if( PRINT_STATE == TYPE_CODE )
				{
					if( PRINT_RESET == false )
					{
						fprintf( out_text, "\n" );
					}

					PRINT_STATE = TYPE_TEXT;
					PRINT_RESET = true;
				}

				Print_Font( 0x100, code, code );

				PRINT_RESET = false;
				continue;
			}
			*/
		} // end ATTR state

		// -----------------------------------------------------------------
		// #################################################################
		// -----------------------------------------------------------------

/*
		// <CHOICE>
		if( code==0xF5 )
		{
			Print_Code( 0, code, code );

			// ----------------------------------------------------------------
			// ----------------------------------------------------------------

			int ptr;

			ptr = GetMSBAddress( Read_Word(1),16 );
			fprintf( out_text, "\n#EMBSET(%d)", embed_count );

			// prepare embed - 16bit ptrs
			EMBED_TYPE embed_ptr;
			embed_ptr.set = base+buffer_ptr-2;
			embed_ptr.write = base+ptr;
			embed_ptr.num = embed_count;
			embed_ptr.type = EMBED_WRITE;

			// OPTIMIZE search list
			iter = embed_list.begin();
			while( iter != embed_list.end() )
			{
				if( ptr < (*iter).write ) break;
				if( ptr == (*iter).write )
				{
					if( (base+buffer_ptr-2) < (*iter).set ) break;
				}
				iter++;
			}

			// add to list
			embed_list.insert( iter, embed_ptr );
			embed_count++;

			// ----------------------------------------------------------------
			// ----------------------------------------------------------------

			ptr = GetMSBAddress( Read_Word(1),16 );
			fprintf( out_text, "\n#EMBSET(%d)", embed_count );

			// prepare embed - 16bit ptrs
			embed_ptr.set = base+buffer_ptr-2;
			embed_ptr.write = base+ptr;
			embed_ptr.num = embed_count;
			embed_ptr.type = EMBED_WRITE;

			// OPTIMIZE search list
			iter = embed_list.begin();
			while( iter != embed_list.end() )
			{
				if( ptr < (*iter).write ) break;
				if( ptr == (*iter).write )
				{
					if( (base+buffer_ptr-2) < (*iter).set ) break;
				}
				iter++;
			}

			// add to list
			embed_list.insert( iter, embed_ptr );
			embed_count++;

			// ----------------------------------------------------------------
			// ----------------------------------------------------------------

			fprintf( out_text, "\n" );

			PRINT_STATE = TYPE_CODE;
			PRINT_RESET = true;

			continue;
		}


		// <FLAG>
		if( code==0xf7 )
		{
			if( Peek_Byte(1) & 0x40 )
			{
				// font tiles
				ATTR_STATE = 0x100;
			}
			else
			{
				// non-font
				ATTR_STATE = 0;
			}
		}
*/

		// -----------------------------------------------------------------
		// -----------------------------------------------------------------
		// -----------------------------------------------------------------

		Print_Code( 0, code, code );
		//Print_Byte( args_table[ code-0xF0 ] ); 


		// inline codes
		if( code==0xE2 || code==0xE3 || code<0xE0 )
		{
			if( PRINT_STATE==TYPE_CODE && PRINT_RESET==false)
			{
				fprintf( out_text, "\n" );
				PRINT_RESET = true;
			}
			PRINT_STATE = TYPE_TEXT;

			continue;
		}

		// -----------------------------------------------------------------
		// -----------------------------------------------------------------
		// -----------------------------------------------------------------

		// formatting
		if( PRINT_STATE == TYPE_TEXT )
		{
			if( PRINT_RESET == false )
			{
				fprintf( out_text, "\n" );
			}

			PRINT_STATE = TYPE_CODE;
			PRINT_RESET = true;
		}
		else
		{
			PRINT_RESET = false;
		}


		// <STOP> codes
		if( code==0xe0 || code==0xe4 || code==0xe5 )
		{
			check_stop = true;
			continue;
		}


		// <LINE>
		if( code==0xe1 )
		{
			if( PRINT_RESET == false )
			{
				fprintf( out_text, "\n" );
			}

			PRINT_RESET = true;
			continue;
		}

		continue;
	}

	// ============================================================
	// ============================================================
	// ============================================================

	// post-init
	//fprintf( out_text, "\n//[%06X-%06X]", base+TEXT_START, base+buffer_ptr );
	//fprintf( out_text, "\n\n" );
}


void Dump_Layer2()
{
	EMBED_ITER iter;
	int DUMP_PTR;

	string_count=0;

	// now dump pointer table
	while( dump_list.size() > 0 )
	{
		EMBED_TYPE dump_ptr;
		int ptr, ptr_addr;

		// init
		dump_ptr = dump_list.front();
		dump_list.pop_front();
		ptr = dump_ptr.write;

		// safety check -- duplicate strings
		string_num--;
		if( (mark_rom[ptr] & TYPE_TEXT) != 0 )
			continue;

		// flag location
		mark_rom[ptr] |= TYPE_TEXT;

		// ROM relocation
		ptr_addr = ptr;

		// ============================================================
		// ############################################################
		// ============================================================

		iter = dump_list.begin();

		// Atlas header
		fprintf( out_text, "\n//[%06X = %04X]", ptr, string_count++ );
		fprintf( out_text, "\n#ALIGN(2)" );

		// find duplicates
		while( iter != dump_list.end() )
		{
			// SPEED
			if( (*iter).write != dump_ptr.write ) break;

			// DUPLICATE
			fprintf( out_text, "\n#WRITEOFS(PtrTable, $%X)", (*iter).set - (*iter).set_base );
			string_num--;
			string_count++;

			// REMOVE
			dump_list.pop_front();
			iter = dump_list.begin();
		}

		// Atlas header
		//fprintf( out_text, "\n#WRITEINDEX($%X,1)", dump_ptr.set-2 );
		//fprintf( out_text, "\n#WRITE(PtrTable)" );
		//fprintf( out_text, "\n#WRITE(MyPtr, $%X)", dump_ptr.set );
		fprintf( out_text, "\n#WRITEOFS(PtrTable, $%X)", dump_ptr.set - dump_ptr.set_base );
		fprintf( out_text, "\n" );

		// ============================================================
		// ############################################################
		// ============================================================

		CACHE_BUFFER( dump_ptr.write );

		// memory check
		if( Tell_Cache() < bank_low[ Tell_Cache()/BANK_SIZE ] )
			bank_low[(Tell_Cache())/BANK_SIZE] = Tell_Cache();

		// dump string
		Dump_Layer3();

		// memory check
		if( Tell_Cache() > bank_high[ Tell_Cache()/BANK_SIZE ] )
			bank_high[ Tell_Cache()/BANK_SIZE ] = Tell_Cache();


		// formatting
		fprintf( out_text, "\n" );
		fprintf( out_text, "\n" );

		// ============================================================
		// ############################################################
		// ============================================================

/*
		// Gameboy bank-16 pointers

		// safety check - MISSED EMBED PTRS
		while( embed_list.size()>0 )
		{
			EMBED_TYPE embed_ptr;
			embed_ptr = embed_list.front();


			if( embed_ptr.set > embed_ptr.write )
			{
				printf( "\n[MISS REWIND] %d, %d = %X [%X]",
					embed_ptr.type, embed_ptr.num, embed_ptr.write, embed_ptr.set );

				// Create log file
				if( !rewind_log )
				{
					char filename[256];
					sprintf( filename, "file//rewind_%X.txt", base+base_ptr );
					rewind_log = fopen( filename, "wa" );
					if( !rewind_log )
					{
						printf( "\n[FILE ERROR] Failed to create %s", filename );
						getc(stdin);
						return;
					}
				}

				// REWIND log
				fprintf( rewind_log, "%X %X %X %X\n", embed_ptr.set, embed_ptr.write, embed_ptr.set_state, embed_ptr.write_state );

				embed_list.pop_front();
				continue;
			}

			// ------------------------------------------------------------
			// ------------------------------------------------------------
			// ------------------------------------------------------------

			if( embed_ptr.set < embed_ptr.write )
			{
				printf( "\n[TRAP FORWARD = GOOD] %d, %d = %X [%X]",
					embed_ptr.type, embed_ptr.num, embed_ptr.write, embed_ptr.set );

				// Trap string
				CACHE_BUFFER( embed_ptr.write );

				// Atlas
				fprintf( out_text, "\n//[%06X] @ %04X", base+buffer_ptr, string_count-1 );

				// FORCE dump
				goto DUMP_STRING;
			}

			embed_list.pop_front();
		}
*/
	}

	// ============================================================
	// ############################################################
	// ============================================================

	// safety check - MISSED REWINDS
	while( embed_rewind.size()>0 )
	{
		EMBED_TYPE embed_ptr;
		EMBED_ITER iter;
		int ptr;

		embed_ptr = embed_rewind.front();
		ptr = embed_ptr.write;

		// OPTIMIZE search list
		iter = embed_list.begin();
		while( iter != embed_list.end() )
		{
			if( ptr < (*iter).write ) break;
			if( ptr == (*iter).write )
			{
				if( (embed_ptr.set) < (*iter).set ) break;
			}
			iter++;
		}

		// SWAP lists
		embed_rewind.pop_front();
		embed_list.insert( iter, embed_ptr );


		printf( "\n[TRAP REWIND = GOOD] %d, %d = %X [%X]",
			embed_ptr.type, embed_ptr.num, embed_ptr.write, embed_ptr.set );

		// Trap string
		DUMP_PTR = embed_ptr.write;

		// Atlas header
		fprintf( out_text, "\n//[%06X = %04X]", DUMP_PTR, string_num );
		fprintf( out_text, "\n\n" );

		// FORCE dump
		//goto DUMP_STRING;
	}

	// ------------------------------------------------------------
	// ############################################################
	// ------------------------------------------------------------

	// safety check - MISSED EMBED PTRS
	while( embed_list.size()>0 )
	{
		EMBED_TYPE embed_ptr;
		embed_ptr = embed_list.front();


		if( embed_ptr.set > embed_ptr.write )
		{
			printf( "\n[MISS REWIND] %d, %d = %X [%X]",
				embed_ptr.type, embed_ptr.num, embed_ptr.write, embed_ptr.set );

			// Create log file
			if( !rewind_log )
			{
				char filename[256];
				sprintf( filename, "file//rewind_%X.txt", base+base_ptr );
				rewind_log = fopen( filename, "wa" );
				if( !rewind_log )
				{
					printf( "\n[FILE ERROR] Failed to create %s", filename );
					getc(stdin);
					return;
				}
			}

			// REWIND log
			fprintf( rewind_log, "%X %X %X %X\n", embed_ptr.set, embed_ptr.write, embed_ptr.set_state, embed_ptr.write_state );

			embed_list.pop_front();
			continue;
		}

		// ------------------------------------------------------------
		// ------------------------------------------------------------
		// ------------------------------------------------------------

		if( embed_ptr.set < embed_ptr.write )
		{
			printf( "\n[TRAP FORWARD = GOOD] %d, %d = %X [%X]",
				embed_ptr.type, embed_ptr.num, embed_ptr.write, embed_ptr.set );

			// Trap string
			DUMP_PTR = embed_ptr.write;

			// Atlas header
			//fprintf( out_text, "\n//[%06X = %04X]", DUMP_PTR, string_num );
			//fprintf( out_text, "\n\n" );

			// FORCE dump
			//goto DUMP_STRING;
		}

		embed_list.pop_front();
	}

	// cleanup
	if( rewind_log ) fclose(rewind_log);

	// ------------------------------------------------------------
	// ############################################################
	// ------------------------------------------------------------

	//fprintf( out_text, "\n//[FILE] %06X-%06X", start_text_offset, last_stop );
	//fprintf( out_text, "\n//////////////////////////////////////////////////////////" );
	//fprintf( out_text, "\n\n\n" );

	//printf( "\n[FILE] %06X-%06X", start_text_offset, last_stop );


	// safety check
	if( string_num != 0 )
	{
		printf( "\n[FAIL-SAFE] Bad text count = %d", string_num );
	}
	string_num = 0;
}


void Dump_Layer2_NT16( int width, int height )
{
	for( int lcv1=0; lcv1<height; lcv1++ )
	{
		for( int lcv2=0; lcv2<width; lcv2++ )
		{
#ifdef MSB
			Print_Byte(1);

			int tile = Read_Byte(1);
			Print_Font( 0, tile, tile );
#else
			int tile = Read_Byte(1);
			Print_Font( 0, tile, tile );

			Print_Byte(1);
#endif
		}

		fprintf( out_text, "\n" );
	}

	fprintf( out_text, "\n" );
}


void Dump_Layer2_NT8( int width, int height )
{
	for( int lcv1=0; lcv1<height; lcv1++ )
	{
		for( int lcv2=0; lcv2<width; lcv2++ )
		{
			int tile = Read_Byte(1);

			Print_Font( 0, tile, tile );
		}

		fprintf( out_text, "\n" );
	}
	
	fprintf( out_text, "\n" );
}


void Dump_Layer_INTRO()
{
	int width, height, embed_count;

	fprintf( out_text, "\n#JMP($6574E)" );
	fprintf( out_text, "\n#W32($29E6)" );
	
	CACHE_BUFFER( 0x6574E );

	embed_count = 0;
	while( Tell_Cache()<0x658b6 )
	{
		fprintf( out_text, "\n#EMBSET(%d)", embed_count++ );
		fprintf( out_text, "\n" );

		Read_Dword(1);
		Print_Byte(3*2);
	}
	fprintf( out_text, "\n" );
	fprintf( out_text, "\n" );

	// ===================================================================
	// ###################################################################
	// ===================================================================

	CACHE_BUFFER( 0x6574E );

	embed_count = 0;
	while( Tell_Cache()<0x658b6 )
	{
		int ptr;

		fprintf( out_text, "\n#EMBWRITE(%d)", embed_count++ );

		ptr = Read_Dword(1);
		Read_Word(1);
		width = Read_Word(1);
		height = Read_Word(1);

		// ------------------------------------------------------------------
		// ------------------------------------------------------------------

		PUSH_CACHE();
		
		CACHE_BUFFER( ptr );
		fprintf( out_text, "\n//[%06X]", Tell_Cache() );
		fprintf( out_text, "\n" );

		for( int lcv1=0; lcv1<height; lcv1++ )
		{
			for( int lcv2=0; lcv2<width; lcv2++ )
			{
				int tile = Read_Byte(1);

				// NT-8
				Print_Font( 0, tile, tile );
			}

			fprintf( out_text, "\n" );
		}

		POP_CACHE();

		// ------------------------------------------------------------------
		// ------------------------------------------------------------------

		fprintf( out_text, "\n" );
	}
}


void Dump_Layer1()
{
	string_num = 0;
	dump_list.clear();

	// ============================================================
	// ############################################################
	// ============================================================

	char filename[256];
	sprintf( filename, "file//rewind_%X.txt", base+base_ptr );
	rewind_log = fopen( filename, "r" );

	// parse REWIND ptrs
	if( rewind_log )
	{
		while(1)
		{
			EMBED_TYPE embed_ptr;
			EMBED_ITER iter = embed_rewind.begin();

			// read line
			fgets( filename, 256, rewind_log );
			if( feof(rewind_log) ) break;

			// add to REWIND list
			sscanf( filename, "%X %X %X %X", &(embed_ptr.set), &(embed_ptr.write), &(embed_ptr.set_state), &(embed_ptr.write_state)  );
			embed_ptr.type = EMBED_SET;
			embed_ptr.num = -1;
			//embed_ptr.state = -1;

			// OPTIMIZE - put in order
			while( iter != embed_rewind.end() )
			{
				if( (embed_ptr.write) < (*iter).write ) break;
				iter++;
			}

			// add to SORTED list
			embed_rewind.insert( iter, embed_ptr );
		}

		// cleanup
		if(rewind_log) fclose(rewind_log);
	}

	// ============================================================
	// ############################################################
	// ============================================================

	FILE *dump_log = fopen( "file/dump_list1.txt", "r" );
	if( dump_log )
	{
		int BASE_SET;

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
			int set, write;
			sscanf( line, "%s", mode );


			// JMP pointer
			if( strcmp(mode,"JMP") == 0 )
			{
				int value;

				sscanf( line, "JMP %X", &value );
				fprintf( out_text, "\n#JMP($%X)", ROM_TO_LINEAR(value) );
				continue;
			}

			// W32 pointer
			if( strcmp(mode,"W32") == 0 )
			{
				int value;

				sscanf( line, "W32 %X", &value );
				fprintf( out_text, "\n#W32($%X)", ROM_TO_LINEAR(value) );
				continue;
			}

			// W16 pointer
			if( strcmp(mode,"W16") == 0 )
			{
				int value;

				sscanf( line, "W16 %X", &value );
				fprintf( out_text, "\n#W16($%X)", ROM_TO_LINEAR(value) );
				continue;
			}


			// LINE formatting
			if( strcmp(mode,"LINE") == 0 )
			{
				fprintf( out_text, "\n" );
				continue;
			}

			
			// TABLE pointer
			if( strcmp(mode,"TABLE-16") == 0 )
			{
				int value;

				sscanf( line, "TABLE-16 %X %X", &BASE_SET, &value );
				fprintf( out_text, "\n#PTRTBL(PtrTable, $%X, 2, MyPtr)", ROM_TO_LINEAR(value) );
				continue;
			}


			// RAW pointer
			if( strcmp(mode,"RAW") == 0 )
			{
				sscanf( line, "RAW %X %X", &set, &write );
				goto ADD_LIST_PTR;
			}


			// LOAD new table
			if( strcmp(mode,"LOAD_TABLE") == 0 )
			{
				mode2[0]=0;
				sscanf( line, "LOAD_TABLE %s %s", mode2, mode );
				Load_Table( 0, mode2 );

				fprintf( out_text, "\n#VAR(Table2,TABLE)" );
				fprintf( out_text, "\n#ADDTBL(\"%s\", Table2)", mode );
				fprintf( out_text, "\n#ACTIVETBL(Table2)" );
				fprintf( out_text, "\n" );
				continue;
			}


			// DUMP text
			if( strcmp(mode,"DUMP") == 0 )
			{
				mode2[0]=0;
				sscanf( line, "DUMP %s", mode2 );
				if( dump_list.size()>0 ) Dump_Layer2();

				fprintf( out_text, "\n// END FILE %s", mode2 );
				fprintf( out_text, "\n///////////////////////////////////////////////" );
				fprintf( out_text, "\n\n" );
				continue;
			}


			// DUMP NT-16 text
			if( strcmp(mode,"DUMP_NT16") == 0 )
			{
				int ptr, width, height;
				sscanf( line, "DUMP_NT16 %X %d %d", &ptr, &width, &height );
	
				// Atlas header
				fprintf( out_text, "\n//[%06X]", ptr );
				fprintf( out_text, "\n" );

				// ----------------------------------------------------------

				CACHE_BUFFER( ROM_TO_LINEAR(ptr) );
				Dump_Layer2_NT16( width, height );
				continue;
			}


			// DUMP NT-8 text
			if( strcmp(mode,"DUMP_NT8") == 0 )
			{
				int ptr, width, height;
				sscanf( line, "DUMP_NT8 %X %d %d", &ptr, &width, &height );

				// Atlas header
				fprintf( out_text, "\n//[%06X]", ptr );
				fprintf( out_text, "\n" );

				// ----------------------------------------------------------

				CACHE_BUFFER( ROM_TO_LINEAR(ptr) );
				Dump_Layer2_NT8( width, height );
				continue;
			}

			// ===================================================================
			// ===================================================================

			// SPECIAL: Battle Golfer Yui
			
			// DUMP INTRO text
			if( strcmp(mode,"DUMP_INTRO") == 0 )
			{
				Dump_Layer_INTRO();

				fprintf( out_text, "\n\n" );
				continue;
			}



			// fail-safe
			continue;

			// ===================================================================
			// ###################################################################
			// ===================================================================

		ADD_LIST_PTR:
			EMBED_TYPE embed_type;
			EMBED_ITER iter = dump_list.begin();

			// add to dump list
			embed_type.set = set;
			embed_type.set_base = BASE_SET;
			embed_type.write = write;
			embed_type.num = string_total++;
			embed_type.type = EMBED_WRITE;
			string_num++;

			// OPTIMIZE - put in order
			while( iter != dump_list.end() )
			{
				if( (embed_type.write) < (*iter).write ) break;
				if( (embed_type.write) == (*iter).write )
				{
					if( (embed_type.set) < (*iter).set ) break;
				}
				iter++;
			}

			// add to SORTED list
			dump_list.insert( iter, embed_type );

			// mark dirty for sub-string checks
			mark_rom[write] |= TYPE_PASS2;
		}

		if(dump_log) fclose(dump_log);
	}

	// ============================================================
	// ############################################################
	// ============================================================

	// memory report
	for( int lcv=0; lcv<0x100; lcv++ )
	{
		if( bank_high[lcv]>0 )
		{
			printf( "\n[%02X] %06X-%06X", lcv, bank_low[lcv], bank_high[lcv] );
		}
	}
}


#undef MSB
