#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

// Konfigurasi Wi-Fi
const char* ssid = "server";
const char* password = "jeris6467";

// Konfigurasi OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Konfigurasi NTP
const char* ntpServer = "tandawaktu.id";
const long gmtOffset_sec = 25200; // UTC+7 (Jakarta)
const int daylightOffset_sec = 0;

// Fungsi untuk inisialisasi OLED
void setupOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal dimulai");
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Inisialisasi...");
  display.display();
}

// Fungsi untuk menampilkan waktu di OLED
void displayTime(struct tm timeInfo) {
  char buffer[64];
  strftime(buffer, sizeof(buffer), "Tanggal: %d-%m-%Y\nWaktu  : %H:%M:%S", &timeInfo);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Waktu NTP BMKG");
  display.println("----------------");
  display.println(buffer);
  display.display();
}

void setup() {
  // Inisialisasi Serial dan OLED
  Serial.begin(115200);
  setupOLED();

  // Menghubungkan ke Wi-Fi
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Menghubungkan ke Wi-Fi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi terhubung");
  display.clearDisplay();
  display.println("Wi-Fi Terhubung!");
  display.display();

  // Mengatur waktu dari NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    displayTime(timeInfo); // Tampilkan waktu ke OLED
  } else {
    Serial.println("Gagal mendapatkan waktu dari NTP");
  }
  delay(1000); // Perbarui waktu setiap 1 detik
}
