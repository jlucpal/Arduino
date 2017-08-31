#include <SPI.h>
#include <Ethernet.h>
#include <math.h>

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80
int pinRojo = 8, pinAzul = 7, pinVerde = 6, pinVentilador=4;
String HTTP_req;            // stores the HTTP request
String readString;
float umbral = 30.0;
const int B=4275;                 // B value of the thermistor
const int R0 = 100000;  
void setup()
{
  Ethernet.begin(mac, ip);  // initialize Ethernet device
  server.begin();           // start to listen for clients
  Serial.begin(9600);       // for diagnostics
  Serial.println("Bienvenido al CampusTec, soy Arduino :)");
  pinMode(pinRojo, OUTPUT);
  pinMode(pinAzul, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  digitalWrite(pinRojo, LOW);
  digitalWrite(pinVerde, LOW);
  digitalWrite(pinAzul, LOW);
  
}

void loop()
{

  //getting the voltage reading from the temperature sensor
  int reading = analogRead(0);

  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 3.3 / 1024;

  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");

  // now print out the temperature
  int a = analogRead(A3);
  float R = 1023.0/(3.3/5*(float)a)-1.0;
  R = 100000.0*R;
  float temperatureC=1.0/(log(R/100000.0)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
  //float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((volatge - 500mV) times 100)
  Serial.print(temperatureC); Serial.println(" degress C");

  // now convert to Fahrenheight
  float temperatureF = (temperatureC * 9 / 5) + 32;
  Serial.print(temperatureF); Serial.println(" degress F");
  Serial.print("\n");
  delay(1000);

  EthernetClient client = server.available();  // try to get client

  if (client) {  // got client?
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        HTTP_req += c;  // save the HTTP request 1 char at a time
        // last line of client request is blank and ends with \n
        // respond to client only after last line received
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: keep-alive");
          client.println();
          // AJAX request for switch state
          if (HTTP_req.indexOf("ajax_switch") > -1) {
            // read switch state and send appropriate paragraph text
            GetSwitchState(client, temperatureC, umbral);
          }
          else {  // HTTP request for web page
            // send web page - contains JavaScript with AJAX calls
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
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
            client.println("p {font-size:20px}");
            client.println("#red {color:red; font-weight:bold;}");
            client.println("#amarillo {color:yellow; font-weight:bold;}");
            client.println("#verde {color:green; font-weight:bold;}");
            client.println("</style>");
            client.println("<title>Temperatura</title>");
            client.println("<script>");
            client.println("function GetSwitchState() {");
            client.println("nocache = \"&nocache=\"\
                                                         + Math.random() * 1000000;");
            client.println("var request = new XMLHttpRequest();");
            client.println("request.onreadystatechange = function() {");
            client.println("if (this.readyState == 4) {");
            client.println("if (this.status == 200) {");
            client.println("if (this.responseText != null) {");
            client.println("document.getElementById(\"switch_txt\")\
.innerHTML = this.responseText;");
            client.println("}}}}");
            client.println(
              "request.open(\"GET\", \"ajax_switch\" + nocache, true);");
            client.println("request.send(null);");
            client.println("setTimeout('GetSwitchState()', 1000);");
            client.println("}");
            client.println("</script>");
            client.println("</head>");
            client.println("<body onload=\"GetSwitchState()\">");
            client.println("<h1>Temperatura</h1>");
            client.println("<hr />");

            client.println(
              "<p id=\"switch_txt\">Temperatura: Sin datos...</p>");
            client.println("</body>");
            client.println("</html>");
          }
          // display received HTTP request on serial port
          //Serial.print(HTTP_req);
          HTTP_req = "";            // finished with request, empty string
          break;
        }
        // every line of text received from the client ends with \r\n
        if (c == '\n') {
          // last character on line of received text
          // starting new line with next character read
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // a text character was received from client
          currentLineIsBlank = false;
        }
      } // end if (client.available())
    } // end while (client.connected())
    delay(1);      // give the web browser time to receive the data
    client.stop(); // close the connection
  } // end if (client)

}

// send the state of the switch to the web browser
void GetSwitchState(EthernetClient cl, float temperatureC, float umbral)
{


  if (temperatureC > umbral) {
    cl.println("<p id=\"red\">");
    cl.println(temperatureC);
    cl.println("</br>");
    cl.println("Temperatura Evelada: Encender Ventilador");
    cl.println("</p>");
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinAzul, LOW);
    digitalWrite(pinVerde, LOW);
    analogWrite(pinVentilador,255);
  }

  else if (temperatureC > umbral -3.0) {
    cl.println("<p id=\"amarillo\">");
    cl.println(temperatureC);
    cl.println("</br>");
    cl.println("Temperatura Media");
    cl.println("</p>");
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinAzul, HIGH);
    digitalWrite(pinVerde, LOW);
    analogWrite(pinVentilador,128);
  }
  else {
    cl.println("<p id=\"verde\">");
    cl.println(temperatureC);
    cl.println("</br>");
    cl.println("Temperatura Adecuada");
    cl.println("</p>");
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinAzul, LOW);
    digitalWrite(pinVerde, HIGH);
    analogWrite(pinVentilador,64);
  }

}
