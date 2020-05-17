void menu_init_loadscreen() {
    drawBmp("/brewmaster.bmp", 0, 0);
    nextframe = millis() + 1000;
}

boolean init_done = false;
void menu_loadscreen(char argument[]) {
    if(millis() < nextframe) {return;}
    if(!init_done) {    
        Serial.println("Connect to Wifi");
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextDatum(BC_DATUM);
        tft.drawString("Loading...", SCREEN_W / 2, SCREEN_H - 40);
        Serial.print("SSID: ");Serial.println(WIFI_SSID);
        Serial.print("PW: ");Serial.println(SECRET_WIFI_PASSWORD);
        setup_wifi(); //Connect to the wifi
        #ifdef ENABLE_WEBSERVER
        setupWebServer();
        #endif
        #ifdef ENABLE_MQTT
        client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
        client.setCallback(callback_new);
        #endif
        #ifdef ENABLE_NTP
        NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
            ntpEvent = event;
            syncEventTriggered = true;
        });
        init_done = true;
    #endif
    }

    scale_tare_scale();
    float test_scale = calcMassfromRaw(scale_tare - STD_TARE_VAL);
    if(test_scale > KEG_DETECT_VAL * 5) {
        tft.setFreeFont(FSS12);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextDatum(BC_DATUM);
        tft.drawString("Waagenfehler: Keg auf der Waage?", SCREEN_W / 2, SCREEN_H - 40);
    }
    else {
    LoadMenuById("1");
    }
}

void menu_init_ap_qr() {
    client_connected = false;
    show_ap_qr();
}

void menu_ap_qr(char argument[]) {
    if(!client_connected) {return;}
    else {LoadMenuById("2");}
}


boolean waitforkeg;
void menu_init_scan_keg() {
    scaned = false;
    waitforkeg = false;
    init_scan_keg();
}

void menu_scan_keg(char argument[]) {
    if(scaned == false) {return;}
    if(calcMassfromRaw(scale_raw-scale_tare) < KEG_DETECT_VAL) {
        if(!waitforkeg) {
            tft.setFreeFont(FSS12);
            tft.setTextDatum(MC_DATUM);
            tft.setTextColor(TFT_BLACK, TFT_BLACK);
            tft.drawString("Scanne das Keg", SCREEN_W/2, SCREEN_H/2);
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.drawString("Stelle das Keg auf die Waage", SCREEN_W/2, SCREEN_H/2);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setTextDatum(BC_DATUM);
            tft.drawString("Tara erneuern", SCREEN_W / 2, SCREEN_H - 12);
            waitforkeg = true;
        }
        return;
    }
    else {
            tft.setFreeFont(FSS12);
            tft.setTextDatum(MC_DATUM);
            tft.setTextColor(TFT_BLACK, TFT_BLACK);
            tft.drawString("Stelle das Keg auf die Waage", SCREEN_W/2, SCREEN_H/2);
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.drawString("Keg erkannt!", SCREEN_W/2, SCREEN_H/2);
            delay(1000);
    }
    if(!fillaccess) {
        LoadMenuById("3");
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", "ok");
    }
    else {
        server.sendHeader("Location", "/filldata.html", true);   //Redirect to our html web page  
        server.send(302, "text/plane","");
        LoadMenuById("4");
    }
}

void menu_init_keg_screen() {
    init_keg_screen();
}

void menu_keg_screen(char argument[]) {
    update_keg_screen();
}

void menu_init_keg_fill() {
    
    init_keg_fill();
    scale_tare_scale();
}

void menu_keg_fill(char argument[]) {
    update_keg_fill();
}

void menu_init_keg_save() {
    init_fill_qr();
}



//button functions
void menu4_longpress_b3(char argument[]) {
    Serial.println("Tare scale on click!");
    scale_tare_scale();
}

void menu4_click_b4(char argument[]) {
    snprintf(keg.filldate, sizeof(keg.filldate), "%d.%d.%d", day(now()), month(now()), year(now()));
    //init_keg_fill();
}

void menu4_click_b3(char argument[]) {
    keg.tara = calcMassfromRaw(scale_raw - scale_tare);
    //init_keg_fill();
}

void menu4_click_b2(char argument[]) {
    keg.full = calcMassfromRaw(scale_raw - scale_tare) - keg.tara;
    //init_keg_fill();
}

void menu2_click_b0(char argument[]) {
    scale_tare_scale();
    tft.setFreeFont(FSS12);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextDatum(BC_DATUM);
    tft.drawString("Tara erneuert.", SCREEN_W / 2, SCREEN_H - 80);
}
