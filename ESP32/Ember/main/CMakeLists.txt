set(WEB_ASSET "${CMAKE_CURRENT_BINARY_DIR}/ember.html.gz")
add_custom_command(
    OUTPUT "${WEB_ASSET}"
    COMMAND gzip -9 -c "${CMAKE_CURRENT_SOURCE_DIR}/data/ember.html" > "${WEB_ASSET}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data/ember.html"
    VERBATIM)
idf_component_register(
    SRCS "main.c" "adc.c" "frequency_count.c" "tacho.c" "speedometer.c" "protocol.c" "ember_web.c"
    INCLUDE_DIRS "."
    REQUIRES esp_adc esp_http_server esp_wifi nvs_flash app_update driver nvs_wifi_connect
    EMBED_FILES "${WEB_ASSET}")
