// Bodmers BMP image rendering function
// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}


void drawBmp(const char *filename, int16_t x, int16_t y) {

  if ((x >= tft.width()) || (y >= tft.height())) return;

  fs::File bmpFS;

  // Open requested file on SD card
  bmpFS = SPIFFS.open(filename, "r");

  if (!bmpFS)
  {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h, row, col;
  uint8_t  r, g, b;

  uint32_t startTime = millis();

  if (read16(bmpFS) == 0x4D42)
  {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
    {
      y += h - 1;

      bool oldSwapBytes = tft.getSwapBytes();
      tft.setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t*  bptr = lineBuffer;
        uint16_t* tptr = (uint16_t*)lineBuffer;
        // Convert 24 to 16 bit colours
        for (uint16_t col = 0; col < w; col++)
        {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        tft.pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
      }
      tft.setSwapBytes(oldSwapBytes);
      Serial.print("Loaded in "); Serial.print(millis() - startTime);
      Serial.println(" ms");
    }
    else Serial.println("BMP format not recognized.");
  }
  bmpFS.close();
}


QRCode qrcode;
//void Display_QRcode(int offset_x, int offset_y, const char* Message){
//  #define element_size 4
//  // Create the QR code ~120 char maximum
//  uint8_t qrcodeData[qrcode_getBufferSize(3)];
//  qrcode_initText(&qrcode, qrcodeData, 3, 0, Message);
//  for (int y = 0; y < qrcode.size; y++) {
//    for (int x = 0; x < qrcode.size; x++) {
//      if (qrcode_getModule(&qrcode, x, y)) {
//        //tft.setColor(EPD_BLACK);
//        tft.fillRect(x*element_size+offset_x,y*element_size+offset_y,element_size,element_size,TFT_BLACK);
//      }
//      else 
//      {
//        //tft.setColor(EPD_WHITE);
//        tft.fillRect(x*element_size+offset_x,y*element_size+offset_y,element_size,element_size,TFT_BLACK);
//      }
//    }
//  }
//}

void show_ap_qr() {
  int element_size = 8;
  tft.fillScreen(TFT_WHITE);
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  char qrbuffer[120];
  snprintf(qrbuffer, sizeof(qrbuffer), "WIFI:T:WPA;S:%s;P:%s;H:true;", ap_ssid, ap_password);
  qrcode_initText(&qrcode, qrcodeData, 3, 0, qrbuffer);
  int offset_x = (SCREEN_W / 2) - (qrcode.size*element_size/2);
    for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        //tft.setColor(EPD_BLACK);
        tft.fillRect(x*element_size+offset_x, y*element_size+20, element_size, element_size, TFT_BLACK);
      }
      else 
      {
        //tft.setColor(EPD_WHITE);
        tft.fillRect(x*element_size+offset_x, y*element_size+20, element_size, element_size, TFT_WHITE);
      }
    }
  }
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSS12);
  tft.setTextDatum(BC_DATUM );
  tft.drawString("QR-Code scannen!", (SCREEN_W / 2), SCREEN_H - 20);
}

QRCode qrcode_fill;

void show_qr(int offset_x, int offset_y, char qr_text[], int element_size) {
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  char qrbuffer[120];
  qrcode_initText(&qrcode_fill, qrcodeData, 3, 0, qr_text);
  tft.fillRect(offset_x, offset_y, qrcode_fill.size*element_size+20, qrcode_fill.size * element_size + 20, TFT_WHITE);
  for (int y = 0; y < qrcode_fill.size; y++) {
    for (int x = 0; x < qrcode_fill.size; x++) {
      if (qrcode_getModule(&qrcode_fill, x, y)) {
        //tft.setColor(EPD_BLACK);
        tft.fillRect(x*element_size+offset_x+10, y*element_size+offset_y+10, element_size, element_size, TFT_BLACK);
      }
      else 
      {
        //tft.setColor(EPD_WHITE);
        tft.fillRect(x*element_size+offset_x+10, y*element_size+offset_y+10, element_size, element_size, TFT_WHITE);
      }
    }
  }
}

void show_qr_center(char qr_text[], int element_size) {
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  int offset_x;
  int offset_y;
  char qrbuffer[120];
  qrcode_initText(&qrcode_fill, qrcodeData, 3, 0, qr_text);
  offset_y = 12;
  offset_x = (SCREEN_W / 2) - (qrcode.size*element_size/2) - 10;
  tft.fillRect(offset_x, offset_y, qrcode_fill.size*element_size+20, qrcode_fill.size * element_size + 20, TFT_WHITE);

  for (int y = 0; y < qrcode_fill.size; y++) {
    for (int x = 0; x < qrcode_fill.size; x++) {
      if (qrcode_getModule(&qrcode_fill, x, y)) {
        //tft.setColor(EPD_BLACK);
        tft.fillRect(x*element_size+offset_x+10, y*element_size+offset_y+10, element_size, element_size, TFT_BLACK);
      }
      else 
      {
        //tft.setColor(EPD_WHITE);
        tft.fillRect(x*element_size+offset_x+10, y*element_size+offset_y+10, element_size, element_size, TFT_WHITE);
      }
    }
  }
}