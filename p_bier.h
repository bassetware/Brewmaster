void handle_keg_request() {
  if (server.args() && scaned == false) {
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "id") {
        keg.id = server.arg(i).toInt();
      }
      if (server.argName(i) == "name") {
        server.arg(i).toCharArray(keg.name, sizeof(keg.name));
        for (int c = 0; c < strlen(keg.name); c++) {
          if (keg.name[c] == '_') {
            keg.name[c] = ' ';
          }
        }
      }
        if (server.argName(i) == "filldate") {
          server.arg(i).toCharArray(keg.filldate, sizeof(keg.filldate));
          for (int c = 0; c < strlen(keg.filldate); c++) {
            if (keg.filldate[c] == '_') {
              keg.filldate[c] = ' ';
            }
          }
        }
        if (server.argName(i) == "brewdate") {
          server.arg(i).toCharArray(keg.brewdate, sizeof(keg.brewdate));
          for (int c = 0; c < strlen(keg.brewdate); c++) {
            if (keg.brewdate[c] == '_') {
              keg.brewdate[c] = ' ';
            }
          }
        }
        if (server.argName(i) == "tara") {
          keg.tara = server.arg(i).toFloat();
        }
        if (server.argName(i) == "full") {
          keg.full = server.arg(i).toFloat();
        }
        if (server.argName(i) == "malt") {
          server.arg(i).toCharArray(keg.malt, sizeof(keg.malt));
          for (int c = 0; c < strlen(keg.malt); c++) {
            if (keg.malt[c] == '_') {
              keg.malt[c] = ' ';
            }
          }
        }
        if (server.argName(i) == "alc") {
          server.arg(i).toCharArray(keg.alc, sizeof(keg.alc));
          for (int c = 0; c < strlen(keg.alc); c++) {
            if (keg.alc[c] == '_') {
              keg.alc[c] = ' ';
            }
          }
        }
        if (server.argName(i) == "hop") {
          server.arg(i).toCharArray(keg.hop, sizeof(keg.hop));
          for (int c = 0; c < strlen(keg.hop); c++) {
            if (keg.hop[c] == '_') {
              keg.hop[c] = ' ';
            }
          }
        }
        if (server.argName(i) == "yeast") {
          server.arg(i).toCharArray(keg.yeast, sizeof(keg.yeast));
          for (int c = 0; c < strlen(keg.yeast); c++) {
            if (keg.yeast[c] == '_') {
              keg.yeast[c] = ' ';
            }
          }
        }
        scaned = true;
        delay(2000);
        //WiFi.softAPdisconnect (true);
    }
  }
}

void handle_access_request() {
     if (server.args()) {
       for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i) == "pw") {
          char arg_password[32];
          server.arg(i).toCharArray(arg_password, sizeof(arg_password));
            if(!strcmp(arg_password, keg_password)) {
            fillaccess = true;
            scan_keg_access(true);
            server.sendHeader("Connection", "close");
            server.send(200, "text/html", "access granted");
          }
          else {
            scan_keg_access(false);
            fillaccess = false;
            server.sendHeader("Connection", "close");
            server.send(200, "text/html", "access denied");
          }
        };
      }
    }
}
