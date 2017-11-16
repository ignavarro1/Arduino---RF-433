/* 
 -------------------------------------------------------------
  EMISOR RF433 ANALOGICO
  Ignacio Navarro

  VCC -> 5V
  DATA -> 4D
  GND -> GND

  Alterna entre los 2 botones del control.
 ------------------------------------------------------------- */

#define rfTransmitPin 4  //RF Transmisor = D4
#define ledPin 13        //Led = D13

const int tamCodigo = 25;      //Tamaño del codigo a transmitir
int codigoATransmitir[tamCodigo]; //Arreglo utilizado para guardar el codigp
int codigoA[]={1,4,4,4,4,5,2,1,5,1,5,1,4,4,5,1,5,1,5,2,2,1,5,2,3}; //Codigo de boton marcado
int codigoB[]={1,2,1,4,4,5,2,1,5,1,5,1,4,4,5,1,5,1,5,2,2,1,5,2,3}; //Codigo de boton sin marcar
int contCodigo = 0;  //Switch para cambiar el comando enviado
int timeDelay=70;      //Variable para calibrar longitud de señal

 
 
void setup(){
  Serial.begin(9600);        //Arranca el Serial
  pinMode(rfTransmitPin, OUTPUT);   //Setea el pin de transmision  
  pinMode(ledPin, OUTPUT);          

  //Inicializa el led 13
  digitalWrite(ledPin, HIGH); 
  delay(1000);
  digitalWrite(ledPin, LOW); 
  delay(1000);
}
 
 
 
void loop(){
  cambioCodigo();    //Cambia entre comando de control
  transmitirCodigo();  //Transmite el codigo

  timeDelay+=20;    //Aumenta 10 microsegundos el delay en cada transmision
  delay(2000);      //Espera 2 segundos para repetir ciclo
}
  
  
  
  
/*---------------------------------------------------------------- 
   cambioCodigo(): Funcion para alternar el codigo del control
-----------------------------------------------------------------*/
void cambioCodigo(){
  if(contCodigo==0){
    for(int i = 0; i<tamCodigo; i++){
      codigoATransmitir[i]=codigoA[i];
    };
    contCodigo=1;
  } else {
    for(int i = 0; i<tamCodigo; i++){
      codigoATransmitir[i]=codigoB[i];
    };
    contCodigo= 0;
  }
}
   
   
   
   
/*-----------------------------------------------------------------
transmitirCodigo():Utilizada para transmitir el codigo de señales
                
-------------------------------------------------------------------*/
void transmitirCodigo() {
  //Se prende el led para señalizar la transmision
  digitalWrite(ledPin, HIGH);
  Serial.println("Enviando codigo");

  //Inicializa las variables
  int largoAlto = 0;
  int largoBajo = 0;

  //Codigo de señales ALTA Y BAJA
  #define SL 6
  #define LL 12
  #define VLL 174
  #define SH 6
  #define LH 12
  #define VLH 124
    
  //Se transmite 2 veces la señal (Varia por cada control).       
  for(int j = 0; j<6; j++){
    if (j==0) {
      digitalWrite(rfTransmitPin,HIGH);
      delayMicroseconds(VLH*timeDelay);
      digitalWrite(rfTransmitPin,LOW);
      delayMicroseconds(VLL*timeDelay);
    } 
    for(int i = 0; i<tamCodigo; i++){
      switch(codigoATransmitir[i]){
        case 1: // SH + SL
          largoAlto=SH;
          largoBajo=SL;
        break;
        case 2: // LH + SL
          largoAlto=LH;
          largoBajo=SL;
        break;
        case 3: // VLH + SL
          largoAlto=VLH;
          largoBajo=SL;
        break;
        case 4: // SH + LL
          largoAlto=SH;
          largoBajo=LL;
        break;
        case 5: // LH + LL
          largoAlto=LH;
          largoBajo=LL;
        break;
        case 6: // VLH + VLL
          largoAlto=VLH;
          largoBajo=SL;
        break;
      }
         
       /* Transmite la señal de ALTA, determinada por el ancho de la señal y el delay */
       digitalWrite(rfTransmitPin, HIGH);     
       delayMicroseconds(largoBajo*timeDelay);


       /* Transmite la señal de BAJA, determinada por el ancho d ela señal y el delay */
       digitalWrite(rfTransmitPin,LOW);     
       delayMicroseconds(largoAlto*timeDelay);
     }
  }
  //Apaga el led al finalizar la transmicion
  Serial.println("Codigo enviado");
  digitalWrite(ledPin, LOW); 
 }
