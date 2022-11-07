#include "ATM0130.h"

BackGround::BackGround() {
  this->x = 0;
  this->y = 0;
  this->sizex = 120;
  this->sizey = 120;
  this->show = true;
}

void BackGround::move() {

}

void BackGround::drawOutSide() {
  uint8_t i;
  //空
  myATM0130.setColor(0x02bd);
  myATM0130.drawFillRectangle(33, 17, 58, 23);
  myATM0130.drawFillRectangle(61, 17, 86, 23);
  for (i = 33; i <= 86; i += 2) {
    myATM0130.drawDot(i, 24);
    myATM0130.drawDot(i + 1, 25);
  }
  myATM0130.setColor(0x645e);
  for (i = 33; i <= 86; i += 2) {
    myATM0130.drawDot(i + 1, 24);
    myATM0130.drawDot(i, 25);
  }
  myATM0130.drawFillRectangle(33, 26, 58, 39);
  myATM0130.drawFillRectangle(61, 26, 86, 39);

  //海
  myATM0130.drawBlock_16px(33, 40, sea0);
  myATM0130.drawBlock_8px(49, 40, sea1[0]);
  myATM0130.drawBlock_8px(49, 48, sea1[1]);
  myATM0130.drawFlipBlock_8px(64, 40, sea1[0]);
  myATM0130.drawFlipBlock_8px(64, 48, sea1[1]);
  myATM0130.drawFlipBlock_16px(72, 40, sea0);
  myATM0130.setColor(0x1ddc);
  myATM0130.drawFillRectangle(57, 40, 59, 44);
  myATM0130.drawFillRectangle(61, 40, 63, 44);
  myATM0130.setColor(0x5e9d);
  myATM0130.drawFillRectangle(57, 45, 59, 53);
  myATM0130.drawFillRectangle(61, 45, 63, 53);

}

void BackGround::draw() {
  myATM0130.clearScreen(0xf739);
  drawOutSide();
  myATM0130.setColor(BLACK16);
  myATM0130.drawLine(16, 0, 16, 55);
  myATM0130.drawLine(103, 0, 103, 55);
  myATM0130.drawLine(17, 55, 31, 55);
  myATM0130.drawLine(89, 55, 102, 55);
  myATM0130.drawRectangle(31, 15, 88, 55);
  myATM0130.drawRectangle(32, 16, 87, 54);
  myATM0130.drawRectangle(59, 16, 60, 54);
  myATM0130.drawLine(16, 55, 0, 96);
  myATM0130.drawLine(103, 55, 119, 96);
}
