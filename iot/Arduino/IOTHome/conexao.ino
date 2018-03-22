void initSerial(){
  Serial.begin(115200);

  delay(10);

  Serial.println("IOT Home");
}

void initRele(){
  pinMode(pinogaragem, OUTPUT); // Declara o relé como uma saída
  pinMode(pinoportao, OUTPUT); // Declara o relé como uma saída
  pinMode(pinotelhado, OUTPUT); // Declara o relé como uma saída
  pinMode(pinolampada, OUTPUT); // Declara o relé como uma saída

  digitalWrite(pinogaragem, HIGH);  
  digitalWrite(pinoportao, HIGH);
  digitalWrite(pinotelhado, HIGH);
  digitalWrite(pinolampada, HIGH);

}

void initWifi(){
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Conectando em ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: "); Serial.println(WiFi.localIP());
}

void reset_config(void) {
  Serial.println("O ESP ira resetar agora");
  delay(1500);
  //Utilizar ESP.restart() 
  //ESP.reset();
  ESP.restart(); 
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Conectando ao MQTT... ");

  uint8_t tentativas = 60;//5 minutos de tentativa
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Tentando novamente conectar ao MQTT em 5 segundos...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       tentativas--;
       if (tentativas == 0) {
         reset_config();
       }
  }
  Serial.println("MQTT Conectado!");
}

void initMQTT(){
  mqtt.subscribe(&subLampada);  
  mqtt.subscribe(&subTelhado);  
  mqtt.subscribe(&subPortao);  
  mqtt.subscribe(&subGaragem);

  Serial.println("Subscribes executados!");
}

void initTime()
{
    configTime(-10800, 0, "pool.ntp.org", "time.nist.gov");
    time_t dataAtual;    

    while (true)
    {
        dataAtual = time(NULL);

        if (dataAtual == 0)
        {
            Serial.println("Falha ao recuperar data atual! Aguardando 2 segundos para tentar novamente.");
            delay(2000);
        }
        else
        {
            sDataAtual = formatarData(dataAtual);
            break;
        }
    }
}
