
# Flight Controller Software Requirements Document (PRD)

**Proje Adı:** NOVACT FC  
**Versiyon:** 1.3 (FreeRTOS Edition)  
**Tarih:** 2025-06-15  
**Hedef Platform:** FreeRTOS (ESP32, STM32, NRF52)  
**Hedef Sistem:** Roket Uçuş Kontrol Sistemi  
**Programlama Dili:** C++17

---

## 1. Amaç

Bu proje, roket sistemlerine özel, modüler ve FreeRTOS üzerinde çalışan, `.proto` tanımları ile `MREQ` publish/subscribe iletişim altyapısını kullanan, gerçek zamanlı, görev güvenli ve platform bağımsız bir uçuş kontrol sistemi geliştirmeyi hedeflemektedir.

Kod tabanı **C++17** standardında yazılacak olup, **ESP32**, **STM32**, **NRF52** gibi mikrodenetleyicilerle uyumlu olacak şekilde yapılandırılacaktır. Proje, FreeRTOS çekirdeğini kullanarak zamanlama, görev yönetimi ve senkronizasyon sağlar.

---

## 2. Sistem Mimarisi

### 2.0 Dosya Yapısı (FreeRTOS uyumlu platformlar için optimize)

```plaintext
project-root/
├── .gitignore
├── .gitmodules
├── CMakeLists.txt
├── sdkconfig.old
├── msg/                   # .proto tanımları (sensor_imu.proto, flight_state.proto ...)
├── include/               # Ortak başlık dosyaları (platformdan bağımsız)
├── src/
│   ├── core/              # platform-bağımsız mantık (state manager, FSM, logger)
│   ├── tasks/             # imu_task.cpp, baro_task.cpp, telemetry_task.cpp ...
│   ├── drivers/           # sensör ve donanım sürücüleri (imu_bmi088.cpp, pyro_gpio.cpp)
│   ├── utils/             # zamanlayıcılar, log formatlayıcılar, CRC vb.
│   └── main.cpp           # FreeRTOS main + scheduler başlatıcı
├── main/                  # Main application components
│   └── CMakeLists.txt
├── freertos/              # FreeRTOS config, port layer, heap, linker vb.
│   ├── FreeRTOSConfig.h
│   └── startup.c
├── README.md
├── mreq                   # MREQ kütüphanesi (topic, registry, subscriber ...)
```

---

## 3. Uçuş Görev Yapısı (Tasks)

| Task             | Görev Tanımı                                                           |
|------------------|-------------------------------------------------------------------------|
| `imu_task`       | IMU verisi okuyup `sensor_imu` topic’ine publish eder                  |
| `baro_task`      | Barometre ile yükseklik tahmini yapar, `sensor_baro` topic’ine yazar   |
| `state_task`     | Roket FSM mantığını yürütür, `flight_state`, `pyro_command` üretir     |
| `pyro_task`      | Pyro komutlarını GPIO çıkışlarına uygular                              |
| `logger_task`    | Tüm sensör ve olay verilerini SD karta kaydeder                         |
| `telemetry_task` | Uçuş durumu ve olayları RF üzerinden iletir                             |

> Her task FreeRTOS `xTaskCreate()` ile oluşturulur ve `vTaskDelay`, `Queue` veya `Semaphore` kullanabilir.

---

## 4. Roket Uçuş Fazları (FSM)

- **Idle** → Sistem bekleme durumunda, `armed == false`
- **Boost** → Roket ivmeli kalkışta, yüksek ivme tespit edilir
- **Coast** → Motor kesilmiş, yükselme devam ediyor
- **Deploy** → Apogee tespiti sonrası paraşüt açılır
- **Recovery** → Paraşütlü iniş ve yere güvenli temas

Triggerlar: ivme düşümü, yükseklik değişim yönü, süre bazlı eşikler

---

## 5. MREQ & Proto Mesajlaşma

### 5.1 Proto Tanımları

- `sensor_imu.proto`
- `sensor_baro.proto`
- `flight_state.proto`
- `pyro_command.proto`
- `event.proto`

### 5.2 Mesaj Akışı

- `imu_task` → `sensor_imu`
- `baro_task` → `sensor_baro`
- `state_task` → `flight_state`, `pyro_command`, `event`
- `pyro_task` ← `pyro_command`
- `logger_task` ← `sensor_*`, `flight_state`, `event`
- `telemetry_task` ← `flight_state`, `event`

---

## 6. Donanım ve Arayüzler

| Donanım           | Protokol        | Açıklama                                         |
|-------------------|------------------|--------------------------------------------------|
| IMU               | BMI088 / BNO055  | SPI/I2C, seçilebilir                             |
| Barometre         | BMP390 (I2C)     | Yükseklik tahmini için                           |
| GNSS (opsiyonel)  | Neo-8M (UART)    | Konum ve zaman senkronizasyonu                   |
| Pyro Kanalı       | GPIO             | Ana ve drogue paraşüt için                       |
| Servo (opsiyonel) | PWM              | Separation veya payload sistemleri               |
| SD Kart           | SPI              | Uçuş log kaydı                                   |
| Telemetri         | XBee SX868 / LoRa| UART üzerinden RF veri iletimi (seçilebilir)     |

---

## 7. Güvenlik ve Hata Yönetimi

- `armed == false` iken tüm çıkışlar devre dışı kalır
- Sensör başlatma sırasında `self-check` yapılır
- Her görev, `heartbeat` topic'i ile çalışıyor mu kontrol edilir
- Tüm olaylar `event.proto` üzerinden loglanır
- Pyro çıkışı için çift doğrulama kullanılabilir (yükseklik + zaman)

---

## 8. Test ve Simülasyon

- `native_test` başlığı altında mock sınıflarla testler
- `unity` veya `ceedling` gibi framework’lerle birim test
- Separation ve pyro trigger simülasyon senaryoları
- Loglar SD kart üzerinden `.csv`/`.json` olarak dışa aktarılır

---

## 9. Yapılandırma ve Derleme

- `mreq/tools/proto_registry.py` ile `.proto` → `Topic` otomasyonu
- `CMake`, `Makefile` veya `PlatformIO` ile yapı sistemi desteği
- ESP32 için `idf.py`, STM32 için `STM32CubeIDE`, NRF52 için `nRF Connect SDK` uyumlu

---

## 10. Lisanslama

MIT lisansı ile açık kaynak sunulacaktır.  
Tüm kaynak kod, mesaj tanımları ve test araçları toplulukla özgürce paylaşılacaktır.
