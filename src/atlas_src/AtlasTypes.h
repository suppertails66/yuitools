#pragma once

#include <map>
#include <string>

/* Misc Functions */
static const unsigned int CMD_JMP1 = 0;
static const unsigned int CMD_JMP2 = 1;
static const unsigned int CMD_SMA = 2;
static const unsigned int CMD_HDR = 3;
static const unsigned int CMD_STRTYPE = 4;
static const unsigned int CMD_ADDTBL = 5;
static const unsigned int CMD_ACTIVETBL = 6;
static const unsigned int CMD_VAR = 7;
/* Pointer Functions */
static const unsigned int CMD_WUB = 8;
static const unsigned int CMD_WBB = 9;
static const unsigned int CMD_WHB = 10;
static const unsigned int CMD_WLB = 11;
static const unsigned int CMD_W16 = 12;
static const unsigned int CMD_W24 = 13;
static const unsigned int CMD_W32 = 14;
static const unsigned int CMD_EMBSET = 15;
static const unsigned int CMD_EMBTYPE = 16;
static const unsigned int CMD_EMBWRITE = 17;
/* Debugging Functions */
static const unsigned int CMD_BREAK = 18;
/* Extended Pointer Functionality */
static const unsigned int CMD_PTRTBL = 19;
static const unsigned int CMD_WRITETBL = 20;
static const unsigned int CMD_PTRLIST = 21;
static const unsigned int CMD_WRITELIST = 22;
static const unsigned int CMD_AUTOWRITETBL = 23;
static const unsigned int CMD_AUTOWRITELIST = 24;
static const unsigned int CMD_CREATEPTR = 25;
static const unsigned int CMD_WRITEPTR = 26;

static const unsigned int CMD_LOADEXT = 27;
static const unsigned int CMD_EXECEXT = 28;
static const unsigned int CMD_DISABLETABLE = 29;
static const unsigned int CMD_DISABLELIST = 30;
static const unsigned int CMD_PASCALLEN = 31;
static const unsigned int CMD_AUTOEXEC = 32;
static const unsigned int CMD_DISABLEEXEC = 33;

static const unsigned int CMD_INSERT = 34;
static const unsigned int CMD_WARN = 35;
static const unsigned int CMD_FILL = 36;
static const unsigned int CMD_W08_BYTE = 37;
static const unsigned int CMD_SAVE_PC = 38;
static const unsigned int CMD_LOAD_PC = 39;
static const unsigned int CMD_SET_INDEX = 40;
static const unsigned int CMD_WRITE_INDEX = 41;
static const unsigned int CMD_ALIGN = 42;
static const unsigned int CMD_EMBCLEAR = 43;
static const unsigned int CMD_SAVE_INDEX = 44;
static const unsigned int CMD_LOAD_INDEX = 45;
static const unsigned int CMD_W08_BYTE_FILE = 46;
static const unsigned int CMD_WRITE_INDEX_FILE = 47;
static const unsigned int CMD_FILL_FILE = 48;
static const unsigned int CMD_EMBSET_REL = 49;
static const unsigned int CMD_EMBWRITE_REL = 50;
static const unsigned int CMD_SAVE_PTRTABLE = 51;
static const unsigned int CMD_LOAD_PTRTABLE = 52;
static const unsigned int CMD_FILL_RANGE = 53;
static const unsigned int CMD_WRITEPTR_OFFSET = 54;

static const unsigned int CommandCount = CMD_WRITEPTR_OFFSET+1;

static const char* CommandStrings[CommandCount] = { "JMP", "JMP", "SMA", "HDR", "STRTYPE",
	"ADDTBL", "ACTIVETBL", "VAR", "WUB", "WBB", "WHB", "WLB", "W16", "W24", "W32",
	"EMBSET", "EMBTYPE", "EMBWRITE", "BREAK", "PTRTBL",
	"WRITE", "PTRLIST", "WRITE", "AUTOWRITE", "AUTOWRITE", "CREATEPTR", "WRITE",
	"LOADEXT", "EXECEXT", "DISABLE", "DISABLE", "PASCALLEN", "AUTOEXEC", "DISABLE",
	
	"INSERT", "WARN", "FILL",
	"W08BYTE", "SAVEPC", "LOADPC",
	"SETINDEX", "WRITEINDEX",
	"ALIGN", "EMBCLEAR",
	"SAVEINDEX", "LOADINDEX",
	"W08BYTE", "WRITEINDEX",
	"FILL", "EMBSETREL", "EMBWRITEREL",
	"SAVEPTRTABLE", "LOADPTRTABLE",
	"FILL",
	"WRITEOFS",
};

// Parameter types
static const unsigned int TypeCount = 11;

static const unsigned int P_INVALID = 0;
static const unsigned int P_VOID = 1;
static const unsigned int P_STRING = 2;
static const unsigned int P_VARIABLE = 3;
static const unsigned int P_NUMBER = 4;
static const unsigned int P_DOUBLE = 5;
static const unsigned int P_TABLE = 6;
static const unsigned int P_POINTERTABLE = 7;
static const unsigned int P_POINTERLIST = 8;
static const unsigned int P_CUSTOMPOINTER = 9;
static const unsigned int P_EXTENSION = 10;

static const char* TypeStrings[TypeCount] = { "INVALID", "VOID", "STRING", "VARIABLE",
	"NUMBER", "DOUBLE", "TABLE", "POINTERTABLE", "POINTERLIST", "CUSTOMPOINTER", "EXTENSION" };

static const unsigned int Types[CommandCount][5] = { { P_NUMBER }, { P_NUMBER, P_NUMBER }, // JMP1 JMP2
{ P_STRING }, { P_NUMBER }, { P_STRING }, { P_STRING, P_TABLE }, { P_TABLE }, // SMA HDR STRTYPE ADDTBL ACTIVETBL
{ P_VARIABLE, P_VARIABLE }, { P_NUMBER }, {P_NUMBER }, // VAR WUB WBB
{ P_NUMBER }, { P_NUMBER }, { P_NUMBER }, { P_NUMBER }, { P_NUMBER }, // WHB WLB W16 W24 W32
{ P_NUMBER }, { P_STRING, P_NUMBER, P_NUMBER }, { P_NUMBER }, // EMBSET EMBTYPE EMBWRITE
{ P_VOID }, // BREAK
{ P_POINTERTABLE, P_NUMBER, P_NUMBER, P_CUSTOMPOINTER }, { P_POINTERTABLE }, // PTRTBL WRITE
{ P_POINTERLIST, P_STRING, P_CUSTOMPOINTER }, { P_POINTERLIST }, // PTRLIST WRITE
{ P_POINTERTABLE, P_STRING }, { P_POINTERLIST, P_STRING }, // AUTOWRITE AUTOWRITE
{ P_CUSTOMPOINTER, P_STRING, P_NUMBER, P_NUMBER }, { P_CUSTOMPOINTER, P_NUMBER }, // CREATEPTR WRITE
{ P_EXTENSION, P_STRING }, { P_EXTENSION, P_STRING }, // LOADEXT EXECEXT
{ P_POINTERTABLE, P_STRING }, { P_POINTERLIST, P_STRING }, // DISABLE DISABLE
{ P_NUMBER }, { P_EXTENSION, P_STRING, P_STRING }, // PASCALLEN AUTOEXEC
{ P_STRING, P_STRING }, // DISABLE

{ P_STRING, P_STRING }, { P_NUMBER, P_STRING }, { P_NUMBER, P_NUMBER }, // INSERT, WARN, FILL
{ P_NUMBER, P_NUMBER }, { P_STRING }, { P_STRING },	// W08_BYTE, SAVE_PC, LOAD_PC
{ P_NUMBER,P_NUMBER }, { P_NUMBER,P_NUMBER },	// SET_INDEX, WRITE_INDEX
{ P_NUMBER },	{ P_VOID },		// ALIGN, EMBCLEAR
{ P_STRING }, { P_STRING },	// SAVE_INDEX, LOAD_INDEX
{ P_NUMBER, P_NUMBER, P_STRING }, { P_NUMBER, P_NUMBER, P_STRING },	// W08_BYTE_FILE, WRITE_INDEX_FILE
{ P_NUMBER, P_NUMBER, P_NUMBER, P_STRING }, { P_NUMBER }, { P_NUMBER },			// FILL_FILE, EMBSET_REL, EMBWRITE_REL
{ P_POINTERTABLE, P_STRING }, { P_POINTERTABLE, P_STRING },			// SAVE_PTRTABLE, LOAD_PTRTABLE
{ P_NUMBER, P_NUMBER, P_NUMBER },		// FILL_RANGE
{ P_POINTERTABLE, P_NUMBER },		// WRITEOFS
};

static const unsigned int ParamCount[CommandCount] = { 1, 2, 1, // JMP1 JMP2 SMA
1, 1, 2, 1, 2, 1, 1, // HDR STRTYPE ADDTBL ACTIVETBL VAR WUB WBB
1, 1, 1, 1, 1, 1, 3, 1, // WHB WLB W16 W24 W32 EMBSET EMBTYPE EMBWRITE
1, // BREAK
4, 1, 3, 1, 2, 2, // PTRTBL WRITE PTRLIST WRITE AUTOWRITE AUTOWRITE
4, 2, 2, 2, 2, 2, 1, 3, 2, // CREATEPTR WRITE LOADEXT EXECEXT DISABLE DISABLE PASCALLEN AUTOEXEC DISABLE

2, 2, 2,	// INSERT, WARN, FILL
2, 1, 1,	// W08_BYTE, SAVE_PC, LOAD_PC
2, 2,		// SET_INDEX, WRITE_INDEX
1, 1,		// ALIGN, EMBCLEAR
1, 1,		// SAVE_INDEX, LOAD_INDEX
3, 3,		// W08_BYTE_FILE, WRITE_INDEX_FILE
4, 1, 1,	// FILL_FILE, EMBSET_REL, EMBWRITE_REL
2, 2,			// SAVE_PTRTABLE, LOAD_PTRTABLE
3,				// FILL_RANGE
2,				// WRITE_OFFSET
};

typedef struct Parameter
{
	std::string Value;
	unsigned int Type;
} Parameter;

typedef struct Command
{
	unsigned int Function;
	std::vector<Parameter> Parameters;
	unsigned int Line;
} Command;

typedef struct AtlasBlock
{
	std::list<Command> Commands;
	std::list<std::string> TextLines;
	unsigned int StartLine;
} AtlasBlock;

typedef std::multimap<std::string,unsigned int> StrCmdMap;
typedef std::multimap<std::string,unsigned int>::const_iterator StrCmdMapIt;

typedef std::map<std::string,unsigned int> StrTypeMap;
typedef std::map<std::string,unsigned int>::const_iterator StrTypeMapIt;

typedef std::vector<Parameter>::iterator ListParamIt;
typedef std::list<std::string>::iterator ListStringIt;
typedef std::list<Command>::iterator ListCmdIt;
typedef std::list<AtlasBlock>::iterator ListBlockIt;