/* English version:
 * 
 * 08/12/24 Antonio_prev@hotmail.com 
 *          https://github.com/AntonioPrevitali/sx1278Bug
 *  
 *   I have encountered a strange phenomenon with SX1278, in particular I am using Arduino Due and the RA01 module inside which there is sx1278.
 *   
 *   Pin 10 of Arduino Due connected to NSS of RA01 module
 *   Pin 9 connected to RST
 *   Pin 2 connected to DIO0
 *   Pin 3.3 volt of Arduino Due connected to 3v3 of RA01 module
 *   Pin GND connected to GND
 *   Pin SCK MISO and MOSI of Arduino Due SPI connector connected to the respective pins on RA01.
 * 
 *   I noticed problems with this low sensitivity SX1278 in reception and I investigated a lot.
 *   What I found leaves me perplexed but it is so striking that I share it here.
 *   I hope that someone with more equipment than I have (spectrum analyzer) can investigate
 *   better than me.
 * 
 *   In some libraries, instead of using an interrupt approach, a polling approach is used
 *   in which the RegIrqFlags register (0x12) is continuously polled in order to
 *   determine when a packet has been received.
 * 
 *   Well I realized that this continuous polling is the source of the problems.
 *   Incredibly I realized that just polling any register
 *   the problem appears!
 * 
 *   So I wrote this code that intentionally does not use any library and that limits itself to polling 
 *   and I chose to use the RegVersion register at address 0x42.
 * 
 *   If you look at the code you are led to think that nothing will happen.
 *   Instead with this code in operation the module transmits a weak RF signal
 *   receivable (with my limited equipment) up to ten meters.
 *   It transmits many carriers at different frequencies spaced about 70Khz apart.
 *   All this signal disturbs the other receivers placed in the vicinity including
 *   itself if the module were actually in reception.
 *   The polling of RegIrqFlags disturbs the reception of the packet you want to receive.
 * 
 *    You are invited to verify what I have discovered and to let me know if you also encounter this phenomenon.
 * 
 * Italy version:
 * 
 * 08/12/24 Antonio_prev@hotmail.com 
 *          https://github.com/AntonioPrevitali/sx1278Bug
 * 
 *          Ho riscontrato uno strano fenomeno con SX1278, in praticolare sto usando
 *          Arduino Due ed il modulo RA01 al cui interno vi è sx1278.
 *          
 *          Pin 10 di arduino due collegato ad NSS del modulo RA01
 *          Pin  9 collegato al RST
 *          Pin  2 colegato DIO0
 *          Pin 3.3 volt di arduino due collegato a 3v3 del modulo RA01
 *          Pin GND collegato al GND
 *          Pin SCK MISO E MOSI di arduino due connettore SPI collegati ai rispettivi pin su RA01.
 *          
 *    
 *          Ho notato problemi con questo SX1278 di bassa sensibilita in ricezione ed ho indagato molto. 
 *          Quello che ho scoperto mi lascia perplesso ma è cosi eclatante che lo condivido qui.
 *          Spero che qualcuno con piu attrezzature delle mie (analizzatore di spettro) possa indagare
 *          meglio di me.
 *          
 *          in alcune librerie anziche usare un approccio ad interrupt viene usato un aproccio a polling
 *          in cui viene interrogato continuamente il registro RegIrqFlags (0x12) al fine di determinare
 *          quando è stato ricevuto un pacchetto.
 *          
 *          Ebbene mi sono accorto che questo continuo polling è la fonte dei problemi.
 *          Incredibilmente mi sono accorto che basta sottoporre a polling un qualsiasi 
 *          registro che il problema si presenta ! 
 *          
 *          Ho quindi scritto questo codice che volutamente non usa nessuna libreria e che si limita       
 *          a fare polling ed ho scelto di usare il registro RegVersion at address 0x42.
 * 
 *          Se osservate il codice siete indotti a pensare che non succedera nulla.
 *          invece con questo codice in funzione il modulo trasmette un debole segnale RF
 *          ricevibile (con le mie ridotte attrezzature) sino a una decina di metri.
 *          Trasmette molte portanti a frequenze diverse intervallate di circa 70Khz una dall'
 *          altra.  
 *          Tutto questo segnale disturba gli altri ricevitori posti nella vicinanze compreso
 *          se stesso qualora il modulo fosse realmente in ricezione.
 *          Il polling di RegIrqFlags distruba la ricezione del pacchetto che si vuole ricevere.
 * 
 *          Siete invitati a verificare quanto da me scoperto ed a comunicarmi se anche voi 
 *          riscontrate questo fenomeno.
 * 
 */

#include <Arduino.h>
#include <SPI.h>

uint8_t RegVersion = 0x42;


uint8_t spi_readRegister(uint8_t address)
{
    uint8_t response;
    
    address = address & 0x7F;

    digitalWrite(10, LOW);

    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(address);
    response = SPI.transfer(0x00);
    SPI.endTransaction();

    digitalWrite(10, HIGH);

    return response;
}



void setup() {

  // Begin serial communication
  Serial.begin(9600);
  while (!Serial);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  delay(100);    // little delay to wait start module.
  Serial.println("Start verification module");
  
  SPI.begin();

  if (spi_readRegister(RegVersion) == 0x12)
  {
      Serial.println("Ok mudule");
  }
  else
  {
      Serial.println("Error mudule not return 0x12 in RegVersion");
  }


  Serial.println("Now with your rf electronics lab equipment check if the module is transmitting any signals.");
  Serial.println("Please share with me your observations and conclusions.");
  Serial.println("Antonio_prev@hotmail.com");
  Serial.println("https://github.com/AntonioPrevitali/sx1278Bug");


}

void loop() {
  // infinite polling of RegVersion
  spi_readRegister(RegVersion);
}
