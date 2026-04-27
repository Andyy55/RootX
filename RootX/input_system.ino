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
  
  if (appMode == 2) {
    if (digitalRead(PIN_UP) == LOW)    { handleNavigasiDeauth("ATAS"); lastPress = millis(); }
    else if (digitalRead(PIN_DOWN) == LOW)  { handleNavigasiDeauth("BAWAH"); lastPress = millis(); }
    else if (digitalRead(PIN_LEFT) == LOW)  { handleNavigasiDeauth("BACK"); lastPress = millis(); }
    else if (digitalRead(PIN_RIGHT) == LOW) { handleNavigasiDeauth("SELECT"); lastPress = millis(); }
    else if (digitalRead(PIN_OK) == LOW)    { handleNavigasiDeauth("OK"); lastPress = millis(); }
    return; // Biar gak bentrok sama menu bawah!
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
     
      if (currentMenu == 0 && currentSub == 0) {
        
        appMode = 1;      
        scannerState = 0; 
        
      } else if (currentMenu == 0 && currentSub == 3) { 
        appMode = 1;
        scannerState = 2;     // LANGSUNG LOMPAT KE HASIL LIST! (Bypass Scan)
        cursorInScanner = 0;  // Reset kursor layar
        scrollPosScanner = 0; // Reset scroll
      }     else if (currentMenu == 0 && currentSub == 2) { 
      if (adaTarget) {
        appMode = 2;     // Masuk mode Deauth
        deauthState = 0; // Munculin "Yakin??"
      } else {
        // POPUP: BELUM SELECT TARGET
        appMode = 1;      // Pinjam mode scanner
        scannerState = 2; // Biar nampilin "BELUM ADA DATA" atau list kosong
      }
      }
     
      
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
    // CUMA TOMBOL "OK" YANG BISA (Tombol Kanan/SELECT dimatiin)
    else if (btn == "OK") {
      if (totalWiFi > 0) {
        targetLockedIdx = scrollPosScanner + cursorInScanner; 
        targetTerkunci = listWiFi[targetLockedIdx];
        adaTarget = true; 
        
        scannerState = 4;   // PINDAH KE MENU CONTEXT
        contextCursor = 0;  // Reset kursor context ke paling atas
      }
    }
    else if (btn == "BACK") {
      scannerState = 0; 
      appMode = 0;      
    }
  }
  // --- LOGIKA SAAT DI MENU CONTEXT (STATE 4) ---
  else if (scannerState == 4) {
    if (btn == "ATAS") {
      // Fitur Scroll Menu Context (Looping)
      contextCursor = (contextCursor > 0) ? contextCursor - 1 : 1; 
    }
    else if (btn == "BAWAH") {
      contextCursor = (contextCursor < 1) ? contextCursor + 1 : 0;
    }
    else if (btn == "OK") {
      if (contextCursor == 0) {
        appMode = 2;     // LOMPAT KE ATTACK DEAUTH
        deauthState = 0; // Mulai dari konfirmasi
      } 
      else if (contextCursor == 1) {
        scannerState = 3; // LOMPAT KE DETAIL VIEW
      }
    }
    else if (btn == "BACK") {
      scannerState = 2; // Batal, balik ke list WiFi
    }
  }
}

void handleNavigasiDeauth(String btn) {
  if (deauthState == 0) { // Layar Konfirmasi
    if (btn == "BACK") appMode = 0; 
    else if (btn == "SELECT" || btn == "OK") { // Biar dua-duanya bisa
      deauthState = 1;
      isDeauthing = true;
    }

  } 
  else if (deauthState == 1) { // Lagi Attack
    if (btn == "BACK") { // Stop Attack
      isDeauthing = false;
      deauthState = 0;
      appMode = 0; // Balik ke menu
    }
  }
}
