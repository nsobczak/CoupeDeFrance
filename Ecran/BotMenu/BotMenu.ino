/**
 *    \file BotMenu.cpp
 *    \brief Code d'affichage de l'écran
 *    \author Nicolas Sobczak
 *    \date octobre 2016
*/
//____________________________________________________________________________________________________
// Ecran
#include "U8glib.h"
#include "M2tk.h"
#include "utility/m2ghu8g.h"
#include "utility/mas.h"

// setup u8g object
U8GLIB_ST7920_128X64_1X u8g(13, 51, 14);  // SPI Com: SCK = en = 13, MOSI = rw = 51, CS = di = 14

// I2C
#include <Wire.h>
#include "i2cCommunication.h"


//____________________________________________________________________________________________________
#define _SENDADRESS_01_ 8
#define _RECEIVEADRESS_01_ 9


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


//____________________________________________________________________________________________________
//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_el_expandable_menu);


//____________________________________________________________________________________________________
/*=== radio button selection  ===*/
uint8_t rb_select_strat = 0;

/**
 * \fn void fn_start_robot(m2_el_fnarg_p fnarg) 
 * \brief fonction qui lance le robot avec les paramètres sélctionnées par la stratégie
 * \param 
 */
void fn_start_robot(m2_el_fnarg_p fnarg) {
  Serial.print("Start robot avec la strategie ");
  Serial.println(rb_select_strat + 1);
}

M2_LABEL(el_rb_label1, NULL, "strategie 1");
M2_RADIO(el_rb_radio1, "v0", &rb_select_strat);

M2_LABEL(el_rb_label2, NULL, "strategie 2");
M2_RADIO(el_rb_radio2, "v1", &rb_select_strat);

M2_LABEL(el_rb_label3, NULL, "strategie 3");
M2_RADIO(el_rb_radio3, "v2", &rb_select_strat);

M2_LABEL(el_rb_label4, NULL, "strategie 4");
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
/*=== combo examples  ===*/

uint8_t select_coord = 0;
uint8_t select_priority = 0;


void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
  m2_SetRoot(&top_el_expandable_menu);
}


void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
  m2_SetRoot(&top_el_expandable_menu);
}


const char *fn_idx_to_color(uint8_t idx)
{
  switch(idx)
  {
    case 0: return "a";
    case 1: return "b";
    case 2: return "c";
  }
  return "";
}


const char *fn_idx_to_priority(uint8_t idx)
{
  switch(idx)
  {
    case 0: return "lowest";
    case 1: return "low";
    case 2: return "medium";
    case 3: return "high";
    case 4: return "highest";
  }
  return "";
}


M2_LABEL(el_label1, NULL, "Init:");
M2_COMBO(el_combo1, NULL, &select_coord, 2, fn_idx_to_color);

M2_LABEL(el_label2, NULL, "Priority: ");
M2_COMBO(el_combo2, "v1", &select_priority, 5, fn_idx_to_priority);

M2_BUTTON(el_cancel, "f4", "cancel", fn_cancel);
M2_BUTTON(el_ok, "f4", "ok", fn_ok);

M2_LIST(list_combo) = { 
    &el_label1, &el_combo1, 
    &el_label2, &el_combo2,  
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_combo_grid, "c2", list_combo);
M2_ALIGN(el_top_combo, "-1|1W64H64", &el_combo_grid);


//____________________________________________________________________________________________________
/*=== number entry ===*/

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

/**
 * \fn void fn_num_go(m2_el_fnarg_p fnarg)
 * \brief Fonction de test i2c
 * \param 
 */
void fn_num_go(m2_el_fnarg_p fnarg) {
  // request 3 bytes from slave device on adress 9
  dataI2C = i2crequest(_RECEIVEADRESS_01_, 3, num_1, 6);       
    
  valueI2C[0] = dataI2C [1];
  valueI2C[1] = dataI2C [2];
  recoveredValueI2C = recoverIntFrom2Bytes(valueI2C);
  Serial.print("\nrecovery : ");  
  Serial.print(num_1);
  Serial.print(" = ");  
  Serial.println(recoveredValueI2C);
  
  Serial.println(); 
}

/*
M2_LABEL(el_num_label1, NULL, "Variable");
M2_U8NUM(el_num_1, NULL, 0, 255, &u8num);
*/
M2_LABEL(el_num_label1, NULL, "Variable:");
M2_U32NUM(el_num_1, "c3", &num_1);

/*
M2_LABEL(el_num_label2, NULL, "Valeur:");
M2_U32NUM(el_num_2, "c5", &num_2);
*/
M2_BUTTON(el_num_zero, "f4", " zero ", fn_num_zero);
M2_BUTTON(el_num_go, "f4", " I2C test ", fn_num_go);
M2_ROOT(el_num_goto_top, "f4", " back ", &top_el_expandable_menu);

M2_LIST(num_list) = { 
    &el_num_label1, &el_num_1, 
    //&el_num_label2, &el_num_2,  
    &el_num_zero, &el_num_go,
    &el_num_goto_top
};
M2_GRIDLIST(el_num_menu_grid, "c2", num_list);
M2_ALIGN(el_top_num_menu, "-1|1W64H64", &el_num_menu_grid);


//____________________________________________________________________________________________________
/*=== multi selection ===*/

#define MULTI_SELECT_CNT 3
const char *multi_select_strings[MULTI_SELECT_CNT] = { "value01", "value02", "value03" };
uint8_t multi_select_status[MULTI_SELECT_CNT] = { 0, 0, 0};

uint8_t el_muse_first = 0;
uint8_t el_muse_cnt = MULTI_SELECT_CNT;

const char *el_muse_strlist_cb(uint8_t idx, uint8_t msg) {
  const char *s = "";
  if ( msg == M2_STRLIST_MSG_SELECT ) {
    if ( multi_select_status[idx] == 0 ) {
      multi_select_status[idx] = 1;
    }
    else {
      multi_select_status[idx] = 0;
    }
  }
  if ( msg == M2_STRLIST_MSG_GET_STR ) {
    s = multi_select_strings[idx];
  }
  if ( msg == M2_STRLIST_MSG_GET_EXTENDED_STR ) {
    if ( multi_select_status[idx] == 0 ) {
      s = " ";
    }
    else {
      s = "*";
    }
  }
  return s;  
}

M2_STRLIST(el_muse_strlist, "l3F0E10W46", &el_muse_first, &el_muse_cnt, el_muse_strlist_cb);
M2_ROOT(el_muse_goto_top, "f4", "Goto Top Menu", &top_el_expandable_menu);

M2_LIST(muse_list) = { 
    &el_muse_strlist, 
    &el_muse_goto_top,  
};
M2_VLIST(el_muse_vlist, "c2", muse_list);
M2_ALIGN(top_el_muse, "-1|1W64H64", &el_muse_vlist);


//____________________________________________________________________________________________________
/*=== File selection dialog ===*/

/* defines the number of additional buttons at the beginning of the STRLIST lines */
#define FS_EXTRA_MENUES 1

/* helper variables for the strlist element */
uint8_t fs_m2tk_first = 0;
uint8_t fs_m2tk_cnt = 0;

const char *fs_strlist_getstr(uint8_t idx, uint8_t msg)  {
  if (msg == M2_STRLIST_MSG_GET_STR)  {
    /* Check for the extra button: Return string for this extra button */
    if ( idx == 0 )
      return "..";
    /* Not the extra button: Return file/directory name */
    mas_GetDirEntry(idx - FS_EXTRA_MENUES);
    return mas_GetFilename();
  } else if ( msg == M2_STRLIST_MSG_GET_EXTENDED_STR ) {
    /* Check for the extra button: Return icon for this extra button */
    if ( idx == 0 )
      return "a";       /* arrow left of the m2icon font */
    /* Not the extra button: Return file or directory icon */
    mas_GetDirEntry(idx - FS_EXTRA_MENUES);
    if ( mas_IsDir() )
      return "A";       /* folder icon of the m2icon font */
    return "B";         /* file icon of the m2icon font */
  } else if ( msg == M2_STRLIST_MSG_SELECT ) {
    /* Check for the extra button: Execute button action */
    if ( idx == 0 ) {
      if ( mas_GetPath()[0] == '\0' )
        m2_SetRoot(&top_el_expandable_menu);      
      else {
        mas_ChDirUp();
        m2_SetRoot(m2_GetRoot());  /* reset menu to first element, send NEW_DIALOG and force recount */
      }
    /* Not the extra button: Goto subdir or return (with selected file) */
    } else {
      mas_GetDirEntry(idx - FS_EXTRA_MENUES);
      if ( mas_IsDir() ) {
        mas_ChDir(mas_GetFilename());
        m2_SetRoot(m2_GetRoot());  /* reset menu to first element, send NEW_DIALOG and force recount */
      } else {
	/* File has been selected. Here: Show the file to the user, but here, we just jump back to the main menu */
        m2_SetRoot(&top_el_expandable_menu);  
      }
    }
  } else if ( msg == M2_STRLIST_MSG_NEW_DIALOG ) {
    /* (re-) calculate number of entries, limit no of entries to 250 */
    if ( mas_GetDirEntryCnt() < 250-FS_EXTRA_MENUES )
      fs_m2tk_cnt = mas_GetDirEntryCnt()+FS_EXTRA_MENUES;
    else
      fs_m2tk_cnt = 250;
  }
  return NULL;
}

M2_STRLIST(el_fs_strlist, "l5F3e15W49", &fs_m2tk_first, &fs_m2tk_cnt, fs_strlist_getstr);
M2_SPACE(el_fs_space, "W1h1");
M2_VSB(el_fs_strlist_vsb, "l5W4r1", &fs_m2tk_first, &fs_m2tk_cnt);
M2_LIST(list_fs_strlist) = { &el_fs_strlist, &el_fs_space, &el_fs_strlist_vsb };
M2_HLIST(el_fs_hlist, NULL, list_fs_strlist);
M2_ALIGN(el_top_fs, "-1|1W64H64", &el_fs_hlist);


// Left entry: Menu name. Submenus must have a '.' at the beginning
// Right entry: Reference to the target dialog box (In this example all menus call the toplevel element again
m2_menu_entry m2_2lmenu_data[] = 
{
  { "Epreuve", NULL },
  { ". Strategie", &el_rb_grid },
  { ". Initialisation", &el_top_combo },
  { "Tests", NULL },
  { ". Test I2C", &el_top_num_menu },
  { ". Test02", &top_el_muse },
  { ". Test03", &top_el_muse },
  //{ ". File Select", &el_top_fs },
  { "Debug ", &top_el_muse },
  { "Top", &top_el_expandable_menu },
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

// m2 object and constructor
M2tk m2(&top_el_expandable_menu, m2_es_arduino, m2_eh_4bs, m2_gh_u8g_ffs);
//M2tk m2(&top_el_expandable_menu, m2_es_arduino_rotary_encoder, m2_eh_4bs, m2_gh_u8g_ffs);
//M2tk m2(&top_el_expandable_menu, m2_es_arduino, m2_eh_4bs, m2_gh_arduino_serial);


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
 * \brief Fonction setup d'arduino
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
  
  /*
  m2.setPin(M2_KEY_SELECT, 7);
  m2.setPin(M2_KEY_ROT_ENC_A, 3);
  m2.setPin(M2_KEY_ROT_ENC_B, 4);
  */
  
  /* mass storage init: simulation environment */
  mas_Init(mas_device_sim, 0);
  
  /* I2C */
  Wire.begin();                    // join i2c bus (address optional for master)
  
  Serial.begin(9600);              // starts the serial communication 
}


/**
 * \fn void loop() 
 * \brief Fonction loop d'arduino
 */
void loop() {
  // menu management
  m2.checkKey();
  if ( m2.handleKey() != 0 ) {
    u8g.firstPage();  
    do {
      m2.checkKey();
      draw();
    } while( u8g.nextPage() );
  }
}

