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
    }
    
    // Core 0 istirahat 50ms biar gak overheat sambil nunggu perintah
    vTaskDelay(50 / portTICK_PERIOD_MS); 
  }
}
