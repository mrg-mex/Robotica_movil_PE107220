// Variables encoder
const int encoder_A = D5;
const int encoder_B = D6;
volatile int contador = 0;
int contador_ref = 0;
bool CW = true;

// Variables puente H
const int driver_1A = D2;
const int driver_2A = D1;
const int driver_12EN = D3;

// Variables para leer serial
const byte n = 32;
char datoRecibido[n];
int datoNumero = 0;

// Variables para el controlador PID
const int tiempoMuestreo = 25;
long tiempo = 0;
int error = 0;
float error_i = 0;
float error_d = 0;
int error_ant = 0;
float pwmControl = 0;
int pwmSalida = 0;
float kp = 100;
float ki = 100;
float kd = 1;
float u = 0;
float y = 0;

void setup(){
   // Configuración de pines para el encoder de cuadratura
   pinMode(encoder_A, INPUT_PULLUP);
   pinMode(encoder_B, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(encoder_A), cambioA, CHANGE);
   attachInterrupt(digitalPinToInterrupt(encoder_B), cambioB, CHANGE);

   // Configuración de pines para puente H
   pinMode(driver_1A, OUTPUT);
   pinMode(driver_2A, OUTPUT);
   pinMode(driver_12EN, OUTPUT);
   digitalWrite(driver_1A, LOW);
   digitalWrite(driver_2A, LOW);
   analogWrite(driver_12EN, 0);
   
   Serial.begin(115200);
}
 
void loop(){
   if (millis() > tiempo + tiempoMuestreo){
     error = datoNumero - contador;
     if(pwmSalida < 1023 && pwmSalida > -1023){
      error_i = error_i + float(error)/40;
     }
     error_d = float(error - error_ant)*40;
     
     pwmControl = kp * float(error) + ki * error_i + kd * error_d;

     u = float(datoNumero)/10;
     y = float(contador)/10;
     Serial.print(u);
     Serial.print("\t ");
     Serial.println(y);

     pwmSalida = int(pwmControl);
     if(datoNumero == 0 && contador == 0){
       pwmSalida = 0; 
       pwmControl = 0; 
       error_i = 0;
     }
     if(pwmSalida > 1023){pwmSalida = 1023;}
     if(pwmSalida < -1023){pwmSalida = -1023;}
     if(pwmSalida >= 0){
       digitalWrite(driver_1A, LOW);
       digitalWrite(driver_2A, HIGH);
       analogWrite(driver_12EN, pwmSalida);
     }
     else{
       digitalWrite(driver_1A, HIGH);
       digitalWrite(driver_2A, LOW);
       analogWrite(driver_12EN, -pwmSalida);
     }

     error_ant = error;
     contador = 0;
     tiempo = millis();
   }

   leerSerial();
}
 
ICACHE_RAM_ATTR void cambioA(){
    if (digitalRead(encoder_A) == digitalRead(encoder_B)){
       CW = true;
       contador++;
    }
    else{
       CW = false;
       contador--;
    }
}
 
ICACHE_RAM_ATTR void cambioB(){
    if (digitalRead(encoder_A) != digitalRead(encoder_B)){
       CW = true;
       contador++;
    }
    else{
       CW = false;
       contador--;
    }
}

void leerSerial() {
    static byte i = 0;
    char fin = '\n';
    char c;
   
    while (Serial.available() > 0) {
        c = Serial.read();
        if (c != fin) {
            datoRecibido[i] = c;
            i++;
            if(i >= n){
              i = n - 1;
            }
        }
        else {
            datoRecibido[i] = '\0';
            i = 0;
            datoNumero = atoi(datoRecibido);
        }
    }
}
