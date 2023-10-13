#ifndef _PlcMemoryStream_
#define _PlcMemoryStream_
#include "PlcPlatform.h"
#include "PlcStream.h"

class PlcBugStream : public PlcStream
{

    size_t write(uint8_t byte)
    {
        return 0;
    }

    void bla()
    {
        //this->write("fdsfsa");

    }

   


};
class PlcMemoryStream : public PlcStream {
public:
    uint8_t* _buffer;
    uint16_t _maxLen;

    uint16_t _size;

    bool _buffer_overflow;
    uint16_t _pos_read;
    uint16_t _pos_write;
    uint16_t _pos_write_header;
   
    
    bool headerMode;
  
    bool _allowWrite;

    size_t write(const uint8_t* buffer, size_t size)
    {
        size_t n = 0;
        while (size--) {
            if (write(*buffer++)) n++;
            else break;
        }
        return n;
    }

    size_t write(uint8_t byte);
    void flush();
    int read();
    int read(int len);
    int peek();
    int available();
    void setBufferContent(uint8_t* buffer, uint16_t content_len);
    void setBufferContentFromProgmem(uint8_t* buffer, uint16_t content_len);
    void setBufferContentPosition(uint16_t read_pos, uint16_t write_pos);
    void setWritePosition(int position);

    int getLenght();
    void setFixedHeader(int size);

    void useHeader();

    void usePayload();

    void copyToString(String *destination)
    {

#ifdef _windows_

        destination->clear();
        for (int i = 0; i < _maxLen; i++)
        {
            destination->push_back(_buffer[i]);

        }
        


#endif

#ifdef _arduino_
      //  destination->reserve(_len);
      //  destination->copy(( char*)_buffer, _len);

        for (int i = 0; i < _maxLen; i++)
        {
           // destination->setCharAt(i,_buffer[i]);

        }




#endif



    }



    int tryWriteOut(PlcStream *stream);

    String name;
    double value;
    bool hasData;
    PlcMemoryStream();
    PlcMemoryStream(const int len);
    
};
#endif
