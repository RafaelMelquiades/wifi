#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include "esp_sntp.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

 // intervalo de atualizacao da hora na internet padrao 1*60*60*1000.

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://wttr.in/Sao+Caetano+do+Sul?m&lang=pt&format=";

String pedirParaApiClima(String codigo);

//==========SERVIDORES NTP===============

const char* NTP1 = "a.st1.ntp.br";
const char* NTP2 = "pool.ntp.org";
const char* FUSO = "<-03>3";
const char* DIAS[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

void setup() 
{
  lcd.init();
  lcd.backlight();
  //Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);

  lcd.print("Inic. conexao WiFi  ");

  while(WiFi.status() != WL_CONNECTED)
  {
    lcd.print(".");
    delay(500);
  }

  lcd.clear();

  lcd.setCursor(4,0);
  lcd.print(">CONECTADO<");

  sntp_set_sync_interval(60*60*1000);
  
  //configuracao de fuso e servidores NTPs e inicia a sicronizacao.
  configTzTime(FUSO, NTP1, NTP2);

  struct tm agora;
  lcd.setCursor(0,1);
  lcd.print("Sincronizando hora");
  while(!getLocalTime(&agora)); // o parametro é o endereço da estrutura tipo tm.
  {
    lcd.print(".");
  }
  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print("Hora Sincronizada!");

  lcd.clear();


}

void loop() 
{
  struct tm agora;
  if(getLocalTime(&agora))
  {
    lcd.setCursor(0, 0);
    lcd.printf("Hoje e %s, %02d:%02d", DIAS[agora.tm_wday], agora.tm_hour, agora.tm_min);
  }
  
  String clima;
  int temperatura;
  int sensacaoTermica;
  clima = pedirParaApiClima("%C");
  temperatura = pedirParaApiClima("%t").toInt();
  sensacaoTermica = pedirParaApiClima("%f").toInt();
  
  
  lcd.setCursor(0,1);
  lcd.printf("Clima: %s", clima.c_str());
  lcd.setCursor(0,2);
  lcd.printf("Temp.: %d C", temperatura);
  lcd.setCursor(0,3);
  lcd.printf("Sens.: %d C     ", sensacaoTermica);
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