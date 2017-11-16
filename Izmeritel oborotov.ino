#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

volatile unsigned long start_t1, stop_t1, start_t2, stop_t2;
volatile uint16_t rpm_cntr1, rpm_cntr2;

char line1[17] = "R1= ";
char line2[17] = "R2= ";

LiquidCrystal_PCF8574 lcd(0x3F);  // установите для ЖК-дисплея 0x27 0x3F для 16-ти символов и 2-строчного дисплея

void setup() {

Serial.begin(115200);
  
  attachInterrupt(digitalPinToInterrupt(2), interrupt1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), interrupt2, RISING);
  
rpm_cntr1 = 0;
rpm_cntr2 = 0;

lcd.begin(16, 2);                    //  инициализировать lcd
lcd.setBacklight(255);
lcd.home(); 
lcd.clear();                         // очищает экран и ставит курсор в первую позицию

 delay(500);

}

void interrupt1() {
if (rpm_cntr1 == 0) start_t1 = micros();
stop_t1 = micros();
rpm_cntr1++;
}
void interrupt2() {
if (rpm_cntr2 == 0) start_t2 = micros();
stop_t2 = micros();
rpm_cntr2++;
}

void loop() {
  
/*
float realRPM1 = (rpm_cntr1 < 2) ? 0.0 : (float)(rpm_cntr1 - 1) * 60000000.0 / (float)(stop_t1 - start_t1);
rpm_cntr1 = 0;
float realRPM2 = (rpm_cntr2 < 2) ? 0.0 : float(rpm_cntr2 - 1) * 60000000.0 / float(stop_t2 - start_t2);
rpm_cntr2 = 0;
*/


float realRPM1 = 0.0;
if (rpm_cntr1 > 0)
{
realRPM1 = (float) (rpm_cntr1 - 1) * 60000000.0f / (float)(stop_t1 - start_t1);

Serial.print("cntr=");
 Serial.flush();
Serial.println(rpm_cntr1);
 
Serial.print("start=");
 Serial.flush();
Serial.println(start_t1);

Serial.print("stop=");
 Serial.flush();
Serial.println(stop_t1);

rpm_cntr1 = 0;
}

//float prSpr1=realRPM1/7 ;                  //Делим на 7 лопастей
       
/*
dtostrf(realRPM1, 8, 0, line1 + 3);          //собираю строчку 4 здесь будет скорост вращ верхнего мотора
//line2[7] = ' ';
*/
dtostrf(rpm_cntr1, 6, 0 , line2 + 3);    //а тут нижнего мотора.


if (realRPM1 > 5)
{
dtostrf(realRPM1, 6, 0, line1 + 3); //собираю строчку 4 здесь будет скорост вращ верхнего мотора
line2[11] = ' '; 
}
/*
if(realRPM2 > 5)
{
dtostrf(realRPM2, 3, 0, line2 + 8); //а тут нижнего мотора.
line2[11] = ' '; 
}
 */

Serial.print("RPM=");
Serial.println(realRPM1);

line1[16] = 0;
line2[16] = 0;

lcd.setCursor(0, 0);
lcd.print(line1);
lcd.setCursor(0, 1);
lcd.print(line2);

}

