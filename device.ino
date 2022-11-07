#include "ATM0130.h"

Device::Device() {
  this->last_millis = 0;

  this->ssid = "";
  this->pass = "";
  readSettings();

  this->isTimeConfigured = false;
  this->isWiFiConnected = false;
  this->tryWiFiConnect = true;

  this->button_state    = 0b00000000;
  this->button_pressed  = 0b00000000;
  this->button_pressing = 0b00000000;
  this->button_release  = 0b00000000;
}

void Device::setButtonState(uint8_t states) {
  this->button_pressed  = ~this->button_state      & states;
  this->button_pressing = this->button_pressing    & states;
  this->button_pressing = this->button_pressing    | this->button_state;
  this->button_release  = this->button_state       & ~states;

  this->button_state    = states;                                        // 今回の状態を保存
}

bool Device::getButtonState(uint8_t button, uint8_t state) {
  if      (state == BUTTON_NOTPRESS) return (bool)( ((~this->button_state) >> button) & 0b1 );
  else if (state == BUTTON_PRESSED ) return (bool)( (this->button_pressed  >> button) & 0b1 );
  else if (state == BUTTON_PRESSING) return (bool)( (this->button_pressing >> button) & 0b1 );
  else if (state == BUTTON_RELEASE ) return (bool)( (this->button_release  >> button) & 0b1 );

  return false;
}

void handleRootGet() {
  String html = "";
  html += "<h1>WiFi Settings</h1>";
  html += "<form method='post'>";
  html += "  <input type='text' name='ssid' placeholder='ssid'><br>";
  html += "  <input type='text' name='pass' placeholder='pass'><br>";
  html += "  <input type='submit'><br>";
  html += "</form>";
  server.send(200, "text/html", html);
}

void handleRootPost() {
  String getssid = server.arg("ssid");
  String getpass = server.arg("pass");

  SPIFFS.begin();
  File f = SPIFFS.open("/wifi_setting.txt", "w");
  f.println(getssid);
  f.println(getpass);
  f.close();
  SPIFFS.end();

  Serial.println("getSSID: " + getssid);
  Serial.println("getPASS: " + getpass);
  

  String html = "";
  html += "<h1>WiFi Settings</h1>";
  html += getssid + "<br>";
  html += getpass + "<br>";
  server.send(200, "text/html", html);
}

void Device::serverBegin(){
  byte mac[6];
  
  if(this->tryWiFiConnect) WiFiEnd();
  
  WiFi.macAddress(mac);
  serverssid = "";
  for (int i = 0; i < 6; i++) {
    serverssid += String(mac[i], HEX);
  }

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(serverssid.c_str(), serverpass.c_str());

  server.on("/", HTTP_GET, handleRootGet);
  server.on("/", HTTP_POST, handleRootPost);
  server.begin();
  this->isServerStarted = true;
}

void Device::serverWaitAccess(){
  server.handleClient();
}

void Device::serverEnd(){
  server.close();
  this->isServerStarted = false;
  readSettings();
}

String Device::getServerssid(){
  return this->serverssid;
}

String Device::getServerpass(){
  return this->serverpass;
}

void Device::WiFiBegin() {
  WiFi.begin(this->ssid, this->pass);
  this->tryWiFiConnect = true;
}

void Device::WiFiConnectCheck(){
  Serial.println("SSID: " + this->ssid);
  Serial.println("PASS: " + this->pass);
  if (WiFi.status() == WL_CONNECTED){
    this->isWiFiConnected = true;
    setTime();
    this->tryWiFiConnect = false;
  }
  else{
    this->isWiFiConnected = false;
  }
}

void Device::WiFiEnd(){
  if(this->isWiFiConnected){
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    this->isWiFiConnected = false;
    this->tryWiFiConnect = false;
  }
}

void Device::setTime(){
  if (this->isWiFiConnected){
    configTime(JST, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  }
}

void Device::getTime(){
  time_t t;
  
  t = time(NULL);
  tm = localtime(&t);
  
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d\n",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec);

  if(tm->tm_year >= 122) this->isTimeConfigured = true;
}

uint8_t Device::getHour(){
  return tm->tm_hour;  
}

uint8_t Device::getMinute(){
  return tm->tm_min;
}

uint8_t Device::getSecond(){
  return tm->tm_sec;
}

bool Device::readSettings(){
  SPIFFS.begin();
  File f = SPIFFS.open("/wifi_setting.txt","r");
  if(f == NULL) return false;
  
  ssid = f.readStringUntil('\n');
  pass = f.readStringUntil('\n');
  f.close();
  SPIFFS.end();

  ssid.trim();
  pass.trim();
  
  return true;
}
