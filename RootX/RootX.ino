#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "photo_data.h"

// --- SETTING LAYAR ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- SETTING PIN JOYSTICK (S3 N16R8 Lu) ---
#define PIN_UP    13
#define PIN_DOWN  12
#define PIN_LEFT  14
#define PIN_RIGHT 15
#define PIN_OK    16

// --- VARIABEL MENU GLOBAL (Biar semua file kenal) ---
bool inSubMenu = false;    // Status: lagi di Logo (false) atau di List (true)
int currentMenu = 0;       // Pilih kategori logo (0-3)
int currentSub = 0;        // Pilih baris menu di dalem list
int topMenu = 0;           // Buat scrolling list menu

void setup() {
  Serial.begin(115200);

  // Setup Input
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_OK, INPUT_PULLUP);

  // Setup I2C buat S3 N16R8 (PINnya)
  Wire.begin(8, 9);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  // --- BOOTING ---
  tampilkanLogoDulu();    // 1. Logo Mahkota + Nama Andy (Identitas Awal)
  tampilkanIntroAnime();  // 2. Glitch -> Foto Anime + Firmware Info (Identity)
  tampilkanTeksSplash();  // 3. Loading Ngetik (Hardware Check)
}

void loop() {
  handleJoystick(); // Cek input dari input_system.ino

  if (inSubMenu == false) {
    tampilkanMenuLogo();  // Tampilan Logo Gede + Panah < >
  } else {
    tampilkanMenuUtama(); // Tampilan List Menu kebawah + Ikon Kecil
  }
}
