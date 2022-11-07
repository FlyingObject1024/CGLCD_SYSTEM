#ifndef DEVICE_H_
#define DEVICE_H_


#define JST     3600* 9
#define CONFIG_LWIP_SNTP_UPDATE_DELAY 3600000

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 4

#define BUTTON_A 0
#define BUTTON_B 1

#define BUTTON_NOTPRESS 0
#define BUTTON_PRESSED  1
#define BUTTON_PRESSING 2
#define BUTTON_RELEASE  3

class Device{
  private:
    volatile uint8_t button_state;
    volatile uint8_t button_pressed;
    volatile uint8_t button_pressing;
    volatile uint8_t button_release;
    
    String serverssid = "";
    String serverpass = "yakoshin";
    
    String ssid;
    String pass;

    const char* settings = "/wifi_setting.txt";
    
    struct tm *tm;
    
  public:  
    bool isServerStarted;
  
    bool tryWiFiConnect;
    bool isWiFiConnected;
    volatile bool isTimeConfigured;
  
    volatile unsigned long last_millis;
    
    Device();
    void setButtonState(uint8_t states);
    bool getButtonState(uint8_t button,uint8_t state);

    void serverBegin();
    void serverWaitAccess();
    void serverEnd();
    String getServerssid();
    String getServerpass();
    
    void WiFiBegin();
    void WiFiConnectCheck();
    void WiFiEnd();
    
    void setTime();
    void getTime();
    
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();
    bool readSettings();
};

#endif
