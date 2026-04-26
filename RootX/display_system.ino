

// --- DAFTAR IKON KECIL (10x10) UNTUK SUB-MENU ---

// Ikon buat Menu WiFi (Samain urutannya sama subMenuWiFi)
const unsigned char* iconListWiFi[] = {
  iconSmall_scan,    
  iconSmall_spam,    
  iconSmall_skull,   
  iconSmall_sniff,   
  iconSmall_wifi
};

// Ikon buat Menu BLE (Contoh isi 3)
const unsigned char* iconListBLE[] = {
  iconSmall_scan,    
  iconSmall_apple,   
  iconSmall_android 
};

const unsigned char* iconListIR[] = {
  iconSmall_ir,    
  iconSmall_tv,   
  iconSmall_ac,
  iconSmall_lock,
  iconSmall_saved
};

const unsigned char* iconListSet[] = {
  iconSmall_bright,    
  iconSmall_wifi,   
  iconSmall_info,
  iconSmall_repeat
};
// ... Buat juga untuk IR dan SETTINGS ...


// --- DAFTAR NAMA MENU TIAP KATEGORI ---

// Isi Menu WiFi (Contoh ada 6)
const char* subMenuWiFi[] = {
  "Scan WiFi",
  "Beacon Spam",
  "Deauth Attack",
  "Select Target",
  "RickRoll SSID",
};

// Isi Menu Bluetooth (Contoh ada 3)
const char* subMenuBLE[] = {
  "BLE Scanner",
  "Spam Apple",
  "Spam Android"
};

// Isi Menu Infrared (Contoh ada 5)
const char* subMenuIR[] = {
  "Read Signal",
  "TV B-Gone",
  "AC Remote",
  "Brute Force",
  "Saved Remotes"
};

// Isi Menu Settings (Contoh ada 4)
const char* subMenuSet[] = {
  "Brightness",
  "WiFi Setup",
  "About RootX",
  "Reboot"
};



void tampilkanMenuLogo() {
  display.clearDisplay();
  
  // --- HEADER TETEP STAY ---
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  if(currentMenu == 0)      display.print("# RootX: WIFI #");
  else if(currentMenu == 1) display.print("# RootX: BLE  #");
  else if(currentMenu == 2) display.print("# RootX: IR   #");
  else                      display.print("# RootX: SETS #");
  
  display.drawLine(0, 9, 128, 9, SSD1306_WHITE);

  // --- LOGO GEDE 32x32 DI TENGAH (x=48, y=22) ---
  const unsigned char* bigIcon;
  if(currentMenu == 0)      bigIcon = logo_wifi_32; 
  else if(currentMenu == 1) bigIcon = logo_ble_32;
  else if(currentMenu == 2) bigIcon = logo_ir_32;
  else                      bigIcon = logo_settings_32;

  display.drawBitmap(48, 22, bigIcon, 32, 32, SSD1306_WHITE);

  // --- TANDA PANAH < DAN > ---
  display.setTextSize(2); // Bikin agak gede biar sangar
  
  // Panah Kiri
  display.setCursor(20, 30); 
  display.print("<");
  
  // Panah Kanan
  display.setCursor(95, 30);
  display.print(">");

  // --- NAVIGASI HINT BAWAH ---
  display.setTextSize(1);
  display.setCursor(40, 56);
  display.print(">SELECT<"); 
  
  display.display();
}


void tampilkanMenuUtama() { 
  display.clearDisplay();
  
  // --- HEADER (Tetep Stay) ---
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  int totalSub = 0; 

  if(currentMenu == 0)      { display.print("# RootX: WIFI #"); totalSub = 5; }
  else if(currentMenu == 1) { display.print("# RootX: BLE  #"); totalSub = 3; }
  else if(currentMenu == 2) { display.print("# RootX: IR   #");  totalSub = 5; }
  else                      { display.print("# RootX: SETS #"); totalSub = 4; }
  display.drawLine(0, 9, 128, 9, SSD1306_WHITE);

  // --- LOOPING LIST MENU ---
  for(int i = 0; i < 5; i++) {
    int itemIndex = topMenu + i;
    if(itemIndex >= totalSub) break; 

    int yPos = 13 + (i * 10); 
    
    // Warna Teks & Kotak Kursor (Pake currentSub)
    if(itemIndex == currentSub) { 
      display.fillRect(0, yPos - 1, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK); 
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    
    // ========================================================
    // --- AMBIL IKON KECIL (10x10) YANG BEDA TIAP BARIS ---
    // ========================================================
    const unsigned char* iconSmall;
    if(currentMenu == 0)      iconSmall = iconListWiFi[itemIndex]; 
    else if(currentMenu == 1) iconSmall = iconListBLE[itemIndex];
    else if(currentMenu == 2) iconSmall = iconListIR[itemIndex];
    else                      iconSmall = iconListSet[itemIndex]; 

    display.drawBitmap(2, yPos - 1, iconSmall, 10, 10, (itemIndex == currentSub ? SSD1306_BLACK : SSD1306_WHITE));
    
    // TULISAN MENU
    display.setCursor(18, yPos);
    if(currentMenu == 0)      display.println(subMenuWiFi[itemIndex]);
    else if(currentMenu == 1) display.println(subMenuBLE[itemIndex]);
    else if(currentMenu == 2) display.println(subMenuIR[itemIndex]);
    else                      display.println(subMenuSet[itemIndex]);
  }
  display.display();
}
