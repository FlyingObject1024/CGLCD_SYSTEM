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

  this->state = 0;

  this->behavior = 0;
  changeState(ANIME_NORMAL);

  this->eyeState = 0;
  this->bodyState = 0;
  this->flip = false;
}

void Character::statusChanger() {
  if (this->stomach > 0) {
    if (this->stomach >= 90) {
      this->life++;
      this->happiness++;
      if (this->happiness  >= 95 && this->life >= 80) this->favorability++;
    }
    this->stomach--;
    this->happiness--;
  }
  else {
    this->life--;
    this->happiness -= 2;
  }
  if (this->happiness <= 10) {
    this->favorability--;
    if (this->favorability) this->favorability = 0;
  }
  if(this->happiness <= 0) this->happiness = 0;
}

void Character::changeState(uint8_t state) {
  this->state = state;
  if (this->state == STATE_SLEEP) animation = (uint8_t*)animations[ANIME_SLEEPING];
  else if (this->state == STATE_STROKE) animation = (uint8_t*)animations[ANIME_STROKE];
  else if (this->state == STATE_STROKE_HAPPY) animation = (uint8_t*)animations[ANIME_STROKE_HAPPY];
  else animation = (uint8_t*)animations[ANIME_NORMAL];
  this->animationNum = 0;
}

void Character::moveAnimation() {

  this->bodyState = pgm_read_byte(&(*animation));
  this->eyeState = pgm_read_byte(&(*(animation + 1)));
  
  this->animation+=2;
  this->animationNum++;

  if (pgm_read_byte(&(*(animation))) == FLIPBODY) {
    this->flip = !this->flip;
    Serial.println("FLIP");
    Serial.println(pgm_read_byte(&(*(animation))));
  }
  if (pgm_read_byte(&(*(animation+1))) == ARRAYSTOPPER) {
    if (this->state == STATE_STROKE) {
      changeState(STATE_STROKE_HAPPY);
      this->happiness++;
    }
    else if (this->state == STATE_STROKE_HAPPY) {
      changeState(STATE_NORMAL);
    }
    else{
      changeState(this->state);
    }
    Serial.printf("NULL: %d\n", NULL);
    Serial.println(pgm_read_byte(&(*(animation + 3))));
  }
  Serial.printf("animationNum: %d,body: %d,eye: %d\n", animationNum, this->bodyState, this->eyeState);
}

void Character::move() {
  if (myATM0130.frame % 10 == 0) {
    this->loopTime--;
    if (this->loopTime == 0) {
      statusChanger();
      this->loopTime = 10;
    }
  }
  if (this->state == STATE_NORMAL) {
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
  moveAnimation();

  if (this->happiness >= 100) this->happiness = 100;
  this->bodyState %= BODY_IMAGENUM;
  this->eyeState %= EYE_HIDE + 1;
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
    this->y = 39;
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
  else if (this->bodyState == BODY_SIT) {
    if (this->flip) {
      myATM0130.drawFlipBlock_4px(x + 17, y + 17, character_eye[this->eyeState * 2]);
      myATM0130.drawFlipBlock_4px(x + 10, y + 17, character_eye[this->eyeState * 2 + 1]);
    }
    else {
      myATM0130.drawBlock_4px(x + 11, y + 17, character_eye[this->eyeState * 2]);
      myATM0130.drawBlock_4px(x + 18, y + 17, character_eye[this->eyeState * 2 + 1]);
    }
  }
  else if(this->state == STATE_STROKE || this->state == STATE_STROKE_HAPPY) {
    if (this->flip) {
      myATM0130.drawFlipBlock_16px(x + 8, y - 4 + animationNum%4/2%2, icon[ICON_STROKE]);
      myATM0130.drawFlipBlock_4px(x + 17, y + 10 + animationNum%4/2%2, character_eye[this->eyeState * 2]);
      myATM0130.drawFlipBlock_4px(x + 10, y + 10 + animationNum%4/2%2, character_eye[this->eyeState * 2 + 1]);
    }
    else {
      myATM0130.drawBlock_16px(x + 9, y - 4 + animationNum%4/2%2, icon[ICON_STROKE]);
      myATM0130.drawBlock_4px(x + 11, y + 10 + animationNum%4/2%2, character_eye[this->eyeState * 2]);
      myATM0130.drawBlock_4px(x + 18, y + 10 + animationNum%4/2%2, character_eye[this->eyeState * 2 + 1]);
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
