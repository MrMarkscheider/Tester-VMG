#include <Wire.h>                                             
#include <LiquidCrystal_PCF8574.h>                            //Подключаем библиотеку LCD экранчика 

volatile unsigned long start_t1, stop_t1, start_t2, stop_t2;  //Переменные для фиксации начала и конца подсчета оборотов
volatile uint16_t rpm_cntr1, rpm_cntr2;                       //Переменные для хранения данных счетчиков

char line1[17] = "RPM1= ";     //Текстовый массив для вывода значений на LCD 1 сторока
char line2[17] = "RPM2= ";     //Текстовый массив для вывода значений на LCD 2 сторока

LiquidCrystal_PCF8574 lcd(0x3F);  // установите для ЖК-дисплея 0x27 или 0x3F для 16-ти символов и 2-строчного дисплея

void setup() {
  
  attachInterrupt(digitalPinToInterrupt(2), interrupt1, RISING); //Пин подключения первого датчика оборотов
  attachInterrupt(digitalPinToInterrupt(3), interrupt2, RISING); //Пин подключения второго датчика оборотов 
  
rpm_cntr1 = 0;   //Обнуляем первый счетчик 
rpm_cntr2 = 0;   //Обнуляем второй счетчик 

lcd.begin(16, 2);                    //  инициализировать lcd
lcd.setBacklight(255);
lcd.home();                          
lcd.clear();                         // очищает экран и ставит курсор в первую позицию
lcd.print("Revolutions");
lcd.setCursor(0, 1);
lcd.print("per minute");

 delay(900);
}

//Функции прерываний для датчиков оборотов.

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
float realRPM1 = (rpm_cntr1 < 2) ? 0.0 : (float)(rpm_cntr1 - 1) * 60000000.0 / (float)(stop_t1 - start_t1);  //можно считать сразу без запрещения прерываний.
rpm_cntr1 = 0;
float realRPM2 = (rpm_cntr2 < 2) ? 0.0 : float(rpm_cntr2 - 1) * 60000000.0 / float(stop_t2 - start_t2);
rpm_cntr2 = 0;
*/

float realRPM1 = 0.0;   
if (rpm_cntr1 > 2){
noInterrupts();
unsigned long tmp_start1 = start_t1;
unsigned long tmp_stop1 = stop_t1;
uint16_t tmp_cntr1 = rpm_cntr1;
rpm_cntr1 = 0; 
interrupts();
realRPM1 = (float) (tmp_cntr1 - 1) * 60000000.0f / (float)(tmp_stop1 - tmp_start1);  //Высчитываем количество оборотов в минуту. 1 датчик
}

float realRPM2 = 0.0;
if (rpm_cntr2 > 2){
noInterrupts();
unsigned long tmp_start2 = start_t2;
unsigned long tmp_stop2 = stop_t2;
uint16_t tmp_cntr2 = rpm_cntr2;
rpm_cntr2 = 0; 
interrupts();
realRPM2 = (float) (tmp_cntr2 - 1) * 60000000.0f / (float)(tmp_stop2 - tmp_start2);  //Высчитываем количество оборотов в минуту. 2 датчик
}

float prSpr1=realRPM1/7 ;           //Если импульсы счета идут не один импульс на один оборот то тут можно пересчитать на фактические обороты. 
float prSpr2=realRPM2/7 ;           //Например если обороты считаем от лопастей куллера то нужно разделить на их колличество.       

dtostrf(prSpr1, 6, 0, line1 + 5);   //собираю строчку 1 здесь будет скорост вращ верхнего мотора, первого
line1[11] = 'r'; 
line1[12] = '/';
line1[13] = 'm';

dtostrf(prSpr2, 6, 0, line2 + 5);   //а тут нижнего мотора.по другому , второго мотора 
line2[11] = 'r';
line2[12] = '/';
line2[13] = 'm'; 
 
line1[16] = 0;
line2[16] = 0;

lcd.setCursor(0, 0);                //выводим на LCD экранчик показания оборотов
lcd.print(line1);
lcd.setCursor(0, 1);
lcd.print(line2);
}
