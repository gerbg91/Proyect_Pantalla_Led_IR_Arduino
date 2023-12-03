// Include the required Arduino libraries:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <IRremote.hpp>

// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 3
#define IR_RECEIVE_PIN 5

// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const uint8_t F_PMAN1 = 6; // numero de frames
const uint8_t W_PMAN1 = 9; // numero de ancho
const uint8_t PROGMEM mando[F_PMAN1 * W_PMAN1] =
{
  0x00, 0x3e, 0x7b, 0xfb, 0x03, 0xfb, 0x7b, 0x3e, 0x00,
  0x00, 0x3e, 0x7b, 0xfb, 0x03, 0xfb, 0x7b, 0x3e, 0x00,
  0x00, 0x3e, 0x7b, 0xfb, 0x03, 0xfb, 0x7b, 0x3e, 0x00,
  0x00, 0x3e, 0x7b, 0xfb, 0x03, 0xfb, 0x7b, 0x3e, 0x00,
  0x00, 0x3e, 0x7b, 0xfb, 0x03, 0xfb, 0x7b, 0x3e, 0x00,
  0x00, 0x3e, 0x7b, 0xfb, 0x03, 0xfb, 0x7b, 0x3e, 0x00
};

//Botones
#define IR_BUTTON_1 69

//Otros valores
// reinicioProcesoDisplay 0 -> Texto
// reinicioProcesoDisplay 1 -> Sprite
int8_t reinicioProcesoDisplay = 0;
int8_t valorIntroducidoMando = 0;
int8_t intensidadDisplay = 3;
int8_t numeroDisplay = 5;
int8_t contadorDisplay = 0;

//Valores de display
#define    BUF_SIZE    75
char textoDisplay[BUF_SIZE] = { "Este es el camino" };
static uint8_t tiempoFrameDisplay = 100;
static uint8_t tiempoPausaDisplay = 1000;
char textoSpriteDisplay[BUF_SIZE] = { "" };
static uint8_t tiempoFrameSpriteDisplay = 100;
static uint8_t tiempoPausaSpriteDisplay = 1000;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);
  myDisplay.begin();
  myDisplay.setIntensity(intensidadDisplay);
  myDisplay.displayClear();
}

void loop() {
  //Obligamos a que siempre entre para leer valores del mando(Por defecto sera 0)
  if (IrReceiver.decode() or 1 == 1) {
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;

    //Guardamos el valor introducido para evaluarlo mas tarde
    if (valorIntroducidoMando == 0 && command == IR_BUTTON_1 ) {
      valorIntroducidoMando = command;
    }

    switch (valorIntroducidoMando) {
      case IR_BUTTON_1: {
          seleccionDisplay();
          break;
        }
      default: {
          seleccionDisplay();
          break;
        }
    }

    if (contadorDisplay == numeroDisplay) {
      Serial.println ("contadorDisplay");
      valorIntroducidoMando = 0;
      contadorDisplay = 0;
    }
  }
}

void seleccionDisplay() {
  if (myDisplay.displayAnimate() and IR_BUTTON_1 == valorIntroducidoMando) {
    if (reinicioProcesoDisplay == 0) {
      mostrarDisplay(1);
      reinicioProcesoDisplay = 1;
    } else if (reinicioProcesoDisplay == 1) {
      mostrarDisplay(2);
      //Vuelve al estado inicial del Display
      reinicioProcesoDisplay = 0;
      contadorDisplay = contadorDisplay + 1;
    }
    myDisplay.displayReset();

  }
}

void mostrarDisplay(int valor) {
  switch (valor) {
    case 1:
      myDisplay.displayText(textoDisplay, PA_CENTER, tiempoFrameDisplay, tiempoPausaDisplay, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      break;
    case 2:
      myDisplay.setSpriteData(mando, W_PMAN1, F_PMAN1, mando, W_PMAN1, F_PMAN1);
      myDisplay.displayText(textoSpriteDisplay, PA_CENTER, tiempoFrameSpriteDisplay, tiempoPausaSpriteDisplay, PA_SPRITE, PA_SPRITE);
      break;
  }
}
