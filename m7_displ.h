#ifndef M7_DISPL_h
#define M7_DISPL_h

#include "Arduino.h"

enum class M7Color:uint16_t
{
  COLOR_BLACK = 0x0000,
  COLOR_BLUE = 0x001F,
  COLOR_AQUA = 0x07FF,
  COLOR_FUCHSIA	= 0xF81F,
  COLOR_GREEN	= 0x0400,
  COLOR_GREY = 0x8410,
  COLOR_LIME = 0x07E0,
  COLOR_MAROON = 0x8000,
  COLOR_NAVY = 0x0010,
  COLOR_OLIVE	= 0x8400,
  COLOR_PURPLE = 0xC00F,
  COLOR_RED	= 0x9800,
  COLOR_SILVER = 0xC618,
  COLOR_TEAL = 0x0410,
  COLOR_YELLOW = 0xFF40,
  COLOR_WHITE = 0xFFFF,
  COLOR_ORANGE = 0xFBE0
};

enum class M7FontSize:uint8_t
{
  FontSize_16 = 16,
  FontSize_24 = 24,
  FontSize_32 = 32,
  FontSize_48 = 48,
  FontSize_64 = 64
};

enum class M7WrongCommandParameters:uint8_t
{
  StatOK = 0,
  WrongPosition = 1,
  TooBigColorValue = 2,
  TooSmallBuffer = 3,
  WrongParameters = 4
};

typedef void (*ButtonClickedHandler)(uint8_t btnId);

typedef struct {
   uint8_t _buttonId;
   ButtonClickedHandler _ClickHandler;
   uint16_t _posTop;
   uint16_t _posBot;
   uint16_t _posLeft;
   uint16_t _posRight;
   bool _enabled;
 }Button_TypedefStruct;

class M7Touchpad
{
  private:
   typedef struct{
      uint16_t posX;
      uint16_t posY;
    }TPPos_TypedefStruct;
    TPPos_TypedefStruct screenClickPosition;
  public:
    M7Touchpad();
    void ClearReceivePosition();
    void TouchpadPosition(uint8_t* recDataPtr, uint8_t dataLength);
    uint8_t CheckIfBtnClicked(Button_TypedefStruct *stuctPtr);
};
class M7Display
{
  public:
    M7Display(uint16_t sizeX, uint16_t sizeY);
    //-----------------------------------------------------------
    void ClearScreen(HardwareSerial &refSer, M7Color bgColor);
    M7WrongCommandParameters ClearScreen(HardwareSerial &refSer, uint16_t bgColor);
    //-----------------------------------------------------------
    M7WrongCommandParameters PrintStringWithBg( HardwareSerial &refSer,
                            const char * str, uint16_t len,
                            M7FontSize fontSize,
    		                    uint16_t posX, uint16_t posY,
                            M7Color textColor, M7Color bgColor);
    M7WrongCommandParameters PrintStringWithBg( HardwareSerial &refSer,
                            const char * str, uint16_t len,
                            M7FontSize fontSize,
    		                    uint16_t posX, uint16_t posY,
                            uint16_t textColor, uint16_t bgColor);
    //-----------------------------------------------------------
    M7WrongCommandParameters PrintStringTrans(HardwareSerial &refSer, const char * str,
              M7FontSize fontSize, uint16_t posX, uint16_t posY, uint16_t textColor);
    M7WrongCommandParameters PrintStringTrans(HardwareSerial &refSer, const char * str, uint8_t len,
              M7FontSize fontSize, uint16_t posX, uint16_t posY, M7Color textColor);
    //-----------------------------------------------------------
    M7WrongCommandParameters PrintString(HardwareSerial &refSer, const char * str, uint8_t len, uint16_t posX, uint16_t posY, M7Color textColor);
    //-----------------------------------------------------------
    M7WrongCommandParameters DrawCircle(HardwareSerial &refSer, uint16_t posX, uint16_t posY, uint16_t radius, M7Color color);
    M7WrongCommandParameters DrawCircle(HardwareSerial &refSer, uint16_t posX, uint16_t posY, uint16_t radius, uint16_t color);
    //-----------------------------------------------------------
    M7WrongCommandParameters FillInColor(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,
                                    uint16_t posXEnd, uint16_t posYEnd, uint16_t color);
    M7WrongCommandParameters FillInColor(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,
                                                                    uint16_t posXEnd, uint16_t posYEnd, M7Color color);
    //-----------------------------------------------------------
    M7WrongCommandParameters DrawADot(HardwareSerial &refSer, uint16_t posX, uint16_t posY, M7Color color);
    M7WrongCommandParameters DrawADot(HardwareSerial &refSer, uint16_t posX, uint16_t posY, uint16_t color);
    //-----------------------------------------------------------
    M7WrongCommandParameters DrawALine(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, M7Color color);
    M7WrongCommandParameters DrawALine(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, uint16_t color);
    //-----------------------------------------------------------
    M7WrongCommandParameters DrawRectangle(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, M7Color color);
    M7WrongCommandParameters DrawRectangle(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, uint16_t color);
    //-----------------------------------------------------------
    M7WrongCommandParameters DisplayImageBaseOnID(HardwareSerial &refSer, uint8_t picNumber, uint16_t posX, uint16_t posY);
    M7WrongCommandParameters DisplayImageFromPath(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart, const char * str, uint8_t len);
    M7WrongCommandParameters DisplayAnimation(HardwareSerial &refSer, uint8_t animationNumber,
                      uint8_t onOffState, uint16_t posXStart, uint16_t posYStart,
                      uint16_t animationStartPictureNumber, uint16_t endAnimationPictureNumber, uint16_t spacingTimeMs);
    M7WrongCommandParameters CutPicture(HardwareSerial &refSer, uint8_t picNumber, uint16_t posXStart, uint16_t posYStart, uint16_t posXEnd, uint16_t posYEnd, uint16_t xImageSize, uint16_t yImageSize);
    //-----------------------------------------------------------
    void BuzzerControl(HardwareSerial &refSer, uint16_t Tb, uint16_t Fb);
    //-----------------------------------------------------------
    void BacklightOn(HardwareSerial &refSer);
    void BacklightOff(HardwareSerial &refSer);
    //-----------------------------------------------------------
    void GetScreenSize(HardwareSerial &refSer);
    //-----------------------------------------------------------
  private:
    uint8_t CheckCoordinates(uint16_t valX, uint16_t valY);
    uint16_t _sizeX;
    uint16_t _sizeY;
};

#endif
