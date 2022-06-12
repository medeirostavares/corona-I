//Softare para o protótipo do ventilador respirador mecânico anucio mercado livre versão 1.2
//Verificar atualizações no link: https://drive.google.com/drive/folders/1BXV64vsrtPBqTw5V-YD1IAJMZtVfNDvE?usp=sharing



//pinagem
const int stepPin = 33;  //PUL+ -Pulse
const int dirPin = 32; //DIR+ -Direction
const int fimdecurso = 34; //Pulsador final 
const int buzzer = 10; //beep

//variaveis
const int stepsxrev = 800; //passos por revolução 
const int mmxrev= 8; // milímetros de deslocamento do eixo por rotação

int stepdelayInsp = 0;    //tempo entre passsos (ms) - inspiracao
int stepdelayExp = 0;     //tempo entre passsos (ms) - expiracao
int stepdelay = 0;        //tempo entre passsos (ms) - geral

int targetsteps =0;         // passos necessario para o final de curso 

int t1 = 500;               //tempo de espera inspiracao (milisegundos)
int t2 = 500;               //tempo de espera expiracao (milisegundos)

int disp = 0 ;              //deslocamento
int RPM = 0;                //respiracoes por minuto
int ratio = 2 ;             //inspiracao/expiracao
int ratioDen = 2 ;          //Para o calculo inspiracao/expiracao
int tCiclo = 0;             //Tempo do ciclo
int tInsp = 0;              //Tempo inspiracao
int tExp = 0;               //Tempo expiracao

int i = 0;                  //contar resp min
unsigned long start, finish, elapsed, over;    //Para calcular o tempo
float h, m, s;


void setup() {
  //comunicação serial 
  Serial.begin(115200);
  Serial.setTimeout(100);

  //saidas entradas
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(fimdecurso,INPUT);
  pinMode(buzzer,OUTPUT);


   //Leitura deslocamento 
   Serial.println("deslocamento  mm (50 - 80):");
   while (!Serial.available() > 0);   // Esperarvalor
   {
      disp = Serial.readString().toInt();
      Serial.print("deslocamento  SELEC: ");
      Serial.println(disp);
      Serial.println(" ");
    }

   //Leitura valor respiracoes p/ minuto
   Serial.println("respiracoes MINUTO (10 -20):");
   while (!Serial.available() > 0);   // esperar valor
   {
      RPM = Serial.readString().toInt();
      Serial.print("respiracoes SELEC: ");
      Serial.println(RPM);
      Serial.println(" ");
    }

   //Leitura valor I/E
   Serial.println("RELACAO ENTRE INSP/RESP (1, 2, 3, 4):");
   while (!Serial.available() > 0);   // Esperars valor
   {
      ratio = Serial.readString().toInt();
      Serial.print("RELACAO I/O SELEC: 1:");
      Serial.println(ratio);
      Serial.println(" ");
    }

    //Calculos iniciais
    tCiclo = (60000/RPM) ;
    Serial.print("Tempo dO ciclo: ");
    Serial.println(tCiclo);
    ratioDen = 1+ratio;
    tInsp = (tCiclo/ratioDen) ;
    Serial.print("Tempo de Inspiracao: ");
    Serial.println(tInsp);
    tExp = ((tCiclo*ratio)/ratioDen) ;
    Serial.print("Tempo de Expiracao: ");
    Serial.println(tExp);

    targetsteps = (disp)*(stepsxrev/mmxrev);   //Passos para o deslocamento 
    Serial.print("Passos necesarios: ");
    Serial.println(targetsteps);

    //velocidades diferentes de respiracao/inspiracao
    //stepdelayInsp = (((float)tInsp*1000)/((float)targetsteps*2)) ;   //microsegundos entre pasos- Inspiracao
    //Serial.println(stepdelayInsp);
    //stepdelayExp = (((float)tExp*1000)/((float)targetsteps*2)) ;     //microsegundos entre pasos - Expriracao
    //Serial.println(stepdelayExp);

    //velocidades constantes de respiracao/inspiracao
    stepdelay = (((float)tInsp*1000)/((float)targetsteps*2)) ;   //microsegundos entre pasos - Insp / Exp
    Serial.print("Delay pasos: ");
    Serial.println(stepdelay);
    stepdelayInsp = stepdelay;
    stepdelayExp = stepdelay;
    

    //Home para iniciar a máquina
    Serial.println(" ");  
    Serial.print("Ventilador mecânico inicio");
    
    digitalWrite(dirPin,HIGH); //inicia movimiento sentido X negativo  
    while (digitalRead(fimdecurso)==LOW){
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(stepdelayExp);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(stepdelayExp);
     }
     Serial.println("  ->  Endstop");
     delay(50);

     start = millis();     // Para contar o tempo
}



 
void loop() {

    //teste para contar o tempo
    i= i + 1;
    Serial.println(" ");
    Serial.print("Respiracao nº: ");
    Serial.println(i);     
    //teste para contar o tempo
     

     //INSPIRACAO 
     Serial.print("INALACAO "); 
     digitalWrite(dirPin,LOW); //inicia movimento sentido X positivo
     for(int i = 0; i < targetsteps; i++) { //loop de movimento de 0 a passos necessarios
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(stepdelayInsp);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(stepdelayInsp);        
      }
      delay(50);

      //Verificar sensor de fim decurso não está pressionado -> 
      if (digitalRead(fimdecurso)==HIGH)  {
        Serial.println(); 
        Serial.println();
        Serial.println("ERROR!!!!");                //alarme parada máquina
        Serial.println();
      }
      
      //delay (t1);     //Espera

      //EXPIRAÇÃO - Até o fim de curso 
      Serial.print(" ->  Exalaçao ");
      digitalWrite(dirPin,HIGH); //inicia movimento sentido X negativo
      while (digitalRead(fimdecurso)==LOW){
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(stepdelayExp);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(stepdelayExp);
       }
       Serial.println(" ->  Endstop");

       if (ratio != 1) {
       delay (((ratio-1)*tInsp)-50);     //Espere o cilindro de expiração <50 
       }


    //teste para contar o tempo
    if (i == RPM) {     
      finish = millis();
      elapsed = finish - start; 
      h = int(elapsed / 3600000); 
      over = elapsed % 3600000; 
      m = int(over / 60000); 
      over = over % 60000; 
      s = int(over / 1000);
      Serial.println("CICLO COMPLETO: ");
      Serial.print(m, 0); 
      Serial.print("m "); 
      Serial.print(s, 0); 
      Serial.println("s ");
      i = 0;
      start = millis();     // Para contar tempo
    }
    //teste para contar tempo


}
