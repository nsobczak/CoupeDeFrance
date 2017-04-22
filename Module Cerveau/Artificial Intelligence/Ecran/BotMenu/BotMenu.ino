/**
 *    \file BotMenu.cpp
 *    \brief Code d'affichage de l'écran
 *    \author Nicolas Sobczak
 *    \date octobre 2016
 */
//____________________________________________________________________________________________________
#include <Arduino.h>

// Ecran
#include <U8glib.h>
#include <M2tk.h>
#include "utility/m2ghu8g.h"
#include "utility/mas.h"

// setup u8g object
U8GLIB_ST7920_128X64_1X u8g(13, 51, 14);  // SPI Com: SCK = en = 13, MOSI = rw = 51, CS = di = 14  //(53, 52, 13);

#define _DEBUG_ true

// I2C
#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 12
#define _SENDADRESS_ 13


//____________________________________________________________________________________________________
//=================================================
// Variables for Menu Process

/**
 *   \brief Declaration de l'encodeur (le bouton)
 *   const int entry = 2;
 *   const int encoderMoins = 52;
 *   const int encoderPlus = 50;
 */
uint8_t uiKeyUpPin = 50;
uint8_t uiKeyDownPin = 52;
uint8_t uiKeySelectPin = 2;
uint8_t uiKeyExitPin = 0;

// uint8_t uiKeyUpPin = 11;
// uint8_t uiKeyDownPin = 10;
// uint8_t uiKeySelectPin = 8;


//____________________________________________________________________________________________________
//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_el_expandable_menu);


//____________________________________________________________________________________________________
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
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, rb_select_strat + 1);
        // envoi
        i2csend3bytes(0, bytesTab[0], bytesTab[1], _SENDADRESS_); // Envoi de rb_select_strat (0 dans le tableau de correspondance)

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

M2_LIST(list_rb) = {
        &el_rb_label1, &el_rb_radio1,
        &el_rb_label2, &el_rb_radio2,
        &el_rb_label3, &el_rb_radio3,
        &el_rb_label4, &el_rb_radio4,
        &el_rb_goto_top, &el_start_robot
};
M2_GRIDLIST(el_rb_grid, "c2",list_rb);
M2_ALIGN(el_top_rb, "-1|1W64H64", &el_rb_grid);


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


// === I2C ===
/**
 * \fn void fn_num_go_i2c(m2_el_fnarg_p fnarg)
 * \brief Fonction de test i2c
 * \param m2_el_fnarg_p fnarg
 */
// void fn_num_go_i2c(m2_el_fnarg_p fnarg) {
//         // request 3 bytes from slave device on adress 9
//         dataI2C = i2crequest(_RECEIVEADRESS_, 3, num_1, 6);
//
//         valueI2C[0] = dataI2C [1];
//         valueI2C[1] = dataI2C [2];
//         recoveredValueI2C = recoverIntFrom2Bytes(valueI2C);
//         if (_DEBUG_) {
//                 Serial.print("\nrecovery : ");
//                 Serial.print(num_1);
//                 Serial.print(" = ");
//                 Serial.println(recoveredValueI2C);
//                 Serial.println();
//         }
// }

M2_BUTTON(el_num_zero, "f4", " zero ", fn_num_zero);
// M2_BUTTON(el_num_go_i2c, "f4", " I2C test ", fn_num_go_i2c);
M2_ROOT(el_num_goto_top, "f4", " back ", &top_el_expandable_menu);

M2_LABEL(el_num_label1, NULL, "Variable:");
M2_U32NUM(el_num_1, "c3", &num_1);

// M2_LIST(num_list_I2C) = {
//         &el_num_label1, &el_num_1,
//         &el_num_zero, &el_num_go_i2c,
//         &el_num_goto_top
// };
// M2_GRIDLIST(el_num_menu_grid_I2C, "c2", num_list_I2C);
// M2_ALIGN(el_top_num_menu, "-1|1W64H64", &el_num_menu_grid_I2C);


// === Asservissement ===
/**
 * \fn void fn_num_go_asservissement_straightAhead(m2_el_fnarg_p fnarg)
 * \brief
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_asservissement_straightAhead(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("straightAhead");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(6, bytesTab[0], bytesTab[1], _SENDADRESS_);
}

/**
 * \fn void fn_num_go_asservissement_straightBackward(m2_el_fnarg_p fnarg)
 * \brief
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_asservissement_straightBackward(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("straightBackward");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(6, bytesTab[0], bytesTab[1], _SENDADRESS_);
}

/**
 * \fn void fn_num_go_asservissement_turnLeft(m2_el_fnarg_p fnarg)
 * \brief
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_asservissement_turnLeft(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("turnLeft");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(6, bytesTab[0], bytesTab[1], _SENDADRESS_);
}

/**
 * \fn void fn_num_go_asservissement_turnRight(m2_el_fnarg_p fnarg)
 * \brief
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_asservissement_turnRight(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("turnRight");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(6, bytesTab[0], bytesTab[1], _SENDADRESS_);
}

/**
 * \fn void fn_num_go_asservissement_global(m2_el_fnarg_p fnarg)
 * \brief
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_asservissement_global(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("turnRight");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(6, bytesTab[0], bytesTab[1], _SENDADRESS_);
}


M2_BUTTON(el_num_go_asservissement_straightAhead, "f4", "ahead", fn_num_go_asservissement_straightAhead);
M2_BUTTON(el_num_go_asservissement_straightBackward, "f4", "backward", fn_num_go_asservissement_straightBackward);
M2_BUTTON(el_num_go_asservissement_turnLeft, "f4", "left", fn_num_go_asservissement_turnLeft);
M2_BUTTON(el_num_go_asservissement_turnRight, "f4", "right", fn_num_go_asservissement_turnRight);
M2_BUTTON(el_num_go_asservissement_global, "f4", " global ", fn_num_go_asservissement_global);

M2_LIST(num_list_asservissement) = {
        &el_num_go_asservissement_straightAhead, &el_num_go_asservissement_straightBackward,
        &el_num_go_asservissement_turnLeft, &el_num_go_asservissement_turnRight,
        &el_num_go_asservissement_global,  &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_asservissement, "c2", num_list_asservissement);
M2_ALIGN(el_top_num_menu_Asservissment, "-1|1W64H64", &el_num_menu_grid_asservissement);


// === Pince ===
/**
 * \fn void fn_num_go_pince_initialisation(m2_el_fnarg_p fnarg)
 * \brief Fonction de test de la pince avec l'i2c
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_pince_initialisation(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("initialisation de la pince");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(6, bytesTab[0], bytesTab[1], _SENDADRESS_);
}


/**
 * \fn void fn_num_go_pince_catch(m2_el_fnarg_p fnarg)
 * \brief Fonction de test de la pince avec l'i2c
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_pince_catch(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("pince catch");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(7, bytesTab[0], bytesTab[1], _SENDADRESS_);
}


/**
 * \fn void fn_num_go_pince_release(m2_el_fnarg_p fnarg)
 * \brief Fonction de test de la pince avec l'i2c
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_pince_release(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("pince release");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(8, bytesTab[0], bytesTab[1], _SENDADRESS_);
}


/**
 * \fn void fn_num_go_pince(m2_el_fnarg_p fnarg)
 * \brief Fonction de test de la pince avec l'i2c
 * \param m2_el_fnarg_p fnarg
 */
void fn_num_go_pince(m2_el_fnarg_p fnarg) {
        // démarrage de la pince
        if (_DEBUG_) Serial.println("demarrage de la pince");
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        // envoi
        i2csend3bytes(9, bytesTab[0], bytesTab[1], _SENDADRESS_);
}

M2_BUTTON(el_num_go_pince_init, "f4", "init", fn_num_go_pince);
M2_BUTTON(el_num_go_pince_catch, "f4", "catch", fn_num_go_pince);
M2_BUTTON(el_num_go_pince_release, "f4", "release", fn_num_go_pince);
M2_BUTTON(el_num_go_pince, "f4", "global", fn_num_go_pince);

M2_LIST(num_list_clamp) = {
        &el_num_go_pince_init, &el_num_go_pince_catch,
        &el_num_go_pince_release, &el_num_go_pince,
        &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_clamp, "c2", num_list_clamp);
M2_ALIGN(el_top_num_menu_Clamp, "-1|1W64H64", &el_num_menu_grid_clamp);


// === Capteurs ===
M2_BUTTON(el_num_go_sensors, "f4", " read sensors ", fn_num_go_pince);
M2_LIST(num_list_sensors) = {
        &el_num_go_sensors,
        &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_sensors, "1", num_list_sensors);
M2_ALIGN(el_top_num_menu_Sensors, "-1|1W64H64", &el_num_menu_grid_sensors);


// === Funny Action ===

M2_BUTTON(el_num_go_funny_action, "f4", " launch funny action ", fn_num_go_pince);
M2_LIST(num_list_funny_action) = {
        &el_num_go_funny_action,
        &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid_funny_action, "c1", num_list_funny_action);
M2_ALIGN(el_top_num_menu_FunnyAction, "-1|1W64H64", &el_num_menu_grid_funny_action);


//____________________________________________________________________________________________________
/*=== Ecran de debug : liste ===*/
uint32_t global_value_01 = 245;
uint32_t global_value_02 = 1;

M2_LABEL(el_num_label_debug_01, NULL, "Var_1:");
M2_U32NUM(el_num_debug_01, "c3r1", &global_value_01);

M2_LABEL(el_num_label_debug_02, NULL, "Var_2:");
M2_U32NUM(el_num_debug_02, "c3r1", &global_value_02);


/**
 * \fn void fn_debug_update(m2_el_fnarg_p fnarg)
 * \brief Fonction de test d'actualisation des valeurs
 * \param m2_el_fnarg_p fnarg
 */
void fn_debug_update(m2_el_fnarg_p fnarg)
{
        if (global_value_02 != 10)
        {
                while (global_value_02 < 10)
                {
                        global_value_02 += 1;
                }
        }
        else
        {
                global_value_02 = 1;
        }
}

M2_BUTTON(el_debug_update, "f4", "MAJ", fn_debug_update);


// === Capteurs Infrarouges ===

M2_LIST(num_list_debug_capteurs_ir) = {
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_goto_top, &el_debug_update
};

M2_GRIDLIST(el_num_list_debug_capteurs_ir, "c4", num_list_debug_capteurs_ir);
M2_ALIGN(el_top_num_menu_debug_capteurs_ir, "-1|1W64H64", &el_num_list_debug_capteurs_ir);


// === Capteurs Ultrasonores ===

M2_LIST(num_list_debug_capteurs_us) = {
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_goto_top, &el_debug_update
};

M2_GRIDLIST(el_num_list_debug_capteurs_us, "c4", num_list_debug_capteurs_us);
M2_ALIGN(el_top_num_menu_debug_capteurs_us, "-1|1W64H64", &el_num_list_debug_capteurs_us);


// === Position ===

M2_LIST(num_list_debug_position) = {
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_label_debug_01, &el_num_debug_01, &el_num_label_debug_02, &el_num_debug_02,
        &el_num_goto_top, &el_debug_update
};

M2_GRIDLIST(el_num_list_debug_position, "c4", num_list_debug_position);
M2_ALIGN(el_top_num_menu_debug_position, "-1|1W64H64", &el_num_list_debug_position);


//____________________________________________________________________________________________________
// Left entry: Menu name. Submenus must have a '.' at the beginning
// Right entry: Reference to the target dialog box (In this example all menus call the toplevel element again
m2_menu_entry m2_2lmenu_data[] =
{
        { "Epreuve", NULL },
        { ". Strategie", &el_rb_grid },
        { "Tests robots", NULL },
        { ". Test Asserv", &el_top_num_menu_Asservissment},
        { ". Test Pince", &el_top_num_menu_Clamp},
        { ". Test Capteurs", &el_top_num_menu_Sensors},
        { ". Test Fun Action", &el_top_num_menu_FunnyAction},
        // { ". Test I2C", &el_top_num_menu },
        { "Debug ", NULL },
        { ". Capteurs IR", &el_top_num_menu_debug_capteurs_ir},
        { ". Capteurs US", &el_top_num_menu_debug_capteurs_us},
        { ". Position", &el_top_num_menu_debug_position},
        //{ "Top", &top_el_expandable_menu },
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
//=================================================
// Draw procedure, Arduino Setup & Loop

/**
 * \fn void draw()
 * \brief Draw procedure
 */
void draw(void) {
        m2.draw();
}


/**
 * \fn void setup()
 * \brief Fonction setup d'arduino: initialisation de l'encodeur
 */
void setup(void) {
        // Connect u8glib with m2tklib
        m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

        // Assign u8g font to index 0
        m2.setFont(0, u8g_font_6x13r);

        // Assign icon font to index 3
        m2.setFont(3, u8g_font_m2icon_7);

        // Setup keys
        m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
        m2.setPin(M2_KEY_PREV, uiKeyUpPin);
        m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
        m2.setPin(M2_KEY_EXIT, uiKeyExitPin);

        /* mass storage init: simulation environment */
        mas_Init(mas_device_sim, 0);

        /* I2C */
        Wire.begin();              // join i2c bus (address optional for master)

        Serial.begin(9600);        // starts the serial communication
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
