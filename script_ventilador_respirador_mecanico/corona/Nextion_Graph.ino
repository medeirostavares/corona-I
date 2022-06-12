
//DISPLAY UTILIZADO:
//NEXTION: 4.3" NX4827T043

//Pinagem:
//TX = 0;
//RX = 1;

#include "Nextion.h" //biblioteca para comunicação com o display NEXTION

bool isPage1 = false; //variável de controle para saber se é a página main que está em tela

//DESLOCAMENTO:
int disp = 50 ; 
#define DISP_MAX 80
#define DISP_MIN 50
#define DISP_FATOR 5

//RESPIRAÇÕES POR MINUTO:
int RPM = 20;             
#define RPM_MAX 20
#define RPM_MIN 10
#define RPM_FATOR 2

//RELAÇÃO INSPIRAÇÃO/EXPIRAÇÃO
int ratio = 1 ;             
#define RATIO_MAX 4
#define RATIO_MIN 1
#define RATIO_FATOR 1

/*
 * DECLARAÇÕES DOS OBJETOS DO NEXTION:  [pageID, componentID, componentName]
 */

//PÁGINAS:
NexPage page0    = NexPage(0, 0, "initPage");
NexPage page1    = NexPage(1, 0, "mainPage");
NexPage page2    = NexPage(2, 0, "hardStopPage");
NexPage page3    = NexPage(3, 0, "softStopPage");
 
//BARRAS DE PROGRESSO:
NexProgressBar barIni     = NexProgressBar(0, 3, "barIni"); //Progress Bar de inicialização
//NexProgressBar barDes     = NexProgressBar(1, 2, "barDes"); //Progress Bar Deslocamento: 50-80mm
//NexProgressBar barResp    = NexProgressBar(1, 7, "barResp"); //Progress Bar Resp/Min: 10-20
//NexProgressBar barInsp    = NexProgressBar(1, 11, "barInsp"); //Progress Bar Insp/Resp: 1-4
//NexProgressBar barOxi     = NexProgressBar(1, 18, "barOxi"); //Progress Bar Oxímetro: 0-100% (oxigenação sanguínea)
NexProgressBar barStat    = NexProgressBar(1, 14, "barStat"); //Progress Bar Status: 0-100% (p.absoluta)

//TARJETAS VALORES:
NexNumber desNumber   = NexNumber(1, 21, "desNumber"); //tarjeta do deslocamento
NexNumber respNumber  = NexNumber(1, 22, "respNumber"); //tarjeta da resp/min
NexNumber inspNumber  = NexNumber(1, 23, "inspNumber"); //tarjeta da insp/resp
NexNumber oxiNumber   = NexNumber(1, 24, "oxiNumber"); //tarjeta do oxímetro

//BOTÕES:
NexButton btStart     = NexButton(1, 17, "btStart"); //botão start da página 1
NexButton btStop      = NexButton(1, 16, "btStop"); //botão stop da página 1
//NexButton btNextPg1   = NexButton(1, 20, "btNextPg1"); //botão next da página 1
//NexButton btPrevPg2   = NexButton(2, 1, "btPrevPg2"); //botão prev da página 2
NexButton btDesMais   = NexButton(1, 1, "btDesMais"); //botão mais do deslocamento na página 1
NexButton btDesMenos  = NexButton(1, 4, "btDesMenos"); //botão menos do deslocamento na página 1
NexButton btRespMais  = NexButton(1, 6, "btRespMais"); //botão mais do resp/min na página 1
NexButton btRespMenos = NexButton(1, 8, "btRespMenos"); //botão menos do resp/min na página 1
NexButton btInspMais  = NexButton(1, 10,"btInspMais"); //botão mais do insp/resp na página 1
NexButton btInspMenos = NexButton(1, 12,"btInspMenos"); //botão menos do insp/resp na página 1

//GRÁFICOS:
//NexWaveform waveCard      = NexWaveform(2, 2, "waveCard"); //componente do gráfico de frequência cardíaca


//Nesse array, declaramos os objetos Nextion que terão interação de eventos touch
NexTouch *nex_listen_list[] = 
{
    &btDesMais,
    &btDesMenos,
    &btRespMais,
    &btRespMenos,
    &btInspMais,
    &btInspMenos,
    &btStart,
    &btStop,
//    &btNextPg1,
//    &btPrevPg2,
    NULL
};

char buffer[100] = {0}; //buffer para armazenar dados string

//variáveis de controle de tempo, para funcionar como um cronômetro
long previousMillis = 0;
const int maxTime = 1000;

void setup() 
{
    Serial.begin(115200);

    nexInit(); //inicializa a comunicação com o nextion

    page0.show();
    barraInicio();
    page1.show();
    
    //INICIALIZA CAMPOS:
    //barDes.setValue(DISP_MIN+20);
    //desNumber.setValue(DISP_MIN);
    
    isPage1 = true;
    
    //callbacks para os eventos de release dos botões:
    btDesMais.attachPop(btDesMaisPopCallback, &btDesMais);
    btDesMenos.attachPop(btDesMenosPopCallback, &btDesMenos);
    btRespMais.attachPop(btRespMaisPopCallback, &btRespMais);
    btRespMenos.attachPop(btRespMenosPopCallback, &btRespMenos);
    btInspMais.attachPop(btInspMaisPopCallback, &btInspMais);
    btInspMenos.attachPop(btInspMenosPopCallback, &btInspMenos);
    btStart.attachPop(btStartPopCallback, &btStart);
    btStop.attachPop(btStopPopCallback, &btStop);
    //btNextPg1.attachPop(btNextPg1PopCallback, &btNextPg1);
    //btPrevPg2.attachPop(btPrevPg2PopCallback, &btPrevPg2);

}

void loop() {

    //essa função trabalha como um listener para os eventos de press e release dos objetos utilizados no NEXTION
    nexLoop(nex_listen_list);

    //verifica se a página atual é a MAIN
    //caso seja, verifica se o tempo passado entra a última passagem e a atual, foi de 1 segundo
    //e então chama a atualização dos aferidores
    /*
    if(isPage1)
    {
      if(millis() - previousMillis >= maxTime)
      {
        previousMillis = millis();
        
        //FUNÇÕES AKI!
      
      }      
    } 
    */   
}

void barraInicio()
{
    int i;   
    for(i=0;i<=100;i++)
    {
      barIni.setValue(i);
      delay(30);
    }  
}

//CALLBACKS DOS BOTÕES:

//callback do botão next da página 1
//void btNextPg1PopCallback(void *ptr)
//{
//  page2.show();
//  isPage1 = false;
//}

//callback do botão prev da página 2
//void btPrevPg2PopCallback(void *ptr)
//{
//  page1.show();
//  isPage1 = true;
//}

//callback do botão + do deslocamento
void btDesMaisPopCallback(void *ptr)
{
  if(disp < DISP_MAX)
  {
    disp = disp + DISP_FATOR;
    //barDes.setValue(disp+(100-DISP_MAX));  //progress bar do deslocamento
    desNumber.setValue(disp);
  }
}

//callback do botão - do deslocamento
void btDesMenosPopCallback(void *ptr)
{
  if(disp > DISP_MIN)
  {
    disp = disp - DISP_FATOR;
    //barDes.setValue(disp+(100-DISP_MAX));  //progress bar do deslocamento
    desNumber.setValue(disp); 
  }
}

//callback do botão + do resp/min
void btRespMaisPopCallback(void *ptr)
{
  if(RPM < RPM_MAX)
  {
    RPM = RPM + RPM_FATOR;
    //barResp.setValue(RPM);  //progress bar do resp/min
    respNumber.setValue(RPM); 
  }
  
}
//callback do botão - do resp/min
void btRespMenosPopCallback(void *ptr)
{
  if(RPM > RPM_MIN)
  {
    RPM = RPM - RPM_FATOR;
    //barResp.setValue(RPM);  //progress bar do resp/min
    respNumber.setValue(RPM); 
  }
  
}

//callback do botão + do insp/resp
void btInspMaisPopCallback(void *ptr)
{
  if(ratio < RATIO_MAX)
  {
    ratio = ratio + RATIO_FATOR;
    //barInsp.setValue(ratio);  //progress bar do resp/min
    inspNumber.setValue(ratio); 
  }
  
}

//callback do botão - do insp/resp
void btInspMenosPopCallback(void *ptr)
{
  if(ratio > RATIO_MIN)
  {
    ratio = ratio - RATIO_FATOR;
    //barInsp.setValue(ratio);  //progress bar do resp/min
    inspNumber.setValue(ratio); 
  }
  
}

//callback do botão start
void btStartPopCallback(void *ptr)
{
  
}

//callback do botão stop
void btStopPopCallback(void *ptr)
{
  
}


//CALLBACKS DAS PAGES:

void page1PopCallback(void *ptr)
{
    dbSerialPrintln("page0PopCallback");
    page2.show();
    isPage1 = false;
}

void page2PopCallback(void *ptr)
{
    dbSerialPrintln("page1PopCallback");
    page1.show();
    isPage1 = true;
}
