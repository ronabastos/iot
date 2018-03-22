#include <ArduinoJson.h>

void initSensores(){
   /*******************DHT***************************/
   Serial.println("Iniciando sensor temp/umid");  
   dht.begin(); //Inicializa o DHT
   Serial.println("Sensor temp/umid iniciado");  
      
/*******************xxx***************************/

}


float lerTemperatura()
{
    return dht.readTemperature();
}

float lerUmidade()
{
    return dht.readHumidity();
}

boolean alertaTemperatura(){

  /*
   * Ler a temperatura ou humidade leva cerca de 250 milisegundos!
   * Lê a temperatura em Celsius (padrão)
  */

  StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  
  bool alertaTemperatura = false;
    
  umidade = (int)lerUmidade(); 
  temperatura = (int) lerTemperatura();
  
  // Checando se qualquer leitura falhou.
  if (isnan(temperatura)) {
    Serial.println("Falha ao ler sensor de temperatura");
    root["temperatura"] = NULL;
  }
  else {
    root["temperatura"] = temperatura;
    if (temperatura > ALERTA_TEMPERATURA){
      alertaTemperatura = true;
    }
  }

  if (isnan(umidade)) {
    Serial.println("Falha ao ler sensor de umidade");
    root["umidade"] = NULL;
  }
  else {
    root["umidade"] = umidade;    
  }

  return alertaTemperatura;
  
}
