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

#define PINMOVIMIENTO 3
#define PINLEDROJO 8
#define PINLEDVERDE 6
#define PINPITO 2
int alarmaEncendida=0;

//////////////////////// Declaración PINES LED ////////////////////////
 
void setup(){
 
  pinMode(PINMOVIMIENTO, INPUT); //configura pin sensor como entrada
   pinMode(PINLEDROJO, OUTPUT); //configura pin LED como salida
   pinMode(PINLEDVERDE, OUTPUT); //configura pin LED como salida
   pinMode(PINPITO,OUTPUT);

   
//////////////////////// Arranque del servidor y serial (NO TOCAR) ////////////////////////

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.begin(9600);
  Serial.println("Bienvenido al CampusTec, soy Arduino :)"); // so I can keep track of what is loaded

//////////////////////// Inicialización pines al arrancar Arduino (cambiar HIGH por LOW)  ////////////////////////

      digitalWrite(PINLEDVERDE,LOW);
      digitalWrite(PINLEDROJO,LOW);
}
 
//////////////////////// PROGRAMA PRINCIPAL (NO TOCAR) ////////////////////////

void loop(){
  
  //Leo la salida del sensor de presencia y la guardo en la variable salidaSensor
  int salidaSensor = digitalRead(PINMOVIMIENTO);
  
  
  
  
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

          Serial.println(salidaSensor); //print to serial monitor for debuging
 
          client.println("HTTP/1.1 200 OK"); //send new page
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
          
          //////////////////////// Cambiar "Alarma con sonido y LEDs" por nombre que quieras ////////////////////////
          client.println("<TITLE>Alarma con sonido y LEDs</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Alarma con sonido y LEDs</H1>");
          client.println("<hr />");
          client.println("<br />");
         
          client.println("<a href=\"/?alarmaOn\"\">Activar alarma</a>");
          client.println("<a href=\"/?alarmaOff\"\">Desactivar alarma</a><br />");     
          
          client.println("</BODY>");
          client.println("</HTML>");
 
 //////////////////////// Fin de la página web (NO TOCAR)////////////////////////
 
          delay(1);
          
          client.stop();
 
 //////////////////////// Control pines Arduino (NO TOCAR) ////////////////////////

          if(readString.indexOf("?alarmaOn") >0)
          {
            alarmaEncendida=1;
          }
          if(readString.indexOf("?alarmaOff") >0)
          {
            alarmaEncendida=0;
          }
                    
          readString="";
 
        delay(2000);
        }
      }
    }
  }
  
  //////////////////////// Si detecta movimiento parpadea un LED y suena la alarma (se puede cambiar el numero dentro de DELAY() por el tiempo que queremos que dure) ////////////////////////

  
  
  if(alarmaEncendida){
  digitalWrite(PINLEDVERDE,HIGH);
  
    if(salidaSensor==1)//checks for on
            {
              Serial.println("DETECTADA PRESENCIA");
              digitalWrite(PINLEDROJO,HIGH);
               digitalWrite(PINPITO,HIGH);
            delay(1000);
            digitalWrite(PINLEDROJO,LOW);
            digitalWrite(PINPITO,LOW);
            delay(1000);
            digitalWrite(PINLEDROJO,HIGH);
            digitalWrite(PINPITO,HIGH);      
            delay(1000);
            digitalWrite(PINLEDROJO,LOW);
            digitalWrite(PINPITO,LOW);
            delay(1000);
            digitalWrite(PINLEDROJO,HIGH);
            digitalWrite(PINPITO,HIGH);
            delay(1000);
            digitalWrite(PINLEDROJO,LOW);
            digitalWrite(PINPITO,LOW);
            delay(1000);
            digitalWrite(PINLEDROJO,HIGH);
            delay(1000);
            digitalWrite(PINLEDROJO,LOW);
            
            delay(1000);
            digitalWrite(PINLEDROJO,HIGH);
            delay(1000);
            digitalWrite(PINLEDROJO,LOW);
            
            delay(1000);         
            }
            else
            {
              //Serial.println(" ------------- ");
              digitalWrite(PINLEDROJO,LOW);  
              digitalWrite(PINPITO,LOW);          
              
            }
  }
  
//////////////////////// Si no detecta nada la alarma está apagada ////////////////////////

  else
  {
      digitalWrite(PINLEDVERDE,LOW);
      digitalWrite(PINLEDROJO,LOW);
  }
  
}
