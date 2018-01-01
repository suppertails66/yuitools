#pragma once

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
//#include <hash_map>
#include "Table.h"
#include "GenericVariable.h"
#include "AtlasParser.h"
#include "AtlasFile.h"
#include "Pointer.h"
#include "AtlasLogger.h"
#include "AtlasStats.h"
#include "PointerHandler.h"
#include "AtlasExtension.h"


//-----------------------------------------------------------------------------
// AtlasCore Functionality
//-----------------------------------------------------------------------------

class AtlasCore
{
public:
	AtlasCore();
	~AtlasCore();

	bool Insert(const char* RomFileName, const char* ScriptFileName);
	void SetDebugging(FILE* output);
	void CreateContext(AtlasContext** Context);
	bool ExecuteExtension(std::string& ExtId, std::string& FunctionName, AtlasContext** Context);
	bool ExecuteExtensionFunction(ExtensionFunction Func, AtlasContext** Context);

private:
	AtlasParser Parser;
	AtlasFile File;
	VariableMap VarMap; // Variable Map for identifiers
	PointerHandler PtrHandler;
	Pointer DefaultPointer;
	EmbeddedPointerHandler EmbPtrs;
	InsertionStatistics Total;
	ExtensionManager Extensions;
	
	bool IsInJmp;
	unsigned int HeaderSize;
	
	bool ExecuteCommand(Command& Cmd);

	void PrintSummary(const char* Title, unsigned int TimeCompleted);
	void PrintStatistics();
	void PrintStatisticsBlock(const char* Title, InsertionStatistics& Stats);

	bool AddTable(Command& Cmd);
	bool ActivateTable(std::string& TableName);
	bool LoadTable(std::string& FileName, Table** Tbl);
};

// Global Core variables
extern unsigned int CurrentLine;
extern AtlasCore Atlas;

// Misc functions
inline unsigned int StringToUInt(std::string& NumberString);
__int64 StringToInt64(std::string& NumberString);
unsigned int GetHexDigit(char digit);