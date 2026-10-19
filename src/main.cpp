#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://wttr.in/Sao+Caetano+do+Sul?m&lang=pt&format=";

String pedirParaApiClima(String codigo);

void setup() 
{
  lcd.init();
  lcd.backlight();
  //Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);

  lcd.print("Inic. conexao WiFi");

  while(WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(0,1);
    lcd.print("#");
    delay(500);
  }

  lcd.setCursor(4,0);
  lcd.print(">CONECTADO<");

  String clima;
  int temperatura;
  int sensacaoTermica;
  clima = pedirParaApiClima("%C");
  temperatura = pedirParaApiClima("%t").toInt();
  sensacaoTermica = pedirParaApiClima("%f").toInt();
  
  lcd.setCursor(0,1);
  lcd.printf("Clima: %s", clima.c_str());
  lcd.setCursor(0,2);
  lcd.printf("Temp. %d C", temperatura);
  lcd.setCursor(0,3);
  lcd.printf("Sens. term. de %d C", sensacaoTermica);
    

}

void loop() 
{

}

String pedirParaApiClima(String codigo)
{
    HTTPClient http;
    http.begin(enderecoAPI + codigo);

    int codigoHTTP = http.GET(); //retorna o codigo HTTP, ex.: 200 -> OK; ou 404 -> not found.
    String resposta;

    if(codigoHTTP == 200)
    {
        resposta = http.getString(); //retorna o conteudo HTML da pagina.
    }

    else
    {
        lcd.printf("Deu erro. Codigo: %d", codigoHTTP);
    }

    http.end();
    return resposta;
}