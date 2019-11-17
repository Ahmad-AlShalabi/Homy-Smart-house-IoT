// Required libraries
#include <ESP8266WiFi.h>
String tempSensorValue="sensor initializing";
String gazSensorValue="sensor initializing";
String alarmValue="";
// WiFi parameters
const char* ssid = "#####";
const char* password = "######";


// Create an instance of the server
WiFiServer server(80);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup() {

  // Start Serial
  Serial.begin(115200);
  delay(10);
// ---------------------test wifi mode----------
//WiFi.mode(WIFI_AP_STA);
//WiFi.softAP(ssid, password);
//IPAddress myIP = WiFi.softAPIP();
//  Serial.println("AP IP address: ");
//  Serial.println(myIP);
//  delay(1000);
//---------------------------------

  // Connect to WiFi network
  //   Serial.println();
  //    Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  //  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void loop() {
  String sensorReading = "0";
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  //======================================================================================================================
  String gaz = "g";
  String temp = "t";
  String sec = "s";
  String inString;
  while (Serial.available() ) {
    char inChar = Serial.read();

    // convert the incoming byte to a char
    // and add it to the string:
    inString += (char)inChar;
    if (inString.startsWith(temp) ) {
      tempSensorValue = Serial.readStringUntil('.');
      break;
    }
//    else if (inString.startsWith(gaz) ) {
//      gazSensorValue = Serial.readStringUntil('.');
//      break;
//    }
    else if (inString.startsWith(sec) ) {
      alarmValue=Serial.readStringUntil('.');
      break;
    }
  }
  //======================================================================================================================
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //  Serial.println(req);
  client.flush();

  // Match the request

    if (req.indexOf("/get_alarm") != -1) {
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    s += alarmValue;
    client.print(s);
    alarmValue="";
  }
   else if (req.indexOf("/get_t") != -1) {
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    s += tempSensorValue;
    client.print(s);

  }
  // Match the request
  else if (req.indexOf("/get_g") != -1) {
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    s += gazSensorValue;
    client.print(s);
  }
   else if (req.indexOf("/ledon") != -1) {
    int i = req.indexOf("ledon");
    String mes = req.substring(i + 5, i + 6);
    Serial.println("TLOR" + mes);

  }
 
  else if (req.indexOf("/ledoff") != -1) {
    int i = req.indexOf("ledoff");
    String mes = req.substring(i + 6, i + 7);
    Serial.println("TLFR" + mes);

  }
  else if (req.indexOf("/opendo") != -1) {
    Serial.println("OTD");

  }
  else if (req.indexOf("/closedo") != -1) {
    Serial.println("CTD");

  }
   else if (req.indexOf("/openca") != -1) {
    Serial.println("OTCA");

  }
  else if (req.indexOf("/closeca") != -1) {
    Serial.println("CTCA");

  }
  else if (req.indexOf("/stopbus") != -1) {
    Serial.println("BF");

  }
  else if (req.indexOf("/enablecon") != -1) {
    Serial.println("CFU");

  }
  else if (req.indexOf("/unablecon") != -1) {
    Serial.println("NCFU");

  }
  else if (req.indexOf("/unablesec") != -1) {
    Serial.println("UF");

  }
  else if (req.indexOf("/enablesec") != -1) {
    Serial.println("UO");

  }
  // Match the request
 
  // Match the request
  else if (req.indexOf("/home") != -1) {
    String pagep1 = mess1();
    String pagep2 = mess2();
    String pagep3 = mess3();
    client.print(pagep1);
    client.print(pagep2);
    client.print(pagep3);
  }
  else if (req.indexOf("/r") != -1) {
    Serial.println(req);

  }
  Serial.flush();
  delay(1);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String mess1 () {
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<title>homy</title>";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "</head>";
  //==================================================== Control Lightting State salon & Bedroom =================================================================
  s += "<div class=\"container\">";
  s += "<h1 style=\"font-size: 70px ; text-align: center; \"> Homy </h1>";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "<div class=\"page-header\">";
  s += "<h2>Enable manual control <input id=\"e\"  class=\"btn btn-default\" style=\"width: 80px;height: 34px;\" type=\"checkbox\" onclick=\"enable()\" ></h2> ";
  s +=  "<h3 id=\"b\"> Control Lightting State:  </h3> ";
  s += " </div>";
  s += "</div>";
  s += " <h3>Salon light</h3> ";
  s += "<div class=\"col-md-2\"><input id=\"1\" class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"On\" onclick=\"on(this)\"></div>";
  s += "<div class=\"col-md-2\"><input  id=\"1\" class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Off\" onclick=\"off(this)\"></div>";
  s += "</div></div>";
  //=================================

  s += "<div class=\"container\">";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "</div>";
  s += " <h3>Bedroom light</h3> ";
  s += "<div class=\"col-md-2\"><input id=\"2\" class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"On\" onclick=\"on(this)\"></div>";
  s += "<div class=\"col-md-2\"><input id=\"2\" class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Off\" onclick=\"off(this)\"></div>";
  s += "</div></div>";
  //================================================== Kitchen & Bathroom ====================================================================
  s += "<div class=\"container\">";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "<div class=\"page-header\">";
  s += "</div></div>";
  s += " <h3>Kitchen light</h3> ";
  s += "<div class=\"col-md-2\"><input id=\"3\" class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"On\" onclick=\"on(this)\"></div>";
  s += "<div class=\"col-md-2\"><input id=\"3\" class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Off\" onclick=\"off(this)\"></div>";
  s += "</div></div>";

  //=====================================
  s += "<div class=\"container\">";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "</div>";
  s += " <h3>Bathroom light</h3> ";
  s += "<div class=\"col-md-2\"><input id=\"4\" class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"On\" onclick=\"on(this)\"></div>";
  s += "<div class=\"col-md-2\"><input id=\"4\" class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Off\" onclick=\"off(this)\"></div>";
  s += "</div></div>";
  //======================================================================================================================
  s += "<script>function on(elem) {$.get(\"/ledon\"+elem.id);}</script>";
  s += "<script>function off(elem) {$.get(\"/ledoff\"+elem.id);}</script>";
  //  s += "<script>function led2on() {$.get(\"/led2on\");}</script>";
  //  s += "<script>function led2off() {$.get(\"/led2off\");}</script>";

  //======================================================================================================================
  return s ;
}

String mess2 () {
  String s;
  //======================================================== RGB ===============================================================

  s += " <div class=\"container\">";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "<div class=\"page-header\">";
  s += "<h2>Select your favorite color:</h2>";
  s += "</div></div>";
  s += " <div class=\"col-md-2\">";
  s += "<input class=\"btn btn-default\" id=\"c\"  style=\"width:100px;height: 50px \"  type=\"color\"  value=\"#ff0000\">";
  s += "</div>";
  s += " <div class=\"col-md-2\">";
  s += "<input type=\"submit\" value=\"Ok\" class=\"btn btn-success\"  style=\"width:75px;height: 50px \" onclick=\"hexToRgb()\">";
  s += "</div>";
  s += " </div>";
  s += "</div>";


  s += "<script>  function hexToRgb() { var hex=document.getElementById(\"c\").value;  var result = /^#?([a-f\\d]{2})([a-f\\d]{2})([a-f\\d]{2})$/i.exec(hex);  var x=result ? {    r: parseInt(result[1], 16),  g: parseInt(result[2], 16),   b: parseInt(result[3], 16)  } : null;  $.get(\"/r\"+x.r+\"g\"+x.g+\"b\"+x.b ); } </script>";

  //=========================================================Security=======================================================================
  s += "<div class=\"container\">";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "<div class=\"page-header\">";
  s += "<h2>Enable home security  <input id=\"sec\"  class=\"btn btn-default\" style=\"width: 80px;height: 34px;\" type=\"checkbox\" onclick=\"enableSec()\" ></h2> ";
  s +=  "<h3 id=\"state\"> Security State:  </h3> ";
//=========================================================Home Door=======================================================================

  s += " </div>";
  s += "</div>";
  s += " <h3>Home Door</h3>";
  s += "<div class=\"col-md-2\"><input  class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"Open\" onclick=\"opendo()\"></div>";
  s += "<div class=\"col-md-2\"><input  class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Close\" onclick=\"closedo()\"></div>";
  s += "</div> </div>";

  // ==================================================================== carage =========================================

 s += "<div class=\"container\">";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "</div>";
  s += " <h3>Carage Door</h3> ";
  s += "<div class=\"col-md-2\"><input  class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Stop\" onclick=\"openca()\"></div>";
   s += "<div class=\"col-md-2\"><input  class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Stop\" onclick=\"closeca()\"></div>";
  s += "</div></div>";
//=========================================================Buzzer=======================================================================
  s += "<div class=\"container\">";
  s += "<div class=\"row\">";
  s += " <div class=\"container\">";
  s += "</div>";
  s += " <h3>Buzzer</h3> ";
  s += "<div class=\"col-md-2\"><input  class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Stop\" onclick=\"stopbus()\"></div>";
  s += "</div></div>";





  s += "<script>function opendo() {$.get(\"/opendo\");}</script>";
  s += "<script>function closedo() {$.get(\"/closedo\");}</script>";
   s += "<script>function openca() {$.get(\"/openca\");}</script>";
  s += "<script>function closeca() {$.get(\"/closeca\");}</script>";
  s += "<script>function stopbus() {$.get(\"/stopbus\");}</script>";

  return s ;


}
String mess3() {
  String s;

  //===================================================Sensor ===================================================================
//  s += " <div class=\"container\">";
//  s += "<div class=\"page-header\">";
//  s += "<h2>Humidity sensor value :</h2>";
//  s += "  </div>";
//  s += "<div id=\"gaz\" class=\"well\"></div>";
//  s += "  </div>";
  //===================================================== sensor =================================================================
  s += " <div class=\"container\">";
  s += "<div class=\"page-header\">";
  s += "<h2>Temperature value :</h2>";
  s += "  </div>";
  s += "<div id=\"demo\" class=\"well\"></div>";
  s += "  </div>";
  s += "<br><br>";

  //================================================================== AutoUpdate====================================================
  s += "<script>function get_t() {$.get(\"/get_t\",function(result){document.getElementById(\"demo\").innerHTML =result;});}</script>";
  s += "<script>   setInterval(function () { get_t();}, 2000);   </script>";

//  s += "<script>function get_g() {$.get(\"/get_g\",function(valueOfGaz){document.getElementById(\"gaz\").innerHTML =valueOfGaz;});}</script>";
//  s += "<script>   setInterval(function () { get_g();}, 2000);   </script>";
  //==========================================Enable manual control============================================================================

  s += "<script> function enable(){ var en=document.getElementById(\"e\"); if(en.checked){ document.getElementById(\"b\").innerHTML=\"Control Lightting State:\"+\" \"+\"Manually Lightting\";  $.get(\"/enablecon\"); } else { document.getElementById(\"b\").innerHTML=\"Control Lightting State:\"+\" \"+\" Automatically Lightting\";  $.get(\"/unablecon\"); } } </script>";
  //====================================================== Enable security ================================================================
  s += "<script> function enableSec(){ var en=document.getElementById(\"sec\"); if(en.checked){ document.getElementById(\"state\").innerHTML=\"Security State:\"+\" \"+\" Enabled\";  $.get(\"/enablesec\"); } else { document.getElementById(\"state\").innerHTML=\"Security State:\"+\" \"+\" Unabled\";  $.get(\"/unablesec\"); } } </script>";

  //===================================================================Alarm ====================================================================
   s += "<script>function get_alarm() {$.get(\"/get_alarm\",function(alarm){if(alarm == \"1\"){alert(\"Security Broken\");} else if(alarm == \"2\"){alert(\"Fire Alarm\");}});}</script>";
  s += "<script>   setInterval(function () { get_alarm();}, 1000);   </script>";
  
  return s;
}
