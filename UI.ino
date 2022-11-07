#include "ATM0130.h"

UI::UI() {
  this->x = 104;
  this->y = 104;
  this->sizex = 120;
  this->sizey = 16;
  this->show = true;

  this->menuOpen = false;
  this->UIcursor = 0;
  this->secondCursor = 0;
}

void UI::move() {
  if (device.getButtonState(BUTTON_A, BUTTON_PRESSED)) {
    if (!this->menuOpen) this->menuOpen = true;
    else this->menuOpen = false;
  }
  else if (device.getButtonState(BUTTON_B, BUTTON_PRESSED)) {
    if (!this->menuOpen) {
      this->UIcursor++;
      this->UIcursor = UIcursor % 7;
    }
    else {
      if     (UIcursor == ICON_HAND) character.eyeState++;
      else if(UIcursor == ICON_FOOD) character.bodyState++;
    }
  }
}

void UI::drawMenubar() {
  //白地
  myATM0130.setColor(WHITE16);
  myATM0130.drawFillRectangle(1, 104, 119, 119);
  myATM0130.drawLine(0, 104, 0, 120);
  myATM0130.drawLine(119, 104, 119, 120);
  
  myATM0130.setColor(BLACK16);
  myATM0130.drawLine(1, 103, 118, 103);
  myATM0130.drawDot(0, 104);
  //icon描画
  for (uint8_t i = 0; i < 6; i++) myATM0130.drawBlock_16px(1 + i * 15, 104, icon[i]);
  //カーソル表示
  if (this->UIcursor != 6) {
    myATM0130.setColor(RED16);
    myATM0130.drawLine(  1 + this->UIcursor * 15, 106,  1 + this->UIcursor * 15, 117);
    myATM0130.drawLine( 16 + this->UIcursor * 15, 106, 16 + this->UIcursor * 15, 117);
    myATM0130.drawLine(  3 + this->UIcursor * 15, 104, 14 + this->UIcursor * 15, 104);
    myATM0130.drawLine(  3 + this->UIcursor * 15, 119, 14 + this->UIcursor * 15, 119);
    myATM0130.drawDot(   2 + this->UIcursor * 15, 105);
    myATM0130.drawDot(   2 + this->UIcursor * 15, 118);
    myATM0130.drawDot(  15 + this->UIcursor * 15, 105);
    myATM0130.drawDot(  15 + this->UIcursor * 15, 118);
  }

  this->drawClock(106, 106);
}

void UI::drawClock(int16_t x, int16_t y) {
  float Rad;
  //盤
  myATM0130.setColor(WHITE16);
  myATM0130.drawFillCircle(x, y, 13);

  if (device.isTimeConfigured) {
    myATM0130.setColor(BLACK16);
  }
  else {
    if (device.isWiFiConnected) myATM0130.setColor(GREEN16);
    else myATM0130.setColor(RED16);
  }
  myATM0130.drawCircle(x, y, 13);
  myATM0130.drawCircle(x, y, 12);

  device.getTime();
  //長針
  myATM0130.setColor(BLACK16);
  Rad = (360 * ((device.getMinute() % 60) / 60.0) - 90) / (180 / PI);
  myATM0130.drawLine(x  , y  , x + cos(Rad) * 9, y + sin(Rad) * 9);
  myATM0130.drawLine(x - 1, y  , x + cos(Rad) * 9, y + sin(Rad) * 9);
  myATM0130.drawLine(x + 1, y  , x + cos(Rad) * 9, y + sin(Rad) * 9);
  myATM0130.drawLine(x  , y - 1, x + cos(Rad) * 9, y + sin(Rad) * 9);
  myATM0130.drawLine(x  , y + 1, x + cos(Rad) * 9, y + sin(Rad) * 9);

  //短針
  myATM0130.setColor(RED16);
  Rad = (360 * ((device.getHour() % 12) / 12.0) - 90) / (180 / PI);
  myATM0130.drawLine(x  , y  , x + cos(Rad) * 5, y + sin(Rad) * 5);
  myATM0130.drawLine(x - 1, y  , x + cos(Rad) * 5, y + sin(Rad) * 5);
  myATM0130.drawLine(x + 1, y  , x + cos(Rad) * 5, y + sin(Rad) * 5);
  myATM0130.drawLine(x  , y - 1, x + cos(Rad) * 5, y + sin(Rad) * 5);
  myATM0130.drawLine(x  , y + 1, x + cos(Rad) * 5, y + sin(Rad) * 5);

  //秒針
  myATM0130.setColor(BLUE16);
  Rad = (360 * ((device.getSecond() % 60) / 60.0) - 90) / (180 / PI);
  myATM0130.drawLine(x, y, x + cos(Rad) * 11, y + sin(Rad) * 11);

}

void UI::drawInfo() {
  myATM0130.setColor(WHITE16);
  myATM0130.drawFillRectangle(1, 60, 118, 119);
  myATM0130.drawLine(0, 60, 0, 120);
  myATM0130.drawLine(119, 60, 119, 120);
  myATM0130.setColor(BLACK16);
  myATM0130.drawLine(1, 59, 118, 59);
  myATM0130.drawDot(0, 60);
  myATM0130.drawDot(119, 60);

  myATM0130.drawBlock_32px(1, 61, character_image[0]);

  myATM0130.drawBlock_16px(34, 61, icon[ICON_GRAYHEART]);
  myATM0130.drawBlock_16px(34, 61, icon[ICON_HEART], (uint8_t)(16 - 16.0 * (((double)character.favorability) / 100.0)), 0, 0, 0);
  myATM0130.drawBlock_16px(34, 77, icon[ICON_GRAYSTMACH]);
  myATM0130.drawBlock_16px(34, 77, icon[ICON_STMACH], (uint8_t)(16 - 16.0 * (((double)character.stomach) / 100.0)), 0, 0, 0);
}

void UI::drawSave() {

}

void UI::drawSetting() {
  if(!device.isServerStarted){
    device.serverBegin();
  }
  myATM0130.clearScreen(BLACK16);
  myATM0130.setColor(GREEN16);
  myATM0130.putStr(0,1,"Please Access to\0");
  myATM0130.putStr(0,9,"SSID:\0");
  myATM0130.putStr(0,17,device.getServerssid());
  myATM0130.putStr(0,25,"PASS:\0");
  myATM0130.putStr(0,33,device.getServerpass());
  myATM0130.setColor(WHITE16);
  myATM0130.putStr(0,49,"URL:\nhttp://192.168.4.1/\0");
}

void UI::draw() {
  myATM0130.drawBlock(30, 70, 16, 16, icon[ICON_HAND]);
  if (this->menuOpen) {
    if      (this->UIcursor == ICON_HAND   ) ;
    else if (this->UIcursor == ICON_FOOD   ) ;
    else if (this->UIcursor == ICON_INFO   ) this->drawInfo();
    else if (this->UIcursor == ICON_SAVE   ) this->drawSave();
    else if (this->UIcursor == ICON_SLEEP  ) ;
    else if (this->UIcursor == ICON_SETTING) this->drawSetting();
    else ;
  }
  else {
    if(device.isServerStarted){
      device.serverEnd();
    }
    this->drawMenubar();
  }
}
