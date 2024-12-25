#define BLYNK_PRINT Serial
#include <Wire.h>                     //Inisiasi Library LCD supaya dapat berkomunikasi
#include <LiquidCrystal_I2C.h>        
#include <OneWire.h>                  // Inisiasi Library sensor DS18B20
#include <DallasTemperature.h>        // Inisiasi Library sensor kelembaban kapasitif
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27,20,4);

//blynk
#define BLYNK_TEMPLATE_ID "TMPL6222-6m2V"                   //Konfigurasi dengan blynk supaya bisa terhubung
#define BLYNK_TEMPLATE_NAME "Proyek akhir ayattuloh"
#define BLYNK_AUTH_TOKEN "PF3Ji1Y9Xkoc-vsBInCKTSdyKogk0mv8" 

const int maxbasah = 1360;   //you need to replace this value with Value_1    //nilai analog yang didapatkan sensor kelembaban tanah pada basah dan kering
const int minkering = 2830;  //you need to replace this value with Value_2
const int pinkelembapan = 35;                                                 //Pin analog sensor kelembaban
int soilMoistureValue = 0;                             //sensor yang menyimpan nilai kelembaban dan persentase kelembaban
int percentage = 0;

#define oneWireBus 33                                 //pin analog sensor suhu tanah
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>                       //library untuk komunikasi antara WIFI dan blynk pada ESP32

// Inisialisasi objek OneWire
OneWire oneWire(oneWireBus);
// Inisialisasi objek DallasTemperature
DallasTemperature sensors(&oneWire);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "hotspot";
char pass[] = "12345678";

void setup()
{
  // Debug console
  Serial.begin(9600);                       //komunikasi dengan baud rate 9600
  lcd.begin(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(6,0);
  lcd.print("WELCOME:)");
  lcd.setCursor(2,1);
  lcd.print("ALAT MONITORING");
  lcd.setCursor(0,2);
  lcd.print("Sensor SUHU,HUMIDITY");
  delay(5000);
  lcd.clear();                            //menghapus tampilan LCD

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);      // Koneksi ke server blynk menggunakan token dan wifi
}

void loop()
{
  Blynk.run();
  //suhu
  sensors.requestTemperatures();                    //meminta nilai sensor suhu
  float suhuCelsius = sensors.getTempCByIndex(0);   
  Serial.print("Suhu dalam Celsius:");
  Serial.print(suhuCelsius);
  Serial.println(" °C");
  lcd.setCursor(0,0);
  lcd.print("Suhu:");
  lcd.setCursor(5, 0);
  lcd.print(suhuCelsius);
  lcd.setCursor(10,0);
  lcd.print("C");
  Blynk.virtualWrite(V6, suhuCelsius);          //mengirim data kelembaban ke server blynk melalui virtual pin V6
  //kelembapan
  int soilMoistureValue = analogRead(pinkelembapan);
  int percentage = map(soilMoistureValue,minkering,maxbasah, 0, 100);   //merubah nilai sensor kelembaban menjadi persentase dari 0-100%
  Serial.print("Kelembapan:");
  Serial.print(percentage);
  Serial.println("%");
  lcd.setCursor(0,1);
  lcd.print("HUM:");
  lcd.setCursor(4,1);
  lcd.print(percentage);
  lcd.print("   ");
  lcd.setCursor(7,1);
  lcd.print("%"); 
  lcd.print("   ");
  lcd.setCursor(1,3);
  lcd.print(soilMoistureValue);
  Blynk.virtualWrite(V2, percentage);
  delay(1000); 
}
