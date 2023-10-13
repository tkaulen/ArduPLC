/*
  PlcPrintable.h - Interface class that allows printing of complex types
  Copyright (c) 2011 Adrian McEwen.  All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef PlcPrintable_h
#define PlcPrintable_h
#include <stdlib.h>
class PlcPrint;
/** The PlcPrintable class provides a way for new classes to allow themselves to be printed.
    By deriving from PlcPrintable and implementing the printTo method, it will then be possible
    for users to print out instances of this class by passing them into the usual
    PlcPrint::print and PlcPrint::println methods.
*/
class PlcPrintable
{
  public:
    virtual size_t printTo(PlcPrint& p) const = 0;
};
#endif
