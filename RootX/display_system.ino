void tampilkanMenuUtama() {
  display.clearDisplay();
  
  // Header Menu
  display.setTextSize(1);
  display.setCursor(25, 0);
  display.print("#RootX Menu#");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  for(int i = 0; i < totalMenu; i++) {
    int yPos = (i * 10) + 15;
    
    if(i == currentMenu) {
      // Sorotan Menu (Invert)
      display.fillRect(0, yPos - 1, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK); 
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    
    // Gambar Ikon Kecil di Kiri
    const unsigned char* icon;
    if(i == 0) icon = icon_wifi;
    else if(i == 1) icon = icon_ble;
    else if(i == 2) icon = icon_ir;
    else if(i == 3) icon = icon_wifi; // Deauth pake ikon wifi
    else icon = icon_settings;

    display.drawBitmap(2, yPos - 1, icon, 16, 16, (i == currentMenu ? SSD1306_BLACK : SSD1306_WHITE));
    
    // Teks Menu
    display.setCursor(22, yPos);
    display.println(menuItems[i]);
  }
  
  display.display();
}
