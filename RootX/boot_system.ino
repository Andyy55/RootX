// 1. Fungsi Tampilan Logo Saja
void tampilkanLogoDulu() {
  display.clearDisplay();
  // Gambar logo mahkota/foto lu dari photo_data.h
  display.drawBitmap(0, 0, my_photo_bmp, 128, 64, SSD1306_WHITE);
  display.display();
  delay(3000); // Logo nampil 3 detik
}

// 2. Fungsi Teks Splash (Setelah Logo Hilang)
void tampilkanTeksSplash() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);

  // Efek ngetik teks hacker
  String baris1 = ">> RootX Loading...";
  String baris2 = ">> Creator: Andyy";
  String baris3 = ">> Version: v1.0.0";
  String baris4 = ">> ROOTX READY!!";

  ketikTeks(baris1, 0, 10);
  delay(500);
  ketikTeks(baris2, 0, 22);
  delay(500);
  ketikTeks(baris3, 0, 34);
  delay(500);
  ketikTeks(baris4, 0, 46);
  
  delay(2000); // Teks nampil 2 detik sebelum masuk menu
}

// Fungsi bantu buat efek ngetik per baris
void ketikTeks(String teks, int x, int y) {
  display.setCursor(x, y);
  for (int i = 0; i < teks.length(); i++) {
    display.print(teks[i]);
    display.display();
    delay(30); 
  }
  display.println();
}
