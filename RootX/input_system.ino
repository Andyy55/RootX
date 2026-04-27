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
      // 1. Ambil baris yang dipilih
      if (totalWiFi > 0) {
        // 1. Kunci posisi kursor 
      targetLockedIdx = scrollPosScanner + cursorInScanner; 
      
      // 2. Kunci data ke brankas
      targetTerkunci = listWiFi[targetLockedIdx];
      adaTarget = true; 
      
      // 3. AKTIFKAN POP-UP!
      scannerState = 3;         // Pindah ke layar detail kilat
      popUpTimer = millis();    // Catat waktu mulai pop-up
    }
    }


    else if (btn == "BACK") {
      scannerState = 0; // Balikin status ke konfirmasi buat next time
      appMode = 0;      // Balik ke list menu utama
    }
  }
}
void handleNavigasiDeauth(String btn) {
  if (deauthState == 0) { // Layar Konfirmasi
    if (btn == "BACK") appMode = 0; 
    else if (btn == "SELECT") {
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
