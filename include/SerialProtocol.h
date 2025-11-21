/*
 * Serial Protocol Handler
 * Implements the packet-based protocol for display control
 * 
 * Protocol: [0xAA] [LEN] [CMD] [DATA...] [CHECKSUM] [0x55]
 * Baud: 115200
 */

#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include <Arduino.h>
#include "state/AppState.h"
#include "ScreenMapping.h"

#define PACKET_START 0xAA
#define PACKET_END 0x55
#define MAX_PACKET_SIZE 32

// Command IDs
#define CMD_SET_STATE 0x01
#define CMD_NOSE_POSITION 0x02
#define CMD_PROGRESS 0x03

class SerialProtocol {
private:
    uint8_t buffer[MAX_PACKET_SIZE];
    uint8_t bufferIndex;
    bool inPacket;
    AppState* appState;
    
    uint8_t calculateChecksum(uint8_t len, uint8_t cmd, uint8_t* data, uint8_t dataLen) {
        uint8_t checksum = len ^ cmd;
        for (uint8_t i = 0; i < dataLen; i++) {
            checksum ^= data[i];
        }
        return checksum;
    }
    
    void processPacket(uint8_t len, uint8_t cmd, uint8_t* data, uint8_t dataLen) {
        switch (cmd) {
            case CMD_SET_STATE: {
                if (dataLen >= 1) {
                    uint8_t stateID = data[0];
                    Serial.printf("[PROTOCOL] Set State: %d\n", stateID);
                    
                    // Map state to screen and switch
                    ScreenID screen = stateToScreen(stateID);
                    appState->changeScreen(screen);
                }
                break;
            }
            
            case CMD_NOSE_POSITION: {
                if (dataLen >= 8) {
                    // Parse X and Y as floats (Little Endian)
                    float x, y;
                    memcpy(&x, &data[0], 4);
                    memcpy(&y, &data[4], 4);
                    
                    // Convert normalized 0.0-1.0 to screen coordinates (600x450)
                    int16_t screenX = (int16_t)(x * 600.0f);
                    int16_t screenY = (int16_t)(y * 450.0f);
                    
                    Serial.printf("[PROTOCOL] Nose Position: %.3f, %.3f -> %d, %d\n", x, y, screenX, screenY);
                    
                    // Update tracking position
                    appState->updateTargetPosition(screenX, screenY);
                }
                break;
            }
            
            case CMD_PROGRESS: {
                if (dataLen >= 1) {
                    uint8_t percent = data[0];
                    Serial.printf("[PROTOCOL] Progress: %d%%\n", percent);
                    
                    // TODO: Update progress bar if on processing screen
                }
                break;
            }
            
            default:
                Serial.printf("[PROTOCOL] Unknown command: 0x%02X\n", cmd);
                break;
        }
    }
    
public:
    SerialProtocol() : bufferIndex(0), inPacket(false) {
        appState = AppState::getInstance();
    }
    
    void begin(uint32_t baud) {
        Serial.begin(baud);
        Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.println("📡 Serial Protocol Ready");
        Serial.println("   Baud: 115200");
        Serial.println("   Format: [AA][LEN][CMD][DATA][CHK][55]");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    }
    
    void update() {
        while (Serial.available()) {
            uint8_t byte = Serial.read();
            
            if (!inPacket) {
                // Wait for start byte
                if (byte == PACKET_START) {
                    inPacket = true;
                    bufferIndex = 0;
                }
            } else {
                // Collect packet bytes
                buffer[bufferIndex++] = byte;
                
                // Check if we have enough for a minimal packet
                if (bufferIndex >= 4) {  // LEN + CMD + at least CHECKSUM + END
                    uint8_t len = buffer[0];
                    uint8_t dataLen = len - 1;  // LEN includes CMD
                    
                    // Check if packet is complete
                    if (bufferIndex >= (2 + dataLen + 2)) {  // LEN + CMD + DATA + CHECKSUM + END
                        uint8_t expectedEnd = buffer[1 + dataLen + 1];
                        
                        if (expectedEnd == PACKET_END) {
                            // Valid packet structure
                            uint8_t cmd = buffer[1];
                            uint8_t* data = &buffer[2];
                            uint8_t receivedChecksum = buffer[1 + dataLen];
                            uint8_t calculatedChecksum = calculateChecksum(len, cmd, data, dataLen);
                            
                            if (receivedChecksum == calculatedChecksum) {
                                // Valid packet!
                                processPacket(len, cmd, data, dataLen);
                            } else {
                                Serial.printf("[PROTOCOL] Checksum error: expected 0x%02X, got 0x%02X\n", 
                                    calculatedChecksum, receivedChecksum);
                            }
                        } else {
                            Serial.println("[PROTOCOL] Invalid packet end");
                        }
                        
                        // Reset for next packet
                        inPacket = false;
                        bufferIndex = 0;
                    }
                }
                
                // Overflow protection
                if (bufferIndex >= MAX_PACKET_SIZE) {
                    Serial.println("[PROTOCOL] Buffer overflow, resetting");
                    inPacket = false;
                    bufferIndex = 0;
                }
            }
        }
    }
};

#endif


