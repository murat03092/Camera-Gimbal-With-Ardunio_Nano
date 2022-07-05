// ============= INCLUDING LIBRARIES AND DECLARING VARIABLES =============
#include<Wire.h>
#include <Servo.h>
#include <MPU6050.h> //Mpu6050 kütüphanesi ekledik
const int MPU_addr=0x68; // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;
float delta_t = 0.005;
float pitchAcc,rollAcc, pitch, roll, pitched;
float Yuksek_filtre= 0.98;

// ============= INITIAL SETUP ===========================================
Servo myservo1, myservo2;
void setup(){
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); 
Wire.write(0); // mpu çalıştırılır.
Wire.endTransmission(true); // haberkeşme başlatılır
Serial.begin(115200);
myservo1.attach(9);
myservo2.attach(11);

}
// ============= MAIN LOOP ===============================================
void loop(){
Wire.beginTransmission(MPU_addr); //0x6B ı2c iletişim adresi belirlendi, adres mpu nun adresidir. ordan çekilir.
Wire.write(0x3B); // sensörden alınan ilk değerin kaydedileceği birim
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true); // 14 Byte lık veri istenir
AcX=Wire.read()<<8|Wire.read(); // 0x3B  & 0x3C // okunan değerler bit kaydırma işlemi ile değişkenlere yazdırılır

AcY=Wire.read()<<8|Wire.read(); // 0x3D  & 0x3E

AcZ=Wire.read()<<8|Wire.read(); // 0x3F  & 0x40

GyX=Wire.read()<<8|Wire.read(); // 0x43  & 0x44

GyY=Wire.read()<<8|Wire.read(); // 0x45  & 0x46

GyZ=Wire.read()<<8|Wire.read(); // 0x47  & 0x48


//Tamamlayıcı filter
long Karesi_P=((long)AcZ*AcZ+(long)AcY*AcY);
long Karesi_R=((long)AcZ*AcZ+(long)AcX*AcX);

pitch+=((-AcY/32.8f)*(delta_t));// ÖRNEKLEME PERİYODU, acy deg/s cinsindendir. toplam acı olarak dereceye çevirilir.
roll+=((-AcX/32.8f)*(delta_t)); //32.8 veri önündeki - de ters acı yapılması için, delta_t=0,005 tir

pitchAcc= atan((AcY/sqrt(Karesi_P))*RAD_TO_DEG); // rad_to_deg 180/3,14 den elde edilen sayıdır.
rollAcc =atan((AcX/sqrt(Karesi_R))*RAD_TO_DEG);

pitch =(Yuksek_filtre*pitch + (1.0f-Yuksek_filtre)*pitchAcc);//pitch-yunuslama
roll =(Yuksek_filtre*roll + (1.0f-Yuksek_filtre)*rollAcc);//roll-dönme


int a = map(roll,-129,108,0,180); //elde edilen değerler servo için sınırlandırılır
int b = map(pitch, -121,121,0,180);

if (b<=90){myservo2.write(b+6);} // kaşıgın dik belirli bir açıda sınırlanmasıdır.
if (b>90){myservo2.write(b-5);} // kaşıgın dik konumda sınırlanmasıdır.

if (a<=89){myservo1.write(a+6);} //kaşığın yatay pozisyonunu koruması için ekleme yapılır
if (a>91){myservo1.write(a-4);}






 
Serial.print("pitch: ");
Serial.print(a);
Serial.print("   |   ");Serial.println("\t");
Serial.print("Roll: ");
Serial.print(b);
Serial.print("   |   ");Serial.println("\t");

}
