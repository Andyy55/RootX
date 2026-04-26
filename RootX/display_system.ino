void tampilkanMenuUtama() {
  display.clearDisplay();
  
  // Header (Garis di y=9)
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 0);
  display.print("# RootX Menu #");
  display.drawLine(0, 9, 128, 9, SSD1306_WHITE);

  // LOOPING 5 BARIS
  for(int i = 0; i < 5; i++) {
    int itemIndex = topMenu + i;
    if(itemIndex >= totalMenu) break;

    // yPos kita mulai di 13 biar gak nabrak garis header (y=9)
    // Jarak per baris pas 10px
    int yPos = 13 + (i * 10); 
    
    if(itemIndex == currentMenu) {
      // Tinggi kotak gue bikin 10px biar gak offset
      display.fillRect(0, yPos - 1, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK); 
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    
    // Gambar Ikon (Setting koordinat X=2 biar rapi)
    const unsigned char* icon;
    if(itemIndex == 0) icon = icon_wifi;
    else if(itemIndex == 1) icon = icon_ble;
    else if(itemIndex == 2) icon = icon_wifi; // Contoh deauth
    else if(itemIndex == 3) icon = icon_ir;
    else icon = icon_settings; // Sisanya pake ikon settings dulu buat tes


    display.drawBitmap(2, yPos - 1, icon, 10, 10, (itemIndex == currentMenu ? SSD1306_BLACK : SSD1306_WHITE));
    
    // Teks Menu
    display.setCursor(18, yPos);
    display.println(menuItems[itemIndex]);
  }
  display.display();
}
