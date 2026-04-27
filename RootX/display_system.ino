

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
  "List Scan",
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
  
  if(currentMenu == 0)      display.print("#> RootX: WIFI");
  else if(currentMenu == 1) display.print("#> RootX: BLE");
  else if(currentMenu == 2) display.print("#> RootX: IR");
  else                      display.print("#> RootX: SETS");
  
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

  if(currentMenu == 0)      { display.print("#> RootX: WIFI"); totalSub = 5; }
  else if(currentMenu == 1) { display.print("#> RootX: BLE "); totalSub = 3; }
  else if(currentMenu == 2) { display.print("#> RootX: IR");  totalSub = 5; }
  else                      { display.print("#> RootX: SETS"); totalSub = 4; }
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

void tampilkanWifiScanner() {
  display.clearDisplay();

  // --- STATE 0: KONFIRMASI ---
  if (scannerState == 0) {
    display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 1); display.print("WIFI SCANNER");

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(40, 25); display.print("Yakin??"); // Pertanyaan di tengah

    display.fillRect(0, 54, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 55); display.print("< CANCEL");
    display.setCursor(95, 55); display.print("YES >");
  }
  
  // --- STATE 1: LOADING SCAN ---
  else if (scannerState == 1) {
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 25); 
    display.print("Scanning Air..."); // Teks loading
    // Kalau scan udah beres dari Core 0, otomatis ganti ke State 2
    if (scanDone) {
      scannerState = 2; 
    }
  }
  
  // --- STATE 2: HASIL LIST (Dinamis) ---
  else if (scannerState == 2) {
  if (totalWiFi == 0) {
      display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(2, 1); display.print("SAVED NETWORKS");

      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 25); display.print("BELUM ADA DATA!");
      display.setCursor(10, 35); display.print("Scan WiFi dulu");

      display.fillRect(0, 54, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(2, 55); display.print("< BACK");
    } else {
    // HEADER
    display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 1); display.printf("SCANNER - FOUND: %d", totalWiFi);
    
    // LIST CONTENT (Highlight Blok)
    for (int i = 0; i < 3; i++) {
      int itemIdx = scrollPosScanner + i;
      if (itemIdx < totalWiFi) {
        int yPos = 14 + (i * 13);
        
        if (i == cursorInScanner) {
          display.fillRect(0, yPos - 1, 128, 12, SSD1306_WHITE);
          display.setTextColor(SSD1306_BLACK);
        } else {
          display.setTextColor(SSD1306_WHITE);
        }

        display.setCursor(1, yPos + 1); display.print(listWiFi[itemIdx].id); display.print(".");
                // ... (Kodingan print ID yang atasnya tetep sama)
        display.setCursor(1, yPos + 1); 
        display.print(listWiFi[itemIdx].id); 
        display.print(".");
        
        // ==========================================
        // LOGIKA MARQUEE (TEKS BERJALAN)
        // ==========================================
        String n = listWiFi[itemIdx].ssid;
        int maxChar = 8; // Batas aman maksimal huruf sebelum nabrak C:

        if (i == cursorInScanner && n.length() > maxChar) {
          // JIKA DI-SELECT & KEPANJANGAN -> JALANKAN ANIMASI GESER
          int kelebihan = n.length() - maxChar;
          
          // millis() / 300 artinya kecepatan geser 1 huruf per 300ms (0.3 detik)
          // + 4 itu ngasih "jeda" (pause) bentar pas teksnya udah mentok kiri
          int offset = (millis() / 300) % (kelebihan + 4); 
          
          // Kalau offset lebih dari sisa huruf, tahan di ujung (biar kebaca belakangnya)
          if (offset > kelebihan) offset = kelebihan; 
          
          display.print(n.substring(offset, offset + maxChar));
        } 
        else {
          // JIKA TIDAK DI-SELECT -> STATIS (DIPOTONG NORMAL)
          if (n.length() > maxChar) {
            display.print(n.substring(0, maxChar));
          } else {
            display.print(n);
          }
        }
        // ==========================================

        // Lanjut print Channel sama Sinyal
        display.setCursor(65, yPos + 1); display.printf("C:%d", listWiFi[itemIdx].channel);
        display.setCursor(95, yPos + 1); display.printf("%ddB", listWiFi[itemIdx].rssi);
      }
    }

    // FOOTER (Target Lock)
    display.fillRect(0, 54, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 55); display.print("< BACK");
    display.setCursor(70, 55); 
    if(targetLockedIdx != -1) {
      display.print("LOCKED ID:"); display.print(listWiFi[targetLockedIdx].id);
    } else {
      display.print("SELECT >");
    }
  }
  }
  
    // --- STATE 3: POP-UP DETAIL TARGET (Setelah Klik Select) ---
  else if (scannerState == 3) {
    display.clearDisplay();
    
    // Header: SELECTED ID
    display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 1);
    display.print("SELECTED ID: "); display.print(targetTerkunci.id);

    // Body: Info Lengkap
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 15); display.print("SSID: "); display.println(targetTerkunci.ssid);
    display.setCursor(0, 25); display.print("CH  : "); display.println(targetTerkunci.channel);
    display.setCursor(0, 35); display.print("RSSI: "); display.print(targetTerkunci.rssi); display.println(" dBm");
    display.setCursor(0, 45); display.print("MAC : "); display.println(targetTerkunci.mac);
    
    display.display();

    // LOGIKA AUTO-BACK: Setelah 1.5 detik (1500ms), balik ke List (State 2)
    if (millis() - popUpTimer > 1500) {
      scannerState = 2; 
    }
  }
  

  display.display();
}
void tampilkanDeauthScreen() {
  display.clearDisplay();
  
  // --- KONFIRMASI (scannerState di mode Deauth kita pinjam aja angkanya) ---
  if (deauthState == 0) {
    display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 1); display.print("DEAUTH ATTACK");
    
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 25); display.print("Attack Target Ini?");
    display.setCursor(10, 35); display.print(targetTerkunci.ssid.substring(0,15));

    display.fillRect(0, 54, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 55); display.print("< CANCEL");
    display.setCursor(95, 55); display.print("YES >");
  } 
  
  // --- PROSES ATTACK ---
  else if (deauthState == 1) {
    display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 1); display.print("ATTACKING...");

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20); display.print("Target: "); display.print(targetTerkunci.ssid);
    display.setCursor(0, 30); display.print("Ch: "); display.print(targetTerkunci.channel);
    
    // Animasi Bar Ganti-ganti
    int bar = (millis() / 50) % 128;
    display.drawFastHLine(0, 45, bar, SSD1306_WHITE);
    
    display.fillRect(0, 54, 128, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 55); display.print("< STOP ATTACK");
  }

  display.display();
}
