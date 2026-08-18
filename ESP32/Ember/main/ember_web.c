#include "ember_web.h"
#include <stdlib.h>
#include <string.h>
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "nvs_wifi_connect.h"
#include "protocol.h"

static const char *TAG = "ember_web";
extern const unsigned char ember_html_gz_start[] asm("_binary_ember_html_gz_start");
extern const unsigned char ember_html_gz_end[] asm("_binary_ember_html_gz_end");

static esp_err_t index_get(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    return httpd_resp_send(req, (const char *)ember_html_gz_start, ember_html_gz_end - ember_html_gz_start);
}

static esp_err_t status_get(httpd_req_t *req)
{
    char json[192];
    ESP_ERROR_CHECK(protocol_status_json(json, sizeof(json)));
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_sendstr(req, json);
}

static esp_err_t ota_post(httpd_req_t *req)
{
    const esp_partition_t *partition = esp_ota_get_next_update_partition(NULL);
    esp_ota_handle_t handle;
    esp_err_t err = esp_ota_begin(partition, OTA_SIZE_UNKNOWN, &handle);
    char buffer[1024];
    int remaining = req->content_len;
    while (err == ESP_OK && remaining > 0) {
        int received = httpd_req_recv(req, buffer, remaining < sizeof(buffer) ? remaining : sizeof(buffer));
        if (received <= 0) { err = ESP_FAIL; break; }
        err = esp_ota_write(handle, buffer, received);
        remaining -= received;
    }
    if (err == ESP_OK) err = esp_ota_end(handle); else esp_ota_abort(handle);
    if (err == ESP_OK) err = esp_ota_set_boot_partition(partition);
    if (err != ESP_OK) return httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, esp_err_to_name(err));
    ESP_LOGI(TAG, "OTA image accepted; restart to apply it");
    return httpd_resp_sendstr(req, "Update installed. Restart Ember to apply it.");
}

static esp_err_t register_handlers(httpd_handle_t server)
{
    const httpd_uri_t routes[] = {
        {.uri = "/", .method = HTTP_GET, .handler = index_get},
        {.uri = "/api/status", .method = HTTP_GET, .handler = status_get},
        {.uri = "/api/ota", .method = HTTP_POST, .handler = ota_post},
    };
    for (size_t i = 0; i < sizeof(routes) / sizeof(routes[0]); ++i) {
        ESP_ERROR_CHECK(httpd_register_uri_handler(server, &routes[i]));
    }
    return ESP_OK;
}

esp_err_t ember_web_start(void)
{
    esp_err_t wifi_result = nvs_wifi_connect();
    int mode = wifi_result == ESP_OK ?
        NVS_WIFI_CONNECT_MODE_STAY_ACTIVE : NVS_WIFI_CONNECT_MODE_RESTART_ESP32;
    return nvs_wifi_connect_start_http_server(mode, register_handlers) ? ESP_OK : ESP_FAIL;
}
