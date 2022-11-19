
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

class GameObject{
  public:
    bool show;
    int16_t x;
    int16_t y;
    uint8_t sizex;
    uint8_t sizey;
    
    virtual void move();
    virtual void draw();

};

class BackGround : public GameObject{
  private:
    class Cloud{
      public:
        uint8_t imageNum;
        int16_t x;
        int16_t y;
        bool show;
        uint8_t downSpeed;
        Cloud();
        void move();
        void draw();
    };
  public:
    BackGround();
    Cloud clouds[10];
    void drawOutSide();
    void move();
    void draw();
};

class Character : public GameObject{
  private:
    void statusChanger();
  public:
    uint8_t life;//体力 0になると倒れる(不可逆)
    uint8_t stomach;//空腹度
    
    int8_t happiness;//楽しさ
    int8_t favorability;//好感度
    //happiness = 100*favorability/(time+favorability)

    
    uint8_t loopTime;//パラメータ変動時間0になったら変動
    

    Character();
    void move();
    void draw();

    void changeState();

    uint8_t action;
    uint8_t actionAnimationNum;
    
    uint8_t eyeState;//現在の目の状態
    uint8_t bodyState;//現在の体の状態
    bool flip;//表示を反転させるフラグ
    void drawEye();
};

class UI : public GameObject{
  private:
    bool menuOpen;
    int8_t UIcursor;
    int8_t secondCursor;


    void moveTouch();
    void moveEat();
    void moveSave();
    void moveSetting();
    
    void drawMenubar();
    
    void drawTouch();
    void drawEat();
    void drawInfo();
    void drawSave();
    void drawSetting();
    
  public:
    UI();
    void move();
    void draw();
    
    void drawClock(int16_t x,int16_t y);
    
};

GameObject* gameobjects[4];
BackGround background;
Character character;
UI userinterface;

#endif
