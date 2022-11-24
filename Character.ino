#include "ATM0130.h"

Character::Character() {
  this->x = 60;
  this->y = 60;
  this->sizex = 32;
  this->sizey = 32;
  this->show = true;
  this->life = 50;
  this->stomach = 50;
  this->happiness = 50;
  this->favorability = 50;

  this->loopTime = 10;

  this->action = 0;
  this->actionAnimationNum = 0;

  this->eyeState = 0;
  this->bodyState = 0;
  this->flip = false;
}

void Character::statusChanger() {
  if (this->stomach > 0) {
    if (this->stomach >= 90) {
      this->life++;
      this->happiness ++;
      if (this->happiness  >= 95 && this->life >= 80) this->favorability++;
    }
    this->stomach--;
    this->happiness --;
  }
  else {
    this->life--;
    this->happiness -= 2;
  }
  if (this->happiness <= 10) {
    this->favorability--;
    if (this->favorability) this->favorability = 0;
  }
}

void Character::changeState(){
}

void Character::move() {
  if (myATM0130.frame % 10 == 0) {
    this->loopTime--;
    if (this->loopTime == 0) {
      statusChanger();
      this->loopTime = 10;
    }
  }
  if (!(bodyState == BODY_SIT || bodyState == BODY_LIE || bodyState == BODY_SLEEP0 || bodyState == BODY_SLEEP1 || bodyState == BODY_DIE)) {
    if (micros() % 100 <= 10) {
      uint8_t vx = micros() % 11 - 5;
      if (vx >= 0) this->flip = !this->flip;
      this->x += vx;
      this->y += micros() % 11 - 5;
      if (this->x < 0) this->x = 0;
      else if (this->x >= 88) this->x = 87;
      if (this->y < 50) this->y = 50;
      else if (this->y >= 88) this->y = 87;
    }
  }
  if(micros() % 100 <= 10) this->flip = !this->flip;
  //this->eyeState++;
  this->eyeState %= 10;
  this->bodyState %= 16;
}

void Character::drawEye() {
  if (this->eyeState == EYE_HIDE
      || this->bodyState == BODY_SAMPLE
      || this->bodyState == BODY_BACK
      || this->bodyState == BODY_DIE   ) {
    return;
  }
  else if (this->bodyState == BODY_LIE) {
    if (this->flip) {
      myATM0130.drawRotateBlock_4px(x + 6, y + 23, character_eye[this->eyeState * 2]);
      myATM0130.drawRotateBlock_4px(x + 6, y + 30, character_eye[this->eyeState * 2 + 1]);
    }
    else {
      myATM0130.drawRotateFlipBlock_4px(x + 22, y + 23, character_eye[this->eyeState * 2]);
      myATM0130.drawRotateFlipBlock_4px(x + 22, y + 30, character_eye[this->eyeState * 2 + 1]);
    }
  }
  else if (this->bodyState == BODY_SLEEP0
           || this->bodyState == BODY_SLEEP1) {
    this->x = 70;
    this->y = 55;
    this->flip = false;
    if (this->flip) {
      myATM0130.drawRotateBlock_4px(x + 6, y + 21, character_eye[this->eyeState * 2]);
      myATM0130.drawRotateBlock_4px(x + 6, y + 27, character_eye[this->eyeState * 2 + 1]);
    }
    else {
      myATM0130.drawRotateFlipBlock_4px(x + 22, y + 21, character_eye[this->eyeState * 2]);
      myATM0130.drawRotateFlipBlock_4px(x + 22, y + 27, character_eye[this->eyeState * 2 + 1]);
    }
  }
  else if(this->bodyState == BODY_SIT){
    if (this->flip) {
      myATM0130.drawFlipBlock_4px(x + 17, y + 17, character_eye[this->eyeState * 2]);
      myATM0130.drawFlipBlock_4px(x + 10, y + 17, character_eye[this->eyeState * 2 + 1]);
    }
    else {
      myATM0130.drawBlock_4px(x + 11, y + 17, character_eye[this->eyeState * 2]);
      myATM0130.drawBlock_4px(x + 18, y + 17, character_eye[this->eyeState * 2 + 1]);
    }
  }
  else {
    if (this->flip) {
      myATM0130.drawFlipBlock_4px(x + 17, y + 10, character_eye[this->eyeState * 2]);
      myATM0130.drawFlipBlock_4px(x + 10, y + 10, character_eye[this->eyeState * 2 + 1]);
    }
    else {
      myATM0130.drawBlock_4px(x + 11, y + 10, character_eye[this->eyeState * 2]);
      myATM0130.drawBlock_4px(x + 18, y + 10, character_eye[this->eyeState * 2 + 1]);
    }
  }
}

void Character::draw() {
  myATM0130.drawBlock_32px(x, y, character_image[this->bodyState], this->flip);
  drawEye();
}
