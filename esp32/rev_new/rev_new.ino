#include <esp_now.h>
#include <WiFi.h>

typedef struct Message {
  char messagedata;  // ตัวอักษรที่ระบุว่าเป็น A, B, C, D, E หรือ F
  float voltage;     // ค่าแรงดันแบตเตอรี่
  int percen;
} Message;

Message receivedData;

const unsigned long timeout = 3000;                       // 3 วินาที
unsigned long lastReceiveTime[6] = { 0, 0, 0, 0, 0, 0 };  // เก็บเวลาล่าสุดของ A-F
char deviceIDs[6] = { 'A', 'B', 'C', 'D', 'E', 'F' };     // รายชื่ออุปกรณ์ A-F

unsigned long lastDisplayTime = 0;  // เวลาสุดท้ายที่แสดงข้อมูล
const unsigned long displayInterval = 300; // ระยะเวลาที่จะแสดงข้อมูลใหม่

// ฟังก์ชัน callback เมื่อได้รับข้อมูล
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // ตรวจหาว่าข้อมูลที่ได้รับมาจากอุปกรณ์ตัวไหน (A-F)
  for (int i = 0; i < 6; i++) {
    if (receivedData.messagedata == deviceIDs[i]) {
      lastReceiveTime[i] = millis();  // อัปเดตเวลาล่าสุด
      break;
    }
  }

  // แสดงค่าที่ได้รับ
  Serial.print("\t");
  Serial.print(receivedData.messagedata);
  Serial.println(receivedData.percen);
  // Serial.println(receivedData.voltage, 2);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(5000);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  unsigned long currentMillis = millis();

  // วนลูปตรวจสอบว่าเวลาผ่านไปนานกว่าหรือเท่ากับ timeout
  for (int i = 0; i < 6; i++) {
    if (currentMillis - lastReceiveTime[i] > timeout) {
      // แสดงค่าที่ไม่มีข้อมูล
      if (currentMillis - lastDisplayTime >= displayInterval) {
        lastDisplayTime = currentMillis; // อัปเดตเวลาที่แสดงผล
        Serial.print("\t");
        Serial.print(deviceIDs[i]);
        Serial.println("0");              // ถ้าไม่มีข้อมูลให้แสดงค่า 0
        //Serial.println("------------------------");
      }
    }
  }

  // คำสั่งหน่วงเวลาน้อยๆ เพื่อให้ loop ทำงานได้ดีขึ้น
  delay(10);  // รอเวลาสั้นๆ เพื่อให้แน่ใจว่า loop ทำงานได้ดี
}
