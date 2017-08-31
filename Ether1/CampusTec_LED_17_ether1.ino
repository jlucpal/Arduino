//////////////////////// Importación de librerias (NO TOCAR) ///////////////////////
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

//////////////////////// Declaración de variables (NO TOCAR) ////////////////////////

Servo myservo;  // crear un objeto servidor
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //dirección MAC
byte ip[] = { 192, 168, 0, 177 }; // dirección IP
byte gateway[] = { 192, 168, 1, 1 }; // puerta de enlace
byte subnet[] = { 255, 255, 255, 0 }; //máscara de subred
EthernetServer server(80); //puerto del servidor
String readString;

//////////////////////// Declaración PINES LED (Se puede cambiar el número del pin) ////////////////////////

#define PINLEDROJO 8
#define PINLEDAMARILLO 7
#define PINLEDVERDE 6

//////////////////////// CONFIGURACIÓN INICIAL (se puede cambiar HIGH por LOW) ////////////////////////

void setup(){

  
  pinMode(PINLEDROJO, OUTPUT); //configura pin LED rojo como salida
  pinMode(PINLEDAMARILLO, OUTPUT); //configura pin LED amarillo como salida
  pinMode(PINLEDVERDE, OUTPUT); //configura pin LED verde como salida
  
  digitalWrite(PINLEDROJO, LOW);    // configura pin LED rojo a nivel bajo de inicio (LED apagado)
  digitalWrite(PINLEDAMARILLO, LOW);    // configura pin LED amarillo a nivel bajo de inicio (LED apagado)
  digitalWrite(PINLEDVERDE, LOW);    // configura pin LED verde a nivel bajo de inicio (LED apagado)
              
  
//////////////////////// Arranque del servidor y serial (NO TOCAR) ////////////////////////
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.begin(9600);
  Serial.println("Bienvenido al CampusTec, soy Arduino :)"); // so I can keep track of what is loaded
}
 
//////////////////////// PROGRAMA PRINCIPAL ////////////////////////

void loop(){
  
//////////////////////// Creación de cliente (NO TOCAR) ////////////////////////  
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        
        if (readString.length() < 100) {
 
          
          readString += c;
          
        }
 
        
        if (c == '\n') {
 
//////////////////////// Creación de la página web (TOCAR PARTES INDICADAS) ////////////////////////

          Serial.println(readString); 
 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
 
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<style TYPE='text/css'>");
          client.println("body {margin:50px 0px; padding:0px; text-align:center; }");
          client.println("h1 {text-align: center; font-family:\"Trebuchet MS\",Arial, Helvetica, sans-serif;}");
          client.println("a {text-decoration:none;width:75px;height:50px; border-radius:5px; font-family:\"Trebuchet MS\",Arial, Helvetica, sans-serif; padding:8px; text-align:center;}");
          
          //////////////////////// Cambiar "grey" por el color de los botones, por ejemplo red, green, blue... ////////////////////////
          client.println("a {background-color:grey;}");
          
          client.println("a:link {color:white;}");
          client.println("a:visited {color:white;}");
          client.println("a:hover {color:white;}");
          client.println("a:active {color:white;}");
          client.println("</style>");
          
          //////////////////////// Cambiar "Encender LEDs" por nombre que quieras ////////////////////////
          client.println("<TITLE>Encender LEDs</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          
           //////////////////////// Cambiar "Encender LEDs" por nombre que quieras ////////////////////////
          client.println("<H1>Encender LEDs</H1>");
          client.println("<hr />");
          client.println("<br />");
         
          client.println("<a href=\"/?ledRojoOn\"\">Encender LED rojo</a>");
          client.println("<a href=\"/?ledRojoOff\"\">Apagar LED rojo</a><br />");     
          client.println("<br />");client.println("<br />");     

          client.println("<a href=\"/?ledAmarilloOn\"\">Encender LED azul</a>");
          client.println("<a href=\"/?ledAmarilloOff\"\">Apagar LED azul</a><br />");     
          client.println("<br />");client.println("<br />");   

          client.println("<a href=\"/?ledVerdeOn\"\">Encender LED verde</a>");
          client.println("<a href=\"/?ledVerdeOff\"\">Apagar LED verde</a><br />");     
          client.println("<br />");client.println("<br />");    
 
          client.println("</BODY>");
          client.println("</HTML>");
 
  //////////////////////// Fin de la página web (NO TOCAR)////////////////////////
  
          delay(1);
          
          client.stop();
 
 //////////////////////// Control pines Arduino (NO TOCAR) ////////////////////////
 
          if(readString.indexOf("?ledRojoOn") >0)
          {
            digitalWrite(PINLEDROJO, HIGH);    // pone el pin del LED rojo a nivel alto
            Serial.println("Enciendo el Led rojo");
          }
          if(readString.indexOf("?ledRojoOff") >0)
          {
            digitalWrite(PINLEDROJO, LOW);    // pone el pin del LED rojo a nivel bajo
            Serial.println("Apago el Led rojo");
          }

          if(readString.indexOf("?ledAmarilloOn") >0)
          {
            digitalWrite(PINLEDAMARILLO, HIGH);    // pone el pin del LED amarillo a nivel alto
            Serial.println("Enciendo el Led amarillo");
          }
          if(readString.indexOf("?ledAmarilloOff") >0)
          {
            digitalWrite(PINLEDAMARILLO, LOW);    // pone el pin del LED amarillo a nivel bajo
            Serial.println("Apago el Led amarillo");
          }

          if(readString.indexOf("?ledVerdeOn") >0)
          {
            digitalWrite(PINLEDVERDE, HIGH);    // pone el pin del LED verde a nivel alto
            Serial.println("Enciendo el Led verde");
          }
          if(readString.indexOf("?ledVerdeOff") >0)
          {
            digitalWrite(PINLEDVERDE, LOW);    // pone el pin del LED verde a nivel bajo
            Serial.println("Apago el Led verde");
          }
          
          readString="";
 
        }
      }
    }
  }
}

