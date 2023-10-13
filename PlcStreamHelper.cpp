#include "PlcStreamHelper.h"

 size_t PlcStreamHelper::write(uint8_t val)
{

	return plcStream->_stream->write(val);


}
