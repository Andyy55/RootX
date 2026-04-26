void handleJoystick() {
  static unsigned long lastPress = 0;
  if (millis() - lastPress < 250) return; // Debounce aman

  if (!inSubMenu) {
    // ==========================================
    // MODE 1: CAROUSEL LOGO (KANAN - KIRI)
    // ==========================================
    if (digitalRead(PIN_RIGHT) == LOW) {
      currentMenu = (currentMenu + 1) % 4; // Ganti 4 sesuai jumlah logo lu
      lastPress = millis();
    }
    else if (digitalRead(PIN_LEFT) == LOW) {
      currentMenu = (currentMenu - 1 + 4) % 4; 
      lastPress = millis();
    }
    else if (digitalRead(PIN_OK) == LOW) {
      inSubMenu = true; // MASUK KE LIST MENU
      currentSub = 0;   // Reset kursor biar mulai dari atas
      topMenu = 0;
      lastPress = millis();
    }
  } 
  else {
    // ==========================================
    // MODE 2: LIST MENU (ATAS - BAWAH)
    // ==========================================
    // Di dalam handleJoystick bagian Mode List (inSubMenu == true)
// Di input_system.ino dalam handleJoystick()
if (digitalRead(PIN_DOWN) == LOW) {
  int limitMenu = 0; // Ganti nama biar gak bentrok
  if(currentMenu == 0)      limitMenu = 5; 
  else if(currentMenu == 1) limitMenu = 3;
  else if(currentMenu == 2) limitMenu = 5;
  else                      limitMenu = 4;

  if (currentSub < (limitMenu - 1)) { // Tambahin kurung biar aman
    currentSub++;
    if (currentSub >= topMenu + 5) topMenu++;
  }
  lastPress = millis();
}


    else if (digitalRead(PIN_UP) == LOW) {
      if (currentSub > 0) {
        currentSub--;
        if (currentSub < topMenu) topMenu--;
      }
      lastPress = millis();
    }
    // --- TOMBOL KIRI UNTUK BACK ---
    else if (digitalRead(PIN_LEFT) == LOW) {
      inSubMenu = false; // KELUAR BALIK KE LOGO
      lastPress = millis();
    }
    else if (digitalRead(PIN_OK) == LOW) {
      // DI SINI NANTI LOGIKA JALANIN FITUR (Contoh: Scan WiFi)
      lastPress = millis();
    }
  }
}
