
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
  
  public:
    BackGround();
    void drawOutSide();
    void move();
    void draw();
};

class Character : public GameObject{
  public:
    uint8_t life;//体力 0になると倒れる(不可逆)
    uint8_t stomach;//空腹度
    
    uint8_t favorability;//表出好感度
    uint8_t hiddenfavorability;//真の好感度
    //favorability = 100*hiddenfavorability/(time+hiddenfavorability)

    
    uint8_t loopTime;//パラメータ変動時間0になったら変動
    

    Character();
    void move();
    void draw();

    
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
