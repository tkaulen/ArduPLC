#include "PlcLogger.h"

void PlcLogger::logSpecial( String s, int value)
{
	PlcLogger::writeOut(s, value);
}

void PlcLogger::log(int logLevel, String s)
{
	if (PlcLogger::_logLevel >= logLevel)	PlcLogger::writeOut(s, 0);



}

  void PlcLogger::log(int logLevel, String s, int value)
 {
	 if (PlcLogger::_logLevel >= logLevel)	PlcLogger::writeOut(s, value);



 }

  void PlcLogger::log(int logLevel, String s, const char* value)
  {

	  if (PlcLogger::_logLevel >= logLevel)
	  {
#ifdef _windows_
		  std::cout << millis();
		  std::cout << "  ";
		  std::cout << "log: ";
		  std::cout << s;
		  std::cout << "=";
		  std::cout << value;
		  std::cout << "\n";

#endif

	  }


  }

  void PlcLogger::logTimed(int logLevel, String s, int value)
  {
	  PlcLogger::timeCycle++;

	  if (PlcLogger::timeCycle % 1000 == 0)
	  {

		  if (PlcLogger::_logLevel >= logLevel)	PlcLogger::writeOut(s, value);
	  }



  }

 void PlcLogger::writeOut(String s, int value)
  {

#ifdef _arduino_
	 Serial.print(millis());
	 Serial.print("  ");

	  Serial.print("log: ");
	  Serial.print(s);
	  Serial.print("=");
	  Serial.println(value);

#endif

#ifdef _windows_
	  std::cout << millis();
	  std::cout << "  ";
	  std::cout << "log: ";
	  std::cout << s;
	  std::cout << "=";
	  std::cout << value;
	  std::cout << "\n";

#endif



  }

  int PlcLogger::tempCounter = 0;
  unsigned int PlcLogger::timeCycle = 0;
