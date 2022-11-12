#include "ATM0130.h"

Character::Character() {
  this->x = 60;
  this->y = 60;
  this->sizex = 32;
  this->sizey = 32;
  this->show = true;
  this->life = 50;
  this->stomach = 50;
  this->favorability = 50;
  this->hiddenFavorability = 50;

  this->loopTime = 10;

  this->action = 0;
  this->actionAnimationNum = 0;

  this->eyeState = 0;
  this->bodyState = 0;
  this->flip = false;  
}

void Character::statusChanger(){
  if(this->stomach > 0){
    if(this->stomach >= 90){
      this->life++;
      this->favorability++;
      if(this->favorability >= 95 && this->life >= 80) this->hiddenFavorability++;
    }
    this->stomach--;
    this->favorability--;
  }
  else{
    this->life--;
    this->favorability -= 2;
  }
  if(this->favorability <= 10){
    this->hiddenFavorability--;
    if(this->hiddenFavorability) this->hiddenFavorability = 0;
  }
}

void Character::move() {
  if(myATM0130.frame % 10 == 0){
    this->loopTime--;
    if(this->loopTime == 0){
      statusChanger();
      this->loopTime = 10;
    }
  }
  if (micros() % 100 <= 10) {
    this->x += micros() % 11 - 5;
    this->y += micros() % 11 - 5;
    if (this->x < 0) this->x = 0;
    else if (this->x >= 88) this->x = 87;
    if (this->y < 50) this->y = 50;
    else if (this->y >= 88) this->y = 87;
  }
  if (micros() % 100 <= 8) {
    this->flip = !this->flip;
  }
  //this->eyeState++;
  this->eyeState %= 10;
  this->bodyState %= 16;
}

void Character::drawEye() {
  if(this->eyeState == EYE_HIDE) return;
  if (this->flip) {
    myATM0130.drawFlipBlock_4px(x+17, y+10, character_eye[this->eyeState*2]);
    myATM0130.drawFlipBlock_4px(x+10, y+10, character_eye[this->eyeState*2 + 1]);
  }
  else{
    myATM0130.drawBlock_4px(x+11, y+10, character_eye[this->eyeState*2]);
    myATM0130.drawBlock_4px(x+18, y+10, character_eye[this->eyeState*2 + 1]);
  }
}

void Character::draw() {
  if (this->flip) myATM0130.drawFlipBlock_32px(x, y, character_image[this->bodyState]);
  else            myATM0130.drawBlock_32px(x, y, character_image[this->bodyState]);
  drawEye();
}
