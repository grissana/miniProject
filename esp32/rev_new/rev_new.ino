#include <esp_now.h>
#include <WiFi.h>

typedef struct Message {
  char messagedata;  // ตัวอักษรที่ระบุว่าเป็น A, B, C, D, E หรือ F
  float voltage;     // ค่าแรงดันแบตเตอรี่
  int percen;
} Message;

Message receivedData;

const unsigned long timeout = 3000;  // 3 วินาที
unsigned long lastReceiveTime[6];    // เก็บเวลาล่าสุดของ A-F
bool isLost[6] = { false, false, false, false, false, false };  // บันทึกว่าตัวไหนเคยหายไปแล้ว
bool isFirstReceive[6] = { false, false, false, false, false, false }; // เช็คว่าอุปกรณ์เคยส่งข้อมูลแล้วหรือยัง
char deviceIDs[6] = { 'A', 'B', 'C', 'D', 'E', 'F' };  // รายชื่ออุปกรณ์ A-F

// ฟังก์ชัน callback เมื่อได้รับข้อมูล
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // ตรวจหาว่าข้อมูลที่ได้รับมาจากอุปกรณ์ตัวไหน (A-F)
  for (int i = 0; i < 6; i++) {
    if (receivedData.messagedata == deviceIDs[i]) {
      lastReceiveTime[i] = millis();  // อัปเดตเวลาล่าสุด
      isLost[i] = false;  // รีเซ็ตสถานะ ถ้าได้รับข้อมูลใหม่
      isFirstReceive[i] = true;  // บันทึกว่าเคยได้รับข้อมูลแล้ว
      break;
    }
  }

  // แสดงค่าที่ได้รับ
  Serial.print("\t");
  Serial.print(receivedData.messagedata);
  Serial.println(receivedData.percen);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(2000);

  // ตั้งค่าเริ่มต้นของ lastReceiveTime เป็นเวลาปัจจุบัน
  for (int i = 0; i < 6; i++) {
    lastReceiveTime[i] = millis();
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  unsigned long currentMillis = millis();

  for (int i = 0; i < 6; i++) {
    // แสดง 0 เฉพาะถ้าเคยเชื่อมต่อแล้วและขาดการเชื่อมต่อ
    if (isFirstReceive[i] && !isLost[i] && (currentMillis - lastReceiveTime[i] > timeout)) {
      Serial.print("\t");
      Serial.print(deviceIDs[i]);
      Serial.println(" 0");  // แสดงค่า 0 ครั้งเดียวเมื่อขาดการเชื่อมต่อ
      //Serial.println("------------------------");

      isLost[i] = true;  // บันทึกว่าเคยแสดงค่า 0 ไปแล้ว
    }
  }

  //delay(10);  // ลดภาระ CPU
}
