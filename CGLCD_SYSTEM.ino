/****************
   ※暫く動かすとリセットが掛かる。要修正。(持って300秒程、短いと10秒以内に発生する)
   ->徐々にスタック(ヒープ)が蓄積されてスタックオーバーフローが発生していると思われる。
   ->電流、電圧の問題の可能性がある。(http://www.netosa.com/blog/2017/10/esp-wroom-02-wdt-reset.html)
   ->画像データの問題の可能性
   ->Serial通信の問題の可能性(https://macsbug.wordpress.com/2016/06/20/esp8266-of-error-message/)]
   ->randomをコメントアウトすると駆動が2,500,000ms以上続いた。
   ->疑似乱数の代わりにmicros()を使用して乱数生成を行う。
   ->駆動50,000,000ms続行
*/

//2022/11/15 2:00 電源耐久試験開始 sleepなし、単三乾電池2本(3.0V)による試験 wifi接続は時間受信最初の1回のみ
// 2022/11/15 11:20:31 最終ログ
//液晶が先に見えなくなる。液晶の電源制御は必須。
//そもそも電圧が3.0Vでは不十分だと思われる。

//deep sleep関連クロックの保持->　https://www.robmiles.com/journal/2020/1/22/esp32-retaining-timing-over-deep-sleep


#include "ATM0130.h"

bool button_press_flag = false;


void checker() {
  //運用情報のチェック
  if (millis() - device.last_millis >= 10000) {
    device.last_millis = millis();
    Serial.print("executeTime: ");
    Serial.println(device.last_millis);
    if(device.isTimeConfigured) device.timeLog();
    else device.readTimeLog();
    int v = ESP.getVcc();
    Serial.println("VCC="+String(v/1000.0)+"V");
  }
  buttonChecker();
}

void buttonChecker() {
  uint8_t button_reader = 0b000;
  if (digitalRead(BUTTON_A_PIN) == LOW) {
    button_reader |= (1 << BUTTON_A);
  }
  if (digitalRead(BUTTON_B_PIN) == LOW) {
    button_reader |= (1 << BUTTON_B);
  }
  device.setButtonState(button_reader);
}

// 割り込み処理
void IRAM_ATTR button_pushed() {
  if (!button_press_flag) {
    button_press_flag = true;
  }
}


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);

  // 割り込みを登録 LOWをトリガーとする
  attachInterrupt(BUTTON_A_PIN, button_pushed, FALLING);
  attachInterrupt(BUTTON_B_PIN, button_pushed, FALLING);

  Serial.begin(115200);
  Serial.println("\nSYSTEM START");
  
  digitalWrite(LED_PIN,HIGH);

  myATM0130.begin();

  device.WiFiBegin();

  gameobjects[0] = &background;
  gameobjects[1] = &character;
  gameobjects[2] = &userinterface;
  gameobjects[3] = NULL;
}

void loop() {
  checker();
  if (device.tryWiFiConnect) {
    device.WiFiConnectCheck();
    if (device.isTimeConfigured) device.WiFiEnd();
  }
  
  if (device.isServerStarted) {
    device.serverWaitAccess();
    if (device.getButtonState(BUTTON_A, BUTTON_PRESSED)){
      device.serverEnd();
    }
  }
  else {
    for (uint8_t i = 0; gameobjects[i] != NULL; i++) {
      gameobjects[i]->move();
      if (gameobjects[i]->show) gameobjects[i]->draw();
    }
    myATM0130.updateScreen();
  }

  yield();
}
