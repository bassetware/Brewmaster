#define PI 3.14159
#define PI2 6.28319

typedef struct keg_props{
    float tara;
    float full;
    long max;
    float dichte;
    char name[128];
    int id;
    char brewdate[20];
    char filldate[20];
    char alc[10];
    char hop[256];
    char malt[256];
    char yeast[256];
    char type;
};

keg_props keg, keg_old;

TFT_eSprite keg_sprite = TFT_eSprite(&tft);



int keg_start_animation;
long keg_next_animation;  
void update_keg_screen() {
    //324,92
    //446,288
    float mass = calcMassfromRaw(scale_raw - scale_tare) - keg.tara;
    float percent = mass/keg.full;
    if(percent < 0) {percent = 0;}
    if(percent > 1) {percent = 1;}
    float v_left = mass / keg.dichte;
    char buffer[64];

    if(millis() > keg_next_animation) {
    keg_sprite.setColorDepth(8);
    keg_sprite.createSprite(100, 210);
    keg_sprite.setTextDatum(MC_DATUM);
    
    keg_sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    

    
        //keg_sprite.fillSprite(TFT_WHITE);
        //keg_sprite.fillRect(5,5,90,200,TFT_BLACK);
        keg_sprite.fillSprite(TFT_BLACK);
        for(int i = 0; i < 90; i++) {
            float middle;
            long color;
            float period = (3*PI2 / 90); 
            if(percent * 100 > 3) {
                middle = round(percent * 200)-3;
                color = TFT_YELLOW;
            }
            else {
                middle = 5;
                color = TFT_RED;
            }
            if(percent * 100 > 3 && percent*100 < 15) {
                color = TFT_RED;
            }
            
            if(percent < 5) {}
            float middle_px = (middle) + (3 * sin(keg_start_animation + i*period));
            int h_px = round(middle_px);
            keg_sprite.drawLine(5+i, 205, 5+i, (205-middle_px), color); //Sprite has 210 pixel height. px205 - 210 is border. so fill goes from 205 200 px up leaving 5 pixel for upper border left. so y2 is 205 - middle
        }
        keg_start_animation = ++keg_start_animation%90;
        /*float middle = round(percent * 200);
        int h_px = round(middle);
        keg_sprite.fillRect(5,205,90,200,TFT_BLACK);*/
        keg_sprite.drawRect(0,0,99,209,TFT_WHITE); //x,y width, heigt, don't confuse with drawline (x1,y1,x2,y2)
        keg_sprite.drawRect(1,1,97,207,TFT_WHITE);
        keg_sprite.drawRect(2,2,95,205,TFT_WHITE);
        keg_sprite.drawRect(3,3,93,203,TFT_WHITE);
        keg_sprite.drawRect(4,4,91,201,TFT_WHITE);
        keg_next_animation = millis() + 20;
        keg_sprite.pushSprite(350, 70, TFT_TRANSPARENT);
        keg_sprite.deleteSprite();

        keg_sprite.setColorDepth(8);
        keg_sprite.createSprite(200, 60);
        keg_sprite.fillSprite(TFT_BLACK);
        keg_sprite.setFreeFont(FSS24);
        keg_sprite.setTextDatum(TL_DATUM);
        keg_sprite.setTextColor(TFT_WHITE, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f l", v_left / 1000);
        keg_sprite.drawString(buffer, 0, 0);
        keg_sprite.pushSprite(150, 150, TFT_TRANSPARENT);
        keg_sprite.deleteSprite();
    }




}


void init_keg_screen() {
 
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(FSS24);
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(keg.name, 24, 20);
    tft.setFreeFont(FSS12);
    char buffer[128];
    sprintf(buffer, "Keg-Nummer: %d", keg.id);
    tft.drawString(buffer, 24, 70);
    tft.setFreeFont(FSS24);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("Rest: ", 24, 150);
    tft.setFreeFont(FSS12);
    tft.setTextDatum(BC_DATUM);
    tft.drawString("Weitere Informationen", SCREEN_W / 2, SCREEN_H - 12);
    update_keg_screen();
}

void init_scan_keg() {
   tft.fillScreen(TFT_BLACK);
   tft.setFreeFont(FSS12);
   tft.setTextDatum(MC_DATUM);
   tft.setTextColor(TFT_WHITE, TFT_BLACK);
   tft.drawString("Scanne das Keg", SCREEN_W/2, SCREEN_H/2);
}

void scan_keg_access(boolean granted) {
   init_scan_keg();
   tft.setFreeFont(FSS12);
   tft.setTextDatum(BC_DATUM);
   if(granted) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("BEARBEITUNG", SCREEN_W/2, SCREEN_H - 20);
   }
   else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("FAILED!", SCREEN_W/2, SCREEN_H - 20);
   }
}

float raw_val_old;

TFT_eSprite sprite_keg_fill = TFT_eSprite(&tft);
void update_keg_fill() {
    if(millis() < nextframe) {return;}
    char buffer[64];
    float screen_mass;
    tft.setTextDatum(TL_DATUM);
    

    if(keg.id != keg_old.id) {
        tft.setFreeFont(FSS12);
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%d", keg_old.id);
        tft.drawString(buffer, 180, 130);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%d", keg.id);
        tft.drawString(buffer, 180, 130);
    }

    if(strcmp(keg.filldate, keg_old.filldate)) {
        tft.setFreeFont(FSS12);
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        //snprintf(buffer, sizeof(buffer), "%d.%d.%d", day(keg_old.filldate), month(keg_old.filldate), year(keg_old.filldate));
        tft.drawString(keg_old.filldate, 180, 160); 
        //tft.drawString(buffer, 300, 160);       
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        //snprintf(buffer, sizeof(buffer), "%d.%d.%d", day(keg.filldate), month(keg.filldate), year(keg.filldate));
        tft.drawString(keg.filldate, 180, 160);
    }

    if(keg.tara != keg_old.tara) {
        tft.setFreeFont(FSS12);
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f kg", keg_old.tara/1000);
        tft.drawString(buffer, 180, 190);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f kg", keg.tara/1000);
        tft.drawString(buffer, 180, 190);
    }

    if(keg.full != keg_old.full) {
        tft.setFreeFont(FSS12);
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f kg", keg_old.full/1000);
        tft.drawString(buffer, 180, 220);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f kg", keg.full/1000);
        tft.drawString(buffer, 180, 220);
    }

    if(keg.dichte != keg_old.dichte) {
        tft.setFreeFont(FSS12);
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f g/ml", keg_old.dichte/1000);
        tft.drawString(buffer, 180, 250);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f g/ml", keg.dichte/1000);
        tft.drawString(buffer, 180, 250);
        
    }

    float screen_raw = calcMassfromRaw(scale_raw - scale_tare) - keg.tara;
    if(raw_val_old != screen_raw) {
        sprite_keg_fill.setColorDepth(8);
        sprite_keg_fill.createSprite(SCREEN_W-180, 60);
        sprite_keg_fill.fillSprite(TFT_BLACK);
        sprite_keg_fill.setFreeFont(FSS24);
        sprite_keg_fill.setTextDatum(TL_DATUM);
        sprite_keg_fill.setTextColor(TFT_WHITE, TFT_BLACK);
        snprintf(buffer, sizeof(buffer), "%.2f kg", screen_raw / 1000);
        sprite_keg_fill.drawString(buffer, 0, 0);
        sprite_keg_fill.pushSprite(180, 70, TFT_TRANSPARENT);
        sprite_keg_fill.deleteSprite();
        raw_val_old = screen_raw;
    }
    if(strcmp(keg.name, keg_old.name)) {
        sprite_keg_fill.setColorDepth(8);
        sprite_keg_fill.createSprite(SCREEN_W-24, 40);
        sprite_keg_fill.fillSprite(TFT_BLACK);
        sprite_keg_fill.setFreeFont(FSS24);
        sprite_keg_fill.setTextDatum(TL_DATUM);
        sprite_keg_fill.setTextColor(TFT_WHITE, TFT_BLACK);
        sprite_keg_fill.drawString(keg.name, 0, 0);
        sprite_keg_fill.pushSprite(24, 12, TFT_TRANSPARENT);
        sprite_keg_fill.deleteSprite();
        
    }


    keg_old = keg;
    nextframe = millis() + 50;
    }


void init_keg_fill() {
    char buffer[64];
    float screen_mass;
    float screen_raw = calcMassfromRaw(scale_raw - scale_tare) - keg.tara;

    tft.fillScreen(TFT_BLACK);
    if(keg.name != keg_old.name) {
        tft.setFreeFont(FSS24);
        tft.setTextDatum(TL_DATUM);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString(keg.name, 24, 12);
    }

    tft.drawString("Masse: ", 24, 70);
    tft.setFreeFont(FSS24);
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    snprintf(buffer, sizeof(buffer), "%.2f kg", calcMassfromRaw(screen_raw) - keg.tara);
    tft.drawString(buffer, 180, 70);

    tft.setFreeFont(FSS12);

    tft.drawString("Nummer:", 24, 130);
    snprintf(buffer, sizeof(buffer), "%d", keg.id);
    tft.drawString(buffer, 180, 130);

    tft.drawString("Datum:", 24, 160);
    //snprintf(buffer, sizeof(buffer), "%d.%d.%d", day(keg.filldate), month(keg.filldate), year(keg.filldate));
    tft.drawString(keg.filldate, 180, 160);

    tft.drawString("Taramasse:", 24, 190);
    Serial.println("Tara");
    Serial.println(keg.tara);
    snprintf(buffer, sizeof(buffer), "%.2f kg", keg.tara/1000);
    tft.drawString(buffer, 180, 190);

    tft.drawString("Vollmasse:", 24, 220);
    snprintf(buffer, sizeof(buffer), "%.2f kg", keg.full/1000);
    tft.drawString(buffer, 180, 220);

    tft.drawString("Dichte:", 24, 250);
    snprintf(buffer, sizeof(buffer), "%.4f g/ml", keg.dichte);
    tft.drawString(buffer, 180, 250);

    tft.setTextDatum(BC_DATUM);
    tft.drawString("Datum | Taramasse | Vollmasse | Speichern", SCREEN_W / 2, SCREEN_H - 12);

    char qrbuffer[120];
    IPAddress IP = WiFi.softAPIP();
    snprintf(qrbuffer, sizeof(qrbuffer), "http://%s/filldata.html", IP.toString().c_str());
    show_qr(SCREEN_W - 150, 130, qrbuffer, 4);   
    keg_old = keg;
    update_keg_fill();
}

void init_fill_qr() {
    tft.fillScreen(TFT_BLACK);
    char qrbuffer[120];
    IPAddress IP = WiFi.softAPIP();
    snprintf(qrbuffer, sizeof(qrbuffer), "http://%s/nfc.html", IP.toString().c_str());
    show_qr_center(qrbuffer, 8);
    tft.setTextDatum(BC_DATUM); 
    tft.drawString("QR-Code scannen", (SCREEN_W / 2), SCREEN_H - 12);
}