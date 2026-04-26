void handleJoystick() {
  static unsigned long lastPress = 0;
  if (millis() - lastPress < 250) return; // Debounce cukup 250ms

  // --- NAVIGASI BAWAH (HANYA BOLEH ADA SATU!) ---
  if (digitalRead(PIN_DOWN) == LOW) {
    if (currentMenu < totalMenu - 1) {
      currentMenu++;
      if (currentMenu >= topMenu + 5) {
        topMenu++;
      }
    } else {
      currentMenu = 0;
      topMenu = 0;
    }
    lastPress = millis();
  }
  
  // --- NAVIGASI ATAS (HANYA BOLEH ADA SATU!) ---
  else if (digitalRead(PIN_UP) == LOW) { // Pake ELSE IF biar gak tabrakan
    if (currentMenu > 0) {
      currentMenu--;
      if (currentMenu < topMenu) {
        topMenu--;
      }
    } else {
      currentMenu = totalMenu - 1;
      topMenu = totalMenu - 5;
      if (topMenu < 0) topMenu = 0;
    }
    lastPress = millis();
  }

  // --- TOMBOL OK ---
  else if (digitalRead(PIN_OK) == LOW) {
    // Nanti isi logic eksekusi di sini
    lastPress = millis();
  }

  // --- KIRI & KANAN (Optional) ---
  else if (digitalRead(PIN_LEFT) == LOW) { lastPress = millis(); }
  else if (digitalRead(PIN_RIGHT) == LOW) { lastPress = millis(); }
}
