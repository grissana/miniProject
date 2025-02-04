long timer_1 = 2000;
long timer_2 = 200;
void setup() {
  Serial.begin(115200);  // เริ่มการใช้งาน Serial Monitor
  randomSeed(analogRead(0));  // ใช้ค่าแอนะล็อกจากพิน A0 เป็น seed สำหรับการสุ่ม
  delay(timer_1);
}

void loop() {
  int randomNumber = random(1, 101);  // สุ่มค่าระหว่าง 1 ถึง 100
  Serial.print(" A");
  Serial.println(randomNumber);  // แสดงผลค่าที่สุ่มได้
  delay(timer_2);  // หน่วงเวลา 1 วินาที
  
  Serial.print(" B");
  Serial.println(randomNumber);  // แสดงผลค่าที่สุ่มได้
  delay(timer_2);  // หน่วงเวลา 1 วินาที
  
  Serial.print(" C");
  Serial.println(randomNumber);  // แสดงผลค่าที่สุ่มได้
  delay(timer_2);  // หน่วงเวลา 1 วินาที
  
  Serial.print(" D");
  Serial.println(randomNumber);  // แสดงผลค่าที่สุ่มได้
  delay(timer_2);  // หน่วงเวลา 1 วินาที
  
  Serial.print(" E");
  Serial.println(randomNumber);  // แสดงผลค่าที่สุ่มได้
  delay(timer_2);  // หน่วงเวลา 1 วินาที
}
