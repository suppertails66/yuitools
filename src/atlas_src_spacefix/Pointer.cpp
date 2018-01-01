#include "stdafx.h"
#include <string>
#include "Pointer.h"
#include "AtlasLogger.h"

using namespace std;

Pointer::Pointer()
{
	AddressType = LINEAR;
	HeaderSize = 0;
}

Pointer::~Pointer()
{
}

bool Pointer::SetAddressType(string& Type)
{
	for(int i = 0; i < AddressTypeCount; i++)
	{
		if(Type == AddressTypes[i])
		{
			AddressType = i;
			return true;
		}
	}

	return false;
}

bool Pointer::SetAddressType(unsigned int Type)
{
	if(Type < AddressTypeCount)
	{
		AddressType = Type;
		return true;
	}
	else
		return false;
}

void Pointer::SetHeaderSize(const unsigned int Size)
{
	HeaderSize = Size;
}

unsigned int Pointer::GetAddress(const unsigned int Address) const
{
	return GetMachineAddress(Address);
}

unsigned int Pointer::GetMachineAddress(unsigned int Address) const
{
	Address -= HeaderSize;

	switch(AddressType)
	{
	case LINEAR:
		return Address;
	case LOROM00:
		return GetLoROMAddress(Address);
	case LOROM80:
		return GetLoROMAddress(Address) + 0x800000;
	case HIROM:
		return GetHiROMAddress(Address);
	case GB:
		return GetGBAddress(Address);
	case GB4xxx:
		return GetGB4xxxAddress(Address);
	case GIZMO:
		return Address;
	case MSB16:
		return GetMSBAddress(Address,16);
	case MSB24:
		return GetMSBAddress(Address,24);
	case MSB32:
		return GetMSBAddress(Address,32);
	default:
		return Address; // Error handling
	}
}

unsigned int Pointer::GetLoROMAddress(unsigned int Offset) const
{
	char bankbyte = (char) ((Offset & 0xFF0000) >> 16);
	unsigned short int Word = (unsigned short int) (Offset & 0xFFFF);
	unsigned int Address = 0;
	
	if(Word >= 0x8000)
		Address = bankbyte * 0x20000 + 0x10000 + Word;
	else
		Address = bankbyte * 0x20000 + Word + 0x8000;

	return Address;
}

unsigned int Pointer::GetHiROMAddress(unsigned int Offset) const
{
	unsigned int Address = 0;

	Address = Offset + 0xC00000;

	return Address;
}

unsigned int Pointer::GetGBAddress(unsigned int Offset) const
{
	unsigned int Address = 0;
	unsigned short int Bank = 0;
	unsigned short int Word = 0;

	Bank = Offset / 0x4000;
	Word = Offset % ((Bank+1) * 0x4000);

	Address = Bank * 0x10000 + Word;

	return Address;
}

unsigned int Pointer::GetGB4xxxAddress(unsigned int Offset) const
{
	unsigned int Address = 0;
	unsigned short int Bank = 0;
	unsigned short int Word = 0;

	Bank = Offset / 0x4000;
	Word = Offset % 0x4000;

	Address = Bank * 0x10000 + Word + 0x4000;

	return Address;
}

unsigned int Pointer::GetMSBAddress(unsigned int Offset, int size) const
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

unsigned char Pointer::GetUpperByte(const unsigned int ScriptPos) const
{
	return GetAddress(ScriptPos) & 0xFF000000;
}



// #WBB(param) - Working

unsigned char Pointer::GetBankByte(const unsigned int ScriptPos) const
{
	return GetAddress(ScriptPos) & 0xFF0000;
}



// #WHB(param) - Working

unsigned char Pointer::GetHighByte(const unsigned int ScriptPos) const
{
	return GetAddress(ScriptPos) & 0xFF00;
}



// #WLB(param) - Working

unsigned char Pointer::GetLowByte(const unsigned int ScriptPos) const
{
	return GetAddress(ScriptPos) & 0xFF;
}



// #W16(param) - Working

unsigned short Pointer::Get16BitPointer(const unsigned int ScriptPos) const
{
	return GetAddress(ScriptPos) & 0xFFFF;
}



// #W24(param) - Working

unsigned int Pointer::Get24BitPointer(const unsigned int ScriptPos) const
{
	return GetAddress(ScriptPos) & 0xFFFFFF;
}



// #W32 - Not tested

unsigned int Pointer::Get32BitPointer(const unsigned int ScriptPos) const
{
	return GetAddress(ScriptPos);
}


//--------------------------- Custom Pointer ----------------------------------
//                                                                           \\
//                                                                           \\

bool CustomPointer::Init(__int64 Offsetting, unsigned int Size, unsigned int HeaderSize)
{
	this->Offsetting = Offsetting;
	SetHeaderSize(HeaderSize);
	switch(Size)
	{
	case 8: case 16: case 24: case 32:
		this->Size = Size;
		break;
	default:
		return false;
	}
	return true;
}

unsigned int CustomPointer::GetSize()
{
	return Size;
}

unsigned int CustomPointer::GetAddress(const unsigned int Address) const
{
	unsigned int Val;

	if( AddressType == MSB16 || AddressType == MSB24 || AddressType == MSB32 )
	{
		Val = (unsigned int) ((__int64)GetMachineAddress(Address - Offsetting));
	}
	else
		Val = (unsigned int) ((__int64)GetMachineAddress(Address) - Offsetting);
	switch(Size)
	{
	case 8:
		return Val & 0xFF;
	case 16:
		return Val & 0xFFFF;
	case 24:
		return Val & 0xFFFFFF;
	case 32:
		return Val;
	default:
		Logger.BugReport(__LINE__, __FILE__, "Bad size in CustomPointer::GetAddress");
		return -1;
	}
}

//--------------------------- Embedded Pointer --------------------------------
//                                                                           \\
//                                                                           \\

EmbeddedPointer::EmbeddedPointer()
{
	TextPos = -1;
	PointerPos = -1;
	Size = 0;
}

EmbeddedPointer::~EmbeddedPointer()
{
}

/*EmbeddedPointer::EmbeddedPointer(const EmbeddedPointer& rhs)
{
	TextPos = rhs.GetTextPosition();
	PointerPos = rhs.GetPointerPosition();
	Size = rhs.GetSize();
}*/

/*EmbeddedPointer& EmbeddedPointer::operator=(const EmbeddedPointer& rhs)
{
	if(this != &rhs)
	{
		TextPos = rhs.GetTextPosition();
		PointerPos = rhs.GetPointerPosition();
		Size = rhs.GetSize();
	}

	return *this;
}*/

// Return true if pointer is ready to write

bool EmbeddedPointer::SetPointerPosition(const unsigned int Address)
{
	PointerPos = Address;
	if(TextPos != -1)
		return true;
	else
		return false;
}

bool EmbeddedPointer::SetTextPosition(const unsigned int Address)
{
	TextPos = Address;
	if(PointerPos != -1)
		return true;
	else
		return false;
}

void EmbeddedPointer::SetSize(const unsigned int size)
{
	Size = size;
}

unsigned int EmbeddedPointer::GetSize() const
{
	return Size;
}

void EmbeddedPointer::SetOffsetting(const __int64 Offsetting)
{
	this->Offsetting = Offsetting;
}

unsigned int EmbeddedPointer::GetPointer() const
{
	unsigned int Val = (unsigned int)(GetAddress(TextPos + Offsetting));

	switch(Size)
	{
	case 8:
		return Val & 0xFF;
	case 16:
		return Val & 0xFFFF;
	case 24:
		return Val & 0xFFFFFF;
	case 32:
		return Val & 0xFFFFFFFF;
	default:
		Logger.BugReport(__LINE__, __FILE__, 
			"Bad embedded pointer size %d in EmbeddedPointer::GetTextPosition", Size);
		return 0;
	}
}

unsigned int EmbeddedPointer::GetTextPosition() const
{
	return TextPos;
}

unsigned int EmbeddedPointer::GetPointerPosition() const
{
	return PointerPos;
}

unsigned int EmbeddedPointer::GetPointerOffset() const
{
	unsigned int Val = (unsigned int)(GetAddress(TextPos-PointerPos + Offsetting));

	switch(Size)
	{
	case 8:
		return Val & 0xFF;
	case 16:
		return Val & 0xFFFF;
	case 24:
		return Val & 0xFFFFFF;
	case 32:
		return Val & 0xFFFFFFFF;
	default:
		Logger.BugReport(__LINE__, __FILE__, 
			"Bad embedded pointer size %d in EmbeddedPointer::GetTextPosition", Size);
		return 0;
	}
}

//------------------------ Embedded Pointer Handler ---------------------------
//                                                                           \\
//                                                                           \\

EmbeddedPointerHandler::EmbeddedPointerHandler()
{
	PtrSize = 0;
	HdrSize = 0;
	Offsetting = 0;
}

EmbeddedPointerHandler::~EmbeddedPointerHandler()
{
}

bool EmbeddedPointerHandler::SetType(std::string& AddressString, const __int64 Offsetting, const unsigned int PointerSize)
{
	this->Offsetting = Offsetting;
	switch(PointerSize)
	{
	case 8: case 16: case 24: case 32:
		PtrSize = PointerSize;
		break;
	default: // Bad size
		return false;
	}
	return SetAddressType(AddressString);
}

void EmbeddedPointerHandler::SetHeaderSize(const unsigned int HeaderSize)
{
	HdrSize = HeaderSize;
}

unsigned int EmbeddedPointerHandler::GetDefaultSize()
{
	return PtrSize;
}

unsigned int EmbeddedPointerHandler::GetSize(const unsigned int PointerNum)
{
	// Move iterator to PointerNum position in list
	if(PtrList.size() + 1 < PointerNum)
	{
		ReportBug("Should not reach code in EmbeddedPointerHandler::GetSize");
		return -1;
	}

	ListEmbPtrIt i = PtrList.begin();
	for(unsigned int j = 0; j < PointerNum; j++)
		i++;

	//if(i == PtrList.end())
	//	ReportBug("Error: Should not reach code in EmbeddedPointerHandler::GetSize");
	//else
		return i->GetSize();
}

bool EmbeddedPointerHandler::SetTextPosition(const unsigned int PointerNum, const unsigned int TextPos)
{
	if(PtrList.size() < PointerNum + 1) // Add default init'd elements
	{
		EmbeddedPointer elem;
		elem.SetAddressType(AddressType);
		elem.SetSize(PtrSize);
		elem.SetHeaderSize (HdrSize);
		elem.SetOffsetting(Offsetting);
		PtrList.insert(PtrList.end(), PointerNum + 1 - PtrList.size(), elem);
	}

	// Move iterator to PointerNum position in list
	ListEmbPtrIt i = PtrList.begin();
	for(unsigned int j = 0;	j < PointerNum; j++)
		i++;

	if(i->GetTextPosition() == -1 && i->GetPointerPosition() == -1)
	{
		i->SetAddressType(AddressType);
		i->SetSize(PtrSize);
		i->SetHeaderSize(HdrSize);
		i->SetOffsetting(Offsetting);
	}

	return i->SetTextPosition(TextPos);
}

bool EmbeddedPointerHandler::SetPointerPosition(const unsigned int PointerNum, const unsigned int PointerPos)
{
	if(PtrList.size() < PointerNum + 1) // Add default init'd elements
	{
		EmbeddedPointer elem;
		elem.SetAddressType(AddressType);
		elem.SetSize(PtrSize);
		elem.SetHeaderSize (HdrSize);
		elem.SetOffsetting(Offsetting);
		PtrList.insert(PtrList.end(), PointerNum + 1 - PtrList.size(), elem);
	}

	// Move iterator to PointerNum position in list
	ListEmbPtrIt i = PtrList.begin();
	for(unsigned int j = 0;	j < PointerNum; j++)
		i++;

	if(i->GetTextPosition() == -1 && i->GetPointerPosition() == -1)
	{
		i->SetAddressType(AddressType);
		i->SetSize(PtrSize);
		i->SetHeaderSize(HdrSize);
		i->SetOffsetting(Offsetting);
	}

	return i->SetPointerPosition(PointerPos);
}

unsigned int EmbeddedPointerHandler::GetPointerValue(const unsigned int PointerNum)
{
	if(PtrList.size() < PointerNum + 1)
		return -1;

	// Move iterator to PointerNum position in list
	ListEmbPtrIt i = PtrList.begin();
	for(unsigned int j = 0;	j < PointerNum; j++)
		i++;

	return i->GetPointer();
}


unsigned int EmbeddedPointerHandler::GetPointerValueOffset(const unsigned int PointerNum)
{
	if(PtrList.size() < PointerNum + 1)
		return -1;

	// Move iterator to PointerNum position in list
	ListEmbPtrIt i = PtrList.begin();
	for(unsigned int j = 0;	j < PointerNum; j++)
		i++;

	return i->GetPointerOffset();
}

	
unsigned int EmbeddedPointerHandler::GetTextPosition(const unsigned int PointerNum)
{
	if(PtrList.size() < PointerNum + 1)
		return -1;

	// Move iterator to PointerNum position in list
	ListEmbPtrIt i = PtrList.begin();
	for(unsigned int j = 0;	j < PointerNum; j++)
		i++;

	return i->GetTextPosition();
}

unsigned int EmbeddedPointerHandler::GetPointerPosition(const unsigned int PointerNum)
{
	if(PtrList.size() < PointerNum + 1)
		return -1;

	// Move iterator to PointerNum position in list
	ListEmbPtrIt i = PtrList.begin();
	for(unsigned int j = 0;	j < PointerNum; j++)
		i++;

	return i->GetPointerPosition();
}

bool EmbeddedPointerHandler::SetAddressType(std::string& Type)
{
	for(int i = 0; i < AddressTypeCount; i++)
	{
		if(Type == AddressTypes[i])
		{
			AddressType = i;
			return true;
		}
	}

	return false;
}

void EmbeddedPointerHandler::Erase()
{
	PtrList.erase( PtrList.begin(), PtrList.end() );
}