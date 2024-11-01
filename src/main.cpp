
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Servo.h>

#include <Wire.h>

#include <WiFiClient.h>
#include <WiFiUdp.h>
 

#include <LiquidCrystal.h>

#include <NTPClient.h>
 
const char* ssid = "FD-33"; //remplacer par le nom de votre réseau
const char* password = "b7d4853d8745"; //remplacer par le mot de passe de votre réseau
 

//paramétrer les pin de votre carte

const int trigPin = 16;  //renseigner la pin trig
const int echoPin = 5; //renseigner la pin echo
Servo monServomoteur; //objet servo

// définition des broches auxquelles on a branché l'afficheur LCD
const int pinRS = 2;      // broche 4 (RS) de l'afficheur branchée à GPIO04 de l'ESP8266
const int pinEnable = 0;  // broche 6 (Enable) de l'afficheur branchée à GPIO05 de l'ESP8266
const int pinD4 = 12;  // broche 11 (D4) de l'afficheur branchée à GPIO12 de l'ESP8266
const int pinD5 = 13;  // broche 12 (D5) de l'afficheur branchée à GPIO13 de l'ESP8266
const int pinD6 = 14;  // broche 13 (D6) de l'afficheur branchée à GPIO14 de l'ESP8266
const int pinD7 = 15;  // broche 14 (D7) de l'afficheur branchée à GPIO15 de l'ESP8266

// Initialisation de la bibliothèque LiquidCrystal en utilisant les broches définies ci-dessus:
LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);

//déclaration des variables

long duration; //variable durée
int distance;  //variable distance

String action = "F";
String action2 = "";

// Affichage initial sur le LCD
String ligne1STR = "";
String ligne2STR = "";

//variable horaire
String heure="";
int heureNum = 25;
String minute="";
int minuteNum = 62;

 // s'il s'agit d'un ESP8266
ESP8266WebServer server(80);

WiFiUDP ntpUDP;

NTPClient temps(ntpUDP, "fr.pool.ntp.org", 3600, 60000);


//Port utilisé 

String construitPage() {

  String bouton1Str, bouton2Str;

  String page = "<html lang=fr-FR><head>";
  page += "<title>Ma boîte aux lettres</title>";
  
  page += "</head><body><h1>Ma boîte aux lettres</h1>";
  page += "<meta charset='utf-8'>";
  page += "<br />";
  page += "<h2>Avez-vous du courrier ? : </h2>";
 
  //condition d'affichage pour la relève su courrier

  if(distance < 15) { //Ajustez la distance en fonction de la taille de votre boîte aux lettres
    page += "OUI ";
  } else {
    page += "NON";
  }

  page += "<br /><br />";
  page += "<form action='/' method='POST'>";
  page += "<h2>Etat de la porte : ";
  if(action=="F"){
    page += "Fermée";
  }
  else if(action == "O"){
    page += "Ouverte";
  }
  page += "</h2>";
  page += "<p>O: ouvrir et F: fermer </p>";
  page += "<p><INPUT type='text' name='action' value='" + action2 + "'></p>";
  
  page += "<INPUT type='submit' value='go'><br><br>";

  page += "</form>";

  page += "<br />";
  page += "<form action='/' method='POST'>";
  page += "<h2>Message à afficher sur votre boîte aux lettres</h2>";
  page += "<p>Ligne 1:</p>";
  page += "<p><INPUT type='text' name='ligne1' value='" + ligne1STR + "'></p>";
  page += "<p>Ligne 2:</p>";
  page += "<p><INPUT type='text' name='ligne2' value='" + ligne2STR + "'></p>";
  page += "<INPUT type='submit' value='Appliquer'><br><br>";
  page += "</form>";

  page += "<br />";
  page += "<form action='/' method='POST'>";
  page += "<h2>Programmer une heure d'ouverture</h2>";
  page += "<p>Heure:</p>";
  page += "<p><INPUT type='text' name='heure' value='" + heure + "'></p>";
  page += "<p>Minute:</p>";
  page += "<p><INPUT type='text' name='minute' value='" + minute + "'></p>";
  page += "<INPUT type='submit' value='Appliquer'><br><br>";
  page += "</form>";

  page += "</body></html>";
  return page;
}

void gestionPage() {
   // Vider la trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Mettre la trigPin on HIGH pour 10 secondes
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Récupérer la valeur de la echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculer la distance
  distance= duration*0.034/2;


  if ( server.hasArg("action") ) {
    action2 = server.arg("action");
    
     if (action == "F" && action2 == "O")  {
    for (int position = 0; position <= 90; position++) {
      monServomoteur.write(position); //Le moteur effectue 1/4 de tour
      delay(15);
    }
    action=action2;
    action2="";
  }

  //Si on veut fermer la porte
  if (action == "O" && action2 == "F")  {
    for (int position = 90; position >= 0; position--) {
      monServomoteur.write(position); //Le moteur effectue 1/4 de tour
      delay(15);
    }
    action=action2;
    action2="";
  }

    
  }

  if ( server.hasArg("ligne1") ) {
    ligne1STR = server.arg("ligne1");
    ligne2STR = server.arg("ligne2");

    // Affichage sur le moniteur série, pour débogage
    Serial.print("Commande recue.  Message: ");
    Serial.println(ligne1STR);
    Serial.println(ligne2STR);

    // mise à jour de l'afficheur LCD
    lcd.clear();
    lcd.print(ligne1STR);
    lcd.setCursor(0, 1); // deuxième ligne
    lcd.print(ligne2STR);

  }


  if ( server.hasArg("heure") ) {
    heure = server.arg("heure");
    minute = server.arg("minute");

    // Affichage sur le moniteur série, pour débogage
    Serial.print("Commande recue.  Message: ");
    Serial.println(heure);
    Serial.println(minute);

  }
  

 
  server.send ( 200, "text/html", construitPage() );

}
 
void setup() 
{
  // initialisation de la communication série
  Serial.begin(9600);

  // initialisation de la sortie pour le servo moteur
  monServomoteur.attach(4);

  // initialisation de la sortie pour le capteur ultrason 

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

   // initialisation de la sortie pour le servo moteur

  Serial.println("Client deconnecter");
  Serial.println(""); 


  // Vider la trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Mettre la trigPin on HIGH pour 10 secondes
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Récupérer la valeur de la echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculer la distance
  distance= duration*0.034/2;

  
  lcd.begin(16, 2);
  lcd.print(ligne1STR);
  lcd.setCursor(0, 1); // deuxième ligne
  lcd.print(ligne2STR);

  //pause
  delay(1000);
  
  // initialisation de la communication WiFi
  WiFi.begin ( ssid, password );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  Serial.println ( "" );
  Serial.print ( "Maintenant connecte a " );
  Serial.println ( ssid );
  Serial.print ( "Adresse IP: " );
  Serial.println ( WiFi.localIP() );

  // On indique le nom de la fonction qui gère l'interraction avec la page web
  server.on ( "/",  gestionPage );

  server.begin();
  Serial.println ( "Serveur HTTP en fonction" );

  temps.begin();
}
 
void loop() 
{

  //Update de l'heure  
  temps.update();
 
  heureNum = heure.toInt(); 
  minuteNum = minute.toInt(); 
  if (temps.getHours() == heureNum && temps.getMinutes() == minuteNum &&  temps.getSeconds() == 00)  {
    for (int position = 0; position <= 90; position++) {
      monServomoteur.write(position); //Le moteur effectue 1/4 de tour
      delay(15);
    }
    action="O";

  }

  delay(500);
server.handleClient();
  delay(500);

}