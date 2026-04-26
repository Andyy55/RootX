void handleJoystick() {
  static unsigned long lastPress = 0;
  if (millis() - lastPress < 200) return;

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
