void tampilkanMenuUtama() {
  display.clearDisplay();
  
  // Header Menu
  display.setTextSize(1);
  display.setCursor(25, 0);
  display.print("#RootX Menu#");
  display.drawLine(0, 9, 128, 9, SSD1306_WHITE);

  // LOOPING MAX 5 BARIS SAJA (Karena layar cuma muat 5)
  for(int i = 0; i < 5; i++) {
    // itemIndex ini index asli dari array menuItems lu
    int itemIndex = topMenu + i; 
    
    // Kalau indexnya udah melebihi total menu, stop ngegambar barisnya
    if(itemIndex >= totalMenu) break; 

    // Posisi Y tetap paten berdasarkan i (0 sampai 4)
    int yPos = 11 + (i * 10);
    
    // Pengecekan warnanya sekarang pake itemIndex
    if(itemIndex == currentMenu) {
      display.fillRect(0, yPos - 1, 128, 11, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK); 
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    
    // Panggil Ikon (Lu atur sendiri sesuai itemIndex ya)
    const unsigned char* icon;
    if(itemIndex == 0) icon = icon_wifi;
    else if(itemIndex == 1) icon = icon_ble;
    else if(itemIndex == 2) icon = icon_wifi; // Contoh deauth
    else if(itemIndex == 3) icon = icon_ir;
    else icon = icon_settings; // Sisanya pake ikon settings dulu buat tes

    // Gambar Ikon & Teks
    display.drawBitmap(2, yPos - 1, icon, 10, 10, (itemIndex == currentMenu ? SSD1306_BLACK : SSD1306_WHITE));
    display.setCursor(18, yPos + 1);
    display.println(menuItems[itemIndex]);
  }
  
  display.display();
}
