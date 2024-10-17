#include <stdio.h>
#include <stdint.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_eth.h>
#include <esp_netif.h>
#include <esp_ping.h>
#include <lwip/ip_addr.h>
#include <mutex>

#define SSID "RepublicaDaBahia2G"
#define PASS "LorenaMacetadora"

class Ethernet {
private:
    Ethernet();
    static void eth_event_handler(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data);
    static void got_ip_event_handler(void *arg, esp_event_base_t event_base,
                                     int32_t event_id, void *event_data);

    static const char *TAG;
    static Ethernet *instance;
    static std::mutex mtx;
    static bool is_connected;
public:
    static Ethernet* getInstance();
    void init();
    bool isConnected();
};

