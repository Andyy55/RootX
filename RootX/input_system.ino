void handleJoystick() {
  static unsigned long lastPress = 0;
  if (millis() - lastPress < 250) return; // Debounce aman

  // --- TAMBAHIN BLOK INI ---
  // CEK KALAU LAGI DI MODE SCANNER (appMode == 1)
  if (appMode == 1) {
    if (digitalRead(PIN_UP) == LOW) { 
      handleNavigasiScanner("ATAS"); 
      lastPress = millis(); 
    }
    else if (digitalRead(PIN_DOWN) == LOW) { 
      handleNavigasiScanner("BAWAH"); 
      lastPress = millis(); 
    }
    else if (digitalRead(PIN_LEFT) == LOW) { 
      handleNavigasiScanner("BACK"); 
      lastPress = millis(); 
    }
    else if (digitalRead(PIN_RIGHT) == LOW) { 
      handleNavigasiScanner("SELECT"); 
      lastPress = millis(); 
    }
    else if (digitalRead(PIN_OK) == LOW) { 
      handleNavigasiScanner("OK"); 
      lastPress = millis(); 
    }
    return; // PENTING: return di sini biar gak lanjut ngeksekusi kodingan menu utama lu di bawahnya!
  }
  // --- BATAS TAMBAHAN ---

  
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
     
      if (currentMenu == 0 && currentSub == 0) {
        
        appMode = 1;      
        scannerState = 0; 
        
      }
      // Nanti buat menu "Beacon Spam" (baris ke-2), lu tinggal tambah:
      // else if (currentMenu == 0 && currentSub == 1) { jalankan spam... }

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

// ==========================================
// LOGIKA NAVIGASI KHUSUS WIFI SCANNER
// ==========================================
void handleNavigasiScanner(String btn) {
  // --- LOGIKA SAAT DI MENU KONFIRMASI (STATE 0) ---
  if (scannerState == 0) {
    if (btn == "BACK") {
      appMode = 0; // Batal, balik ke list menu utama
    } 
    else if (btn == "SELECT" || btn == "OK") { // Tombol YES ditekan
      scannerState = 1;     // Pindah ke layar loading
      triggerScan = true;   // Tarik pelatuk Core 0!
      scanDone = false;     // Reset flag
      cursorInScanner = 0;  // Reset kursor
      scrollPosScanner = 0; // Reset scroll
    }
  }
  // --- LOGIKA SAAT LOADING SCAN (STATE 1) ---
  else if (scannerState == 1) {
    // Biarin tombol mati rasa pas lagi scan biar gak error
    // Atau kalau mau bisa dibatalin:
    if (btn == "BACK") {
      scannerState = 0; // Batalin nunggu, balik ke konfirmasi
    }
  }
  // --- LOGIKA SAAT DI HASIL LIST (STATE 2) ---
  else if (scannerState == 2) {
    if (btn == "ATAS") {
      if (cursorInScanner > 0) cursorInScanner--;
      else if (scrollPosScanner > 0) scrollPosScanner--;
    } 
    else if (btn == "BAWAH") {
      if (cursorInScanner < 2 && (scrollPosScanner + cursorInScanner) < (totalWiFi - 1)) cursorInScanner++;
      else if ((scrollPosScanner + 3) < totalWiFi) scrollPosScanner++;
    }
    else if (btn == "SELECT" || btn == "OK") {
      targetLockedIdx = scrollPosScanner + cursorInScanner; // KUNCI TARGET!
    }
    else if (btn == "BACK") {
      scannerState = 0; // Balikin status ke konfirmasi buat next time
      appMode = 0;      // Balik ke list menu utama
    }
  }
}
