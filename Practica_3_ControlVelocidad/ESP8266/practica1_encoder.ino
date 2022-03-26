const int encoder_A = D5;
const int encoder_B = D6;
volatile int contador = 0;
int contador_ref = 0;
bool CW = true;

void setup(){
   // Configuración de pines para el encoder de cuadratura
   pinMode(encoder_A, INPUT_PULLUP);
   pinMode(encoder_B, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(encoder_A), cambioA, CHANGE);
   attachInterrupt(digitalPinToInterrupt(encoder_B), cambioB, CHANGE);

   Serial.begin(115200);
}
 
void loop(){
   if (contador_ref != contador){
      contador_ref = contador;
      Serial.println(contador_ref);
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
