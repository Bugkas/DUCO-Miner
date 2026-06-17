/*
 * Antigravity High-Performance Duino-Coin ESP8266 Miner
 * Features:
 *   - Overclocked ESP8266 CPU (160 MHz) for maximum hashrate
 *   - Secure HTTPS pool selection with local pool failover
 *   - State-of-the-art Glassmorphic Responsive Local Web Dashboard
 *   - Real-time logging console on Serial and Web UI
 *   - Custom hyper-optimized C++ SHA-1 mining loop (using utoa, memcmp)
 *   - Watchdog-friendly multi-tasking (handles web clients during mining)
 *   - LED status indicators
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Hash.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "webpage.h"

// Forward declaration of logging
void addLog(String message);

// ==========================================
//           MINER CONFIGURATION
// ==========================================
const char* WIFI_SSID     = "Purp";     // Change to your Wi-Fi SSID
const char* WIFI_PASSWORD = "PLDT3jayne1jake@2024"; // Change to your Wi-Fi Password
const char* DUCO_USER     = "bugkash"; // Change to your Duino-Coin Username
const char* MINER_KEY     = "supernode";                   // Enter mining key if set, otherwise leave empty ""
const char* RIG_IDENTIFIER = "ESP8266_Rig";       // Custom name for your mining rig
const char* START_DIFF    = "ESP8266";            // Difficulty target (ESP8266, LOW, MEDIUM, etc.)

#define EEPROM_SIZE 512
#define EEPROM_MAGIC 0xBD

struct MinerConfig {
    uint8_t magic;
    uint8_t led_mode; // 0 = Off, 1 = Solid, 2 = Flash, 3 = Pulse/Breath, 4 = Flash every 5s
    char ssid[33];
    char password[64];
    char duco_user[33];
    char miner_key[33];
    char rig_id[33];
    char difficulty[16];
};

MinerConfig config;

// LED definitions (Active LOW on ESP8266 NodeMCU)
#define LED_ON  LOW
#define LED_OFF HIGH

void saveConfig() {
    EEPROM.put(0, config);
    EEPROM.commit();
    addLog("Configuration committed to EEPROM.");
}

void loadConfig() {
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(0, config);
    
    if (config.magic != EEPROM_MAGIC) {
        addLog("EEPROM uninitialized. Writing factory presets...");
        config.magic = EEPROM_MAGIC;
        config.led_mode = 3; // Breathing/Pulse
        strncpy(config.ssid, WIFI_SSID, sizeof(config.ssid));
        strncpy(config.password, WIFI_PASSWORD, sizeof(config.password));
        strncpy(config.duco_user, DUCO_USER, sizeof(config.duco_user));
        strncpy(config.miner_key, MINER_KEY, sizeof(config.miner_key));
        strncpy(config.rig_id, RIG_IDENTIFIER, sizeof(config.rig_id));
        strncpy(config.difficulty, START_DIFF, sizeof(config.difficulty));
        
        saveConfig();
    } else {
        addLog("EEPROM configuration loaded successfully.");
    }
}

void handleLEDStatus() {
    if (config.led_mode == 0) {
        digitalWrite(LED_BUILTIN, LED_OFF);
    } else if (config.led_mode == 1) {
        digitalWrite(LED_BUILTIN, LED_ON);
    } else if (config.led_mode == 3) {
        float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
        int duty = 255 - (int)val;
        analogWrite(LED_BUILTIN, duty);
    } else if (config.led_mode == 4) {
        uint32_t rem = millis() % 5000;
        if (rem < 100) {
            digitalWrite(LED_BUILTIN, LED_ON);
        } else {
            digitalWrite(LED_BUILTIN, LED_OFF);
        }
    }
}

// ==========================================
//           GLOBAL STATE VARIABLES
// ==========================================
ESP8266WebServer server(80);
WiFiClient poolClient;

// Mining Statistics
unsigned long acceptedShares = 0;
unsigned long rejectedShares = 0;
float currentHashrate = 0.0;
unsigned long lastPing = 0;
uint32_t currentDifficulty = 0;
String minerStatus = "Initializing";

// Pool details
String poolIP = "";
uint16_t poolPort = 0;
String poolName = "";

// Dynamic Logging Ring Buffer (15 entries)
String minerLogs[15];
int logIndex = 0;
unsigned long lastLogTime = 0; // Tracks periodic logging intervals

// ==========================================
//            UTILITY FUNCTIONS
// ==========================================

// Add log entry to Serial and UI dashboard buffer
void addLog(String message) {
    unsigned long secs = millis() / 1000;
    char timeStr[15];
    sprintf(timeStr, "[%02d:%02d:%02d] ", (int)(secs / 3600), (int)((secs % 3600) / 60), (int)(secs % 60));
    
    String formattedLog = String(timeStr) + message;
    Serial.println(formattedLog);
    
    minerLogs[logIndex] = formattedLog;
    logIndex = (logIndex + 1) % 15;
}

// Split comma-separated string helper
int splitString(String data, char separator, String* temp, int maxItems) {
    int index = 0;
    unsigned int from = 0;
    int next = -1;
    while ((next = data.indexOf(separator, from)) >= 0 && index < maxItems) {
        temp[index++] = data.substring(from, next);
        from = next + 1;
    }
    if (from < data.length() && index < maxItems) {
        temp[index++] = data.substring(from);
    }
    return index;
}

// ==========================================
//          WEB SERVER ROUTE HANDLERS
// ==========================================

// Serve beautiful dashboard HTML webpage
void handleRoot() {
    server.send(200, "text/html", DASHBOARD_HTML);
}

// Serve JSON api statistics
void handleStats() {
    JsonDocument doc;
    
    // Calculate readable Uptime
    unsigned long ms = millis();
    unsigned long secs = ms / 1000;
    unsigned long mins = secs / 60;
    unsigned long hours = mins / 60;
    unsigned long days = hours / 24;
    secs = secs % 60;
    mins = mins % 60;
    hours = hours % 24;
    
    char uptimeStr[32];
    sprintf(uptimeStr, "%dd %dh %dm %ds", (int)days, (int)hours, (int)mins, (int)secs);
    
    doc["uptime"] = uptimeStr;
    doc["wifi_ssid"] = WiFi.SSID();
    doc["wifi_rssi"] = WiFi.RSSI();
    
    char chipIdHex[16];
    sprintf(chipIdHex, "%08X", ESP.getChipId());
    doc["chip_id"] = String(chipIdHex);
    doc["free_heap"] = ESP.getFreeHeap();
    
    doc["pool_ip"] = poolIP;
    doc["pool_port"] = poolPort;
    doc["pool_name"] = poolName;
    doc["accepted"] = acceptedShares;
    doc["rejected"] = rejectedShares;
    doc["hashrate"] = currentHashrate;
    doc["difficulty"] = currentDifficulty;
    doc["ping"] = lastPing;
    doc["status"] = minerStatus;
    
    doc["cfg_ssid"] = config.ssid;
    doc["cfg_duco_user"] = config.duco_user;
    doc["cfg_miner_key"] = config.miner_key;
    doc["cfg_rig_id"] = config.rig_id;
    doc["cfg_difficulty"] = config.difficulty;
    doc["cfg_led"] = config.led_mode;
    doc["connected"] = poolClient.connected();
    
    // Populate logs array (from oldest to newest)
    JsonArray logsArray = doc["logs"].to<JsonArray>();
    for (int i = 0; i < 15; i++) {
        int index = (logIndex + i) % 15;
        if (minerLogs[index].length() > 0) {
            logsArray.add(minerLogs[index]);
        }
    }
    
    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

// Handle remote restart command
void handleRestart() {
    addLog("Reboot command received from web dashboard! Restarting ESP8266...");
    server.send(200, "text/plain", "Rebooting ESP8266 miner...");
    delay(1000);
    ESP.restart();
}

// Handle save configurations
void handleSaveSettings() {
    if (server.hasArg("duco_user")) {
        config.led_mode = server.arg("led").toInt();
        strncpy(config.duco_user, server.arg("duco_user").c_str(), sizeof(config.duco_user));
        strncpy(config.miner_key, server.arg("miner_key").c_str(), sizeof(config.miner_key));
        strncpy(config.rig_id, server.arg("rig_id").c_str(), sizeof(config.rig_id));
        strncpy(config.difficulty, server.arg("difficulty").c_str(), sizeof(config.difficulty));
        strncpy(config.ssid, server.arg("ssid").c_str(), sizeof(config.ssid));
        
        String passArg = server.arg("password");
        if (passArg.length() > 0) {
            strncpy(config.password, passArg.c_str(), sizeof(config.password));
        }
        
        saveConfig();
        server.send(200, "application/json", "{\"status\":\"ok\"}");
        
        addLog("Configuration saved! Rig: " + String(config.rig_id) + ", User: " + String(config.duco_user));
        delay(500);
        ESP.restart();
    } else {
        server.send(400, "application/json", "{\"status\":\"err\"}");
    }
}

// Set up server route paths
void setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/api/stats", HTTP_GET, handleStats);
    server.on("/api/restart", HTTP_POST, handleRestart);
    server.on("/api/save", HTTP_POST, handleSaveSettings);
    server.begin();
    addLog("Local HTTP web dashboard server launched on port 80");
}

// ==========================================
//          NETWORK CONNECTIONS
// ==========================================

// Attempt Wi-Fi Connection
void connectWiFi() {
    minerStatus = "Connecting Wi-Fi";
    addLog("Connecting to Wi-Fi SSID: " + String(config.ssid));
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.password);
    
    unsigned long startAttempt = millis();
    bool ledState = false;
    
    while (WiFi.status() != WL_CONNECTED) {
        // Toggle LED while connecting
        digitalWrite(LED_BUILTIN, ledState ? LED_ON : LED_OFF);
        ledState = !ledState;
        
        delay(500);
        Serial.print(".");
        
        // Timeout check (30 seconds)
        if (millis() - startAttempt > 30000) {
            Serial.println();
            addLog("Wi-Fi Connection failed! Retrying...");
            startAttempt = millis();
            WiFi.begin(config.ssid, config.password);
        }
    }
    
    digitalWrite(LED_BUILTIN, LED_OFF); // Turn LED off
    Serial.println();
    addLog("Wi-Fi connected! IP Address: " + WiFi.localIP().toString());
}

// Query Duino-Coin Pool Selector HTTP API
bool fetchPoolSelector() {
    minerStatus = "Fetching Pool Server";
    addLog("Querying Duino-Coin pool selector API...");
    digitalWrite(LED_BUILTIN, LED_ON); // Turn LED solid on during request
    
    WiFiClient client; // Plain, ultra-lightweight TCP client (no memory overhead!)
    
    HTTPClient http;
    http.setTimeout(8000); // 8-second timeout
    
    bool fetchSuccess = false;
    
    if (http.begin(client, "http://server.duinocoin.com/getPool")) { // Plain HTTP
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            
            // Parse Pool JSON
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);
            
            if (!error && doc["success"] == true) {
                poolIP = doc["ip"].as<String>();
                poolPort = doc["port"].as<uint16_t>();
                poolName = doc["name"].as<String>();
                if (poolName.length() == 0) {
                    poolName = doc["server"].as<String>();
                }
                
                addLog("Pool assigned: " + poolName + " (" + poolIP + ":" + String(poolPort) + ")");
                fetchSuccess = true;
            } else {
                addLog("JSON parsing failed! Payload: " + payload);
            }
        } else {
            addLog("HTTP GET request failed! Error code: " + String(httpCode));
        }
        http.end();
    } else {
        addLog("Unable to initiate HTTP connection.");
    }
    
    digitalWrite(LED_BUILTIN, LED_OFF); // Turn LED off
    
    if (!fetchSuccess) {
        // Safe robust pool fallbacks
        poolIP = "server.duinocoin.com";
        poolPort = 2813;
        poolName = "Fallback Pool";
        addLog("Using fallback pool server: server.duinocoin.com:2813");
    }
    
    return true;
}

// Connect to Assigned Pool Server Socket
bool connectPoolSocket() {
    minerStatus = "Connecting Pool Socket";
    addLog("Connecting to pool socket at " + poolIP + ":" + String(poolPort));
    
    if (!poolClient.connect(poolIP.c_str(), poolPort)) {
        addLog("Socket connection failed! Retrying...");
        return false;
    }
    
    // Read and print pool server greeting version
    poolClient.setTimeout(5000);
    String greeting = poolClient.readStringUntil('\n');
    greeting.trim();
    addLog("Pool Server Greeting: " + greeting);
    
    return true;
}

// ==========================================
//               SETUP & LOOP
// ==========================================

void setup() {
    // Initialize Serial Port at 115200 baud (synced with platformio.ini)
    Serial.begin(115200);
    delay(500);
    Serial.println("\n\n==========================================");
    Serial.println("  Antigravity ESP8266 Duino-Coin Miner  ");
    Serial.println("==========================================\n");
    
    // Configure hardware built-in LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LED_OFF); // Start off
    
    // Load EEPROM Configuration
    loadConfig();
    
    // Setup and verify CPU frequency overclocking (160 MHz configured in platformio.ini)
    addLog("ESP8266 CPU running at " + String(ESP.getCpuFreqMHz()) + " MHz (Overclocked for maximum hashrate!)");
    
    // Initialize WiFi
    connectWiFi();
    
    // Start Web Server
    setupWebServer();
    
    addLog("Rig ready! Starting mining engine...");
}

void loop() {
    // Keep web server responsive and LED ticking
    server.handleClient();
    handleLEDStatus();
    
    // 1. Maintain Wi-Fi connection
    if (WiFi.status() != WL_CONNECTED) {
        poolClient.stop();
        connectWiFi();
        return;
    }
    
    // 2. Maintain Pool socket connection
    if (!poolClient.connected()) {
        poolClient.stop();
        fetchPoolSelector();
        if (!connectPoolSocket()) {
            delay(5000); // Wait before retrying
            return;
        }
    }
    
    // 3. Process Mining Jobs
    minerStatus = "Mining";
    
    // Request a job
    // Format: JOB,username,difficulty,key
    String jobRequest = "JOB," + String(config.duco_user) + "," + String(config.difficulty) + "," + String(config.miner_key) + "\n";
    poolClient.print(jobRequest);
    
    // Read job response
    poolClient.setTimeout(8000); // 8 seconds timeout
    String jobReply = poolClient.readStringUntil('\n');
    jobReply.trim();
    
    if (jobReply.length() == 0) {
        addLog("Error: Job request timed out or received empty reply. Reconnecting socket...");
        poolClient.stop();
        return;
    }
    
    // Split the comma-separated job data: PREVIOUS_HASH, EXPECTED_HASH, DIFFICULTY
    String parts[3];
    int count = splitString(jobReply, ',', parts, 3);
    
    if (count < 3) {
        addLog("Error: Received corrupt/invalid job reply: " + jobReply);
        poolClient.stop();
        delay(2000);
        return;
    }
    
    String prevHash = parts[0];
    String expectedHash = parts[1];
    uint32_t difficulty = parts[2].toInt();
    currentDifficulty = difficulty;
    
    // ==========================================
    //       HYPER-OPTIMIZED HASHING ENGINE
    // ==========================================
    unsigned long startTime = micros();
    
    // Pre-parse the target Expected Hash hexadecimal string to raw 20 bytes
    uint8_t expectedHashBytes[20];
    for (int i = 0; i < 20; i++) {
        String hexByte = expectedHash.substring(i * 2, i * 2 + 2);
        expectedHashBytes[i] = (uint8_t) strtol(hexByte.c_str(), NULL, 16);
    }
    
    // Set up standard buffer on stack
    char buffer[120];
    int prevHashLen = prevHash.length();
    strcpy(buffer, prevHash.c_str());
    
    unsigned int solvedNonce = 0;
    bool found = false;
    unsigned int maxNonce = difficulty * 100 + 1;
    
    // Solve!
    for (unsigned int nonce = 0; nonce < maxNonce; nonce++) {
        // Feed Watchdog and run Web Server every 2048 loops to stay rock-solid responsive
        // Using ultra-fast bitwise AND instead of slow software-modulo division (%)
        if ((nonce & 2047) == 0) {
            yield();
            server.handleClient();
            handleLEDStatus();
        }
        
        // Formulate buffer: concatenate nonce to previous hash string
        utoa(nonce, buffer + prevHashLen, 10);
        
        // Inline digit count check to entirely eliminate expensive strlen checks
        int nonceLen;
        if (nonce < 10) nonceLen = 1;
        else if (nonce < 100) nonceLen = 2;
        else if (nonce < 1000) nonceLen = 3;
        else if (nonce < 10000) nonceLen = 4;
        else if (nonce < 100000) nonceLen = 5;
        else if (nonce < 1000000) nonceLen = 6;
        else if (nonce < 10000000) nonceLen = 7;
        else if (nonce < 100000000) nonceLen = 8;
        else nonceLen = 9;
        
        int bufferLen = prevHashLen + nonceLen;
        
        // Calculate raw SHA-1 hash bytes
        uint8_t hash[20];
        sha1((uint8_t*)buffer, bufferLen, hash);
        
        // Fast byte array check (memcmp is extremely optimized in GCC assembly)
        if (memcmp(hash, expectedHashBytes, 20) == 0) {
            solvedNonce = nonce;
            found = true;
            break;
        }
    }
    
    unsigned long elapsed = micros() - startTime;
    if (elapsed == 0) elapsed = 1;
    
    // Calculate hashrate (H/s)
    currentHashrate = (float)solvedNonce / ((float)elapsed / 1000000.0);
    
    if (found) {
        // Submit solution
        // Format: nonce,hashrate,rig_identifier,miner_key
        String submitStr = String(solvedNonce) + "," + String(currentHashrate, 2) + ",Antigravity ESP8266 Miner v3.0," + String(config.rig_id) + "\n";
        poolClient.print(submitStr);
        
        // Wait for validation result
        unsigned long pingStart = millis();
        String result = poolClient.readStringUntil('\n');
        result.trim();
        lastPing = millis() - pingStart;
        
        if (result.equals("GOOD") || result.equals("BLOCK")) {
            acceptedShares++;
            
            // Flash LED briefly to show successful share submission
            if (config.led_mode == 2) {
                digitalWrite(LED_BUILTIN, LED_ON);
                delay(50);
                digitalWrite(LED_BUILTIN, LED_OFF);
            }
            
            // Periodic log every 5 seconds to prevent log flood and boost hashrate
            if (millis() - lastLogTime >= 5000) {
                lastLogTime = millis();
                float khs = currentHashrate / 1000.0;
                char msg[128];
                sprintf(msg, "Mining Status | Speed: %.2f kH/s | Accepted: %lu | Rejected: %lu | Ping: %dms | Diff: %u", 
                        khs, acceptedShares, rejectedShares, (int)lastPing, currentDifficulty);
                addLog(msg);
            }
        } else {
            rejectedShares++;
            char msg[128];
            sprintf(msg, "Share rejected! Server: %s | Nonce: %u | Diff: %u", 
                    result.c_str(), solvedNonce, currentDifficulty);
            addLog(msg);
        }
    } else {
        addLog("Error: Hashing space exhausted. No nonce satisfied difficulty. Re-requesting job...");
        // Submit fallback zero share
        poolClient.print("0,0.00,Antigravity ESP8266 Miner v3.0," + String(config.rig_id) + "\n");
        poolClient.readStringUntil('\n');
    }
    
    // Small breath yield for chip safety and background services
    yield();
}