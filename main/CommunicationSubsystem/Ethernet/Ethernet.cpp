#include "Ethernet.hpp"

const char *Ethernet::TAG = "ETHERNET";
Ethernet* Ethernet::instance = nullptr;
std::mutex Ethernet::mtx; 
bool Ethernet::is_connected = false;

Ethernet::Ethernet() {}

Ethernet* Ethernet::getInstance() {
    std::lock_guard<std::mutex> lock(mtx); 
    if (instance == nullptr) {
        instance = new Ethernet();
    }
    return instance;
}

void Ethernet::eth_event_handler(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data) {

    if (event_data == nullptr) {
        ESP_LOGE(TAG, "event_data is null");
        return;
    }
    
    uint8_t mac_addr[6] = {0};
    esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

    switch (event_id) {
    case ETHERNET_EVENT_CONNECTED:
        esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
        ESP_LOGI(TAG, "Ethernet Link Up");
        ESP_LOGI(TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
                    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        break;
    case ETHERNET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Ethernet Link Down");
        break;
    case ETHERNET_EVENT_START:
        ESP_LOGI(TAG, "Ethernet Started");
        break;
    case ETHERNET_EVENT_STOP:
        ESP_LOGI(TAG, "Ethernet Stopped");
        break;
    default:
        break;
    }
}

void Ethernet::got_ip_event_handler(void *arg, esp_event_base_t event_base,
                                     int32_t event_id, void *event_data) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    if (event_data == nullptr) {
        ESP_LOGE(TAG, "event_data is null");
        return;
    }

    ESP_LOGI(TAG, "Ethernet Got IP Address");
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    ESP_LOGI(TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
    ESP_LOGI(TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
    ESP_LOGI(TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    is_connected = true;
}


void Ethernet::init() {
    esp_netif_init();
    esp_event_loop_create_default();

    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *eth_netif = esp_netif_new(&cfg);

    esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL);

    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();                      // apply default common MAC configuration
    eth_esp32_emac_config_t esp32_emac_config = ETH_ESP32_EMAC_DEFAULT_CONFIG(); // apply default vendor-specific MAC configuration
    esp32_emac_config.smi_mdc_gpio_num = 23;                                     // alter the GPIO used for MDC signal
    esp32_emac_config.smi_mdio_gpio_num = 18;                                    // alter the GPIO used for MDIO signal
    esp_eth_mac_t *mac = esp_eth_mac_new_esp32(&esp32_emac_config, &mac_config); // create MAC instance

    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();    // apply default PHY configuration
    phy_config.phy_addr = 0;                                   // alter the PHY address according to your board design
    phy_config.reset_gpio_num = 5;                             // alter the GPIO used for PHY reset
    esp_eth_phy_t *phy = esp_eth_phy_new_lan87xx(&phy_config); // create PHY instance

    esp_eth_config_t config = ETH_DEFAULT_CONFIG(mac, phy); // apply default driver configuration
    esp_eth_handle_t eth_handle = NULL;                     // after the driver is installed, we will get the handle of the driver
    esp_err_t ret = esp_eth_driver_install(&config, &eth_handle);           // install driver
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install Ethernet driver: %s", esp_err_to_name(ret));
        return;
    }
    esp_netif_attach(eth_netif, esp_eth_new_netif_glue(eth_handle));
    esp_eth_start(eth_handle);
}


bool Ethernet::isConnected() {
    return is_connected;
}