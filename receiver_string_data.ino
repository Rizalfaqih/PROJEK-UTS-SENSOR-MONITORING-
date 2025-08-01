
#define FREQUENCY_868
#define E220_30
#define LoRa_E220_DEBUG
#include "LoRa_E220.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX
LoRa_E220 e220ttl(&mySerial, 2, 6, 5); // AUX, M0, M1

// Definisi struct sesuai dengan transmitter
struct weatherData {
  int humidity;
  float temperature;
  bool rain;
};

void setup() {
  Serial.begin(9600);
  delay(500);

  e220ttl.begin();
  delay(500);

  Serial.println("Receiver Siap...");
}

void loop() {
  if (e220ttl.available() > 0) {
    // Coba terima struct terlebih dahulu
    ResponseStructContainer rsc = e220ttl.receiveMessage(sizeof(weatherData));

    if (rsc.status.code == 1) {
      // Data struct diterima
      weatherData receivedWeather;
      memcpy(&receivedWeather, rsc.data, sizeof(receivedWeather));

      Serial.println("Data struct diterima:");
      Serial.print("Humidity: ");
      Serial.print(receivedWeather.humidity);
      Serial.println("%");

      Serial.print("Temperature: ");
      Serial.print(receivedWeather.temperature);
      Serial.println(" °C");

      Serial.print("Rain: ");
      Serial.println(receivedWeather.rain ? "Yes" : "No");

    } else {
      // Jika bukan struct, coba sebagai String
      ResponseContainer rc = e220ttl.receiveMessage();
      if (rc.status.code == 1) {
        Serial.println("Data string diterima:");
        Serial.println(rc.data);
      } else {
        Serial.print("Gagal menerima data, status: ");
        Serial.println(rc.status.getResponseDescription());
      }
    }
  }
}

void printParameters(struct Configuration configuration) {
  DEBUG_PRINTLN("----------------------------------------");
  DEBUG_PRINT(F("HEAD : "));  DEBUG_PRINT(configuration.COMMAND, HEX); DEBUG_PRINT(" "); DEBUG_PRINT(configuration.STARTING_ADDRESS, HEX); DEBUG_PRINT(" "); DEBUG_PRINTLN(configuration.LENGHT, HEX);
  DEBUG_PRINTLN(F(" "));
  DEBUG_PRINT(F("AddH : "));  DEBUG_PRINTLN(configuration.ADDH, HEX);
  DEBUG_PRINT(F("AddL : "));  DEBUG_PRINTLN(configuration.ADDL, HEX);
  DEBUG_PRINTLN(F(" "));
  DEBUG_PRINT(F("Chan : "));  DEBUG_PRINT(configuration.CHAN, DEC); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.getChannelDescription());
  DEBUG_PRINTLN(F(" "));
  DEBUG_PRINT(F("SpeedParityBit     : "));  DEBUG_PRINT(configuration.SPED.uartParity, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.SPED.getUARTParityDescription());
  DEBUG_PRINT(F("SpeedUARTDatte     : "));  DEBUG_PRINT(configuration.SPED.uartBaudRate, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.SPED.getUARTBaudRateDescription());
  DEBUG_PRINT(F("SpeedAirDataRate   : "));  DEBUG_PRINT(configuration.SPED.airDataRate, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.SPED.getAirDataRateDescription());
  DEBUG_PRINTLN(F(" "));
  DEBUG_PRINT(F("OptionSubPacketSett: "));  DEBUG_PRINT(configuration.OPTION.subPacketSetting, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.OPTION.getSubPacketSetting());
  DEBUG_PRINT(F("OptionTranPower    : "));  DEBUG_PRINT(configuration.OPTION.transmissionPower, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.OPTION.getTransmissionPowerDescription());
  DEBUG_PRINT(F("OptionRSSIAmbientNo: "));  DEBUG_PRINT(configuration.OPTION.RSSIAmbientNoise, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.OPTION.getRSSIAmbientNoiseEnable());
  DEBUG_PRINTLN(F(" "));
  DEBUG_PRINT(F("TransModeWORPeriod : "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.WORPeriod, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
  DEBUG_PRINT(F("TransModeEnableLBT : "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.enableLBT, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
  DEBUG_PRINT(F("TransModeEnableRSSI: "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.enableRSSI, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
  DEBUG_PRINT(F("TransModeFixedTrans: "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.fixedTransmission, BIN); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());
  DEBUG_PRINTLN("----------------------------------------");
}
