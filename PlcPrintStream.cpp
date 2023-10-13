#include "PlcPrintStream.h"
#ifdef _windows_
#include <Windows.h>
 PlcPrintStream::PlcPrintStream() : PlcStream()
{

     hasNext = false;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);
}
  boolean PlcPrintStream::getNext()
 {
	 if (hasNext)
	 {
		 return true;
	 }
	 if (_kbhit())
	 {
		 key = _getch();
		 hasNext = true;
		 return true;
	 }
	 return false;
 }
int PlcPrintStream::availableForWrite() {
    return 1;
}
size_t PlcPrintStream::write(uint8_t val)
{
    std::cout << val;
    return 1;
}
int PlcPrintStream::available()
{
	if (getNext())
	{
		return 1;
	}
    return 0;
}
int PlcPrintStream::read()
{
	if (getNext())
	{
		hasNext = false;
		print((char)key);
		if (key == 0xd)
		{
			print((char)0xa);
		}
		return key;
	}
    return 0;
}
int PlcPrintStream::peek()
{
	if (getNext())
	{
		return key;
	}
    return 0;
}
#endif
