#include <Arduino.h>
#include <HTTPClient.h>
#include <fileHandler.h>

#define led_2 2

const char *ssid = "medson";
const char *password = "12345678";

HTTPClient http;
String response;
bool flag = false;

void testeHttp();
void verificaConexao();
void lerArquivo();

void setup() {
  
  Serial.begin(115200);
  //Serial2.begin(115200);
  pinMode(led_2, OUTPUT);
  WiFi.begin(ssid, password);

  //inicialização do SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  verificaConexao();
}

void loop() {
  
  verificaConexao();
  if( (WiFi.status() == WL_CONNECTED) && (!flag)){
    testeHttp();
    flag = true;
    lerArquivo();
  }

  Serial.println("Tamanho: " + String(response.length()));
  delay(5000);
}

void testeHttp() {
  const char *url = "https://drive.usercontent.google.com/download?id=1JaGn-W9lgkW2hfHmmrop-Nt4MBsl716N&export=download&authuser=0&confirm=t&uuid=6c07dfbb-4584-4c3c-bd58-82c89277fe8e&at=APZUnTWPsPSw96xW9B8LLsZgFHjI:1708982102738";

  http.begin(url);

  int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_PARTIAL_CONTENT) {

        File file = SPIFFS.open("/TesteHex.txt", FILE_WRITE);

        if(!file){
            Serial.println("Erro ao abrir o arquivo.");
            return;
        }
        //salva o corpo da resposta de uma requisição HTTP em um arquivo
        http.writeToStream(&file);
        file.close();

        Serial.println("HTTP Code: " + String(httpResponseCode));

    } else {
        Serial.print("Erro na solicitação. Código de resposta: ");
        Serial.println(httpResponseCode);
    }

    http.end();

}

void verificaConexao() {
  if (WiFi.status() != WL_CONNECTED){
    digitalWrite(led_2, LOW);
  } else {
    digitalWrite(led_2, HIGH);
  }
}

// Lê e imprime o conteúdo do arquivo
void lerArquivo() {

  File file = SPIFFS.open("/TesteHex.txt", FILE_READ);
  if (!file) {
    Serial.println("Erro ao abrir o arquivo para leitura.");
    return;
  }

  
  while (file.available()) {

    //Serial2.println("mensagem da esp s1: ");
    char c = file.read();

    Serial.print(c);

    //String line = file.readStringUntil('\n'); //lê o conteúdo do arquivo até encontrar um caractere de nova linha (\n)
    //char buffer[line.length() + 1];//soma 1 ao length da linha. Isso é crucial porque as strings em C requerem um caractere finalizador (null terminator, \0) para indicar o final da string. O próprio terminador null não é um caractere imprimível, mas é usado por funções como Serial.print para saber onde a string termina.
    //line.toCharArray(buffer, sizeof(buffer));
    //Serial2.write(buffer, sizeof(buffer) - 1); // exclui o terminador null
  }

  file.close();
}