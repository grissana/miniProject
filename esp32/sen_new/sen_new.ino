#include <esp_now.h>
#include <WiFi.h>

#define BATTERY_PIN 0  // ใช้ ADC34 อ่านแรงดันแบตเตอรี่ (หากใช้ ESP32)


// กำหนดช่วงแรงดันที่ต้องการให้เป็นเปอร์เซ็นต์
const float minADC = 2047.0;     // ค่าที่อ่านได้เมื่อแบตเตอรี่อ่อนสุด 2.5v = 2047
const float maxADC = 2950;  // ค่าที่อ่านได้เมื่อแบตเตอรี่เต็ม 3439.8 = 4.2v / 2950 = 3.6v

uint8_t receiverMac[] = { 0x8C, 0x4B, 0x14, 0x09, 0x29, 0xC4 };  // MAC ของตัวรับ 0x8C, 0x4B, 0x14, 0x09, 0x29, 0xC4 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

// โครงสร้างข้อมูลที่ส่ง
typedef struct Message {
  char messagedata;  // ข้อความ (เช่น 'A')
  float voltage;     // ค่าแรงดันแบตเตอรี่
  int percen;        // เปอร์เซ็นต์แบตเตอรี่
} Message;

Message dataToSend;  // ข้อมูลที่จะส่ง

// ฟังก์ชัน callback เมื่อส่งข้อมูลสำเร็จหรือผิดพลาด
void OnDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // ตั้งค่าโหมด Wi-Fi เป็น Station

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);  // ตั้งค่า callback

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(BATTERY_PIN, INPUT);
}

void loop() {
  int rawValue = analogRead(BATTERY_PIN);  // อ่านค่าจากขา A0
  //Serial.println(rawValue);
  //float voltage = rawValue * (5.0 / 4095);  // คำนวณแรงดัน (V)
  float voltage = (rawValue / 4095.0) * 2.6 * 2;  // คำนวณแรงดันจริง
    // คำนวณเปอร์เซ็นต์แบตเตอรี่
  int percen = map(rawValue, minADC, maxADC, 0, 100);

  // ปรับช่วงแรงดันแบตเตอรี่ (2.5V - 4.2V) เป็นเปอร์เซ็นต์
  // float percen = map(rawValue, 2048, 3440, 0, 100);

  // จำกัดค่าเปอร์เซ็นต์ให้อยู่ในช่วง 0-100%
  percen = constrain(percen, 0.0, 100.0);
  int percenInt = round(percen);  // ปัดทศนิยมเป็นจำนวนเต็มตามกฎคณิตศาสตร์

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V | Battery: ");
  Serial.print(percenInt);
  Serial.println(" %");

  // กำหนดข้อมูลที่ต้องการส่ง
  dataToSend.voltage = voltage;
  dataToSend.messagedata = 'C';   // ใช้อักษรตัวเดียว (char) 0.4 0.6 0.8 1.5 2.0
  dataToSend.percen = percenInt;  // ส่งค่าเปอร์เซ็นต์แบตเตอรี่

  esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&dataToSend, sizeof(dataToSend));

  Serial.print("Message: ");
  Serial.print(dataToSend.messagedata);
  Serial.print(" | Battery Voltage: ");
  Serial.print(voltage);
  Serial.print(" V | Battery Percent: ");
  Serial.println(dataToSend.percen);

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Send failed");
  }

  delay(300);  // ส่งข้อมูลทุก ๆ 2 วินาที
}
