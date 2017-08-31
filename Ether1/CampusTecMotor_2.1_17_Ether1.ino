//////////////////////// Importación de librerias (NO TOCAR) ///////////////////////
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

//////////////////////// Declaración de variables (NO TOCAR) ////////////////////////

Servo myservo;  // crear un objeto servidor
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //dirección MAC
byte ip[] = { 192, 168, 0, 177 }; // dirección IP
byte gateway[] = { 192, 168, 0, 1 }; // puerta de enlace
byte subnet[] = { 255, 255, 255, 0 }; //máscara de subred
EthernetServer server(80); //puerto del servidor
String readString;

//////////////////////// Declaración PINES LED (Se puede cambiar el número del pin) ////////////////////////

#define PINMOTOR 4
#define PINPITO 3


//////////////////////// CONFIGURACIÓN INICIAL ////////////////////////

void setup() {

  pinMode(PINMOTOR, OUTPUT);


  //////////////////////// Arranque del servidor y serial (NO TOCAR) ////////////////////////
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.begin(9600);
  Serial.println("Bienvenido al CampusTec, soy Arduino :)"); // so I can keep track of what is loaded
  digitalWrite(PINMOTOR, LOW);

}

//////////////////////// PROGRAMA PRINCIPAL ////////////////////////

void loop() {
analogWrite(PINPITO,0);
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

          //////////////////////// Cambiar "Activar/Desactivar Ventilador" por nombre que quieras ////////////////////////
          client.println("<TITLE>Activar/Desactivar Ventilador</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          //////////////////////// Cambiar "Encender LEDs" por nombre que quieras ////////////////////////
          client.println("<H1>Activar/Desactivar Ventilador</H1>");
          client.println("<hr />");
          client.println("<br />");

          client.println(" <a href=\"/?motoraOn\"\">Arrancar Ventilador</a> ");
          client.println("<a href=\"/?motoraOff\"\">Parar Ventilador</a><br /> ");
          client.println("<br />  <br /> ");

          client.println("<a href=\"/?motora0\"\">Ventilador 0%</a> ");
          client.println("<a href=\"/?motora25\"\">Ventilador 25%</a> ");
          client.println("<a href=\"/?motora50\"\">Ventilador 50%</a> ");
          client.println("<a href=\"/?motora75\"\">Ventilador 75%</a> ");
          
          client.println("<a href=\"/?motora100\"\">Ventilador 100%</a><br /><br /> <br /><br /> ");

          client.println("</BODY>");
          client.println("</HTML>");

          //////////////////////// Fin de la página web (NO TOCAR)////////////////////////


          delay(1);

          client.stop();

          //////////////////////// Control pines Arduino (NO TOCAR) ////////////////////////
          if (readString.indexOf("?motoraOn") > 0)
          {
            analogWrite(PINMOTOR, 255);
            Serial.println("Arranco el motor");
          }
          if (readString.indexOf("?motoraOff") > 0)
          {
            analogWrite(PINMOTOR, 0);
            Serial.println("Paro el motor");
          }

          
          if (readString.indexOf("?motora0") > 0)
          {
            Serial.println("Pongo el motor al 0%");
            analogWrite(PINMOTOR, 0);
          }
          if (readString.indexOf("?motora25") > 0)
          {
            Serial.println("Pongo el motor al 25%");
            analogWrite(PINMOTOR, 64);
          }
          if (readString.indexOf("?motora50") > 0)
          {
            Serial.println("Pongo el motor al 50%");
            analogWrite(PINMOTOR, 128);
          }
          if (readString.indexOf("?motora75") > 0)
          {
            Serial.println("Pongo el motor al 75%");
            analogWrite(PINMOTOR, 192);
          }
          if (readString.indexOf("?motora100") > 0)
          {
            Serial.println("Pongo el motor al 100%");
            analogWrite(PINMOTOR, 255);
          }

          
          readString = "";

        }
      }
    }
  }
}
