#include <DmxSimple.h>
#include <Artnet.h> //Only supports Ethernet
#include <EasySevenSeg.h>
#include <Arduino.h>

// Ethernet Config Values
uint8_t mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
ArtnetReceiver artnet;

//Artnet Config Values
const int MAX_CHANNELS = 512; //Max is 512
const int MAX_UNIVERSE = 8; //0 - 15
EasySevenSeg numDisplay;
const int buttonPin = 30;

//For Runtime
uint8_t activeChannels[MAX_CHANNELS]; //Array containing all active channel
int selectedUniverse = 0;  // 0 - 15
int oldButtonState = LOW;
int buttonState = HIGH;

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

//============================================================
//=======================[SETUP]==============================
//============================================================

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println("Artnet Node - Ethernet");
    Serial.println("Optimized Version");
    Serial.println();

    int dmxPin = 16;
    Serial.println("Configuring dmx...");
    DmxSimple.usePin(dmxPin);
    DmxSimple.maxChannel(MAX_CHANNELS);

    Serial.println("Conneting to network...");
    Ethernet.begin(mac);

    Serial.println("Starting Art-Net...");
    artnet.begin();
    artnet.subscribe(selectedUniverse, callback);
    for (unsigned char & activeChannel : activeChannels) {
        activeChannel = 0; //Load all channels into memory
        DmxSimple.write(activeChannel, 0); //Reset all Channels to 0
    }

    Serial.println("Starting display...");
    numDisplay.begin(41, 40, 39, 38, 37, 36, 35, 34);
    numDisplay.printDigit(selectedUniverse + 1);
    pinMode(buttonPin, INPUT);

    Serial.println("Setup complete");
    Serial.println();
    Serial.println("Info:");
    Serial.print("Local IP address: ");
    Serial.println(Ethernet.localIP());
    Serial.print("Max universe: ");
    Serial.println(MAX_UNIVERSE);
    Serial.print("Max channels: ");
    Serial.println(MAX_CHANNELS);
    Serial.print("TX pin: ");
    Serial.println(dmxPin);
    Serial.println();
}

//============================================================
//========================[LOOP]==============================
//============================================================

void loop() {
    switch (Ethernet.maintain()) {
        case 1:
            Serial.println("Error: renewed failed");
            break;
        case 2:
            Serial.println("Renewed successful");
            Serial.print("Local IP address: ");
            Serial.println(Ethernet.localIP());
            break;
        case 3:
            Serial.println("Error: rebind failed");
            break;
        case 4:
            Serial.println("Rebind successful");
            Serial.print("Local IP address: ");
            Serial.println(Ethernet.localIP());
            break;
        default:
            break;
    }

    updateUniverse();
    artnet.parse();  // check if artnet packet has come and execute callback
}

void updateUniverse() {
    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW && oldButtonState == HIGH) {
        selectedUniverse += 1;
        if (selectedUniverse > MAX_UNIVERSE) selectedUniverse = 0;

        artnet.clear_subscribers();
        artnet.subscribe(selectedUniverse, callback);
        for (unsigned char & activeChannel : activeChannels) {
            activeChannel = 0; //Load all channels into memory
            DmxSimple.write(activeChannel, 0); //Reset all Channels to 0
        }

        Serial.print("Updated universe id (0 - 15) to ");
        Serial.println(selectedUniverse);
        numDisplay.printDigit(selectedUniverse + 1);
        delay(800);
    }
    oldButtonState = buttonState;
}
