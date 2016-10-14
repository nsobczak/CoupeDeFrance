/**
 *    \file Ecran_test.ino
 *    \brief Code d'affichage de l'écran
 *    \author Arthur Duytschaever & Nicolas Sobczak
 *    \date octobre 2016
*/


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "logoCrysteo.h"
#include "Ecran.h"
#include "U8glib.h"


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#define MENU_ITEMS 4
#define _MENU1_ "Strategie"
#define _MENU2_ "Test"
#define _MENU3_ "Debug"
#define _MENU4_ "Back"

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4


/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
//Declaration de l'objet UG8 + pin du SPI
U8GLIB_ST7920_128X64_1X u8g(13, 51, 14);  // SPI Com: SCK = en = 13, MOSI = rw = 51, CS = di = 14

//Declaration de l'encodeur = le bouton
const int entry = 2; 
const int encoderMoin = 52;
const int encoderPlus = 50;

//Declaration de trucs inconnus
uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;
int menu_current = 0;
uint8_t i, h;
u8g_uint_t w, d;
int selectMenu = 10;

//Definition du nom des menus
const char *menu_strings[MENU_ITEMS] = {_MENU1_, _MENU2_, _MENU3_, _MENU4_};



/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
 
/**____________________________________________________
 *   \fn void ecranDacceuil(void)
 *   \param void
 *   Fonction qui affiche le logo au debut
 */
void ecranDacceuil(void) {
  u8g.firstPage();
   do {
    u8g.drawBitmapP(0, 0, 16, 64, logoCrysteo);
  } while(u8g.nextPage());
}


/**____________________________________________________
 *   \fn void drawMenu(void)
 *   \param void
 *   Afficher le menu après le logo + bande de selection
 */
void drawMenu(void) {
  uint8_t i, h;
  u8g_uint_t w, d;
   
  u8g.setFont(u8g_font_ncenB10);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, i*h, menu_strings[i]);
  }
}


/**____________________________________________________
 *   \fn void updateMenu(void)
 *   \param void
 *   Fonction qui selectionne le menu sur lequel le curseur est place
 */
void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      menu_current++;
      if ( menu_current >= MENU_ITEMS )
        menu_current = 0;
      menu_redraw_required = 1;
      break;
    case KEY_PREV:
      if ( menu_current == 0 )
        menu_current = MENU_ITEMS;
      menu_current--;
      menu_redraw_required = 1;
      break;
    case KEY_SELECT:
      for(int indice=0;indice<=MENU_ITEMS;indice++){
        if(menu_current==indice){
          selectMenu = indice;
        }
      }
      break;  
  }
}


/**____________________________________________________
 *   \fn void uiStep(void)
 *   \param void
 *   Fonction qui initialise la configuration des pins
 */
void uiStep(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(encoderMoin) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(encoderPlus) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(entry) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  //else if ( digitalRead(uiKeyBack) == LOW )
    //uiKeyCodeFirst = KEY_BACK;
  else 
    uiKeyCodeFirst = KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}


//_______________________________
//Test de l'objet
Ecran monEcran;

/**____________________________________________________
 *   \fn void Menu(void)
 *   \param void
 *   Fonction qui affiche qqch
 */
void Menu(void){
  u8g.setPrintPos(0,0); 
  //initialisation de l'objet
  monEcran.setNom("string", 0);
  monEcran.setNom("string", 1);
  monEcran.setNom("string", 2);
  monEcran.setNom("string", 3);
  monEcran.setNom("string", 4);
  for (int i = 0; i < 5; ++i){
    monEcran.setValue(3.2*i, i);
  }
  //affichage a l'ecran
  for (int i = 0; i < 5; ++i){
      u8g.setPrintPos(0, 0 + 11 * i);
      u8g.print(monEcran.getNom(i));
      u8g.print(": ");
      u8g.print(monEcran.getValue(i));
  }
}


/**____________________________________________________
 *   \fn void Menu10(void)
 *   \param void
 *   Fonction qui affiche qqch mais un autre qqch
 */
void Menu10(void){
  u8g.drawBox(0,0,128,64);
  u8g.setPrintPos(0,0); 
  //initialisation de l'objet
  monEcran.setNom("s", 0);
  monEcran.setNom("s", 1);
  monEcran.setNom("s", 2);
  monEcran.setNom("s", 3);
  monEcran.setNom("s", 4);
  for (int i = 0; i < 5; ++i){
    monEcran.setValue(3.2*i, i);
  }
  //affichage a l'ecran
  for (int i = 0; i < 5; ++i){
      u8g.setPrintPos(0, 0 + 11 * i);
      u8g.print(monEcran.getNom(i));
      u8g.print(": ");
      u8g.print(monEcran.getValue(i));
  }
}


/**____________________________________________________
 *   \fn void SelectMenu(int selectMenu)
 *   \param int selectMenu
 *   Fonction qui affiche le menu correspondant à la selection du curseur
 */
void SelectMenu(int selectMenu){// fonction beug.....
  //Serial.println(selectMenu);
  //u8g.firstPage();
  switch(selectMenu){
    case 0 :    
        /*u8g.firstPage();// potentiellement source de beug*/
      do{
        Menu10();
    }while(u8g.nextPage());
    case 1 : 
        u8g.firstPage();
    do{
      Menu();
    }while(u8g.nextPage());
    case 2 :
        u8g.firstPage();
    do{
      Menu();
    }while(u8g.nextPage());
    case 3 :
        u8g.firstPage();
    do{
      ecranDacceuil();
    }while(u8g.nextPage());
  }
}


//____________________________________________________
//____________________________________________________
void setup(void) {
  //Attachinterrupt()
    Serial.begin(9600);
    pinMode(entry,INPUT_PULLUP);
    pinMode(encoderMoin,INPUT_PULLUP);
    pinMode(encoderPlus,INPUT_PULLUP);
    //digitalWrite(entry,HIGH);
      u8g.setColorIndex(1); // Affichage en mode N&B
   
  //u8g.firstPage(); // Sélectionne la 1er page mémoire de l'écran
  ecranDacceuil();
  delay(2000);
  uiStep();
  // flip screen, if required
   //u8g.setRot180();
                                   // setup key detection and debounce algorithm
  menu_redraw_required = 1;
}


void loop(void) {
  
  uiStep();                                     // check for key press 
  if (  menu_redraw_required != 0 ) {
    u8g.firstPage();
    do  {
      drawMenu();
    } while( u8g.nextPage() );
    menu_redraw_required = 0;
  }
   
   SelectMenu(selectMenu);


  selectMenu = 100;
  updateMenu();     
}



