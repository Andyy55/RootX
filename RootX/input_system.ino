void handleJoystick() {
  static unsigned long lastPress = 0;
  if (millis() - lastPress < 200) return; // Debounce gue naikin ke 200ms biar gak lompat

  // --- BAWAH ---
  if (digitalRead(PIN_DOWN) == LOW) {
    if (currentMenu < totalMenu - 1) {
      currentMenu++;
      // Kalau kursor ngelewatin batas layar (5 menu), topMenu ikut turun
      if (currentMenu >= topMenu + 5) {
        topMenu++;
      }
    } else {
      // Mentok bawah? Balik ke paling atas
      currentMenu = 0;
      topMenu = 0;
    }
    lastPress = millis();
  }
  
  // --- ATAS ---
  if (digitalRead(PIN_UP) == LOW) {
    if (currentMenu > 0) {
      currentMenu--;
      // Kalau kursor naik ngelewatin topMenu, topMenu ikut naik
      if (currentMenu < topMenu) {
        topMenu--;
      }
    } else {
      // Mentok atas? Balik ke paling bawah
      currentMenu = totalMenu - 1;
      topMenu = totalMenu - 5;
      if (topMenu < 0) topMenu = 0;
    }
    lastPress = millis();
  }


  
  // --- BAWAH ---
  if (digitalRead(PIN_DOWN) == LOW) {
    currentMenu++;
    if (currentMenu >= totalMenu) {
      currentMenu = 0;                   // Balik ke menu pertama
      topMenu = 0;                       // Layar balik ke atas
    } else if (currentMenu >= topMenu + 5) { // 5 itu jumlah max menu di layar
      topMenu++;                         // Layar geser ke bawah 1 baris
    }
    lastPress = millis();
  }



  // ATAS - Kurangi index menu
  if (digitalRead(PIN_UP) == LOW) {
    currentMenu--;
    if (currentMenu < 0) currentMenu = totalMenu - 1;
    lastPress = millis();
  }
  
  // BAWAH - Tambah index menu
  if (digitalRead(PIN_DOWN) == LOW) {
    currentMenu++;
    if (currentMenu >= totalMenu) currentMenu = 0;
    lastPress = millis();
  }

  // KIRI - Bisa buat Back (Opsional)
  if (digitalRead(PIN_LEFT) == LOW) {
    // Back logic
    lastPress = millis();
  }

  // KANAN - Bisa buat Detail (Opsional)
  if (digitalRead(PIN_RIGHT) == LOW) {
    // Detail logic
    lastPress = millis();
  }

  // OK / TENGAH - Masuk Fitur
  if (digitalRead(PIN_OK) == LOW) {
    // Jalankan fitur currentMenu
    lastPress = millis();
  }
}
