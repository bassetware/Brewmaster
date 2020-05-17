const PROGMEM char* p_index = 
"<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center>"
                "<b>Master Name:</b> "MASTER_NAME"<br>"
                "<b>Softwareversion:</b> "SW_VERSION"<br>"
            "</td>"
        "</tr>"
        "<tr>"
            "<td colspan=2>"
            "<center>"
            "<b>Webserver runing!</b>"
            "</td>"
        "</tr>"
        "<tr>"
        #ifdef ENABLE_WEBUPDATE  UPDATE_URL
        "<tr>"
            "<td colspan=2>"
                "<center>"
                "<b>Webupdate runing!</b><br>"
                "<a href="UPDATE_URL">go to webupdate</a>" 
            "</td>"
        "</tr>"
        #endif
        #ifdef ENABLE_NTP
        "<tr>"
            "<td colspan=2>"
                "<center>"
                "<b>NTP runing!</b>"
            "</td>"
        "</tr>"
        #endif
        #ifdef ENABLE_MQTT
        "<tr>"
            "<td colspan=2>"
                "<center>"
                "<b>MQTT runing!</b>"
            "</td>"
        "</tr>"
        #endif
"</table>";
