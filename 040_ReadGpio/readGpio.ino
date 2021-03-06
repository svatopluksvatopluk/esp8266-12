#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LED 2   //Define connection of LED

const char* ssid = "dlink-4212"; // remplacer par le SSID de votre WiFi
const char* password = "nhyyi53680"; // remplacer par le mot de passe de votre WiFi

ESP8266WebServer server(80); // on instancie un serveur ecoutant sur le port 80

void setup(void){
  Serial.begin(115200);
  
  // on demande la connexion au WiFi
  WiFi.begin(ssid, password);
  Serial.println("");

  // on attend d'etre connecte au WiFi avant de continuer
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // on affiche l'adresse IP qui nous a ete attribuee
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // On bascule le port en sortie
  pinMode(LED, OUTPUT);

  // on definit ce qui doit etre fait lorsque la route /bonjour est appelee
  // ici on va juste repondre avec un "hello !"
  server.on("/bonjour", [](){
    server.send(200, "text/plain", "hello !");
  });
  
  // Allumer la LED avec la route /on
  server.on("/on", [](){
  digitalWrite(LED, HIGH);
    server.send(200, "text/plain", "allumer LED !");
  });
  
  // Eteindre la LED avec la route /off
  server.on("/off", [](){
  digitalWrite(LED, LOW);
    server.send(200, "text/plain", "éteindre LED !");
  });

  // Allumer ou éteindre la LED avec un paramètre /led?state=on /led?state=off
  server.on("/led", []() {
    String state=server.arg("state");
    if (state == "on") digitalWrite(LED, LOW);
    else if (state == "off") digitalWrite(LED, HIGH);
    server.send(200, "text/plain", "Led is now " + state);
  });
  
  // Lire le GPIO5
  server.on("/gpio5", []() {
   if(digitalRead(5) == HIGH) {
     server.send(200, "text/plain", "Niveau 3.3V");
   } else {
     server.send(200, "text/plain", "Niveau 0V");
   }
  });

  // on commence a ecouter les requetes venant de l'exterieur
  server.begin();
}

void loop(void){
  // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
  server.handleClient();
}

