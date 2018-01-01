#include "stdafx.h"
#include <cstdio>
#include <string>
#include "AtlasFile.h"
#include "Table.h"
#include "AtlasLogger.h"
#include "AtlasStats.h"
#include "AtlasCore.h"
#include "AtlasExtension.h"

using namespace std;

AtlasFile::AtlasFile()
{
	file = NULL;
	Filesize = 0;

	MaxScriptPos = -1;
	ActiveTbl = NULL;
	BytesInserted = 0;
	TotalBytes = 0;
	TotalBytesSkipped = 0;

	StrType = STR_ENDTERM;
	PascalLength = 1;
}

AtlasFile::~AtlasFile()
{
	if(file != NULL)
		fclose(file);
}

bool AtlasFile::OpenFile(const char* FileName)
{
	file = fopen(FileName, "r+b");
	if(file == NULL)
		return false;
	else
	{
		fseek(file, 0, SEEK_END);
		Filesize = ftell(file);
		fseek(file, 0, SEEK_SET);
		return true;
	}
}

void AtlasFile::Write(const void* Data, const unsigned int Size, 
					  const unsigned int DataCount, const unsigned int Pos)
{
	unsigned int OldPos = ftell(file);
	fseek(file, Pos, SEEK_SET);
	fwrite(Data, Size, DataCount, file);
	fseek(file, OldPos, SEEK_SET);
}

unsigned int AtlasFile::GetPos()
{
	return ftell(file);
}

FILE* AtlasFile::GetFile()
{
	return file;
}

void AtlasFile::GetScriptBuf(std::list<TBL_STRING>& Strings)
{
	Strings = ActiveTbl->StringTable;
}

void AtlasFile::SetScriptBuf(std::list<TBL_STRING>& Strings)
{
	ActiveTbl->StringTable = Strings;
}

unsigned int AtlasFile::GetStringType()
{
	return StrType;
}

void AtlasFile::Move(const unsigned int Pos, const unsigned int ScriptBound)
{
	if(file)
		fseek(file, Pos, SEEK_SET);
	MaxScriptPos = ScriptBound;
}

void AtlasFile::Move(const unsigned int Pos)
{
	if(file)
		fseek(file, Pos, SEEK_SET);
}

void AtlasFile::SetTable(Table* Tbl)
{
	FlushText();
	ActiveTbl = Tbl;
}

bool AtlasFile::SetStringType(std::string& Type)
{
	for(int i = 0; i < StringTypeCount; i++)
	{
		if(Type == StringTypes[i])
		{
			StrType = i;
			return true;
		}
	}

	return false;
}

bool AtlasFile::SetPascalLength(unsigned int Length)
{
	switch(Length)
	{
	case 1: case 2: case 3: case 4:
		PascalLength = Length;
		break;
	default:
		return false;
	}
	return true;
}

bool AtlasFile::DisableWrite(string& EndTag, bool isPointerTable)
{
	if(isPointerTable)
	{
		std::map<string, PointerTable*>::iterator it;
		it = TblAutoWrite.find(EndTag);
		if(it == TblAutoWrite.end())
			return false;
		TblAutoWrite.erase(it);
	}
	else
	{
		std::map<string, PointerList*>::iterator it;
		it = ListAutoWrite.find(EndTag);
		if(it == ListAutoWrite.end())
			return false;
		ListAutoWrite.erase(it);
	}
	return true;
}

bool AtlasFile::DisableAutoExtension(string& FuncName, string& EndTag)
{
	std::map<string, ExtensionFunction>::iterator it;
	it = ExtAutoWrite.find(EndTag);
	if(it == ExtAutoWrite.end())
	{
		Logger.ReportError(CurrentLine, "'%s' has not been defined as an autoexec end token", EndTag);
		return false;
	}
	ExtAutoWrite.erase(it);
	return true;
}

bool AtlasFile::AutoWrite(PointerList* List, string& EndTag)
{
	bool EndTokenFound = false;
	for(size_t i = 0; i < ActiveTbl->EndTokens.size(); i++)
	{
		if(EndTag == ActiveTbl->EndTokens[i])
			EndTokenFound = true;
	}
	if(EndTokenFound)
		ListAutoWrite.insert(map<string,PointerList*>::value_type(EndTag, List));
	return EndTokenFound;
}

bool AtlasFile::AutoWrite(PointerTable* Tbl, string& EndTag)
{
	bool EndTokenFound = false;
	for(size_t i = 0; i < ActiveTbl->EndTokens.size(); i++)
	{
		if(EndTag == ActiveTbl->EndTokens[i])
			EndTokenFound = true;
	}
	if(EndTokenFound)
		TblAutoWrite.insert(map<string,PointerTable*>::value_type(EndTag, Tbl));
	return EndTokenFound;
}

bool AtlasFile::AutoWrite(AtlasExtension* Ext, string& FuncName, string& EndTag)
{
	bool EndTokenFound = false;
	ExtensionFunction Func;

	for(size_t i = 0; i < ActiveTbl->EndTokens.size(); i++)
	{
		if(EndTag == ActiveTbl->EndTokens[i])
			EndTokenFound = true;
	}
	Func = Ext->GetFunction(FuncName);
	if(!EndTokenFound)
	{
		Logger.ReportError(CurrentLine, "'%s' has not been defined as an end token in the active table", EndTag);
		return false;
	}
	if(Func == NULL)
	{
		Logger.ReportError(CurrentLine, "Function 's' could not be found in the extension", FuncName);
		return false;
	}

	ExtAutoWrite.insert(map<string,ExtensionFunction>::value_type(EndTag, Func));
	return true;
}

bool AtlasFile::InsertText(string& Text, unsigned int Line)
{
	if(ActiveTbl == NULL)
	{
		// Add error
		printf("No active table loaded\n");
		return false;
	}
	unsigned int BadCharPos = 0;
	if(ActiveTbl->EncodeStream(Text, BadCharPos) == -1) // Failed insertion, char missing from tbl
	{
		// Add error
		Logger.ReportError(Line, "Character '%c' missing from table", Text[BadCharPos]);
		Logger.ReportError(Line, "%s", Text.c_str());
		return false;
	}
	return true;
}

bool AtlasFile::FlushText()
{
	static unsigned int Size;
	static unsigned int Address;
	static unsigned int WritePos;
	AtlasContext* Context = NULL;

	if(ActiveTbl == NULL)
		return false;
	if(ActiveTbl->StringTable.empty())
		return true;

	if(StrType == STR_ENDTERM)
	{
		for(ListTblStringIt i = ActiveTbl->StringTable.begin();
			i != ActiveTbl->StringTable.end(); i++)
		{
			if(!i->EndToken.empty()) // Autowrite
			{
				ListIt = ListAutoWrite.find(i->EndToken);
				if(ListIt != ListAutoWrite.end())
				{
					Address = ListIt->second->GetAddress(GetPos(), Size, WritePos);
					if(Address != -1)
					{
						Write(&Address, Size/8, 1, WritePos);
						Logger.Log("%6u AUTOWRITE Invoked ScriptPos $%X PointerPos $%X PointerValue $%08X\n", CurrentLine,
							GetPos(), WritePos, Address);
						Stats.IncAutoPointerWrites();
					}
					else
						Stats.IncFailedListWrites();
				}
				TblIt = TblAutoWrite.find(i->EndToken);
				if(TblIt != TblAutoWrite.end())
				{
					Address = TblIt->second->GetAddress(GetPos(), Size, WritePos);
					Write(&Address, Size/8, 1, WritePos);
					Logger.Log("%6u AUTOWRITE Invoked ScriptPos $%X PointerPos $%X PointerValue $%08X\n", CurrentLine,
						GetPos(), WritePos, Address);
					Stats.IncAutoPointerWrites();
				}
				ExtIt = ExtAutoWrite.find(i->EndToken);
				if(ExtIt != ExtAutoWrite.end())
				{
					Atlas.CreateContext(&Context);
					bool Success = Atlas.ExecuteExtensionFunction(ExtIt->second, &Context);
					delete Context;
					Context = NULL;
					if(!Success)
					{
						Logger.ReportError(CurrentLine, "Autoexecuting extension with end token '%s' failed", i->EndToken);
						return false;
					}
					else
					Logger.Log("%6u AUTOWRITE Invoked ScriptPos $%X PointerPos $%X PointerValue $%08X\n", CurrentLine,
						GetPos(), WritePos, Address);
				}
			}

			WriteString(i->Text);
		}
	}
	else if(StrType == STR_PASCAL)
	{
		for(ListTblStringIt i = ActiveTbl->StringTable.begin();
			i != ActiveTbl->StringTable.end(); i++)
		{
			if(!i->EndToken.empty()) // Autowrite
			{
				ListIt = ListAutoWrite.find(i->EndToken);
				if(ListIt != ListAutoWrite.end())
				{
					Address = ListIt->second->GetAddress(GetPos(), Size, WritePos);
					if(Address != -1)
					{
						Write(&Address, Size/8, 1, WritePos);
						Logger.Log("%6u AUTOWRITE Invoked ScriptPos $%X PointerPos $%X PointerValue $%08X\n", CurrentLine,
							GetPos(), WritePos, Address);
						Stats.IncAutoPointerWrites();
					}
					else
						Stats.IncFailedListWrites();
				}
				TblIt = TblAutoWrite.find(i->EndToken);
				if(TblIt != TblAutoWrite.end())
				{
					Address = ListIt->second->GetAddress(GetPos(), Size, WritePos);
					Write(&Address, Size/8, 1, WritePos);
					Logger.Log("%6u AUTOWRITE Invoked ScriptPos $%X PointerPos $%X PointerValue $%08X\n", CurrentLine,
						GetPos(), WritePos, Address);
					Stats.IncAutoPointerWrites();
				}
			}

			WritePascalString(i->Text);
		}
	}
	else
	{
		// Nonexisting type...bug
		Logger.BugReportLine(__LINE__, __FILE__, "Nonexistant string type in AtlasFile::FlushText");
	}
	ActiveTbl->StringTable.clear();

	return true;
}

inline bool AtlasFile::WriteString(string& text)
{
	unsigned int size = (unsigned int)text.length();
	unsigned int maxwrite = GetMaxWritableBytes();

	Stats.AddScriptBytes(size);

	if(maxwrite < size)
	{
		TotalBytesSkipped += (size - maxwrite);
		size = maxwrite;
	}

	fwrite(text.data(), 1, size, file);
	BytesInserted += size;

	return true;
}

inline bool AtlasFile::WritePascalString(string& text)
{
	unsigned int size = (unsigned int)text.length();
	unsigned int maxwrite = GetMaxWritableBytes();

	Stats.AddScriptBytes(size);

	if(maxwrite < size)
	{
		TotalBytesSkipped += (size - maxwrite);
		size = maxwrite;
	}

	fwrite(&size, PascalLength, 1, file);
	fwrite(text.c_str(), 1, size, file);
	BytesInserted += PascalLength + size;

	return true;
}

unsigned int AtlasFile::GetMaxWritableBytes()
{
	unsigned int CurPos = ftell(file);
	if(CurPos >= MaxScriptPos)
		return 0;
	return MaxScriptPos - CurPos;
}