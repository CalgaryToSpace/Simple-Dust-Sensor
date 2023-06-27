#pragma once

// include up to 4 wifi credentials here; all will be tried in order
// the first one to connect is the one that's used

// when building, duplicate this file to "include/wifi_credentials.h" and fill in the credentials

const char wifi1_ssid[] = "ssid1";
const char wifi1_password[] = "password";

const char wifi2_ssid[] = "ssid2";
const char wifi2_password[] = "password";

const char wifi3_ssid[] = "ssid3";
const char wifi3_password[] = "password";

const char wifi4_ssid[] = "ssid4";
const char wifi4_password[] = "password";

const char wifi_enterprise_ssid[] = "ssid_enterprise";
const char wifi_enterprise_username[] = "enter_username_here, or empty string";
const char wifi_enterprise_password[] = "password";


// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char cayenne_username[] = "MQTT_USERNAME";
char cayenne_password[] = "MQTT_PASSWORD";
char cayenne_client_id[] = "CLIENT_ID";
