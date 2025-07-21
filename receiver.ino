
#define FREQUENCY_868
#define E220_30
#define LoRa_E220_DEBUG
#include "LoRa_E220.h"

SoftwareSerial mySerial(3, 4); // Arduino RX <-- e220 TX, Arduino TX --> e220 RX
LoRa_E220 e220ttl(&mySerial, 2, 6, 5); // AUX M0 M1

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

struct weatherData {
  int humidity;
  float temperature;
  bool rain;
};

void setup() {
  Serial.begin(9600);
  delay(500);
  e220ttl.begin();
  while (!Serial) {};
  delay(500);
  ResponseStructContainer c;
  c = e220ttl.getConfiguration();
  Configuration configuration = *(Configuration*) c.data;

  //  ----------------------- DEFAULT TRANSPARENT -----------------------
  configuration.ADDL = 0x02;  // Low byte of address
  configuration.ADDH = 0x00; // High byte of address
  configuration.CHAN = 18; // 868 MHz for Exxx-900 modules, choose 23 for Exxx-400 to set 433 MHz
  configuration.SPED.uartBaudRate = UART_BPS_9600; // Serial baud rate
  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24; // Air baud rate
  configuration.SPED.uartParity = MODE_00_8N1; // Parity bit
  configuration.OPTION.subPacketSetting = SPS_200_00; // Packet size
  configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED; // Need to send special command
  configuration.OPTION.transmissionPower = POWER_30; // Device power
  configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED; // Enable RSSI info
  configuration.TRANSMISSION_MODE.fixedTransmission = FT_TRANSPARENT_TRANSMISSION; // Transmission mode
  configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED; // Check interference
  configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011; // WOR timing
  configuration.CRYPT.CRYPT_H = 0x00;  // encryption high byte, default: 0x00
  configuration.CRYPT.CRYPT_L = 0x00;  // encryption low byte, default: 0x00
  /* Set configuration changed and set to hold the configuration; chose
    WRITE_CFG_PWR_DWN_LOSE to not save the configuration permanently */
  ResponseStatus rs = e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  Serial.println(rs.getResponseDescription());
  Serial.println(rs.code);
  c = e220ttl.getConfiguration();
  // It's important get configuration pointer before all other operation
  configuration = *(Configuration*) c.data;
  Serial.println(c.status.getResponseDescription());
  Serial.println(c.status.code);
  printParameters(configuration);
  c.close();

  Serial.println("Hi, waiting for weather data...");
}

void loop() {
  if (e220ttl.available() > 1) {
    // read the String message
    ResponseStructContainer rsc = e220ttl.receiveMessage(sizeof(weatherData));
    weatherData currentWeather = *(weatherData*) rsc.data;
    Serial.print("Humidity     [%]: ");
    Serial.println(currentWeather.humidity);
    Serial.print("Temperature [°C]: ");
    Serial.println(currentWeather.temperature);
    Serial.print("Rain            : ");
    if (currentWeather.rain) {
      Serial.println("yes");
    }
    else {
      Serial.println("no");
    }
    Serial.println();
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
