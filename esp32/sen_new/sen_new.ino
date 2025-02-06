#include <esp_now.h>
#include <WiFi.h>

#define BATTERY_PIN 0  // ใช้ ADC34 อ่านแรงดันแบตเตอรี่ (หากใช้ ESP32)

uint8_t receiverMac[] = { 0x8C, 0x4B, 0x14, 0x09, 0x29, 0xC4 };  // MAC ของตัวรับ

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
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue / 4095.0) * 3.3 * 2;  // คำนวณแรงดันจริง

  // ปรับช่วงแรงดันแบตเตอรี่ (1.0V - 3.6V) เป็นเปอร์เซ็นต์
  float percen = (voltage - 1.0) / (3.6 - 1.0) * 100.0;

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
  dataToSend.messagedata = 'A';  // ใช้อักษรตัวเดียว (char)
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
