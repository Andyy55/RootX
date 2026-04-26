void handleJoystick() {
  static unsigned long lastPress = 0;
  if (millis() - lastPress < 150) return; // Debounce

  // --- ATAS ---
  if (digitalRead(PIN_UP) == LOW) {
    currentMenu--;
    if (currentMenu < 0) {
      currentMenu = totalMenu - 1;       // Balik ke menu paling bawah
      topMenu = totalMenu - 5;           // Layar geser nampilin 5 menu terbawah
      if (topMenu < 0) topMenu = 0;      // Jaga-jaga kalau menu < 5
    } else if (currentMenu < topMenu) {
      topMenu--;                         // Layar geser ke atas 1 baris
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
