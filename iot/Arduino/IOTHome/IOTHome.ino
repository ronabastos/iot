#include <Time.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"
#include "config.h"
//#include <EmonLib.h>


/**************************************** Global State *********************************************/
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/**************************************** Feeds Publish********************************************/
Adafruit_MQTT_Publish pubTemperatura = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura");
Adafruit_MQTT_Publish pubUmidade = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/umidade");
Adafruit_MQTT_Publish pubEnergia = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/energia");
Adafruit_MQTT_Publish pubChuva = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/chuva");
//Adafruit_MQTT_Publish pFimcurso = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/fimcurso");

/**************************************** Feeds Subscribe******************************************/
Adafruit_MQTT_Subscribe subLampada = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lampada");
Adafruit_MQTT_Subscribe subTelhado = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/telhado");
Adafruit_MQTT_Subscribe subPortao = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/portao");
Adafruit_MQTT_Subscribe subGaragem = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/garagem");

/**************************************** Variaveis Globais****************************************/
static int umidade = 0; 
static int temperatura = 0;
static int intervalo = INTERVALO;
static String sDataAtual;
int pinogaragem = PINOGARAGEM;
int pinoportao = PINOPORTAO;
int pinotelhado = PINOTELHADO;
int pinolampada = PINOLAMPADA;
int pinochuva = PINOCHUVA;
int ValorADC;
int tentativas = 5;
/**************************************** Sketch Code **********************************************/

void MQTT_connect();

void setup() {

  initSerial();

  initRele();
  
  initWifi();

  initMQTT();

  initSensores();

  initTime();

  MQTT_connect();
  
}


void loop() {

  /*DEPOIS DE FINALIZADO O PROJETO, EXCLUIR LINHAS DOS PRINTS*/
  
  String valorSensor;
  
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  //Envia mensagem a cada 10 segundos
  while ((subscription = mqtt.readSubscription(10000))) {
    if (subscription == &subLampada) {
      Serial.print(F("Vlr Lampada recebido: "));
      Serial.println((char *)subLampada.lastread);
      valorSensor = (char *)subLampada.lastread;
      Serial.println("|" + valorSensor + "|");
      valorSensor.trim();
      valorSensor.toUpperCase();
      
      if(valorSensor.equals("ON")){
        digitalWrite(pinolampada, LOW);
      }
      else{
        digitalWrite(pinolampada, HIGH);
      }

      Serial.println("|" + valorSensor + "|");
    }
    if (subscription == &subTelhado) {
      Serial.print(F("Vlr Telhado Recebido: "));
      Serial.println((char *)subTelhado.lastread);
      digitalWrite(pinotelhado, !digitalRead(pinotelhado)); 
      delay(700);
      digitalWrite(pinotelhado, !digitalRead(pinotelhado)); 
      Serial.print(F("Relê acionado "));
    }
    if (subscription == &subPortao) {
      Serial.print(F("Vlr Portao Recebido: "));
      Serial.println((char *)subPortao.lastread);
      digitalWrite(pinoportao, !digitalRead(pinoportao)); 
      delay(700);
      digitalWrite(pinoportao, !digitalRead(pinoportao)); 
      Serial.print(F("Relê acionado "));
    }
    if (subscription == &subGaragem) {
      Serial.print(F("Vlr Garagem Recebido: "));
      Serial.println((char *)subGaragem.lastread);
      digitalWrite(pinogaragem, !digitalRead(pinogaragem)); 
      delay(700);
      digitalWrite(pinogaragem, !digitalRead(pinogaragem)); 
      Serial.print(F("Relê acionado "));
    }
  }
 
  alertaTemperatura();

  
   
/*
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    Serial.println( (char *)lampada.lastread);
    if (subscription == &lampada) {
      initTime();
      Serial.println( sDataAtual + ": Valor Recebido: ");
      Serial.println((char *)lampada.lastread);
    }
  }*/

  //Temperatura
  initTime();
  Serial.println(sDataAtual + ": Enviando temperatura: ");
  Serial.print(temperatura);
  Serial.print("...");
  if (! pubTemperatura.publish(temperatura)) {
    Serial.println(F("Falhou"));
    tentativas--;
  } else {
    Serial.println(F("OK!"));
  }

  //Umidade
  initTime();
  Serial.println(sDataAtual + ": Enviando umidade: ");
  Serial.print(umidade);
  Serial.print("...");
  if (! pubUmidade.publish(umidade)) {
    Serial.println(F("Falhou"));
  } else {
    Serial.println(F("OK!"));
  }

  //Chuva
  String chuva = String();
  ValorADC = analogRead(pinochuva);   //978 -> 3,3V
  if (ValorADC <= 750)
    chuva = String("Chuvendo: ") + ValorADC;       
  if (ValorADC <= 950 && ValorADC > 750)
     chuva = String("Chuva moderada: ") + ValorADC; 
  if (ValorADC > 950)
    chuva = String("Sem chuva: ") + ValorADC; 
   
  initTime();
  Serial.println(sDataAtual + ": Enviando status de chuva: ");
  Serial.print(chuva);
  Serial.print("...");
  if (! pubChuva.publish(ValorADC)) {
    Serial.println(F("Falhou"));
  } else {
    Serial.println(F("OK!"));
  }


  if (tentativas == 0) {
    reset_config();
  }
  
/*
  //Energia
  initTime();
  Serial.println(sDataAtual + ": Enviando valor da energia: "));
  Serial.print(energia);
  Serial.print("...");
  if (! pubenergia.publish(energia)) {
    Serial.println(F("Falhou"));
  } else {
    Serial.println(F("OK!"));
  }*/


  /*
  time(&this_second); // until time is set, it remains 0
    if (this_second != last_second)
    {
        last_second = this_second;
        if ((this_second % 60) == 0)
        {
            Serial.print(ctime(&this_second));
        }
    }
  */
}



