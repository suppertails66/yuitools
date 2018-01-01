#pragma once
#include <list>
#include <string>
#include "Pointer.h"
#include "GenericVariable.h"
#include "notwindows.h"

class PointerHandler
{
public:
	PointerHandler(VariableMap* Map);
	bool CreatePointer(std::string& PtrId, std::string& AddressType,
		__int64 Offsetting,	unsigned int Size, unsigned int HeaderSize);
	unsigned int GetPtrAddress(std::string& PtrId, unsigned int ScriptPos, unsigned int& Size);
	bool CreatePointerList(std::string& ListId, const char* Filename, std::string& PtrId);
	bool CreatePointerTable(std::string& TblId, unsigned int Start, unsigned int Increment,
		std::string& PtrId);
	unsigned int GetListAddress(std::string& ListId, unsigned int ScriptPos,
		unsigned int& Size, unsigned int& WritePos);
	unsigned int GetTableAddress(std::string& TblId, unsigned int ScriptPos,
		unsigned int& Size,	unsigned int& WritePos);
	bool SetTableAddress(std::string& TblId, unsigned int WritePos);
private:
	VariableMap* Map;
};

class PointerList
{
public:
	PointerList();
	~PointerList();

	bool Create(const char* Filename, CustomPointer& CustPointer);
	unsigned int GetAddress(unsigned int TextPosition, unsigned int& Size, unsigned int& WritePos);
private:
	std::list<unsigned int> LocationList;
	std::list<unsigned int>::iterator LocationIt;
	unsigned int Location;
	CustomPointer Pointer;
};

class PointerTable
{
public:
	PointerTable();
	~PointerTable();
	bool Create(unsigned int Inc, unsigned int StartOffset, CustomPointer& CustPointer);
	unsigned int GetAddress(unsigned int TextPosition, unsigned int& Size, unsigned int& WritePos);
	bool SetAddress(unsigned int WritePos);

private:
	unsigned int Increment;
	unsigned int CurOffset;
	CustomPointer Pointer;
};
