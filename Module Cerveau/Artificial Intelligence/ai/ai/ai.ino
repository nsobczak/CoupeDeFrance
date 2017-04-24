/**
 *    \file ai.ino
 *    \brief bot artificial intelligence
 *    \author Nicolas Sobczak
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* === Notes ===
   On sélectionne le programme avec l'écran puis on tire la tirette.

   TODO:
 * si on détecte que la tirette est tirée => on lance le programme
 * le programme lance un timer + déplacement vers un cylindre
 * à la fin des 90", on lance la funny action
      - si c'est la fin des 90" - le temps de lâcher un cylindre,
 * si on a un cylindre, on le lache
 * dans tous les cas, on s'arrête et on attends de dépasser les 90" pour lancer la funny action avec une fonction
      - sinon
 * on corrige avec l'asservissement
 * si on détecte l'adversaire => manoeuvre dévitement
 * si on détecte un cylindre => on le ramasse

 */

/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#include <Arduino.h>
#define _DEBUG_ true

// I2C
#include <Wire.h>
#include "i2cCommunication.h"

// Ecran
#include <U8glib.h>
#include <M2tk.h>
#include "utility/m2ghu8g.h"
#include "utility/mas.h"

// setup u8g object
U8GLIB_ST7920_128X64_1X u8g(13, 51, 14);  // SPI Com: SCK = en = 13, MOSI = rw = 51, CS = di = 14  //(53, 52, 13);

// Bot
#include "Bot.h"


Bot elPadre;
bool epreuveFaite;

//____________________________________________________________________________________________________
/* ======================================================================================================
 *      Variables for Debug Menu
 * ======================================================================================================
 */
uint32_t irBackValue;
uint32_t irFrontBottomLeftValue;
uint32_t irFrontBottomCenterValue;
uint32_t irFrontBottomRightValue;
uint32_t irFrontTopValue;

uint32_t usBackValue;
uint32_t usLeftValue;
uint32_t usFrontValue;
uint32_t usRightValue;

uint32_t x_position;
uint32_t y_position;
uint32_t angle_position;


//____________________________________________________________________________________________________
/* ======================================================================================================
 *      Variables for Menu Process
 * ======================================================================================================
 */
/*
 *   \brief Declaration de l'encodeur (le bouton)
 *   const int entry = 8;
 *   const int encoderMoins = 10;
 *   const int encoderPlus = 11;
 */
uint8_t uiKeyUpPin = 50;
uint8_t uiKeyDownPin = 52;
uint8_t uiKeySelectPin = 2;
uint8_t uiKeyExitPin = 0;


//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_el_expandable_menu);


//____________________________________________________________________________________________________
/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
// === Asservissement ===
/**
 * \fn void testAsservissement_goStraightAhead
 * \param m2_el_fnarg_p fnarg
 * \brief test de l'asservissement: ligne droite vers l'avant
 */
void testAsservissement_goStraightAhead(m2_el_fnarg_p fnarg)
{
        do
        {
                if (_DEBUG_) Serial.println("botGoForward(0.4, 0.5);");
                elPadre.getAsservissement().botGoForward(0.4, 0.5);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleStraightOrderEnd();
}

/**
 * \fn void testAsservissement_goStraightAhead
 * \param m2_el_fnarg_p fnarg
 * \brief test de l'asservissement: ligne droite vers l'arrière
 */
void testAsservissement_goBackward(m2_el_fnarg_p fnarg)
{
        do
        {
                if (_DEBUG_) Serial.println("botGoBackward(0.2, 0.25);");
                elPadre.getAsservissement().botGoBackward(0.2, 0.25);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleStraightOrderEnd();
}

/**
 * \fn void testAsservissement_turnAroundRight
 * \param m2_el_fnarg_p fnarg
 * \brief test de l'asservissement: rotation vers la droite
 */
void testAsservissement_turnAroundRight(m2_el_fnarg_p fnarg)
{
        do
        {
                if (_DEBUG_) Serial.println("botTurnAroundRight(PI/2, 0.3);");
                elPadre.getAsservissement().botTurnAroundRight(PI/2, 0.3);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleRotationOrderEnd();
}

/**
 * \fn void testAsservissement_turnAroundLeft
 * \param m2_el_fnarg_p fnarg
 * \brief test de l'asservissement: rotation vers la gauche
 */
void testAsservissement_turnAroundLeft(m2_el_fnarg_p fnarg)
{
        do
        {
                if (_DEBUG_) Serial.println("botTurnAroundRight(PI/2, 0.3);");
                elPadre.getAsservissement().botTurnAroundLeft(PI/2, 0.3);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleRotationOrderEnd();
}

/**
 * \fn void testAsservissement
 * \param m2_el_fnarg_p fnarg
 * \brief test global de l'asservissement
 */
void testAsservissement(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) Serial.println("=== _ASSERVISSEMENT_ ===");
        testAsservissement_goStraightAhead(fnarg);
        testAsservissement_turnAroundRight(fnarg);
        testAsservissement_goBackward(fnarg);
}


// === Clamp ===
/**
 * \fn void testClampInitialisation
 * \param m2_el_fnarg_p fnarg
 * \brief test d'initialisation de la pince
 */
void testClampInitialisation(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) Serial.println("initialisation");
        elPadre.getClamp().initialisation();
        delay(9000); //pour laisser le temps à l'action de se réaliser
}

/**
 * \fn void testClampCatch
 * \param m2_el_fnarg_p fnarg
 * \brief test de la prise de cylindre de la pince
 */
void testClampCatch(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) Serial.println("catch");
        elPadre.getClamp().catchCylinder();
        delay(5000); //pour laisser le temps à l'action de se réaliser
}


/**
 * \fn void testClampBringUp
 * \param m2_el_fnarg_p fnarg
 * \brief test de la montee de cylindre de la pince
 */
void testClampBringUp(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) Serial.println("bring up");
        elPadre.getClamp().bringUpCylinder();
        delay(5000); //pour laisser le temps à l'action de se réaliser
}


/**
 * \fn void testClampRelease
 * \param m2_el_fnarg_p fnarg
 * \brief test du lachage de cylindre de la pince
 */
void testClampRelease(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) Serial.println("release");
        elPadre.getClamp().releaseCylinder();
        delay(8500); //pour laisser le temps à l'action de se réaliser
}

/**
 * \fn void testClampRelease
 * \param m2_el_fnarg_p fnarg
 * \brief test global de la pince: initialisation + attraper + relacher
 */
void testClamp(m2_el_fnarg_p fnarg)
{
        //Test pince
        if (_DEBUG_) Serial.println("\n===_TEST_CLAMP_===");
        testClampInitialisation(fnarg);
        testClampCatch(fnarg);
        testClampRelease(fnarg);
}


// === Sensors ===
/**
 * \fn void testSensors
 * \param m2_el_fnarg_p fnarg
 * \brief test de la réception de la valeur des capteurs
 */
void testSensors(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) Serial.println("\n===_TEST_SENSORS_===");
        elPadre.getSensorsBoard().updateAllSensorsValue();
        irBackValue = (uint32_t)elPadre.getSensorsBoard().getInfraredSensorBackValue();
        irFrontBottomLeftValue = (uint32_t)elPadre.getSensorsBoard().getInfraredSensorFrontBottomLeftValue();
        irFrontBottomCenterValue = (uint32_t)elPadre.getSensorsBoard().getInfraredSensorFrontBottomCenterValue();
        irFrontBottomRightValue = (uint32_t)elPadre.getSensorsBoard().getInfraredSensorFrontBottomRightValue();
        irFrontTopValue = (uint32_t)elPadre.getSensorsBoard().getInfraredSensorFrontTopValue();
        usBackValue = (uint32_t)elPadre.getSensorsBoard().getUltrasonicBackValue();
        usFrontValue = (uint32_t)elPadre.getSensorsBoard().getUltrasonicFrontValue();
        usLeftValue = (uint32_t)elPadre.getSensorsBoard().getUltrasonicLeftValue();
        usRightValue = (uint32_t)elPadre.getSensorsBoard().getUltrasonicRightValue();
        if (_DEBUG_)
        {
                Serial.print("\t getInfraredSensorBackValue : \t"); Serial.println(irBackValue);
                Serial.print("\t getInfraredSensorFrontBottomLeftValue : \t"); Serial.println(irFrontBottomLeftValue);
                Serial.print("\t getInfraredSensorFrontBottomCenterValue : \t"); Serial.println(irFrontBottomCenterValue);
                Serial.print("\t getInfraredSensorFrontBottomRightValue : \t"); Serial.println(irFrontBottomRightValue);
                Serial.print("\t getInfraredSensorFrontTopValue : \t"); Serial.println(irFrontTopValue);
                Serial.print("\t getUltrasonicBackValue : \t"); Serial.println(usBackValue);
                Serial.print("\t getUltrasonicFrontValue : \t"); Serial.println(usFrontValue);
                Serial.print("\t getUltrasonicLeftValue : \t"); Serial.println(usLeftValue);
                Serial.print("\t getUltrasonicRightValue : \t"); Serial.println(usRightValue);
                delay(500);
        }
}


// === Funny Action ===
/**
 * \fn void testFunnyAction
 * \param m2_el_fnarg_p fnarg
 * \brief test de lancement de la funny action
 */
void testFunnyAction(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) Serial.println("=== _TEST_FUNNY_ACTION_ ===");
        elPadre.startFunnyActionTimer();
}


// === Position ===
/**
 * \fn void testSensors
 * \param m2_el_fnarg_p fnarg
 * \brief fonction de mise à jour de la position du robot
 */
void update_angle_and_position(m2_el_fnarg_p fnarg)
{
        x_position = (uint32_t)elPadre.getAsservissement().getX_position();
        y_position = (uint32_t)elPadre.getAsservissement().getY_position();
        angle_position = (uint32_t)elPadre.getAsservissement().getAngle_position();
}


//_______________________________________________________________________________________________________
/**
 * \fn void startBotIfTiretteTiree()
 * \brief fonction qui lance la routine d'épreuve du robot si la tirette est tiree
 */
void startBotIfTiretteTiree()
{
        //Code final
        if (!epreuveFaite && elPadre.isTiretteTiree())
        {
                Wire.begin();
                elPadre.initializePosition();
                elPadre.buildBase();
                epreuveFaite = true;
        }
}




//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/* ======================================================================================================
 *      Ecran menus
 * ======================================================================================================
 */
/*=== strategie  ===*/
uint8_t rb_select_strat = 0;

/**
 * \fn void fn_start_robot(m2_el_fnarg_p fnarg)
 * \brief fonction qui lance le robot avec les paramètres sélctionnées par la stratégie
 * \param m2_el_fnarg_p fnarg
 */
void fn_start_robot(m2_el_fnarg_p fnarg)
{
        if (_DEBUG_) {
                Serial.print("Start robot avec la strategie ");
                Serial.println(rb_select_strat + 1);
        }
        switch (rb_select_strat) {
        case 0: //blue safe
                elPadre.setColorNumber(1);
                elPadre.setStrategyNumber(1);
                break;
        case 1: //yellow safe
                elPadre.setColorNumber(2);
                elPadre.setStrategyNumber(1);
                break;
        case 2: //blue risky
                elPadre.setColorNumber(1);
                elPadre.setStrategyNumber(2);
                break;
        default: //yellow risky
                elPadre.setColorNumber(2);
                elPadre.setStrategyNumber(2);
                break;
        }
        startBotIfTiretteTiree();
}


M2_LABEL(el_rb_label1, NULL, "blue safe");
M2_RADIO(el_rb_radio1, "v0", &rb_select_strat);

M2_LABEL(el_rb_label2, NULL, "yellow safe");
M2_RADIO(el_rb_radio2, "v1", &rb_select_strat);

M2_LABEL(el_rb_label3, NULL, "blue risky");
M2_RADIO(el_rb_radio3, "v2", &rb_select_strat);

M2_LABEL(el_rb_label4, NULL, "yellow risky");
M2_RADIO(el_rb_radio4, "v3", &rb_select_strat);

M2_ROOT(el_rb_goto_top, NULL, " Back ", &top_el_expandable_menu);
M2_BUTTON(el_start_robot, "f4", "Go", fn_start_robot);

M2_LIST(list_rb_strategie) =
{
        &el_rb_label1, &el_rb_radio1,
        &el_rb_label2, &el_rb_radio2,
        &el_rb_label3, &el_rb_radio3,
        &el_rb_label4, &el_rb_radio4,
        &el_rb_goto_top, &el_start_robot
};
M2_GRIDLIST(el_rb_grid_strategie, "c2",list_rb_strategie);
M2_ALIGN(el_top_rb_strategie, "-1|1W64H64", &el_rb_grid_strategie);


//____________________________________________________________________________________________________
/*=== Ecran de tests ===*/

uint32_t num_1 = 0;
uint32_t num_2 = 0;
byte* dataI2C = new byte[3];
byte valueI2C[2];
int recoveredValueI2C;


/**
 * \fn void fn_num_zero(m2_el_fnarg_p fnarg)
 * \brief Fonction de remise à zero des valeurs
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_zero(m2_el_fnarg_p fnarg) {
        num_1 = 0;
        num_2 = 0;
}

M2_BUTTON(el_num_zero, "f4", " zero ", fn_num_zero);
M2_ROOT(el_num_goto_top, "f4", " back ", &top_el_expandable_menu);

M2_LABEL(el_num_label1, NULL, "Variable:");
M2_U32NUM(el_num_1, "c3", &num_1);


// === Asservissement ===

M2_BUTTON(el_num_go_asservissement_straightAhead, "f4", "ahead", testAsservissement_goStraightAhead);
M2_BUTTON(el_num_go_asservissement_straightBackward, "f4", "backward", testAsservissement_goBackward);
M2_BUTTON(el_num_go_asservissement_turnLeft, "f4", "left", testAsservissement_turnAroundLeft);
M2_BUTTON(el_num_go_asservissement_turnRight, "f4", "right", testAsservissement_turnAroundRight);
M2_BUTTON(el_num_go_asservissement_global, "f4", " global ", testAsservissement);

M2_LIST(num_list_asservissement) =
{
        &el_num_go_asservissement_straightAhead, &el_num_go_asservissement_straightBackward,
        &el_num_go_asservissement_turnLeft, &el_num_go_asservissement_turnRight,
        &el_num_go_asservissement_global,  &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_asservissement, "c2", num_list_asservissement);
M2_ALIGN(el_top_num_menu_Asservissment, "-1|1W64H64", &el_num_menu_grid_asservissement);


// === Pince ===

M2_BUTTON(el_num_go_pince_init, "f4", "init", testClampInitialisation);
M2_BUTTON(el_num_go_pince_catch, "f4", "catch", testClampCatch);
M2_BUTTON(el_num_go_pince_bringup, "f4", "bringup", testClampBringUp);
M2_BUTTON(el_num_go_pince_release, "f4", "release", testClampRelease);
M2_BUTTON(el_num_go_pince, "f4", "global", testClamp);

M2_LIST(num_list_clamp) =
{
        &el_num_go_pince_init, &el_num_go_pince_catch,
        &el_num_go_pince_bringup, &el_num_go_pince_release,
        &el_num_go_pince, &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_clamp, "c2", num_list_clamp);
M2_ALIGN(el_top_num_menu_Clamp, "-1|1W64H64", &el_num_menu_grid_clamp);


// === Capteurs ===

M2_BUTTON(el_num_go_sensors, "f4", " read sensors ", testSensors);
M2_LIST(num_list_sensors) =
{
        &el_num_go_sensors,
        &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_sensors, "1", num_list_sensors);
M2_ALIGN(el_top_num_menu_Sensors, "-1|1W64H64", &el_num_menu_grid_sensors);


// === Funny Action ===

M2_BUTTON(el_num_go_funny_action, "f4", " launch funny action ", testFunnyAction);
M2_LIST(num_list_funny_action) =
{
        &el_num_go_funny_action,
        &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_funny_action, "c1", num_list_funny_action);
M2_ALIGN(el_top_num_menu_FunnyAction, "-1|1W64H64", &el_num_menu_grid_funny_action);


//____________________________________________________________________________________________________
/*=== Ecran de debug : liste ===*/

// === Capteurs Infrarouges ===

M2_LABEL(el_num_label_debug_irBackValue, NULL, "BT:");
M2_U32NUM(el_num_debug_el_num_label_debug_irBackValue, "c3r1", &irBackValue);

M2_LABEL(el_num_label_debug_irFrontBottomLeftValue, NULL, "FBL:");
M2_U32NUM(el_num_debug_el_num_label_debug_irFrontBottomLeftValue, "c3r1", &irFrontBottomLeftValue);

M2_LABEL(el_num_label_debug_irFrontBottomCenterValue, NULL, "FBC:");
M2_U32NUM(el_num_debug_el_num_label_debug_irFrontBottomCenterValue, "c3r1", &irFrontBottomCenterValue);

M2_LABEL(el_num_label_debug_irFrontBottomRightValue, NULL, "FBR:");
M2_U32NUM(el_num_debug_el_num_label_debug_irFrontBottomRightValue, "c3r1", &irFrontBottomRightValue);

M2_LABEL(el_num_label_debug_irFrontTopValue, NULL, "FT:");
M2_U32NUM(el_num_debug_el_num_label_debug_irFrontTopValue, "c3r1", &irFrontTopValue);

M2_BUTTON(el_num_update_irSensors, "f4", "maj", testSensors);

M2_LIST(num_list_debug_capteurs_ir) =
{
        &el_num_label_debug_irBackValue, &el_num_debug_el_num_label_debug_irBackValue
        , &el_num_label_debug_irFrontTopValue, &el_num_debug_el_num_label_debug_irFrontTopValue,
        &el_num_label_debug_irFrontBottomLeftValue, &el_num_debug_el_num_label_debug_irFrontBottomLeftValue
        , &el_num_label_debug_irFrontBottomRightValue, &el_num_debug_el_num_label_debug_irFrontBottomRightValue,
        &el_num_label_debug_irFrontBottomCenterValue, &el_num_debug_el_num_label_debug_irFrontBottomCenterValue,
        &el_num_goto_top, &el_num_update_irSensors
};

M2_GRIDLIST(el_num_list_debug_capteurs_ir, "c4", num_list_debug_capteurs_ir);
M2_ALIGN(el_top_num_menu_debug_capteurs_ir, "-1|1W64H64", &el_num_list_debug_capteurs_ir);


// === Capteurs Ultrasonores ===

M2_LABEL(el_num_label_debug_usBackValue, NULL, "Bck:");
M2_U32NUM(el_num_debug_el_num_label_debug_usBackValue, "c3r1", &usBackValue);

M2_LABEL(el_num_label_debug_usFrontValue, NULL, "Frt:");
M2_U32NUM(el_num_debug_el_num_label_debug_usFrontValue, "c3r1", &usFrontValue);

M2_LABEL(el_num_label_debug_usLeftValue, NULL, "Lft:");
M2_U32NUM(el_num_debug_el_num_label_debug_usLeftValue, "c3r1", &usLeftValue);

M2_LABEL(el_num_label_debug_usRightValue, NULL, "Rgt:");
M2_U32NUM(el_num_debug_el_num_label_debug_usRightValue, "c3r1", &usRightValue);

M2_BUTTON(el_num_update_usSensors, "f4", "maj", testSensors);

M2_LIST(num_list_debug_capteurs_us) =
{
        &el_num_label_debug_usBackValue, &el_num_debug_el_num_label_debug_usBackValue
        , &el_num_label_debug_usFrontValue, &el_num_debug_el_num_label_debug_usFrontValue,
        &el_num_label_debug_usLeftValue, &el_num_debug_el_num_label_debug_usLeftValue
        , &el_num_label_debug_usRightValue, &el_num_debug_el_num_label_debug_usRightValue,
        &el_num_goto_top, &el_num_update_usSensors
};

M2_GRIDLIST(el_num_list_debug_capteurs_us, "c4", num_list_debug_capteurs_us);
M2_ALIGN(el_top_num_menu_debug_capteurs_us, "-1|1W64H64", &el_num_list_debug_capteurs_us);


// === Position ===

M2_LABEL(el_num_label_debug_x_position, NULL, "X:");
M2_U32NUM(el_num_debug_x_position, "c3r1", &x_position);

M2_LABEL(el_num_label_debug_y_position, NULL, "Y:");
M2_U32NUM(el_num_debug_y_position, "c3r1", &y_position);

M2_LABEL(el_num_label_debug_angle_position, NULL, "Angle:");
M2_U32NUM(el_num_debug_angle_position, "c3r1", &angle_position);

M2_BUTTON(el_num_update_angle_and_position, "f4", "maj", update_angle_and_position);

M2_LIST(num_list_debug_position) =
{
        &el_num_label_debug_x_position, &el_num_debug_x_position,
        &el_num_label_debug_y_position, &el_num_debug_y_position,
        &el_num_label_debug_angle_position, &el_num_debug_angle_position,
        &el_num_goto_top, &el_num_update_angle_and_position
};

M2_GRIDLIST(el_num_list_debug_position, "c2", num_list_debug_position);
M2_ALIGN(el_top_num_menu_debug_position, "-1|1W64H64", &el_num_list_debug_position);


//____________________________________________________________________________________________________
// Left entry: Menu name. Submenus must have a '.' at the beginning
// Right entry: Reference to the target dialog box (In this example all menus call the toplevel element again
m2_menu_entry m2_2lmenu_data[] =
{
        { "Strategie", &el_rb_grid_strategie },
        { "Tests robots", NULL },
        { ". Test Asserv", &el_top_num_menu_Asservissment},
        { ". Test Pince", &el_top_num_menu_Clamp},
        { ". Test Capteurs", &el_top_num_menu_Sensors},
        { ". Test Fun Action", &el_top_num_menu_FunnyAction},
        { "Debug ", NULL },
        { ". Capteurs IR", &el_top_num_menu_debug_capteurs_ir},
        { ". Capteurs US", &el_top_num_menu_debug_capteurs_us},
        { ". Position", &el_top_num_menu_debug_position},
        { NULL, NULL },
};

// The first visible line and the total number of visible lines.
// Both values are written by M2_2LMENU and read by M2_VSB
uint8_t m2_2lmenu_first;
uint8_t m2_2lmenu_cnt;

// M2_2LMENU definition
// Option l4 = four visible lines
// Option e15 = first column has a width of 15 pixel
// Option W43 = second column has a width of 43/64 of the display width
M2_2LMENU(el_2lmenu,"l4F3e15W43",&m2_2lmenu_first,&m2_2lmenu_cnt, m2_2lmenu_data,65,102,'\0');
M2_SPACE(el_space, "W1h1");
M2_VSB(el_vsb, "l4W2r1", &m2_2lmenu_first, &m2_2lmenu_cnt);
M2_LIST(list_2lmenu) = { &el_2lmenu, &el_space, &el_vsb };
M2_HLIST(el_hlist, NULL, list_2lmenu);
M2_ALIGN(top_el_expandable_menu, "-1|1W64H64", &el_hlist);

M2tk m2(&top_el_expandable_menu, m2_es_arduino, m2_eh_4bs, m2_gh_u8g_ffs);


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/* ======================================================================================================
 *      Draw procedure, Arduino Setup & Loop
 * ======================================================================================================
 */

void stopAll()
{
        elPadre.getAsservissement().botStop();
}

/**
 * \fn void draw()
 * \brief Draw procedure
 */
void draw(void)
{
        m2.draw();
}


/**
 * \fn void setup()
 * \brief Fonction setup d'arduino: initialisation de l'encodeur
 */
void setup(void)
{
        /* ECRAN */
        m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon); // Connect u8glib with m2tklib

        m2.setFont(0, u8g_font_6x13r); // Assign u8g font to index 0

        m2.setFont(3, u8g_font_m2icon_7); // Assign icon font to index 3

        // Setup keys
        m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
        m2.setPin(M2_KEY_PREV, uiKeyUpPin);
        m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
        m2.setPin(M2_KEY_EXIT, uiKeyExitPin);

        mas_Init(mas_device_sim, 0); // mass storage init: simulation environment

        /* I2C */
        Wire.begin();              // join i2c bus (address optional for master)

        /* AUTRES */
        attachInterrupt(_PIN_BOUTON_ARRET_URGENCE_, stopAll, RISING);    // Interruption sur appui du bouton d'arret d'urgence
        epreuveFaite = false;

        Serial.begin(115200);        // starts the serial communication
}


/**
 * \fn void loop()
 * \brief Fonction loop d'arduino: rafraichissement de l'écran
 */
void loop()
{
        // menu management
        m2.checkKey();
        if ( m2.handleKey() != 0 )
        {
                u8g.firstPage();
                do {
                        m2.checkKey();
                        draw();
                } while( u8g.nextPage() );
        }
}
