//AE-ATM0310B3 with Arduino UNO R3
//2020/01/12

#include "ATM0130.h"

#define swap(x,y) do{uint8_t temp=x;x=y;y=temp;}while(0)

ATM0130::ATM0130(uint8_t pin_dat_cmd, uint8_t pin_reset) {
  
  dat_cmd = pin_dat_cmd;
  reset = pin_reset;
  pinMode(dat_cmd, OUTPUT);
  pinMode(reset, OUTPUT);

  pinMode(SS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

  spi_settings = SPISettings(16000000, MSBFIRST, SPI_MODE0);
  SPI.begin();
  //↑これがなかったから表示されていなかった。

  setFigColor(0x0000);
  setCharColor(0xFFFF);
  setCharColorBG(0x00);
  
}

void ATM0130::begin(void) {
  digitalWrite(SS, HIGH);
  resetLCD();
  SPI.beginTransaction(spi_settings);
  digitalWrite(SS, LOW);

  writeReg(0x11);
  delay(100);

  writeReg(0x36);  //MADCTL
  writeData(0x00);
  //MY=0
  //MX=0
  //MV=0
  //ML=0
  //RGB=0
  //MH=0
  writeReg(0x3A);
  writeData(0x55); //65K color , 16bit / pixel

  ////--------------------------------ST7789V Frame rate

  writeReg(0xb2);
  writeData(0x0c);
  writeData(0x0c);
  writeData(0x00);
  writeData(0x33);
  writeData(0x33);

  delay(2);

  writeReg(0xb7);
  writeData(0x75);

  delay(2);
  ////---------------------------------ST7789V Power
  writeReg(0xc2);
  writeData(0x01);

  delay(2);

  writeReg(0xc3);
  writeData(0x10);

  delay(2);

  writeReg(0xc4);
  writeData(0x20);

  delay(2);

  writeReg(0xc6);
  writeData(0x0f);

  writeReg(0xb0);
  writeData(0x00);
  writeData(0xf0); //RRRR RGGGG GGGB BBBB

  delay(2);

  writeReg(0xD0);
  writeData(0xA4);
  writeData(0xA1);
  delay(2);

  ////--------------------------------ST7789V gamma
  writeReg(0x21);

  delay(2);

  writeReg(0xbb);
  writeData(0x3b);

  delay(2);

  writeReg(0xE0);    //Set Gamma
  writeData(0xF0);
  writeData(0x0b);
  writeData(0x11);
  writeData(0x0e);
  writeData(0x0d);
  writeData(0x19);
  writeData(0x36);
  writeData(0x33);
  writeData(0x4b);
  writeData(0x07);
  writeData(0x14);
  writeData(0x14);
  writeData(0x2c);
  writeData(0x2e);

  delay(2);

  writeReg(0xE1);    //Set Gamma
  writeData(0xF0);
  writeData(0x0d);
  writeData(0x12);
  writeData(0x0b);
  writeData(0x09);
  writeData(0x03);
  writeData(0x32);
  writeData(0x44);
  writeData(0x48);
  writeData(0x39);
  writeData(0x16);
  writeData(0x16);
  writeData(0x2d);
  writeData(0x30);

  writeReg(0x2A);
  writeData(0x00);
  writeData(0x00);
  writeData(0x00);
  writeData(0xEF);

  writeReg(0x2B);
  writeData(0x00);
  writeData(0x00);
  writeData(0x00);
  writeData(0xEF);

  writeReg(0x29);    //Display on

  delay(2);
  writeReg(0x2c);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}


void ATM0130::setFigColor(uint8_t r, uint8_t g, uint8_t b) {
  fig_color = convRGB(r, g, b);
}

void ATM0130::setFigColor(uint16_t c) {
  fig_color = c;
}

void ATM0130::drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  uint16_t loop = width * height;
  uint8_t colorH = fig_color >> 8;
  uint8_t colorL = fig_color & 0x00FF;

  SPI.beginTransaction(spi_settings);
  digitalWrite(SS, LOW);
  setWindow(x, y, width, height);

  digitalWrite(dat_cmd, HIGH);
  for (uint16_t i = 0; i < loop; i++) {
    SPI.transfer(colorH);
    SPI.transfer(colorL);
  }
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}

void ATM0130::setCharPlace(uint8_t x, uint8_t y) {
  char_x = x;
  char_y = y;
}

void ATM0130::setCharColor(uint8_t r, uint8_t g, uint8_t b) {
  char_color = convRGB(r, g, b);
}

void ATM0130::setCharColor(uint16_t c) {
  char_color = c;
}

void ATM0130::setCharColorBG(uint8_t r, uint8_t g, uint8_t b) {
  char_color_bg = convRGB(r, g, b);
}

void ATM0130::setCharColorBG(uint16_t c) {
  char_color_bg = c;
}

void ATM0130::print(char ch) {
  if (char_x > 235) {
    char_x = 0;
    char_y += 8;
  }
  if (char_y > 232) {
    char_x = 0;
    char_y = 0;
  }

  if (ch == '\n') {
    char_x = 0;
    char_y += 8;
  }
  else {
    setCharQueue(ch);
    SPI.beginTransaction(spi_settings);
    digitalWrite(SS, LOW);
    writeCharQueue();
    char_x += 6;
    digitalWrite(SS, HIGH);
    SPI.endTransaction();
  }
}

void ATM0130::print(String str) {
  uint8_t len = str.length();
  for (uint8_t i = 0; i < len; i++)
    print(str.charAt(i));
}

void ATM0130::writeReg(uint8_t data) {
  digitalWrite(dat_cmd, LOW);
  SPI.transfer(data);
}

void ATM0130::writeData(uint8_t data) {
  digitalWrite(dat_cmd, HIGH);
  SPI.transfer(data);
}

void ATM0130::resetLCD(void) {
  digitalWrite(reset, HIGH);
  delay(20);
  digitalWrite(reset, LOW);
  delay(20);
  digitalWrite(reset, HIGH);
  delay(20);
}

void ATM0130::setWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  writeReg(0x2A);
  writeData(0x00);
  writeData(x);
  writeData(0x00);
  writeData(x + width - 1);

  writeReg(0x2B);
  writeData(0x00);
  writeData(y);
  writeData(0x00);
  writeData(y + height - 1);

  writeReg(0x2c);
}

void ATM0130::putPixel(uint16_t color) {
  digitalWrite(dat_cmd, HIGH);
  SPI.transfer(color >> 8);
  SPI.transfer(color & 0xFF);
}

void ATM0130::setCharQueue(uint8_t c) {
  if ((c >= 0x20) && (c <= 0x7E)) {
    c -= 0x20;
    for (uint8_t i = 0; i < 5; i++) {
      char_queue[i] = chars[5 * c + i];
    }
  }
  else {
    for (uint8_t i = 0; i < 5; i++) {
      char_queue [i] = 0xFF;
    }
  }
}

void ATM0130::writeCharQueue() {
  setWindow(char_x, char_y, 5, 8);
  for (uint8_t i = 0; i < 5; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      if ((char_queue[i] & (0x80 >> j)) > 0) {
        putPixel(char_color);
      }
      else {
        putPixel(char_color_bg);
      }
    }
  }
  setWindow(char_x + 5, char_y, 1, 8);
  for (uint8_t i = 0; i < 8; i++) {
    putPixel(char_color_bg);
  }
}

uint16_t ATM0130::convRGB(uint8_t red, uint8_t green, uint8_t blue) {
  uint16_t color = 0;
  color = blue >> 3;
  color |= ((green & 0xFC) << 3);
  color |= ((red & 0xF8) << 8);
  return color;
}




inline void ATM0130::updatePoint(int16_t x, int16_t y, uint16_t color){
  if(x < 0 || WIDTH <= x || y < 0 || HEIGHT <= y) return;
  backScreen[y][x] = color;
}

inline void ATM0130::updateBlock_8px(int16_t x, int16_t y, uint16_t block[8][8]){
  uint8_t firsti = 0,endi = 8;
  uint8_t firstj = 0,endj = 8;//firsti,firstjを経由しないと下の二重ループがちゃんと機能しない。
  if(x < 0) firsti = abs(x);
  else if(x+endi > WIDTH) endi = WIDTH;
  if(y < 0) firstj = abs(y);
  else if(y+endj > HEIGHT) endj = HEIGHT;

  for(uint8_t j=firstj;j<endj;j++){
    for(uint8_t i=firsti;i<endi;i++){
      backScreen[y+j][x+i] = block[j][i];
    }
  }
  return;
}

inline void ATM0130::updateBlock_16px(int16_t x, int16_t y, uint16_t block[16][16]){
  int i = 0,endi = 16;
  int j = 0,endj = 16;
  if(x < 0) i -= x;
  else if(x+endi > WIDTH) endi = WIDTH;
  if(y < 0) j -= y;
  else if(y+endj > HEIGHT) endj = HEIGHT;

  for(i;i<endi;i++){
    for(j;j<endj;j++){
      backScreen[y+j][x+i] = block[j][i];
    }
  }  
}

inline void ATM0130::updateBlock_32px(int16_t x, int16_t y, uint16_t block[32][32]){
  int i = 0,endi = 32;
  int j = 0,endj = 32;
  if(x < 0) i -= x;
  else if(x+endi > WIDTH) endi = WIDTH;
  if(y < 0) j -= y;
  else if(y+endj > HEIGHT) endj = HEIGHT;

  for(i;i<endi;i++){
    for(j;j<endj;j++){
      backScreen[y+j][x+i] = block[j][i];
    }
  }  
}

void ATM0130::updateScreen(){
  uint8_t colorH;
  uint8_t colorL;
  SPI.beginTransaction(spi_settings);
  digitalWrite(SS, LOW);
  setWindow(0, 0, WIDTH*2, HEIGHT*2);

  digitalWrite(dat_cmd, HIGH);
  for (uint8_t i = 0; i < HEIGHT; i++) {
    uint8_t j = 0;
    for(j = 0; j < WIDTH; j++){
      colorH = backScreen[i][j] >> 8;
      colorL = backScreen[i][j] & 0x00FF;
      SPI.transfer(colorH);
      SPI.transfer(colorL);
      SPI.transfer(colorH);
      SPI.transfer(colorL);
    }
    for(j = 0; j < WIDTH; j++){
      colorH = backScreen[i][j] >> 8;
      colorL = backScreen[i][j] & 0x00FF;
      SPI.transfer(colorH);
      SPI.transfer(colorL);
      SPI.transfer(colorH);
      SPI.transfer(colorL);
    }
  }
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}

void ATM0130::clearScreen(uint16_t color){
  for(int i=0;i<WIDTH;i++){
    for(int j=0;j<HEIGHT;j++){
      backScreen[j][i] = color; 
    }
  }
}

void ATM0130::drawDot(int16_t x, int16_t y, uint16_t color){
  backScreen[y][x] = color;
}

void ATM0130::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color){
  uint8_t x, y, tmp;
  double slope = 0;

  if(x0 <       0) x0 =          0;
  if(y0 <       0) y0 =          0;
  if(x0 >=  WIDTH) x0 = WIDTH  - 1;
  if(y0 >= HEIGHT) y0 = HEIGHT - 1;
  if(x1 <       0) x1 =          0;
  if(y1 <       0) y1 =          0;
  if(x1 >=  WIDTH) x1 = WIDTH  - 1;
  if(y1 >= HEIGHT) y1 = HEIGHT - 1;
  if(x0 == x1 && y0 == y1){
    drawDot(x0,y0,color);
    return;
  }
  
  if (abs(x1 - x0) > abs(y1 - y0)) {
    if (x1 < x0) {                         // 座標値の入れ替え
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
    }
    slope = (double)(y1 - y0) / (x1 - x0); // 傾斜率の算出
    for (int i=0; i<=(x1-x0); i++) {
      x = x0 + i;
      y = y0 + slope * i;
      drawDot(x, y, color);
    }
    } else {
    if (y1 < y0) {                         // 座標値の入れ替え
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
    }
    slope = (double)(x1 - x0) / (y1 - y0); // 傾斜率の算出
    for (int i=0; i<=(y1-y0); i++) {
      x = x0 + slope * i;
      y = y0 + i;
      drawDot(x, y, color);
    }
  }
}
