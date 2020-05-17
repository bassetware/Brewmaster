#define MENU_ENTRIES 7
#define MENU_BUTTONS 4
#include "OneButton.h"

OneButton button0(B1, true);
OneButton button1(B2, true);
OneButton button2(B3, true);
OneButton button3(B4, true);

typedef struct menu_timeout {
    long last_button; //when was the last button button1_pressed
    long timeout_delay; //how long until timeout
    boolean timout_active;
    boolean is_timeout;
};

menu_timeout timeout_menu;
void (*button_press[MENU_BUTTONS])(char[]);
void (*button_long_press[MENU_BUTTONS])(char[]);


typedef struct menu_entry {
    void (*button_pressed[MENU_BUTTONS])(char[]);  /* data */
    void (*button_long_pressed[MENU_BUTTONS])(char[]);
    //void (*button1_pressed)();
    //void (*button2_pressed)();
    //void (*button3_pressed)();
    char argument[MENU_BUTTONS][16];
    char argument_longpress[MENU_BUTTONS][16];
    char caption[16];
    void (*menu_animation)(char[]);
    void (*init_animation)();
    char animation_argument[16];
} ;


menu_entry menu[MENU_ENTRIES];

int active_entry;

//Load and show a new entry... scroll down from top
void LoadMenuById(char argument[]) {
    int id_num = atoi(argument);
    Serial.println(menu[id_num].caption);
    Serial.println(menu[active_entry].caption);
    char buffer_new[16];
    char buffer_old[16];
    timeout_menu.is_timeout = false;
    timeout_menu.last_button = millis();

    active_entry = id_num;
        if(menu[active_entry].init_animation != NULL) {
        menu[active_entry].init_animation();
    }
} 

//LoadMenuById and LoadSubMenuById are basically the same as they all refer to the menu_entry array, the scrolling effect is a different type
void LoadSubMenuById(char argument[]) {
    int id_num = atoi(argument);
    
    Serial.println(menu[id_num].caption);
    Serial.println(menu[active_entry].caption);
    char buffer_new[16];
    char buffer_old[16];
    timeout_menu.is_timeout = false;
    timeout_menu.last_button = millis();

    active_entry = id_num;
        if(menu[active_entry].init_animation != NULL) {
        menu[active_entry].init_animation();
    }
} 

void menu_tick() {
  button0.tick();
  button1.tick();
  button2.tick();
  button3.tick();
}



//set active menu entry
void set_active_entry(int entry) {
    active_entry = entry;
} 

int get_active_entry() {
    return active_entry;
}

void activate_timeout() {
    timeout_menu.timout_active = true;
    timeout_menu.last_button = millis();
}

void deactivate_timeout() {
    timeout_menu.timout_active = false;
}

boolean toggle_timeout() {
    timeout_menu.timout_active = !timeout_menu.timout_active;
    timeout_menu.last_button = millis();
    return timeout_menu.timout_active;
}

boolean get_timeout_status() {
    return timeout_menu.timout_active;
}

void menu_timeout(char argument[]) {
 return;
}

void menu_animation() {
    if(menu[active_entry].menu_animation == NULL) {return;}
    if(millis() - timeout_menu.last_button > timeout_menu.timeout_delay && timeout_menu.timout_active && !timeout_menu.is_timeout) {
        LoadMenuById("0");
        timeout_menu.is_timeout = true;
        return;
    }
    menu[active_entry].menu_animation(menu[active_entry].caption);

}



//evaluate the buttons
void button0_clicked() {
    if(menu[active_entry].button_pressed[0] == NULL) {return;}
    menu[active_entry].button_pressed[0](menu[active_entry].argument[0]);
    timeout_menu.last_button = millis();
}
void button1_clicked() {
    if(menu[active_entry].button_pressed[1] == NULL) {return;}
    menu[active_entry].button_pressed[1](menu[active_entry].argument[1]);
    timeout_menu.last_button = millis();
}
void button2_clicked() {
    if(menu[active_entry].button_pressed[2] == NULL) {return;}
    menu[active_entry].button_pressed[2](menu[active_entry].argument[2]);
    timeout_menu.last_button = millis();
}
void button3_clicked() {
    if(menu[active_entry].button_pressed[3] == NULL) {return;}
    menu[active_entry].button_pressed[3](menu[active_entry].argument[3]);
    timeout_menu.last_button = millis();
}

void button0_long_clicked() {
    if(menu[active_entry].button_long_pressed[0] == NULL) {return;}
    menu[active_entry].button_long_pressed[0](menu[active_entry].argument_longpress[0]);
    timeout_menu.last_button = millis();
}

void button1_long_clicked() {
    if(menu[active_entry].button_long_pressed[1] == NULL) {return;}
    menu[active_entry].button_long_pressed[1](menu[active_entry].argument_longpress[1]);
    timeout_menu.last_button = millis();
}

void button2_long_clicked() {
    if(menu[active_entry].button_long_pressed[2] == NULL) {return;}
    menu[active_entry].button_long_pressed[2](menu[active_entry].argument_longpress[2]);
    timeout_menu.last_button = millis();
}

void button3_long_clicked() {
    if(menu[active_entry].button_long_pressed[3] == NULL) {return;}
    menu[active_entry].button_long_pressed[3](menu[active_entry].argument_longpress[3]);
    timeout_menu.last_button = millis();
}

//init the menu
#include "menu_funcs.h"

void init_menu() {
    //first init the buttons
    button0.attachClick(button0_clicked);
    button1.attachClick(button1_clicked);
    button2.attachClick(button2_clicked);
    button3.attachClick(button3_clicked);

    button0.attachLongPressStart(button0_long_clicked);
    button1.attachLongPressStart(button1_long_clicked);
    button2.attachLongPressStart(button2_long_clicked);
    button3.attachLongPressStart(button3_long_clicked);

    //Init timeout
    timeout_menu.last_button = millis();
    timeout_menu.timeout_delay = 10000; //delay in millis
    timeout_menu.timout_active = false;
    timeout_menu.is_timeout = false;

    //Menu 0 --> Loading Screen.
    menu[0].button_pressed[0] = NULL;
    strcpy(menu[0].argument[0], "1");
    menu[0].button_pressed[1]= NULL;
    strcpy(menu[0].argument[1], "1");
    menu[0].button_pressed[2] = NULL;
    strcpy(menu[0].argument[2], "1");
    menu[0].button_pressed[3] = NULL;
    strcpy(menu[0].argument[3], "1");

    menu[0].button_long_pressed[0] = NULL;
    strcpy(menu[0].argument_longpress[0], "1");
    menu[0].button_long_pressed[1]= NULL;
    strcpy(menu[0].argument_longpress[1], "1");
    menu[0].button_long_pressed[2] = NULL;
    strcpy(menu[0].argument_longpress[2], "1");
    menu[0].button_long_pressed[3] = NULL;
    strcpy(menu[0].argument_longpress[3], "1");

    strcpy(menu[0].caption, "\load");
    menu[0].menu_animation = menu_loadscreen;
    menu[0].init_animation = menu_init_loadscreen;

    //Menu 1 --> QR CODE
    menu[1].button_pressed[0] = NULL;
    strcpy(menu[1].argument[0], "2");
    menu[1].button_pressed[1]= NULL;
    strcpy(menu[1].argument[1], "2");
    menu[1].button_pressed[2] = NULL;
    strcpy(menu[1].argument[2], "3");
    menu[1].button_pressed[3] = NULL;
    strcpy(menu[1].argument[3], "\clock");
    strcpy(menu[1].caption, "\qrcode");

    menu[1].button_long_pressed[0] = NULL;
    strcpy(menu[1].argument_longpress[0], "1");
    menu[1].button_long_pressed[1]= NULL;
    strcpy(menu[1].argument_longpress[1], "1");
    menu[1].button_long_pressed[2] = NULL;
    strcpy(menu[1].argument_longpress[2], "1");
    menu[1].button_long_pressed[3] = NULL;
    strcpy(menu[1].argument_longpress[3], "1");

    menu[1].menu_animation = menu_ap_qr;
    menu[1].init_animation = menu_init_ap_qr;

    //Menu 2 --> Kegscan
    menu[2].button_pressed[0] = menu2_click_b0;
    strcpy(menu[2].argument[0], "3");
    menu[2].button_pressed[1]= NULL;
    strcpy(menu[2].argument[1], "2");
    menu[2].button_pressed[2] = NULL;
    strcpy(menu[2].argument[2], "3");
    menu[2].button_pressed[3] = NULL;
    strcpy(menu[2].argument[3], "1");

    menu[2].button_long_pressed[0] = NULL;
    strcpy(menu[2].argument_longpress[0], "1");
    menu[2].button_long_pressed[1]= NULL;
    strcpy(menu[2].argument_longpress[1], "1");
    menu[2].button_long_pressed[2] = NULL;
    strcpy(menu[2].argument_longpress[2], "1");
    menu[2].button_long_pressed[3] = NULL;
    strcpy(menu[2].argument_longpress[3], "1");

    strcpy(menu[2].caption, "\kegscan");
    menu[2].menu_animation = menu_scan_keg;
    menu[2].init_animation = menu_init_scan_keg;


    //Menu 3 --> Standard Zapfen
    menu[3].button_pressed[0] = NULL;
    strcpy(menu[3].argument[0], "4");
    menu[3].button_pressed[1]= NULL;
    strcpy(menu[3].argument[1], "2");
    menu[3].button_pressed[2] = NULL;
    strcpy(menu[3].argument[2], "3");
    menu[3].button_pressed[3] = NULL;
    strcpy(menu[3].argument[3], "1");

    menu[3].button_long_pressed[0] = NULL;
    strcpy(menu[3].argument_longpress[0], "1");
    menu[3].button_long_pressed[1]= NULL;
    strcpy(menu[3].argument_longpress[1], "1");
    menu[3].button_long_pressed[2] = NULL;
    strcpy(menu[3].argument_longpress[2], "1");
    menu[3].button_long_pressed[3] = NULL;
    strcpy(menu[3].argument_longpress[3], "1");

    strcpy(menu[3].caption, "\keg_screen");
    menu[3].menu_animation = menu_keg_screen;
    menu[3].init_animation = menu_init_keg_screen;

    //Menu 4 --> Bearbeitung
    menu[4].button_pressed[0] = LoadMenuById;
    strcpy(menu[4].argument[0], "5");
    menu[4].button_pressed[1]= menu4_click_b2;
    strcpy(menu[4].argument[1], "2");
    menu[4].button_pressed[2] = menu4_click_b3;
    strcpy(menu[4].argument[2], "4");
    menu[4].button_pressed[3] = menu4_click_b4;

    menu[4].button_long_pressed[0] = NULL;
    strcpy(menu[4].argument_longpress[0], "1");
    menu[4].button_long_pressed[1]= NULL;
    strcpy(menu[4].argument_longpress[1], "1");
    menu[4].button_long_pressed[2] = menu4_longpress_b3;
    strcpy(menu[4].argument_longpress[2], "1");
    menu[4].button_long_pressed[3] = NULL;
    strcpy(menu[4].argument_longpress[3], "1");

    strcpy(menu[4].argument[3], "1");
    strcpy(menu[4].caption, "Weather");
    menu[4].menu_animation = menu_keg_fill;
    menu[4].init_animation = menu_init_keg_fill;

    //Menu 5 --> Heizung
    menu[5].button_pressed[0] = LoadMenuById;
    strcpy(menu[5].argument[0], "4");
    menu[5].button_pressed[1]= NULL;
    strcpy(menu[5].argument[1], "2");
    menu[5].button_pressed[2] = NULL;
    strcpy(menu[5].argument[2], "5");
    menu[5].button_pressed[3] = NULL;
    strcpy(menu[5].argument[3], "1");
    strcpy(menu[5].caption, "Heizung");
    menu[5].menu_animation = NULL;
    menu[5].init_animation = menu_init_keg_save;

    menu[5].button_long_pressed[0] = NULL;
    strcpy(menu[5].argument_longpress[0], "1");
    menu[5].button_long_pressed[1]= NULL;
    strcpy(menu[5].argument_longpress[1], "1");
    menu[5].button_long_pressed[2] = NULL;
    strcpy(menu[5].argument_longpress[2], "1");
    menu[5].button_long_pressed[3] = NULL;
    strcpy(menu[5].argument_longpress[3], "1");
}