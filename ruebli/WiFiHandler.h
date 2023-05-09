#ifndef WiFiHandler_h
#define WiFiHandler_h
#include "Arduino.h"
class WiFiHandler {
  public:
          WiFiHandler();
          bool hasNewImage();
          int getNewImage();
  private:
          int* _image;
};
#endif
