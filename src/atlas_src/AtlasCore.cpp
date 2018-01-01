//-----------------------------------------------------------------------------
// AtlasCore - A class to insert Atlas-type scripts
// By Steve Monaco (stevemonaco@hotmail.com)
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
//#include <hash_map>
#include <string>
#include <sstream>
#include <ctime>
#include "Table.h"
#include "AtlasCore.h"
#include "AtlasTypes.h"
#include "GenericVariable.h"
#include "AtlasFile.h"
#include "Pointer.h"
#include "AtlasLogger.h"
#include "AtlasStats.h"
#include "PointerHandler.h"
#include "AtlasExtension.h"

using namespace std;

int index_number, index_bump, index_size;
int set_align;

// Constructor

AtlasCore Atlas;
unsigned int CurrentLine = 1;

AtlasCore::AtlasCore() : PtrHandler(&VarMap), Parser(&VarMap), Extensions(&VarMap)
{
	CurrentLine = 1;
	HeaderSize = 0;
	IsInJmp = false;
}

// Destructor
AtlasCore::~AtlasCore()
{
}

bool AtlasCore::Insert(const char* RomFileName, const char* ScriptFileName)
{
	ifstream script;
	script.open(ScriptFileName, ios::in);
	if(!script.is_open())
	{
		printf("Unable to open script file '%s'\n", ScriptFileName);
		return false;
	}

	if(!File.OpenFile(RomFileName))
	{
		printf("Unable to open target file '%s'\n", RomFileName);
		return false;
	}

	// Parse file
	bool ParseSuccess = false;
	clock_t ParseStart = clock();
	ParseSuccess = Parser.ParseFile(script);
	clock_t ParseTime = clock() - ParseStart;

	PrintSummary("Parsing", ParseTime);

	if(!ParseSuccess)
		return false;

	// Insert file
	clock_t InsertionStart = clock();
	for(ListBlockIt Block = Parser.Blocks.begin(); Block != Parser.Blocks.end(); Block++)
	{
		// Execute list of commands
		for(ListCmdIt com = Block->Commands.begin(); com != Block->Commands.end(); com++)
		{
			CurrentLine = com->Line;
			if(!ExecuteCommand(*com))
				goto InsertionSummary;
		}
		if(Block->StartLine != -1)
			CurrentLine = Block->StartLine;
		for(ListStringIt text = Block->TextLines.begin(); text != Block->TextLines.end(); text++)
		{
			if(!File.InsertText(*text, CurrentLine))
				goto InsertionSummary;
			CurrentLine++;
		}
		File.FlushText();
	}

InsertionSummary:
	clock_t InsertionTime = clock() - InsertionStart;

	
	// INDEX # summary
	printf( "\n\nINDEX # = %X\n\n", index_number );
	PrintSummary("Insertion", InsertionTime);


	PrintSummary("Insertion", InsertionTime);

	Stats.End(CurrentLine); // Hack for the last line
	PrintStatistics();

	return true;
}

void AtlasCore::SetDebugging(FILE* output)
{
	if(output == NULL)
		Logger.SetLogStatus(false);
	else
		Logger.SetLogStatus(true);
	Logger.SetLogSource(output);
}

void AtlasCore::PrintStatistics()
{
	const char* Frame = "+------------------------------------------------------------------------------";

	if(Stats.Stats.size() == 0);
	else if(Stats.Stats.size() == 1)
	{
		PrintStatisticsBlock("Total", Stats.Stats.front());
	}
	else // Print each block's content
	{
		unsigned int blocknum = 1;
		char buf[127];
		for(ListStatsIt i = Stats.Stats.begin(); i != Stats.Stats.end(); i++)
		{
			_snprintf(buf, 127, "Block %d", blocknum);
			PrintStatisticsBlock(buf, *i);
			blocknum++;
		}

		Stats.GenerateTotalStats(Total);

		// Print out the total statistics
		printf("%s\n", Frame);
		printf("| Total Statistics\n");
		printf("| Script Size %u\n", Total.ScriptSize);
		printf("| Script Inserted %u\n", Total.ScriptSize - Total.ScriptOverflowed);
		if(Total.ScriptOverflowed != 0)
			printf("| Script Overflowed %u\n", Total.ScriptOverflowed);
		if(Total.MaxBound != -1)
			printf("| Space Remaining %u\n", Total.SpaceRemaining);
		printf("|\n");

		if(Total.HasCommands())
		{
			printf("| Command Execution Listing\n");
			for(int j = 0; j < CommandCount; j++)
			{
				if(Total.ExecCount[j] != 0)
					printf("| %s: %u\n", CommandStrings[j], Total.ExecCount[j]);
			}
		}

		printf("| Pointer Listing\n");
		printf("| General Pointers Written: %u\n", Total.PointerWrites);
		if(Total.EmbPointerWrites != 0)
			printf("| Embedded Pointers Written: %u\n", Total.EmbPointerWrites);
		if(Total.AutoPointerWrites != 0)
			printf("| Autowrite Pointers Written: %u\n", Total.AutoPointerWrites);
		if(Total.FailedListWrites != 0)
			printf("| Failed PointerList Writes: %u\n", Total.FailedListWrites);
		if(Total.ExtPointerWrites != 0)
			printf("| Extension Pointer Writes: %u\n", Total.ExtPointerWrites);
		printf("%s\n\n", Frame);
	}
}

void AtlasCore::PrintStatisticsBlock(const char* Title, InsertionStatistics& Stats)
{
	const char* Frame = "+------------------------------------------------------------------------------";

	printf("%s\n", Frame);
	printf("| %s\n|   Start: Line %u  File Position $%X", Title, Stats.LineStart, Stats.StartPos);
	if(Stats.MaxBound != -1)
		printf("  Bound $%X", Stats.MaxBound);
	printf("\n");
	printf("|   End: Line %u File Position $%X\n", Stats.LineEnd, Stats.StartPos + Stats.ScriptSize - Stats.ScriptOverflowed);
	printf("%s\n", Frame);
	printf("| Script size %u\n", Stats.ScriptSize);
	printf("| Bytes Inserted %u", Stats.ScriptSize - Stats.ScriptOverflowed);
	if(Stats.ScriptOverflowed != 0)
		printf("\n| Script Overflowed %u", Stats.ScriptOverflowed);
	if(Stats.MaxBound != -1)
		printf("\n| Space Remaining %u", Stats.SpaceRemaining);
	printf("\n|\n");

	if(Stats.HasCommands())
	{
		printf("| Command Execution Listing\n");
		for(int j = 0; j < CommandCount; j++)
		{
			if(Stats.ExecCount[j] != 0)
				printf("|   %s: %u\n", CommandStrings[j], Stats.ExecCount[j]);
		}
		printf("|\n");
	}

	printf("| Pointer Listing\n");
	printf("|   General Pointers Written: %u\n", Stats.PointerWrites);
	if(Stats.EmbPointerWrites != 0)
		printf("|   Embedded Pointers Written: %u\n", Stats.EmbPointerWrites);
	if(Stats.AutoPointerWrites != 0)
		printf("|   Autowrite Pointers Written: %u\n", Stats.AutoPointerWrites);
	if(Stats.FailedListWrites != 0)
		printf("|   Failed PointerList Writes: %u\n", Stats.FailedListWrites);
	if(Stats.ExtPointerWrites != 0)
		printf("|   Extension Pointer Writes: %u\n", Stats.ExtPointerWrites);
	printf("%s\n\n", Frame);
}

void AtlasCore::PrintSummary(const char* Title, unsigned int TimeCompleted)
{
	unsigned int SumErrors = 0;
	unsigned int SumWarnings = 0;
	printf("%s summary: %u msecs\n", Title, TimeCompleted);

	// Print errors and warnings
	for(ListErrorIt i = Logger.Errors.begin(); i != Logger.Errors.end(); i++)
	{
		if(i->Severity == FATALERROR)
		{
			printf("Error: ");
			SumErrors++;
		}
		else if(i->Severity == WARNING)
		{
			printf("Warning: ");
			SumWarnings++;
		}
		printf("%s on line %d\n", i->Error.c_str(), i->LineNumber);
	}
	Logger.Errors.clear();
	printf("%s - %u error(s), %u warning(s)\n\n", Title, SumErrors, SumWarnings);
}

bool AtlasCore::ExecuteCommand(Command& Cmd)
{
	static unsigned int PtrValue;
	static unsigned int PtrNum;
	static unsigned int PtrPos;
	static unsigned int Size;
	static bool Success;
	static PointerList* List = NULL;
	static PointerTable* Tbl = NULL;
	static AtlasContext* Context = NULL;
	static AtlasExtension* Ext = NULL;
	string FuncName;

	static FILE *fp;
	static char buffer[1024];	
	static unsigned int num;
	static unsigned int ssrpg_pascal;

	if(IsInJmp && Cmd.Function != CMD_JMP1 && Cmd.Function != CMD_JMP2)
		Stats.AddCmd(Cmd.Function);
	else
		Total.AddCmd(Cmd.Function);

	switch(Cmd.Function)
	{
	case CMD_JMP1:
		File.Move(StringToUInt(Cmd.Parameters[0].Value), -1);
		Logger.Log("%6u JMP       ROM Position is now $%X\n", Cmd.Line, StringToUInt(Cmd.Parameters[0].Value));
		Stats.NewStatsBlock(File.GetPos(), -1, Cmd.Line);
		IsInJmp = true;
		return true;
	case CMD_JMP2:
		File.Move(StringToUInt(Cmd.Parameters[0].Value), StringToUInt(Cmd.Parameters[1].Value));
		Logger.Log("%6u JMP       ROM Position is now $%X with max bound of $%X\n",
			Cmd.Line, StringToUInt(Cmd.Parameters[0].Value), StringToUInt(Cmd.Parameters[1].Value));
		Stats.NewStatsBlock(File.GetPos(), StringToUInt(Cmd.Parameters[1].Value), Cmd.Line);
		IsInJmp = true;
		return true;
	case CMD_SMA:
		Success = DefaultPointer.SetAddressType(Cmd.Parameters[0].Value);
		if(Success)
			Logger.Log("%6u SMA       Addressing type is now '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str());
		return Success;
	case CMD_HDR:
		unsigned int Size;
		Size = StringToUInt(Cmd.Parameters[0].Value);
		EmbPtrs.SetHeaderSize(Size);
		DefaultPointer.SetHeaderSize(Size);
		HeaderSize = Size;
		Logger.Log("%6u HDR       Header size is now $%X\n", Cmd.Line, StringToUInt(Cmd.Parameters[0].Value));
		return true;
	case CMD_STRTYPE:
		Success = File.SetStringType(Cmd.Parameters[0].Value);
		if(Success)
			Logger.Log("%6u STRTYPE   String type is now '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str());
		return Success;
	case CMD_ADDTBL:
		Success = AddTable(Cmd);
		if(Success)
			Logger.Log("%6u ADDTBL    Added table '%s' as '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str(), Cmd.Parameters[0].Value.c_str());
		return Success;
	case CMD_ACTIVETBL:
		Success = ActivateTable(Cmd.Parameters[0].Value);
		if(Success)
			Logger.Log("%6u ACTIVETBL Active table is now '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str());
		return Success;
	case CMD_VAR: // Already handled by AtlasParser, should never be here
		return true;
	case CMD_WUB:
		PtrValue = DefaultPointer.GetUpperByte(File.GetPos());
		File.Write(&PtrValue, 1, 1, StringToUInt(Cmd.Parameters[0].Value));
		Logger.Log("%6u WUB       ScriptPos $%X PointerPos $%X PointerValue $%02X\n", Cmd.Line,
			File.GetPos(), StringToUInt(Cmd.Parameters[0].Value), PtrValue);
		return true;
	case CMD_WBB:
		PtrValue = DefaultPointer.GetBankByte(File.GetPos());
		File.Write(&PtrValue, 1, 1, StringToUInt(Cmd.Parameters[0].Value));
		Logger.Log("%6u WBB       ScriptPos $%X PointerPos $%X PointerValue $%02X\n", Cmd.Line,
			File.GetPos(), StringToUInt(Cmd.Parameters[0].Value), PtrValue);
		return true;
	case CMD_WHB:
		PtrValue = DefaultPointer.GetHighByte(File.GetPos());
		File.Write(&PtrValue, 1, 1, StringToUInt(Cmd.Parameters[0].Value));
		Logger.Log("%6u WHB       ScriptPos $%X PointerPos $%X PointerValue $%02X\n", Cmd.Line,
			File.GetPos(), StringToUInt(Cmd.Parameters[0].Value), PtrValue);
		return true;
	case CMD_WLB:
		PtrValue = DefaultPointer.GetLowByte(File.GetPos());
		File.Write(&PtrValue, 1, 1, StringToUInt(Cmd.Parameters[0].Value));
		Logger.Log("%6u WLB       ScriptPos $%X PointerPos $%X PointerValue $%02X\n", Cmd.Line,
			File.GetPos(), StringToUInt(Cmd.Parameters[0].Value), PtrValue);
		return true;
	case CMD_W16:
		PtrValue = DefaultPointer.Get16BitPointer(File.GetPos());
		File.Write(&PtrValue, 2, 1, StringToUInt(Cmd.Parameters[0].Value));
		Logger.Log("%6u W16       ScriptPos $%X PointerPos $%X PointerValue $%04X\n", Cmd.Line,
			File.GetPos(), StringToUInt(Cmd.Parameters[0].Value), PtrValue);
		return true;
	case CMD_W24:
		PtrValue = DefaultPointer.Get24BitPointer(File.GetPos());
		File.Write(&PtrValue, 3, 1, StringToUInt(Cmd.Parameters[0].Value));
		Logger.Log("%6u W24       ScriptPos $%X PointerPos $%X PointerValue $%06X\n", Cmd.Line,
			File.GetPos(), StringToUInt(Cmd.Parameters[0].Value), PtrValue);
		return true;
	case CMD_W32:
		PtrValue = DefaultPointer.Get32BitPointer(File.GetPos());
		File.Write(&PtrValue, 4, 1, StringToUInt(Cmd.Parameters[0].Value));
		Logger.Log("%6u W32       ScriptPos $%X PointerPos $%X PointerValue $%08\n", Cmd.Line,
			File.GetPos(), StringToUInt(Cmd.Parameters[0].Value), PtrValue);
		return true;
	case CMD_EMBSET:
		PtrNum = StringToUInt(Cmd.Parameters[0].Value);
		Success = EmbPtrs.SetPointerPosition(PtrNum, File.GetPos());
		Size = EmbPtrs.GetSize(PtrNum);
		if(Size == -1)
			return false;
		Logger.Log("%6u EMBSET    Pointer Position %u set to $%X\n", Cmd.Line, PtrNum, File.GetPos());
		if(Success) // Write out embedded pointer
		{
			PtrValue = EmbPtrs.GetPointerValue(PtrNum);
			if(File.GetMaxWritableBytes() > Size / 8)
			{
				File.Write(&PtrValue, Size / 8, 1, File.GetPos());
				Logger.Log("%6u EMBSET    Triggered Write: ScriptPos $%X PointerPos $%X PointerValue $%X Size %dd", Cmd.Line,
					EmbPtrs.GetTextPosition(PtrNum), File.GetPos(), PtrValue, Size);
				File.Move(File.GetPos() + Size/8);
				Stats.IncEmbPointerWrites();
			}
			else
				Logger.Log("%6u EMBSET    Failed to write due to insufficient space\n");
		}
		else // Reserve space so the embedded pointer and script don't compete
		{    // for the same part of the file
			if(File.GetMaxWritableBytes() > Size / 8)
			{
				unsigned int Zero = 0;
				File.Write(&Zero, Size/8, 1, File.GetPos());
				File.Move(File.GetPos() + Size/8);
			}
			else
				Logger.Log("%6u EMBSET    Failed to write due to insufficient space\n");
		}
		return true;
	case CMD_EMBTYPE:
		Success = EmbPtrs.SetType(Cmd.Parameters[0].Value, StringToInt64(Cmd.Parameters[2].Value), StringToUInt(Cmd.Parameters[1].Value));
		if(!Success)
			Logger.ReportError(Cmd.Line, "Bad size %d for EMBTYPE",
				StringToUInt(Cmd.Parameters[0].Value));
		else
			Logger.Log("%6u EMBTYPE   Embedded Pointer size %u Offsetting %I64d\n", Cmd.Line, StringToUInt(Cmd.Parameters[1].Value), StringToInt64(Cmd.Parameters[0].Value));
		return Success;
	case CMD_EMBWRITE:
		PtrNum = StringToUInt(Cmd.Parameters[0].Value);
		Success = EmbPtrs.SetTextPosition(PtrNum, File.GetPos());
		Size = EmbPtrs.GetSize(PtrNum);
		if(Size == -1)
			return false;
		Logger.Log("%6u EMBWRITE  Pointed Position %u set to $%X\n", Cmd.Line, PtrNum, File.GetPos());
		if(Success) // Write out embedded pointer
		{
			PtrPos = EmbPtrs.GetPointerPosition(PtrNum);
			PtrValue = EmbPtrs.GetPointerValue(PtrNum);
			if(File.GetMaxWritableBytes() > Size / 8)
			{
				File.Write(&PtrValue, Size/8, 1, PtrPos);
				Logger.Log("%6u EMBWRITE  Triggered Write: ScriptPos $%X PointerPos $%X PointerValue $%X Size %dd\n", Cmd.Line,
					File.GetPos(), PtrPos, PtrValue, Size);
				Stats.IncEmbPointerWrites();
			}
			else
				Logger.Log("%6u EMBWRITE  Failed to write due to insufficient space\n");
		}
		return true;
	case CMD_BREAK:
		return false;
	case CMD_PTRTBL:
		Success = PtrHandler.CreatePointerTable(Cmd.Parameters[0].Value, StringToUInt(Cmd.Parameters[1].Value), StringToUInt(Cmd.Parameters[2].Value), Cmd.Parameters[3].Value);
		if(Success)
			Logger.Log("%6u PTRTBL    Pointer Table '%s' created StartPos $%X Increment %dd CustomPointer '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str(),
				StringToUInt(Cmd.Parameters[1].Value), StringToUInt(Cmd.Parameters[2].Value), Cmd.Parameters[3].Value.c_str());
		return Success;
	case CMD_WRITETBL:
		PtrValue = PtrHandler.GetTableAddress(Cmd.Parameters[0].Value, File.GetPos(), Size, PtrPos);
		if(PtrValue == -1)
			return false;
		File.Write(&PtrValue, Size/8, 1, PtrPos);
		Logger.Log("%6u WRITE     PointerTable '%s' ScriptPos $%X PointerPos $%X PointerValue $%08X\n", Cmd.Line,
			Cmd.Parameters[0].Value.c_str(), File.GetPos(), PtrPos, PtrValue);
		return true;
	case CMD_PTRLIST:
		Success = PtrHandler.CreatePointerList(Cmd.Parameters[0].Value, Cmd.Parameters[1].Value.c_str(), Cmd.Parameters[2].Value);
		if(Success)
			Logger.Log("%6u PTRTBL    Pointer List '%s' created from file '%s' CustomPointer '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str(),
				Cmd.Parameters[1].Value.c_str(), Cmd.Parameters[2].Value.c_str());
		return Success;
	case CMD_WRITELIST:
		PtrValue = PtrHandler.GetListAddress(Cmd.Parameters[0].Value, File.GetPos(), Size, PtrPos);
		if(PtrValue == -1)
			return false;
		File.Write(&PtrValue, Size/8, 1, PtrPos);
		Logger.Log("%6u WRITE     PointerList '%s' ScriptPos $%X PointerPos $%X PointerValue $%08X\n", Cmd.Line,
			Cmd.Parameters[0].Value.c_str(), File.GetPos(), PtrPos, PtrValue);
		return true;
	case CMD_AUTOWRITETBL:
		Tbl = (PointerTable*)VarMap.GetVar(Cmd.Parameters[0].Value)->GetData();
		if(Tbl == NULL)
		{
			Logger.ReportError(CurrentLine, "Identifier '%s' has not been initialized with PTRTBL", Cmd.Parameters[0].Value.c_str());
			return false;
		}
		Success = File.AutoWrite(Tbl, Cmd.Parameters[1].Value);
		if(!Success)
			Logger.ReportError(CurrentLine, "'%s' has not been defined as an end token in the active table", Cmd.Parameters[1].Value.c_str());
		else
			Logger.Log("%6u AUTOWRITE EndTag '%s' PointerTable '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str(),
				Cmd.Parameters[1].Value.c_str());
		return Success;
	case CMD_AUTOWRITELIST:
		List = (PointerList*)VarMap.GetVar(Cmd.Parameters[0].Value)->GetData();
		if(List == NULL)
		{
			Logger.ReportError(CurrentLine, "Identifier '%s' has not been initialized with PTRLIST", Cmd.Parameters[0].Value.c_str());
			return false;
		}
		Success = File.AutoWrite(List, Cmd.Parameters[1].Value);
		if(!Success)
			Logger.ReportError(CurrentLine, "'%s' has not been defined as an end token in the active table", Cmd.Parameters[1].Value.c_str());
		else
			Logger.Log("%6u AUTOWRITE EndTag '%s' PointerList '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str(),
				Cmd.Parameters[1].Value.c_str());
		return Success;
	case CMD_CREATEPTR:
		Success = PtrHandler.CreatePointer(Cmd.Parameters[0].Value, Cmd.Parameters[1].Value,
			StringToInt64(Cmd.Parameters[2].Value), StringToUInt(Cmd.Parameters[3].Value), HeaderSize);
		if(Success)
			Logger.Log("%6u CREATEPTR CustomPointer '%s' Addressing '%s' Offsetting %I64d Size %dd HeaderSize $%X\n", Cmd.Line,
				Cmd.Parameters[0].Value.c_str(), Cmd.Parameters[1].Value.c_str(), StringToInt64(Cmd.Parameters[2].Value), StringToUInt(Cmd.Parameters[3].Value), HeaderSize);
		return Success;
	case CMD_WRITEPTR:
		PtrValue = PtrHandler.GetPtrAddress(Cmd.Parameters[0].Value, File.GetPos(), Size);
		PtrPos = StringToUInt(Cmd.Parameters[1].Value);
		if(PtrValue == -1)
			return false;
		File.Write(&PtrValue, Size/8, 1, PtrPos);
		Logger.Log("%6u WRITE     CustomPointer '%s' ScriptPos $%X PointerPos $%X PointerValue $%08X\n", Cmd.Line,
			Cmd.Parameters[0].Value.c_str(), File.GetPos(), PtrPos, PtrValue);
		return true;
	case CMD_LOADEXT:
		Success = Extensions.LoadExtension(Cmd.Parameters[0].Value, Cmd.Parameters[1].Value);
		if(Success)
			Logger.Log("%6u LOADEXT   Loaded extension %s successfully\n", Cmd.Line, Cmd.Parameters[1].Value.c_str());
		return Success;
	case CMD_EXECEXT:
		return ExecuteExtension(Cmd.Parameters[0].Value, Cmd.Parameters[1].Value, &Context);
	case CMD_DISABLETABLE:
		Tbl = (PointerTable*)VarMap.GetVar(Cmd.Parameters[0].Value)->GetData();
		if(Tbl == NULL)
		{
			Logger.ReportError(CurrentLine, "Identifier '%s' has not been initialized with PTRTBL", Cmd.Parameters[0].Value.c_str());
			return false;
		}
		Success = File.DisableWrite(Cmd.Parameters[1].Value, true);
		if(!Success)
			Logger.ReportError(CurrentLine, "'%s' has not been defined as an autowrite end token", Cmd.Parameters[1].Value.c_str());
		else
			Logger.Log("%6u DISABLE   EndTag '%s' PointerTable '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str(),
				Cmd.Parameters[1].Value.c_str());
		return Success;
	case CMD_DISABLELIST:
		List = (PointerList*)VarMap.GetVar(Cmd.Parameters[0].Value)->GetData();
		if(List == NULL)
		{
			Logger.ReportError(CurrentLine, "Identifier '%s' has not been initialized with PTRLIST", Cmd.Parameters[0].Value.c_str());
			return false;
		}
		Success = File.DisableWrite(Cmd.Parameters[1].Value, false);
		if(!Success)
			Logger.ReportError(CurrentLine, "'%s' has not been defined as an autowrite end token", Cmd.Parameters[1].Value.c_str());
		else
			Logger.Log("%6u DISABLE   EndTag '%s' PointerList '%s'\n", Cmd.Line, Cmd.Parameters[1].Value.c_str(),
				Cmd.Parameters[0].Value.c_str());
		return Success;
	case CMD_PASCALLEN:
		Success = File.SetPascalLength(StringToUInt(Cmd.Parameters[0].Value));
		if(Success)
			Logger.Log("%6u PASCALLEN Length for pascal strings set to %u\n", Cmd.Line, StringToUInt(Cmd.Parameters[0].Value));
		else
			Logger.ReportError(CurrentLine, "Invalid length %u for PASCALLEN", StringToUInt(Cmd.Parameters[0].Value));
		return Success;
	case CMD_AUTOEXEC:
		Ext = (AtlasExtension*)VarMap.GetVar(Cmd.Parameters[0].Value)->GetData();
		if(Ext == NULL)
		{
			Logger.ReportError(CurrentLine, "Identifier '%s' has not been initialized with LOADEXT", Cmd.Parameters[0].Value.c_str());
			return false;
		}
		Success = File.AutoWrite(Ext, Cmd.Parameters[1].Value, Cmd.Parameters[2].Value);
		if(Success)
			Logger.Log("%6u AUTOEXEC  EndTag '%s' Extension '%s'\n", Cmd.Line, Cmd.Parameters[0].Value.c_str(),
				Cmd.Parameters[1].Value.c_str());
		return Success;
	case CMD_DISABLEEXEC:
		Success = File.DisableAutoExtension(Cmd.Parameters[0].Value, Cmd.Parameters[1].Value);
		if(Success)
			Logger.Log("%6u DISABLE   EndTag '%s' Extension Function '%s'\n", Cmd.Line, Cmd.Parameters[1].Value.c_str(),
				Cmd.Parameters[0].Value.c_str());
		return Success;

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

	case CMD_INSERT:
		fp = fopen( Cmd.Parameters[0].Value.c_str(), "rb" );
		if( !fp )
		{
			printf( "File not found @ %s\n", Cmd.Parameters[0].Value.c_str() );
			return false;
		}
		while( num = fread( buffer, 1, 1024, fp ) )
		{
			fwrite( buffer, 1, num, File.GetFile() );
		}
		fclose( fp );
		return true;


	case CMD_WARN:
		if( ftell( File.GetFile() ) > StringToUInt(Cmd.Parameters[0].Value) )
		{
			printf( "BARF %d (%X) bytes @ %s\n", 
				ftell( File.GetFile() ) - StringToUInt(Cmd.Parameters[0].Value),
				ftell( File.GetFile() ) - StringToUInt(Cmd.Parameters[0].Value),
				Cmd.Parameters[1].Value.c_str() );
		}
		else
		{
			printf( "OKAY %d (%X) bytes @ %s\n", 
				StringToUInt(Cmd.Parameters[0].Value) - ftell( File.GetFile() ),
				StringToUInt(Cmd.Parameters[0].Value) - ftell( File.GetFile() ),
				Cmd.Parameters[1].Value.c_str() );
		}
		return true;


	case CMD_FILL:
		if( ftell( File.GetFile() ) < StringToUInt(Cmd.Parameters[0].Value) )
		{
			int value, ptr;
			
			ptr = StringToUInt(Cmd.Parameters[0].Value);
			value = StringToUInt(Cmd.Parameters[1].Value);
			while( ftell(File.GetFile()) < ptr )
			{
				fputc( value, File.GetFile() );
			}
		}
		return true;


	case CMD_W08_BYTE:
		{
			int value, ptr, seek;
			
			ptr = StringToUInt(Cmd.Parameters[0].Value);
			value = StringToUInt(Cmd.Parameters[1].Value);

			seek = ftell(File.GetFile());
			fseek(File.GetFile(),ptr,SEEK_SET);
			fputc(value,File.GetFile());
			fseek(File.GetFile(),seek,SEEK_SET);
		}
		return true;


	case CMD_SAVE_PC:
		{
			fp = fopen( Cmd.Parameters[0].Value.c_str(), "w" );
			fprintf( fp,"%X",File.GetPos() );
			fclose( fp );
		}
		return true;


	case CMD_LOAD_PC:
		{
			int new_pc;

			fp = fopen( Cmd.Parameters[0].Value.c_str(), "r" );
			if( !fp )
			{
				printf( "File not found @ %s\n", Cmd.Parameters[0].Value.c_str() );
				return false;
			}
			fscanf( fp,"%X",&new_pc );
			fclose( fp );

			//case CMD_JMP1:
			File.Move(new_pc, -1);
			Logger.Log("%6u JMP       ROM Position is now $%X\n", Cmd.Line, new_pc);
			Stats.NewStatsBlock(File.GetPos(), -1, Cmd.Line);
			IsInJmp = true;
			return true;
		}


	case CMD_SET_INDEX:
		{
			index_number = StringToUInt(Cmd.Parameters[0].Value);
			index_size = StringToUInt(Cmd.Parameters[1].Value);
		}
		return true;


	case CMD_WRITE_INDEX:
		{
			int ptr, bump, seek;
			
			ptr = StringToUInt(Cmd.Parameters[0].Value);
			bump = StringToUInt(Cmd.Parameters[1].Value);

			seek = ftell(File.GetFile());
			fseek(File.GetFile(),ptr,SEEK_SET);

			if(index_size>=1) fputc((index_number>>0)&0xff,File.GetFile());
			if(index_size>=2) fputc((index_number>>8)&0xff,File.GetFile());
			if(index_size>=3) fputc((index_number>>16)&0xff,File.GetFile());
			if(index_size>=4) fputc((index_number>>24)&0xff,File.GetFile());

			index_number += bump;
			fseek(File.GetFile(),seek,SEEK_SET);
		}
		return true;


	case CMD_ALIGN:
		{
			int set_align;

			set_align = StringToUInt(Cmd.Parameters[0].Value);
			while( (ftell(File.GetFile()) % set_align) != 0 )
			{
				fputc( 0x00, File.GetFile() );
			}
		}
		return true;


	case CMD_EMBCLEAR:
		{
			EmbPtrs.Erase();
		}
		return true;

	
	case CMD_SAVE_INDEX:
		{
			fp = fopen( Cmd.Parameters[0].Value.c_str(), "w" );
			fprintf( fp,"%X %X",index_number, index_bump );
			fclose( fp );
		}
		return true;


	case CMD_LOAD_INDEX:
		{
			fp = fopen( Cmd.Parameters[0].Value.c_str(), "r" );
			if( !fp )
			{
				printf( "File not found @ %s\n", Cmd.Parameters[0].Value.c_str() );
				return false;
			}
			fscanf( fp,"%X %X", &index_number, &index_bump );
			fclose( fp );
			return true;
		}


	case CMD_W08_BYTE_FILE:
		{
			int value, ptr;
			FILE *fp;
			
			ptr = StringToUInt(Cmd.Parameters[0].Value);
			value = StringToUInt(Cmd.Parameters[1].Value);

			fp = fopen( Cmd.Parameters[2].Value.c_str(), "rb+" );
			if(!fp)
			{
				printf( "\n[W08_BYTE] File not found - %s", Cmd.Parameters[2].Value.c_str() );
				return true;
			}

			// write one byte TO FILE
			fseek(fp,ptr,SEEK_SET);
			fputc(value,fp);
			
			fclose(fp);
		}
		return true;


	case CMD_WRITE_INDEX_FILE:
		{
			int ptr, bump;
			FILE *fp;
			
			ptr = StringToUInt(Cmd.Parameters[0].Value);
			bump = StringToUInt(Cmd.Parameters[1].Value);

			fp = fopen( Cmd.Parameters[2].Value.c_str(), "rb+" );
			if(!fp)
			{
				printf( "\n[WRITE_INDEX] File not found - %s", Cmd.Parameters[2].Value.c_str() );
				return true;
			}

			fseek(fp,ptr,SEEK_SET);

			// write LSB index TO FILE
			if(index_size>=1) fputc((index_number>>0)&0xff,fp);
			if(index_size>=2) fputc((index_number>>8)&0xff,fp);
			if(index_size>=3) fputc((index_number>>16)&0xff,fp);
			if(index_size>=4) fputc((index_number>>24)&0xff,fp);

			index_number += bump;
			fclose( fp );
		}
		return true;


	case CMD_FILL_FILE:
		{
			int value, start, stop;
			FILE *fp;
			
			start = StringToUInt(Cmd.Parameters[0].Value);
			stop = StringToUInt(Cmd.Parameters[1].Value);
			value = StringToUInt(Cmd.Parameters[2].Value);

			// Open target file
			fp = fopen( Cmd.Parameters[3].Value.c_str(), "rb+" );
			if(!fp)
			{
				printf( "\n[FILL] File not found - %s", Cmd.Parameters[3].Value.c_str() );
				return true;
			}

			// Pad write
			fseek(fp,start,SEEK_SET);
			while( ftell(fp) < stop )
			{
				fputc( value, fp );
			}
			fclose( fp );
		}
		return true;


	case CMD_EMBSET_REL:
		PtrNum = StringToUInt(Cmd.Parameters[0].Value);
		Success = EmbPtrs.SetPointerPosition(PtrNum, File.GetPos());
		Size = EmbPtrs.GetSize(PtrNum);
		if(Size == -1)
			return false;
		Logger.Log("%6u EMBSET    Pointer Position %u set to $%X\n", Cmd.Line, PtrNum, File.GetPos());
		if(Success) // Write out embedded pointer
		{
			PtrValue = EmbPtrs.GetPointerValueOffset(PtrNum);

			if(File.GetMaxWritableBytes() > Size / 8)
			{
				File.Write(&PtrValue, Size / 8, 1, File.GetPos());
				Logger.Log("%6u EMBSET    Triggered Write: ScriptPos $%X PointerPos $%X PointerValue $%X Size %dd", Cmd.Line,
					EmbPtrs.GetTextPosition(PtrNum), File.GetPos(), PtrValue, Size);
				File.Move(File.GetPos() + Size/8);
				Stats.IncEmbPointerWrites();
			}
			else
				Logger.Log("%6u EMBSET    Failed to write due to insufficient space\n");
		}
		else // Reserve space so the embedded pointer and script don't compete
		{    // for the same part of the file
			if(File.GetMaxWritableBytes() > Size / 8)
			{
				unsigned int Zero = 0;
				File.Write(&Zero, Size/8, 1, File.GetPos());
				File.Move(File.GetPos() + Size/8);
			}
			else
				Logger.Log("%6u EMBSET    Failed to write due to insufficient space\n");
		}
		return true;

	
	case CMD_EMBWRITE_REL:
		PtrNum = StringToUInt(Cmd.Parameters[0].Value);
		Success = EmbPtrs.SetTextPosition(PtrNum, File.GetPos());
		Size = EmbPtrs.GetSize(PtrNum);
		if(Size == -1)
			return false;
		Logger.Log("%6u EMBWRITE  Pointed Position %u set to $%X\n", Cmd.Line, PtrNum, File.GetPos());
		if(Success) // Write out embedded pointer
		{
			PtrPos = EmbPtrs.GetPointerPosition(PtrNum);
			PtrValue = EmbPtrs.GetPointerValueOffset(PtrNum);

			if(File.GetMaxWritableBytes() > Size / 8)
			{
				File.Write(&PtrValue, Size/8, 1, PtrPos);
				Logger.Log("%6u EMBWRITE  Triggered Write: ScriptPos $%X PointerPos $%X PointerValue $%X Size %dd\n", Cmd.Line,
					File.GetPos(), PtrPos, PtrValue, Size);
				Stats.IncEmbPointerWrites();
			}
			else
				Logger.Log("%6u EMBWRITE  Failed to write due to insufficient space\n");
		}
		return true;


	case CMD_SAVE_PTRTABLE:
		{
			FILE *fp;

			PtrValue = PtrHandler.GetTableAddress(Cmd.Parameters[0].Value, File.GetPos(), Size, PtrPos);
			if(PtrValue == -1)
				return false;


			// Open target file
			fp = fopen( Cmd.Parameters[1].Value.c_str(), "w" );
			if(!fp)
			{
				printf( "\n[FILL] File not found - %s", Cmd.Parameters[1].Value.c_str() );
				return true;
			}

			// Write PTRTABLE pos
			fprintf( fp,"%X",PtrPos );
			fclose( fp );
		}
		return true;


	case CMD_LOAD_PTRTABLE:
		{
			FILE *fp;
			
			// Open target file
			fp = fopen( Cmd.Parameters[1].Value.c_str(), "r" );
			if(!fp)
			{
				printf( "\n[FILL] File not found - %s", Cmd.Parameters[1].Value.c_str() );
				return true;
			}

			// Read PTRTABLE pos
			fscanf( fp,"%X",&PtrPos );
			fclose( fp );


			PtrValue = PtrHandler.SetTableAddress(Cmd.Parameters[0].Value, PtrPos);
			if(PtrValue == -1)
				return false;
		}
		return true;


	case CMD_FILL_RANGE:
		{
			int value, start, stop;
			int seek;
			
			start = StringToUInt(Cmd.Parameters[0].Value);
			stop = StringToUInt(Cmd.Parameters[1].Value);
			value = StringToUInt(Cmd.Parameters[2].Value);

			// Pad write
			seek = ftell(File.GetFile());
			fseek(File.GetFile(),start,SEEK_SET);
			while( File.GetPos() < stop )
			{
				fputc( value, File.GetFile() );
			}
			fseek(File.GetFile(),seek,SEEK_SET);
		}
		return true;


	case CMD_WRITEPTR_OFFSET:
		PtrValue = PtrHandler.GetTableAddress(Cmd.Parameters[0].Value, File.GetPos(), Size, PtrPos);
		PtrHandler.SetTableAddress(Cmd.Parameters[0].Value, PtrPos);

		PtrPos += StringToUInt(Cmd.Parameters[1].Value);
		if(PtrValue == -1)
			return false;
		
		File.Write(&PtrValue, Size/8, 1, PtrPos);
		Logger.Log("%6u WRITE     CustomPointer '%s' ScriptPos $%X PointerPos $%X PointerValue $%08X\n", Cmd.Line,
			Cmd.Parameters[0].Value.c_str(), File.GetPos(), PtrPos, PtrValue);
		return true;

		
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

	default:
		Logger.BugReport(__LINE__, __FILE__, "Bad Cmd #%u", Cmd.Function);
		return false;
	}
}

bool AtlasCore::ActivateTable(std::string& TableName)
{
	Table* Tbl = (Table*)(VarMap.GetVar(TableName))->GetData();
	if(Tbl == NULL)
	{
		ostringstream ErrorStr;
		ErrorStr << "Uninitialized variable " << TableName << " used";
		Logger.ReportError(CurrentLine, "Uninitialized variable '%s' used", TableName);
		return false;
	}
	else
	{
		File.SetTable(Tbl);
		return true;
	}
}

bool AtlasCore::AddTable(Command& Cmd)
{
	Table* Tbl;
	GenericVariable* Var;
	Tbl = (Table*)(VarMap.GetVar(Cmd.Parameters[1].Value))->GetData();
	if(!LoadTable(Cmd.Parameters[0].Value, &Tbl))
		return false;

	Var = new GenericVariable(Tbl, P_TABLE);
	VarMap.SetVar(Cmd.Parameters[1].Value, Var);
	return true;
}

bool AtlasCore::LoadTable(std::string& FileName, Table** Tbl)
{
	if(*Tbl != NULL) // Initialized already, overwrite
	{
		delete Tbl;
		Tbl = NULL;
	}
	*Tbl = new Table;
    int Result = (*Tbl)->OpenTable(FileName.c_str());

	ostringstream ErrorStr;
	switch(Result)
	{
	case TBL_OK:
		break;
	case TBL_PARSE_ERROR:
		Logger.ReportError(CurrentLine, "The table file '%s' is incorrectly formatted", FileName);
		return false;
	case TBL_OPEN_ERROR:
		Logger.ReportError(CurrentLine, "The table file '%s' could not be opened", FileName);
		return false;
	}

	return true;
}


void AtlasCore::CreateContext(AtlasContext** Context)
{
	if(*Context == NULL)
		*Context = new AtlasContext;
	(*Context)->CurrentLine = CurrentLine;
	(*Context)->ScriptPos = File.GetPos();
	(*Context)->ScriptRemaining = File.GetMaxWritableBytes();
	(*Context)->Target = File.GetFile();
	File.GetScriptBuf((*Context)->StringTable);
	(*Context)->PointerPosition = 0;
	(*Context)->PointerSize = 0;
	(*Context)->PointerValue = 0;
}


bool AtlasCore::ExecuteExtension(std::string& ExtId, std::string& FunctionName,
										 AtlasContext** Context)
{
	bool Success = false;
	CreateContext(Context);
	unsigned int DllRet = Extensions.ExecuteExtension(ExtId, FunctionName, Context);
	if(DllRet == -1)
	{
		DllRet = NO_ACTION;
		Success = false;
	}
	if(DllRet > MAX_RETURN_VAL)
	{
		Logger.ReportWarning(CurrentLine, "Extension returned invalid value %u", DllRet);
		Success = false;;
	}
	if(DllRet & REPLACE_TEXT)
	{
		File.SetScriptBuf((*Context)->StringTable);
		Logger.Log("%6u EXECEXT   REPLACE_TEXT\n", CurrentLine);
		Success = true;
	}
	if(DllRet & WRITE_POINTER)
	{
		unsigned int Size = (*Context)->PointerSize;
		if(Size == 8 || Size == 16 || Size == 24 || Size == 32)
		{
			Size /= 8;
			File.Write(&(*Context)->PointerValue, (*Context)->PointerSize, 1, (*Context)->PointerPosition);
			Logger.Log("%6u EXECEXT   WRITE_POINTER ScriptPos $%X PointerPos $%X PointerValue $%06X\n", CurrentLine,
				File.GetPos(), (*Context)->PointerPosition, (*Context)->PointerValue);
			Success = true;
			Stats.IncExtPointerWrites();
		}
		else
		{
			Logger.ReportError(CurrentLine, "EXTEXEC   Extension function '%s' returning WRITE_POINTER has an unsupported PointerSize field", FunctionName);
			Success = false;
		}
	}

	delete (*Context);
	*Context = NULL;
	Logger.Log("%6u EXTEXEC   Executed function '%s' from '%s' successfully\n", CurrentLine, FunctionName.c_str(), ExtId.c_str());
	return true;
}

bool AtlasCore::ExecuteExtensionFunction(ExtensionFunction Func, AtlasContext** Context)
{
	unsigned int DllRet = Func(Context);
	bool Success = false;
	if(DllRet > MAX_RETURN_VAL)
	{
		Logger.ReportWarning(CurrentLine, "Extension returned invalid value %u", DllRet);
		Success = false;
	}

	if(DllRet & REPLACE_TEXT)
	{
		File.SetScriptBuf((*Context)->StringTable);
		Logger.Log("%6u EXECEXT   REPLACE_TEXT\n", CurrentLine);
		Success = true;
	}
	if(DllRet & WRITE_POINTER)
	{
		unsigned int Size = (*Context)->PointerSize;
		if(Size == 8 || Size == 16 || Size == 24 || Size == 32)
		{
			Size /= 8;
			File.Write(&(*Context)->PointerValue, (*Context)->PointerSize, 1, (*Context)->PointerPosition);
			Logger.Log("%6u EXTEXEC   WRITE_POINTER ScriptPos $%X PointerPos $%X PointerValue $%06X\n", CurrentLine,
				File.GetPos(), (*Context)->PointerPosition, (*Context)->PointerValue);
			Success = true;
			Stats.IncExtPointerWrites();
		}
		else
		{
			Logger.ReportError(CurrentLine, "EXTEXEC   Extension function returning WRITE_POINTER has an unsupported PointerSize field");
			Success = false;
		}
	}

	delete (*Context);
	*Context = NULL;
	return true;
}

//-----------------------------------------------------------------------------
// StringToUInt() - Converts a $ABCD string from hexadecimal radix else decimal
// Status - Working
//-----------------------------------------------------------------------------

unsigned int StringToUInt(std::string& NumberString)
{
	unsigned int offset = 0;

	if(NumberString[0] == '$')
	{
		offset = strtoul(NumberString.substr(1, NumberString.length()).c_str(), NULL, 16);
	}
	else
		offset = strtoul(NumberString.c_str(), NULL, 10);

	return offset;
}

//-----------------------------------------------------------------------------
// StringToInt64() - Converts a string to int64
// Status - Works+Fixed
//-----------------------------------------------------------------------------

__int64 StringToInt64(string& NumberString)
{
	__int64 Num = 0;
	bool bNeg = false;
	size_t Pos = 0;
	unsigned __int64 Mult;

	if(NumberString[Pos] == '$') // hex
	{
		Pos++;
		if(NumberString[Pos] == '-')
		{
			bNeg = true;
			Pos++;
		}
		size_t i = NumberString.length() - 1;
		Num += GetHexDigit(NumberString[i]);
		i--;
		Mult = 16;
		for(i; i >= Pos; i--, Mult*=16)
			Num += Mult * GetHexDigit(NumberString[i]);
	}
	else // dec
	{
		if(NumberString[Pos] == '-')
		{
			bNeg = true;
			Pos++;
		}
		size_t i = NumberString.length() - 1;
		Num += GetHexDigit(NumberString[i]);
		if(i != 0)
		{
			i--;
			Mult = 10;
			for(i; i > Pos; i--, Mult*=10)
				Num += Mult * (NumberString[i] - '0');
			Num += Mult * (NumberString[i] - '0'); // prevent underflow of i
		}
	}

	if(bNeg)
		Num = -Num;
	return Num;
}

unsigned int GetHexDigit(char digit)
{
	switch(digit)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A': case 'a':
		return 10;
	case 'B': case 'b':
		return 11;
	case 'C': case 'c':
		return 12;
	case 'D': case 'd':
		return 13;
	case 'E': case 'e':
		return 14;
	case 'F': case 'f':
		return 15;
	default:
		return 0;
	}
}