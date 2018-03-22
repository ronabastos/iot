

/*************************Constantes *********************************/
#define ALERTA_TEMPERATURA 34
#define MESSAGE_MAX_LEN 256
#define INTERVALO 2000
#define PINOGARAGEM 5     //Pino para comunicação com rele da portao garagem
#define PINOPORTAO 4     //Pino para comunicação com rele da portao social
#define PINOTELHADO 12     //Pino para comunicação com rele da telhado retratil
#define PINOLAMPADA 13     //Pino para comunicação com rele da lmapada garagem
#define PINOCHUVA 0     //Pino para comunicação com rele da sensor de chuva

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "RRB"
#define WLAN_PASS       "7e2d7rod911tram515"

/********************Sensor de Temperatura e Umidade**************************/
#define PINODHT 2     //Pino para comunicação com DHT
#define TIPODHT DHT22 //DHT22(AM2302/ AM2321)
 
DHT dht(PINODHT, TIPODHT);    //Configuraçãoes do DHT

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  "ronabastos"
#define AIO_KEY       "42f7a4f0d6ca467299ec32d5ebe57e9c"




