#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Totalplay-53CS_EXT";
const char* password = "Camarena75#";

WebServer server(80);

const int ledPin = 2;
bool ledState = LOW;
unsigned long lastRequestTime = 0; // Variable para almacenar el tiempo de la última solicitud recibida
const unsigned long requestInterval = 1000; // Intervalo mínimo entre solicitudes en milisegundos (1 segundo)

void inicio() {
  // Obtener el tiempo actual
  unsigned long currentTime = millis();

  // Verificar si ha pasado suficiente tiempo desde la última solicitud
  if (currentTime - lastRequestTime >= requestInterval) {
    // Actualizar el tiempo de la última solicitud
    lastRequestTime = currentTime;

    // Cambiar el estado del LED según la solicitud recibida
    if (server.hasArg("led1")) {
      if (server.arg("led1") == "on" && ledState == LOW) {
        ledState = HIGH;
        digitalWrite(ledPin, HIGH);
      } else if (server.arg("led1") == "off" && ledState == HIGH) {
        ledState = LOW;
        digitalWrite(ledPin, LOW);
      }
    }
  }

  // HTML de respuesta
  String html = "<html><body>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>Trabajo 11 Node MCU</title>\n";
  html += "<h1>Control de LED con ESP32</h1>";
  html += "<form method='get'>";
  if (ledState == LOW) {
    html += "<button type='submit' name='led1' value='on'>Encender LED</button>";
  } else {
    html += "<button type='submit' name='led1' value='off'>Apagar LED</button>";
  }
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  // Comunicación entre Arduino y ESP32
  Serial.begin(115200);
  Serial.println("Conectando . . . . . . ");
  Serial.println(ssid);
  // Configuración Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Esperando conexión
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Wifi conectado a ");
  Serial.println(ssid);
  Serial.print("Dirección IP asignada por router: ");
  Serial.println(WiFi.localIP());

  // Raíz del servidor web
  server.on("/", inicio);
  // Iniciar el servidor
  server.begin();
}

void loop() {
  // Solicitudes al servidor web
  server.handleClient();
}




