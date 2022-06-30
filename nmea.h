#ifndef nmeaH
#define nmeaH
//-----------------------------------------------------------------------------------
class Nmea {
  public:
  static const int buffSize = 100;
  Nmea();
  bool   decode(char sym);
  char*  getSentence();
  char*  getSentenceName();
  double getLatitude()       { return latitude;}
  double getLongitude()      { return longitude;}
  double getAltitude()       { return altitude;}
  float  getHdop()           { return hdop;}
  int    getNumSat()         { return numSat;}
  int    getFixQuality()     { return fixQuality;}
  int    getDistance()       { return distance;}
  int    getTimeToExecute()  { return timeToExecute;}
  int    getBatteryPercent() { return batteryPercent;}
  double getFixLatitude()    { return fixLatitude; }
  double getFixLongitude()   { return fixLongitude; }
  int    getFixRadius()      { return fixRadius; }
  int    getTimeLimit()      { return timeLimit; }
  char*  getImei()           { return imei; }
  
  private:
  char   buff[buffSize];
  char   name[4];
  int    pos;
  bool   foundBegin;
  double latitude;
  double longitude;
  double altitude;
  float  hdop;
  int    numSat;
  int    fixQuality;
  int    distance;
  int    timeToExecute;
  int    batteryPercent;
  double fixLatitude;
  double fixLongitude;
  int    fixRadius;
  int    timeLimit;
  char   imei[16];
  bool   checkSum();
  bool   parse();
  bool   parseGGA();
  bool   parseRMC();
  bool   parsePLEDB();
  bool   parsePLEDA();
  bool   parsePLEDC();
};
//-----------------------------------------------------------------------------------
#endif
