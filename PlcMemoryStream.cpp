#include "PlcMemoryStream.h"
/*size_t PlcMemoryStream::write(uint8_t byte) {
	if (!_allowWrite) return -1;
	if (headerMode)
	{
		if (_pos_write_header >= _len) {
			_buffer_overflow = true;
			return 0;
		}
		_buffer[_pos_write_header] = byte;
		++_pos_write_header;
		return 1;
	}
	else
	{
		if (_pos_write >= _len) {
			_buffer_overflow = true;
			return 0;
		}
		_buffer[_pos_write] = byte;
		++_pos_write;
		return 1;
	}
}*/

size_t PlcMemoryStream::write(uint8_t byte) {
	if (!_allowWrite) return -1;

	if (_size >= _maxLen)
	{
		_buffer_overflow = true;
		return 0;

    }
	_size++;
		_buffer[_pos_write] = byte;
		++_pos_write;
		if (_pos_write >= _maxLen) _pos_write =0;
		return 1;
	
}
void PlcMemoryStream::flush() {
	memset(_buffer, 0, _maxLen);
	_pos_write = 0;
	_pos_write_header = 0;
	_pos_read = 0;
	_size = 0;
	headerMode = false;
}
/*int PlcMemoryStream::read() {
	if (_pos_read >= _maxLen) {
		_buffer_overflow = true;
		return -1;
	}
	if (_pos_read >= _pos_write) {
		return -1;
	}
	return _buffer[_pos_read++];
}*/

int PlcMemoryStream::read() {
	if (_size ==0) {
		_buffer_overflow = true;
		return -1;
	}
	_size--;
	
	unsigned char az = _buffer[_pos_read++];

	if (_pos_read >= _maxLen) _pos_read = 0;

	
	return az;
}

int PlcMemoryStream::read(int len) {

	if (_size < len) {
		_buffer_overflow = true;
		return -1;
	}
	_size -= len;

	_pos_read = ((_pos_read + len) % _maxLen);


	
	return _buffer[_pos_read];
}
int PlcMemoryStream::peek() {

	if (_size == 0) {
		_buffer_overflow = true;
		return -1;
	}
	
	
	return _buffer[_pos_read];
}
int PlcMemoryStream::available() {
	
	return _size;
}
void PlcMemoryStream::setBufferContent(uint8_t* buffer, uint16_t content_len) {
	/*memset(_buffer, 0, _maxLen);
	memcpy(_buffer, buffer, content_len);
	_buffer_overflow = false;
	_pos_write = content_len;
	_pos_read = 0;*/
}
void PlcMemoryStream::setBufferContentFromProgmem(uint8_t* buffer, uint16_t content_len) {
	/*
	memset(_buffer, 0, _maxLen);
#ifdef _arduino_
	memcpy_P(_buffer, buffer, content_len);
#endif
	_buffer_overflow = false;
	_pos_write = content_len;
	_pos_read = 0;*/
}
void PlcMemoryStream::setBufferContentPosition(uint16_t read_pos, uint16_t write_pos) {
	_pos_write = write_pos;
	_pos_read = read_pos;
}
void PlcMemoryStream::setWritePosition(int position)
{
	_pos_write = position;
}
int PlcMemoryStream::getLenght()
{
	return _pos_write;
}
void PlcMemoryStream::setFixedHeader(int size)
{
	_pos_write = size;
}
void PlcMemoryStream::useHeader()
{
	headerMode = true;
}
void PlcMemoryStream::usePayload()
{
	headerMode = false;
}
int PlcMemoryStream::tryWriteOut(PlcStream* stream)
{
	if (availableIntern() == false) return 1;
	while (availableIntern() && stream->availableForWrite())
	{
		int av = available();
		//uint8_t value = peek();
		unsigned char tempBuffer[100];

		for (int i = 0; i < av; i++)
		{
			tempBuffer[i] = this->_buffer[((this->_pos_read +i) % this->_maxLen)];

		}
		
		int len = stream->write(tempBuffer, av);
		if (len <= 0)
		{
			
			return 0;
		}
		read(len);
		av = available();
		int u = 0;
	}
	if (availableIntern() == false) return 1;
	return 0;
}
PlcMemoryStream::PlcMemoryStream() : PlcStream() {
	hasData = false;
	_size = 0;
	_maxLen = 20;
	_buffer = (uint8_t*)malloc(_maxLen);
	_buffer_overflow = false;
	_pos_write = 0;
	_pos_read = 0;
	headerMode = false;
	_allowWrite = true;
	memset(_buffer, 0, _maxLen);
}
PlcMemoryStream::PlcMemoryStream(const int len) : PlcStream() {
	hasData = false;
	_maxLen = len;
	_size = 0;
	_buffer = (uint8_t*)malloc(_maxLen);
	_buffer_overflow = false;
	_pos_write = 0;
	_pos_read = 0;
	headerMode = false;
	_allowWrite = true;
	memset(_buffer, 0, _maxLen);
}