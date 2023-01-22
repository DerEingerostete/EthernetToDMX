// Please include ArtnetEther.h to use Artnet on the platform
// which can use both WiFi and Ethernet
//#include <ArtnetEther.h>
#include <DmxSimple.h>
#include <Artnet.h> //Only supports Ethernet
#include <Arduino.h>

// Ethernet Config Values
const IPAddress ip(192, 168, 0, 201);
uint8_t mac[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};
ArtnetReceiver artnet;

//Artnet Config Values
const uint32_t SELECTED_UNIVERSE = 1;  // 0 - 15
const int MAX_CHANNELS = 512;

//For Runtime
uint8_t activeChannels[MAX_CHANNELS]; //Array containing all active channel

void callback(const uint8_t* data, const uint16_t size) {
    uint16_t finalSize = min(MAX_CHANNELS, size);
    if (finalSize > 512) {
        finalSize = 512;
        Serial.print("DMX cut off. More than 512 channels (");
        Serial.print(finalSize);
        Serial.println(") were recieved");
    }

    for (uint16_t i = 0; i < finalSize; i++) {
        uint8_t value = data[i];
        if (value == activeChannels[i]) continue;
        activeChannels[i] = value;
        DmxSimple.write(i + 1, value);
    }
}

void setup() {
    Serial.begin(115200);
    DmxSimple.usePin(16);
    DmxSimple.maxChannel(512);

    Ethernet.begin(mac, ip);
    artnet.begin();
    // artnet.subscribe_net(0);     // optionally you can change
    // artnet.subscribe_subnet(0);  // optionally you can change

    // you can also use pre-defined callbacks
    artnet.subscribe(SELECTED_UNIVERSE, callback);
    for (unsigned char & activeChannel : activeChannels) {
        activeChannel = 0; //Load all channels into memory
        DmxSimple.write(activeChannel, 0); //Reset all Channels to 0
    }

    Serial.println("Setup complete");
    Serial.println("Artnet Node - Ethernet");
    Serial.println("Optimized Version");
    Serial.println();
}

void loop() {
    artnet.parse();  // check if artnet packet has come and execute callback
}
