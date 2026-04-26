#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "photo_data.h"

// --- SETTING LAYAR ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- SETTING PIN JOYSTICK (SESUAIKAN COLOKAN LU) ---
#define PIN_UP    13
#define PIN_DOWN  12
#define PIN_LEFT  14
#define PIN_RIGHT 27
#define PIN_OK    26

// --- VARIABEL MENU ---
int currentMenu = 0;
const int totalMenu = 5;
String menuItems[] = {"WIFI SCANNER", "BLE SPAMMER", "DEAUTH ATTACK", "IR REMOTE", "SETTINGS"};

void setup() {
  Serial.begin(115200);

  // Setup Input
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_OK, INPUT_PULLUP);

  // Setup I2C buat S3 N16R8 (Pake Pin 1 & 2 biar aman)
  Wire.begin(1, 2);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  // --- URUTAN BOOTING (SESUAI REQUEST LU) ---
  tampilkanLogoDulu();    // 1. Muncul Logo
  tampilkanTeksSplash();  // 2. Logo Hilang, Muncul Teks
  // 3. Setelah ini otomatis masuk loop (Menu)
}

void loop() {
  handleJoystick();      // Baca input 5 arah
  tampilkanMenuUtama();  // Gambar Menu di Layar
  delay(10);
}
