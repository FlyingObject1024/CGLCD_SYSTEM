
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

enum{
  ACTION_NORMAL,
  ACTION_
};

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
    
    int8_t favorability;//表出好感度
    int8_t hiddenFavorability;//真の好感度
    //favorability = 100*hiddenfavorability/(time+hiddenFavorability)

    
    uint8_t loopTime;//パラメータ変動時間0になったら変動
    

    Character();
    void move();
    void draw();

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

    void drawMenubar();
    void drawInfo();
    void drawSave();
    void drawSetting();
    
  public:
    UI();
    void move();
    void draw();
    
    void drawClock(int16_t x,int16_t y);
    
};

#endif
