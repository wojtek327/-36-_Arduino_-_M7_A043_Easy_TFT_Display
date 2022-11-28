#include "Arduino.h"
#include "m7_displ.h"

M7Touchpad::M7Touchpad()
{
  screenClickPosition.posY = 0;
  screenClickPosition.posX = 0;
}

void M7Touchpad::ClearReceivePosition()
{
  screenClickPosition.posY = 0;
  screenClickPosition.posX = 0;
}

void M7Touchpad::TouchpadPosition(uint8_t* recDataPtr, uint8_t length)
{
  uint8_t dataReady = 0;
  uint16_t i = 1;

  if(*(recDataPtr + 0) == 'T' &&
      *(recDataPtr + 1) == 'X' &&
      *(recDataPtr + 2) == 'Y')
  {
    while(length)
    {
      if((recDataPtr + length) == '\n')
      {
        dataReady = 1;
      }
      else if(dataReady && *(recDataPtr + length) >= 0x30 && *(recDataPtr + length) <= 0x39)
      {
          if(dataReady == 1)
          {
            screenClickPosition.posY += (uint16_t)(*(recDataPtr + length) - 0x30) * i;
          }
          if(dataReady == 2)
          {
            screenClickPosition.posX += (uint16_t)(*(recDataPtr + length)- 0x30) * i;
          }
          i *= 10;
      }
      else if(*(recDataPtr + length) == ' ')
      {
        dataReady = 2;
        i = 1;
      }
      length--;
    }
  }
}

uint8_t M7Touchpad::CheckIfBtnClicked(Button_TypedefStruct *stuctPtr)
{
  if(stuctPtr->_enabled &&
     (screenClickPosition.posX < stuctPtr->_posRight) &&
     (screenClickPosition.posX > stuctPtr->_posLeft) &&
     (screenClickPosition.posY > stuctPtr->_posTop) &&
     (screenClickPosition.posY < stuctPtr->_posBot))
     {
       return 1;
    }
    return 0;
}
//=========================================================================================
/*
  + Draw a circle CIRCLE Xa Ya R C
  + Fill in color CLR Xa Ya Xe Ye C
  + Clear screen CLS C
  + Cut a picture CUT Pn Xa Ya Xb Yb Xs Ys
  + Draw a dot DOT Xa Ya C
  + Draw a line LINE Xa Ya Xe Ye C
  + Backlight on LEDON
  + Backlight off LEDOFF
  + LOAD PIC PIC Pn Xa Ya
  + Display image files directly LOAD Xa Ya Path
  + Draw a rectangle RECT Xa Ya Xe Ye C
  + Get screen size SIZE
  + Display alphabetic string STR Xa Ya C Str
  + Buzzer control BUZ Tb Fb
*/

M7Display::M7Display(uint16_t sizeX, uint16_t sizeY){
  _sizeX = sizeX;
  _sizeY = sizeY;
}
//------------------------------------------------------------------------------
void M7Display::ClearScreen(HardwareSerial &refSer, M7Color bgColor) {
  char bufferOut[16];
  sprintf(bufferOut, "CLS %d\n\r", bgColor);
  refSer.println(bufferOut);
}
M7WrongCommandParameters M7Display::ClearScreen(HardwareSerial &refSer, uint16_t bgColor) {
  if(bgColor > 65535) {
    return M7WrongCommandParameters::TooBigColorValue;
  }

	char bufferOut[16];
	sprintf(bufferOut, "CLS %u\n\r", bgColor);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::PrintStringWithBg( HardwareSerial &refSer,
                        const char * str, uint16_t len,
                        M7FontSize fontSize,
		                    uint16_t posX, uint16_t posY,
                        uint16_t textColor, uint16_t bgColor) {
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(bgColor > 65535) {
    return M7WrongCommandParameters::TooBigColorValue;
  }

  if(len > 100) {
    return M7WrongCommandParameters::TooSmallBuffer;
  }

  char text[len + 1];
  memset(text, 0, sizeof text);
  memcpy(text, str, len);

  char bufferOut[128];
  memset(bufferOut, 0, sizeof bufferOut);
  sprintf(bufferOut, "HZB%u %u %u %u %u %s\n\r", (uint16_t)fontSize, posX, posY, textColor, bgColor, text);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}

M7WrongCommandParameters M7Display::PrintStringWithBg( HardwareSerial &refSer,
                        const char * str, uint16_t len,
                        M7FontSize fontSize,
		                    uint16_t posX, uint16_t posY,
                        M7Color textColor, M7Color bgColor) {
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(len > 100) {
    return M7WrongCommandParameters::TooSmallBuffer;
  }

  char text[len + 1];
  memset(text, 0, sizeof text);
  memcpy(text, str, len);

  char bufferOut[128];
  memset(bufferOut, 0, sizeof bufferOut);
  sprintf(bufferOut, "HZB%u %u %u %u %u %s\n\r", (uint16_t)fontSize, posX, posY, textColor, bgColor, text);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::PrintStringTrans(HardwareSerial &refSer, const char * str, M7FontSize fontSize, uint16_t posX, uint16_t posY, uint16_t textColor) {
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(textColor > 65535) {
    return M7WrongCommandParameters::TooBigColorValue;
  }

	char bufferOut[128];
	sprintf(bufferOut, "HZ%u %u %u %u %s\n\r", (uint16_t)fontSize, posX, posY, textColor, str);
	refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
M7WrongCommandParameters M7Display::PrintStringTrans(HardwareSerial &refSer, const char * str, uint8_t len, M7FontSize fontSize, uint16_t posX, uint16_t posY, M7Color textColor) {
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char text[len + 1];
  memset(text, 0, sizeof text);
  memcpy(text, str, len);

	char bufferOut[128];
	sprintf(bufferOut, "HZ%u %u %u %u %s\n\r", (uint16_t)fontSize, posX, posY, textColor, text);
	refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::PrintString(HardwareSerial &refSer, const char * str, uint8_t len, uint16_t posX, uint16_t posY, M7Color textColor) {
  if(len > 100) {
    return M7WrongCommandParameters::TooSmallBuffer;
  }

  char text[len + 1];
  memset(text, 0, sizeof text);
  memcpy(text, str, len);

  char bufferOut[128];
  memset(bufferOut, 0, sizeof bufferOut);
  sprintf(bufferOut, "STR %u %u %u %s\n\r", posX, posY, textColor, text);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::DrawCircle(HardwareSerial &refSer, uint16_t posX, uint16_t posY, uint16_t radius, M7Color color){
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[20];
  sprintf(bufferOut, "CIRCLE %u %u %u %u\n\r", posX, posY, radius, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
M7WrongCommandParameters M7Display::DrawCircle(HardwareSerial &refSer, uint16_t posX, uint16_t posY, uint16_t radius, uint16_t color){
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(color > 65535) {
    return M7WrongCommandParameters::TooBigColorValue;
  }

  char bufferOut[20];
  sprintf(bufferOut, "CIRCLE %u %u %u %u\n\r", posX, posY, radius, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::FillInColor(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, M7Color color) {
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[20];
  sprintf(bufferOut, "CLR %u %u %u %u %u\n\r", posXStart, posYStart, posXEnd, posYEnd, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
M7WrongCommandParameters M7Display::FillInColor(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart, uint16_t posXEnd, uint16_t posYEnd, uint16_t color) {
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(color > 65535) {
    return M7WrongCommandParameters::TooBigColorValue;
  }

  char bufferOut[20];
  sprintf(bufferOut, "CLR %u %u %u %u %u\n\r", posXStart, posYStart, posXEnd, posYEnd, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::DrawADot(HardwareSerial &refSer, uint16_t posX, uint16_t posY, M7Color color){
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[20];
  sprintf(bufferOut, "DOT %u %u %u\n\r", posX, posY, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
M7WrongCommandParameters M7Display::DrawADot(HardwareSerial &refSer, uint16_t posX, uint16_t posY, uint16_t color){
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(color > 65535) {
    return M7WrongCommandParameters::TooBigColorValue;
  }

  char bufferOut[20];
  sprintf(bufferOut, "DOT %u %u %u\n\r", posX, posY, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//--------------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::DrawALine(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, M7Color color) {
  if(CheckCoordinates(posYStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[20];
  sprintf(bufferOut, "LINE %u %u %u %u %u\n\r", posXStart, posYStart, posXEnd, posYEnd, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
M7WrongCommandParameters M7Display::DrawALine(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, uint16_t color){
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(color > 65535) {
    return M7WrongCommandParameters::TooBigColorValue;
  }

  char bufferOut[20];
  sprintf(bufferOut, "LINE %u %u %u %u %u\n\r", posXStart, posYStart, posXEnd, posYEnd, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//--------------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::DrawRectangle(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, M7Color color) {
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[20];
  sprintf(bufferOut, "RECT %u %u %u %u %u\n\r", posXStart, posYStart, posXEnd, posYEnd, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
M7WrongCommandParameters M7Display::DrawRectangle(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart,uint16_t posXEnd, uint16_t posYEnd, uint16_t color) {
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[20];
  sprintf(bufferOut, "RECT %u %u %u %u %u\n\r", posXStart, posYStart, posXEnd, posYEnd, color);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//--------------------------------------------------------------------------------------
M7WrongCommandParameters M7Display::DisplayImageFromPath(HardwareSerial &refSer, uint16_t posXStart, uint16_t posYStart, const char * str, uint8_t len){
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  if(len > 100) {
    return M7WrongCommandParameters::TooSmallBuffer;
  }

  char text[len + 1];
  memset(text, 0, sizeof text);
  memcpy(text, str, len);

  char bufferOut[128];
  memset(bufferOut, 0, sizeof bufferOut);
  sprintf(bufferOut, "LOAD %u %u %s\n\r", posXStart, posYStart, text);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}

M7WrongCommandParameters M7Display::DisplayImageBaseOnID(HardwareSerial &refSer, uint8_t picNumber, uint16_t posX, uint16_t posY)
{
  if(CheckCoordinates(posX, posY)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[30];
  memset(bufferOut, 0, sizeof bufferOut);
  sprintf(bufferOut, "PIC %u %u %u\n\r", picNumber, posX, posY);

  return M7WrongCommandParameters::StatOK;
}

M7WrongCommandParameters M7Display::CutPicture(HardwareSerial &refSer,
                                                uint8_t picNumber, uint16_t posXStart, uint16_t posYStart,
                                                uint16_t posXEnd, uint16_t posYEnd, uint16_t xImageSize, uint16_t yImageSize)
{
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }

  char bufferOut[30];
  memset(bufferOut, 0, sizeof bufferOut);
  sprintf(bufferOut, "CUT %u %u %u %u %u %u %u\n\r", picNumber, posXStart, posYStart,
                                                posXEnd, posYEnd, xImageSize, yImageSize);

  return M7WrongCommandParameters::StatOK;
}

M7WrongCommandParameters M7Display::DisplayAnimation(HardwareSerial &refSer, uint8_t animationNumber,
                  uint8_t onOffState, uint16_t posXStart, uint16_t posYStart,
                  uint16_t animationStartPictureNumber, uint16_t endAnimationPictureNumber, uint16_t spacingTimeMs)
{
  if(CheckCoordinates(posXStart, posYStart)) {
    return M7WrongCommandParameters::WrongPosition;
  }
  else if(animationNumber < 1 || animationNumber > 5) {
    return M7WrongCommandParameters::WrongParameters;
  }
  else if(onOffState > 1){
    return M7WrongCommandParameters::WrongParameters;
  }

  char bufferOut[128];
  memset(bufferOut, 0, sizeof bufferOut);
  sprintf(bufferOut, "M%u %u %u %u %u %u %u\n\r", animationNumber, onOffState, posXStart, posYStart,
                        animationStartPictureNumber, endAnimationPictureNumber, spacingTimeMs);
  refSer.print(bufferOut);

  return M7WrongCommandParameters::StatOK;
}
//--------------------------------------------------------------------------------------
void M7Display::BuzzerControl(HardwareSerial &refSer, uint16_t Tb, uint16_t Fb) {
  char bufferOut[20];
  sprintf(bufferOut, "BUZ %u %u\n\r", Tb, Fb);
  refSer.print(bufferOut);
}
//--------------------------------------------------------------------------------------
void M7Display::BacklightOn(HardwareSerial &refSer)
{
  char bufferOut[10] = "LEDON\n\r";
  refSer.print(bufferOut);
}
void M7Display::BacklightOff(HardwareSerial &refSer)
{
  char bufferOut[10] = "LEDOFF\n\r";
  refSer.print(bufferOut);
}
//--------------------------------------------------------------------------------------
void M7Display::GetScreenSize(HardwareSerial &refSer)
{
  char bufferOut[7] = "SIZE\n\r";
  refSer.print(bufferOut);
}
//--------------------------------------------------------------------------------------
static uint8_t M7Display::CheckCoordinates(uint16_t valX, uint16_t valY)
{
  if(valX > _sizeX && valY > _sizeY){
    return 1;
  }
  return 0;
}
