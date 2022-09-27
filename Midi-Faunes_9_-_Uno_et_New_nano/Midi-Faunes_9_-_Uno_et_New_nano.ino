#include <MIDI.h>
#include "PitchToNote.h"
#include <nRF24L01.h>
#include <RF24.h>


#define PIN_RADIO_CE 9
#define PIN_RADIO_CSN 10
#define PIN_BOUTON_DANSE 4
#define PIN_BOUTON_CALEBASSE 5
#define PIN_BOUTON_ORAGE 6

//Initialisation des variables pour la lumière
int varLum = 2; //Puissance luminosité
int joueNote = 0; //Flag pour la lumière

//Initialisation des variables pour les boutons
const unsigned long intervalBouton = 3000; //Valeur de l'anti-rebond deluxe
unsigned long currentMillis; //Variable du temps
unsigned long previousMillisDanse = 0, previousMillisCalebasse = 0, previousMillisOrage = 0;
int etatBoutonDanse=HIGH, etatBoutonCalebasse=HIGH, etatBoutonOrage=HIGH;


RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

void setup()
{

  //Je déclare le pinMode de mes boutons
  pinMode(PIN_BOUTON_DANSE, INPUT_PULLUP);
  pinMode(PIN_BOUTON_CALEBASSE, INPUT_PULLUP);
  pinMode(PIN_BOUTON_ORAGE, INPUT_PULLUP);
  
	//Initialisation radio
	const byte address[6] = "00001";
	radio.begin();
	radio.openReadingPipe(0, address);
	radio.setPALevel(RF24_PA_MAX);
	radio.startListening();
  
	//Initialisation MIDI - Envoie note de stop
	MIDI_CREATE_DEFAULT_INSTANCE(); 
	MIDI.begin();
  MIDI.sendNoteOn(pitchC7,127, 2); //Je joue la note
 

//Serial.begin(9600); //POUR DEBUG

  delay(100);
}


void loop()
{
  MIDI_CREATE_DEFAULT_INSTANCE();
  
  //Je lis l'état des boutons
  etatBoutonDanse = digitalRead(PIN_BOUTON_DANSE);
  etatBoutonCalebasse = digitalRead(PIN_BOUTON_CALEBASSE);
  etatBoutonOrage = digitalRead(PIN_BOUTON_ORAGE);
  
  //Je vérifie la lumière via radio
  if(radio.available())
  {
    radio.read(&varLum, sizeof(int));
  }

/*
Serial.println(varLum); //POUR DEBUG
if (varLum>=1) {Serial.println("sombre !");}
else if (varLum <1) {Serial.println("Lumineux !");}
*/


//    !!!!!!!!!! LUMIERE !!!!!!!!!!
  if (varLum > 30) //s'il fait lumineux
      {
        if (joueNote)  //Si la note est déjà jouée
        {}  //Je ne fais rien de plus
        else  //Si aucune la note n'était pas en cours de jeu
        {
          MIDI.sendNoteOn(pitchA0,127, 2); //Je joue la note
          joueNote = 1; //Et je change le Flag
        }   
  
       }
              
  else  //S'il fait sombre
      {
        if (joueNote)  //Si une note est en cours de jeu
        {
          MIDI.sendNoteOff(pitchA0,1,2); //J'arrête la note
          joueNote = 0; //Et je change le Flag en "non-joué"
        }  
        
        else //Si une note était joué avant
        {}  //Je ne fait rien
       } 
 



//    !!!!!!!!!! BOUTONS !!!!!!!!!!
 
  //Je lis le temps
  currentMillis = millis();

  if(etatBoutonDanse == LOW) //Si le bouton est appuyé
  {
    if (currentMillis - previousMillisDanse >= intervalBouton) //et si l'interval anti-rebond-deluxe est attendu)
    {
          MIDI.sendNoteOn(pitchB1, 127, 1);
          MIDI.sendNoteOff(pitchB1, 0, 1);  //J'envoie une commande activée par le bouton !
      previousMillisDanse = currentMillis;
    }
  }



  if(etatBoutonCalebasse == LOW) //Si le bouton est appuyé
  {
    if(currentMillis - previousMillisCalebasse >= intervalBouton) //et si l'interval anti-rebond-deluxe est attendu)
    {
      MIDI.sendNoteOn(pitchB2, 127, 1);
          MIDI.sendNoteOff(pitchB2, 0, 1);//J'envoie une commande activée par le bouton !
      previousMillisCalebasse = currentMillis;
    }
  }


  if(etatBoutonOrage == LOW) //Si le bouton est appuyé
  {
    if(currentMillis - previousMillisOrage >= intervalBouton) //et si l'interval anti-rebond-deluxe est attendu)
    {
      MIDI.sendNoteOn(pitchB3, 127, 1);
          MIDI.sendNoteOff(pitchB3, 0, 1);  //J'envoie une commande activée par le bouton !
      previousMillisOrage = currentMillis;
    }
  }

}          
