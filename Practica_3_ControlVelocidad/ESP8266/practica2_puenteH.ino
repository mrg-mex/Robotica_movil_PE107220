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
   if (contador_ref != contador){
      contador_ref = contador;
      Serial.println(contador_ref);
   }
   leerSerial();
   if(datoNumero >= 0){
     digitalWrite(driver_1A, LOW);
     digitalWrite(driver_2A, HIGH);
     analogWrite(driver_12EN, datoNumero);
   }
   else{
     digitalWrite(driver_1A, HIGH);
     digitalWrite(driver_2A, LOW);
     analogWrite(driver_12EN, -datoNumero);
   }
   delay(100);
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
