#pragma once
#include <cstdio>
#include <string>
#include <map>
#include <iterator>
#include "Table.h"
#include "PointerHandler.h"
#include "AtlasExtension.h"

static const unsigned int STR_ENDTERM = 0;
static const unsigned int STR_PASCAL = 1;
static const unsigned int StringTypeCount = 2;
static const char* StringTypes[StringTypeCount] = { "ENDTERM", "PASCAL" };

class AtlasFile
{
public:
	AtlasFile();
	~AtlasFile();
	bool AutoWrite(PointerList* List, std::string& EndTag);
	bool AutoWrite(PointerTable* Tbl, std::string& EndTag);
	bool AutoWrite(AtlasExtension* Ext, std::string& FuncName, std::string& EndTag);
	bool AtlasFile::DisableAutoExtension(std::string& FuncName, std::string& EndTag);
	bool DisableWrite(std::string& EndTag, bool isPointerTable);
	bool OpenFile(const char* FileName);
	unsigned int GetMaxBound() { return MaxScriptPos; }
	unsigned int GetBytesInserted() { return BytesInserted; }
	unsigned int GetBytesOverflowed() { return TotalBytesSkipped; }
	void Move(const unsigned int Pos, const unsigned int ScriptBound);
	void Move(const unsigned int Pos);
	void Write(const void* Data, const unsigned int Size, const unsigned int DataCount, const unsigned int Pos);
	unsigned int GetPos();
	void SetTable(Table* Tbl);
	bool SetStringType(std::string& Type);
	bool SetPascalLength(unsigned int Length);
	bool InsertText(std::string& Text, unsigned int Line);
	bool FlushText();

	unsigned int GetMaxWritableBytes();
	FILE* GetFile();
	void GetScriptBuf(std::list<TBL_STRING>& Strings);
	void SetScriptBuf(std::list<TBL_STRING>& Strings);
	unsigned int GetStringType();

private:
	FILE* file;
	unsigned int Filesize;
	Table* ActiveTbl;
	PointerHandler* PtrHandler;
	std::map<std::string, PointerList*> ListAutoWrite;
	std::map<std::string, PointerTable*> TblAutoWrite;
	std::map<std::string, ExtensionFunction> ExtAutoWrite;
	std::map<std::string, PointerList*>::iterator ListIt;
	std::map<std::string, PointerTable*>::iterator TblIt;
	std::map<std::string, ExtensionFunction>::iterator ExtIt;

	inline bool WriteString(std::string& text);
	inline bool WritePascalString(std::string& text);

	unsigned int MaxScriptPos;
	unsigned int BytesInserted;
	unsigned int TotalBytesSkipped;
	unsigned int TotalBytes;

	unsigned int StrType;
	unsigned int PascalLength;
};