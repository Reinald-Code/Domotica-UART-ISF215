#include <Servo.h>

// --- CONFIGURACIÓN DE HARDWARE ---
const uint8_t PIN_LED_OK = 7;       
const uint8_t PIN_LED_ALERTA = 8;   
const uint8_t PIN_BUZZER = 9;       
const uint8_t PIN_SERVO_PUERTA = 5;  
const uint8_t PIN_SERVO_VENTANA = 6; 
const uint8_t PIN_TRIG = 10;        
const uint8_t PIN_ECHO = 11;        

const uint32_t REFRESH_SENSOR_MS = 250;
const uint8_t DISTANCIA_UMBRAL_CM = 20; // 0.2 metros

Servo servoPuerta;
Servo servoVentana;

bool puertaAbierta = false;
bool ventanaAbierta = false;
bool alarmaActiva = false;
uint32_t tUltimoPing = 0;

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_ALERTA, OUTPUT);
  pinMode(PIN_LED_OK, OUTPUT);
  
  servoPuerta.attach(PIN_SERVO_PUERTA);
  servoVentana.attach(PIN_SERVO_VENTANA);
  
  actualizarActuadores();
}

void loop() {
  uint32_t tiempoActual = millis();
  
  if (tiempoActual - tUltimoPing >= REFRESH_SENSOR_MS) {
    tUltimoPing = tiempoActual;
    verificarSensorProximidad();
  }
}

void verificarSensorProximidad() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  uint32_t duracion = pulseIn(PIN_ECHO, HIGH, 20000); 
  if(duracion == 0) {
    alarmaActiva = false;
    return;
  }
  
  uint8_t distancia = duracion * 0.034 / 2;
  
  // Filtro de 0.03 metros para evitar ruido
  if (distancia < DISTANCIA_UMBRAL_CM && distancia > 3) {
    alarmaActiva = true;
    puertaAbierta = false;  
    ventanaAbierta = false; 
    actualizarActuadores();
  } else if (distancia >= DISTANCIA_UMBRAL_CM) {
    alarmaActiva = false;
  }
}

void actualizarActuadores() {
  servoPuerta.write(puertaAbierta ? 90 : 0);
  servoVentana.write(ventanaAbierta ? 90 : 0);
}