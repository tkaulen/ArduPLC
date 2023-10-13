#pragma once
#include "PlcStream.h"
#include "PlcMemoryStream.h"
#include "PlcStreamExtend.h"
#include "PlcAdapter.h"
#include "PlcLogger.h"
class PlcStdIO : public PlcIO
{
public:
	PlcMemoryStream* memoryStream;
	PlcStream* externStream;
	PlcNonBlockingParser plcNonBlockingParser;
	int parseStatus;
	PlcAdapter* curentAdapter;
	bool echo;
	PlcStdIO(PlcStream* stream,int bufferSize = 40) :PlcIO()
	{
		echo = false;
		parseStatus = 0;
		curentAdapter = NULL;
		this->externStream = stream;
		this->memoryStream = new PlcMemoryStream(bufferSize);
		//this->stream = stream;
	}
#ifdef _arduino_
	PlcStdIO(Stream* stream) :PlcIO()
	{
		parseStatus = 0;
		curentAdapter = NULL;
		this->externStream = new PlcStreamExtend(stream);
		this->memoryStream = new PlcMemoryStream(19);
		//this->stream = stream;
	}
#endif



	

	size_t write(const uint8_t* buffer, size_t size)
	{
		
		
		return memoryStream->write(buffer, size);

	}
	size_t write(uint8_t c)
	{
		return memoryStream->write(c);


	}
	bool onChanged(PlcAdapter* plcAdapter, int index)
	{

		if (!this->echo && plcAdapter->origin == true)
		{
			return false;


		}
		if (memoryStream != NULL )
		{
			uint16_t _size = memoryStream->_size;
			uint16_t _pos_read = memoryStream->_pos_read;
			uint16_t _pos_write = memoryStream->_pos_write;
			//stream->flush();
			memoryStream->print(plcAdapter->name);
			memoryStream->print("=");
			plcAdapter->toStream(memoryStream);
			memoryStream->print(";");
			memoryStream->println();
			if (memoryStream->_buffer_overflow)
			{
				memoryStream->_size = _size;
				memoryStream->_pos_read = _pos_read;
				memoryStream->_pos_write = _pos_write;
				memoryStream->_buffer_overflow = false;
				return true;
				int u = 0;
			}
			return false;
		}
	}
	void parse()
	{
		unsigned char az = 0;
		switch (parseStatus)
		{
		case 0:
			plcNonBlockingParser.beginParse();
			parseStatus = 1;
			break;
		case 1:
			if (plcNonBlockingParser.parseToken(externStream))
			{
				if (plcNonBlockingParser.az == '=')
				{
					curentAdapter = this->find(plcNonBlockingParser.symbol);
					if (curentAdapter != NULL)
					{
						curentAdapter->beginParse(&plcNonBlockingParser);
						parseStatus = 2;
						break;
					}
					else
					{
						parseStatus = 0;
						break;
					}
				}
				else
				{
					
						curentAdapter = this->find("cmd");
						curentAdapter->endParse(&plcNonBlockingParser);

						int u = 0;
					
					parseStatus = 0;
					break;
				}
			}
			break;
		case 2:
			if (curentAdapter->parseToken(&plcNonBlockingParser, externStream))
			{
				parseStatus = 0;
			}
		default: break;
		}
	}
	void onTask()
	{
		if (externStream != NULL)
		{
			memoryStream->tryWriteOut(externStream);
		}
		parse();
	}
};
