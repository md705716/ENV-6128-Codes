/*
  Testing strategy: connect each 5V ANALOG-IN input pin alternatively to +5V on the same connector.
*/

#include <Arduino.h>
#include <Arduino_EdgeControl.h>

constexpr unsigned int adcResolution { 12 };

constexpr pin_size_t electrodeChannels [] {
    INPUT_05V_CH01,
    INPUT_05V_CH02,
    INPUT_05V_CH03,
    INPUT_05V_CH04,
};

// These values are specific to each ISB
// Make sure to change these values if the below
// don't match with what you're working with
// Sensor Type: OX B431
// Sensor Number: 204042101
//const float we_zero = 0.216;
//const float aux_zero = 0.234;
//const float sensitivity = 0.369;
//const float gain = -0.73;

//const float adj = sensitivity * gain;

//float concentration;

struct Voltages {
    float volt3V3;
    float volt5V;
};

float getAverageAnalogRead(int pin)
{
    constexpr size_t loops { 100 };
    constexpr float toV { 3.3f / float { (1 << adcResolution) - 1 } };
    constexpr float rDiv { 17.4f / ( 10.0f + 17.4f) };

    int tot { 0 };

    for (auto i = 0u; i < loops; i++)
        tot += Input.analogRead(pin);
    const auto avg = static_cast<float>(tot) * toV / static_cast<float>(loops);

    return avg / rDiv;
}

void setup()
{
    Serial.begin(9600);

    auto startNow = millis() + 2500;
    while (!Serial && millis() < startNow)
        ;

    delay(1000);
    Serial.println("Hello, Challenge!");

    Power.on(PWR_3V3);
    Power.on(PWR_VBAT);

    Wire.begin();
    Expander.begin();

    Serial.print("Waiting for IO Expander Initialization...");
    while (!Expander) {
        Serial.print(".");
        delay(100);
    }
    Serial.println(" done.");

    Input.begin();
    Input.enable();

    analogReadResolution(adcResolution);
}

void loop()
{
    float we_nox;
    float aux_nox;
    float we_oz;
    float aux_oz;

    we_nox = getAverageAnalogRead(electrodeChannels[0]);
    aux_nox = getAverageAnalogRead(electrodeChannels[1]);
    we_oz = getAverageAnalogRead(electrodeChannels[2]);
    aux_oz = getAverageAnalogRead(electrodeChannels[3]);

    Serial.print("WE_NOX :");
    Serial.print(we_nox);
    Serial.println(" (V)");

    Serial.print("AUX_NOX:");
    Serial.print(aux_nox);
    Serial.println(" (V)");
    
    Serial.print("WE_OZ :");
    Serial.print(we_oz);
    Serial.println(" (V)");

    Serial.print("AUX_OZ:");
    Serial.print(aux_oz);
    Serial.println(" (V)");
    
    
    delay(1000);
//    auto [ voltsMuxer, voltsInput ] = getAverageAnalogRead(inputChannels[inputChannelIndex]);
}
