#ifndef PAGE_FILL_H
#define PAGE_FILL_H


const char PAGE_FILL[] PROGMEM = R"=====(<html>
	<head>
	</head>
	<body>
		<hr>
		<Strong>MQTT-Konfiguration</strong>
		<form action="" method="get">
		<table border="0" cellspacing="0" cellpadding="3" style="width:420px">
		<colgroup span=2 width="50%">
		<tr><td align="right">Name</td><td><input type="text" id="name" name="name" value=""></td></tr>
		<tr><td align="right">Fassnummer</td><td><input type="text" id="id" name="id" value=""></td></tr></td></tr>
		<tr><td align="right">Abflldatum</td><td><input type="text" id="filldate" name="filldate" value=""></td></tr>
		<tr><td align="right">Taramasse Fass</td><td><input type="text" id="tara" name="tara" value=""></td></tr>
		<tr><td align="right">Vollmasse</td><td><input type="text" id="full" name="full" value=""></td></tr>
		<tr><td align="right">Alkohol</td><td><input type="text" id="alc" name="alc" value=""></td></tr>
		<tr><td align="right">Malz</td><td><input type="text" id="malt" name="malt" value=""></td></tr>
		<tr><td align="right">Hopfen</td><td><input type="text" id="hop" name="hop" value=""></td></tr>
		<tr><td align="right">Hefe</td><td><input type="text" id="yeast" name="yeast" value=""></td></tr>
		<tr><td align="right"><input type="button" onclick="setValues('/scripts/filldata')" class="btn btn--m btn--blue" value="Aktualisieren"></td><td><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
		</table>
		<hr>
		<script>
		setValues("/scripts/filldata");
		function GetState() {
			setValues("/scripts/filldata");
		}
		</script>
	</body>

</html>

)=====";


const char PAGE_NFC[] PROGMEM = R"=====(<html>
	<head>
	</head>
	<body>
		<hr>
		<Strong>NFC-Daten</strong><br>
		<div id="nfc">N/A</div><br>
		Link kopieren und in einen NFC-Tag schreiben.<br>
		<input type="button" onclick="setValues('/scripts/nfcdata')" class="btn btn--m btn--blue" value="Aktualisieren">
		<hr>
		<script>
		setValues("/scripts/nfcdata");
		function GetState() {
			setValues("/scripts/nfcdata");
		}
		</script>
	</body>

	</html>
)=====";

const char PAGE_FILL_DENIED[] PROGMEM = R"=====(<html>
	<head>
	</head>
	<body>
        Zugang verwährt.
	</body>

	</html>
)=====";

unsigned char h2int(char c) {
    if (c >= '0' && c <='9')
        return((unsigned char)c - '0');
    if (c >= 'a' && c <='f')
        return((unsigned char)c - 'a' + 10);
    if (c >= 'A' && c <='F')
        return((unsigned char)c - 'A' + 10);
    return(0);
}


String urldecode(String input) {
    char c;
    String ret = "";

    for(byte t = 0; t < input.length(); t++) {
        c = input[t];
        if (c == '+') c = ' ';
        if (c == '%') {
            t++;
            c = input[t];
            t++;
            c = (h2int(c) << 4) | h2int(input[t]);
        }
        ret.concat(c);
    }
    return ret;
}



void handle_filldata_request() {
    if(fillaccess) {
        if (server.args()) {
            for(int i = 0; i < server.args(); i++) {
				if(server.argName(i) ==  "name") {urldecode(server.arg(i)).toCharArray(keg.name, sizeof(keg.name));}
				if(server.argName(i) ==  "id") {keg.id = server.arg(i).toInt();}
				if(server.argName(i) ==  "filldate") {urldecode(server.arg(i)).toCharArray(keg.filldate, sizeof(keg.filldate));}
				if(server.argName(i) ==  "tara") {keg.tara = server.arg(i).toFloat();}
				if(server.argName(i) ==  "full") {keg.full = server.arg(i).toFloat();}
				if(server.argName(i) ==  "malt") {urldecode(server.arg(i)).toCharArray(keg.malt, sizeof(keg.malt));}
				if(server.argName(i) ==  "alc") {urldecode(server.arg(i)).toCharArray(keg.alc, sizeof(keg.alc));}
				if(server.argName(i) ==  "hop") {urldecode(server.arg(i)).toCharArray(keg.hop, sizeof(keg.hop));}
				if(server.argName(i) ==  "yeast") {urldecode(server.arg(i)).toCharArray(keg.yeast, sizeof(keg.yeast));}
			}
        }
        sendPage(PAGE_FILL, sizeof(PAGE_FILL), PTYPE_HTML);
    }
    else {
        sendPage(PAGE_FILL_DENIED, sizeof(PAGE_FILL_DENIED), PTYPE_HTML);
    }
}

void send_filldata() {
    String values ="";
	values += "name|input|" + (String)keg.name + "\n";
	values += "id|input|" + (String)keg.id + "\n";
	values += "filldate|input|" + (String)keg.filldate + "\n";
	values += "tara|input|" + (String)keg.tara + "\n";
	values += "full|input|" + (String)keg.full + "\n";
	values += "alc|input|" + (String)keg.alc + "\n";
	values += "malt|input|" + (String)keg.malt + "\n";
	values += "hop|input|" + (String)keg.hop + "\n";
	values += "yeast|input|" + (String)keg.yeast + "\n";
	server.send(200, PTYPE_PLAIN, values);
}

void handle_nfc() {
	sendPage(PAGE_NFC, sizeof(PAGE_NFC), PTYPE_HTML);
}

void send_nfc_data() {
	Serial.println("starte senden von daten");
	String values = "";
	IPAddress IP = WiFi.softAPIP();
	values += "nfc|div|http://" + IP.toString() + "/keg.html?";
	Serial.println("name");
	char buffer[120];
	snprintf(buffer, sizeof(buffer), "%s", keg.name);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "name=" + (String)buffer + "&";

	snprintf(buffer, sizeof(buffer), "%d", keg.id);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "id=" + (String)buffer + "&";

	snprintf(buffer, sizeof(buffer), "%s", keg.filldate);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "filldate=" + (String)buffer + "&";

	if (strcmp(keg.brewdate, "")) {
		snprintf(buffer, sizeof(buffer), "%s", keg.brewdate);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "brewdate=" + (String)buffer + "&";
	}

values += "tara=" + (String)keg.tara + "&";
values += "full=" + (String)keg.full + "&";

if (strcmp(keg.alc, "")) {
	snprintf(buffer, sizeof(buffer), "%s", keg.alc);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "alc=" + (String)buffer + "&";
}

if (strcmp(keg.malt, ""))
{
	snprintf(buffer, sizeof(buffer), "%s", keg.malt);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "malt=" + (String)buffer + "&";
}
if (strcmp(keg.hop, "")) 
{
	snprintf(buffer, sizeof(buffer), "%s", keg.hop);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "hop=" + (String)buffer + "&";
}
if (strcmp(keg.yeast, ""))
{
	snprintf(buffer, sizeof(buffer), "%s", keg.yeast);
	for (int c = 0; c < strlen(buffer); c++)
	{
		if (buffer[c] == ' ')
		{
			buffer[c] = '_';
		}
	}
	values += "yeast=" + (String)buffer;
}
server.send(200, PTYPE_PLAIN, values);
}
#endif