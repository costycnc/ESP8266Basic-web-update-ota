#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

//#define BASIC_TFT

//ESP8266 Basic Interpreter
//HTTP://ESP8266BASIC.COM
//
//The MIT License (MIT)
//
//Copyright (c) 2015 Michael Molinari
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

//Onewire temperature sensor code contributed by Rotohammer.

//Significant speed improvements submitted by cicciocb
//Uploading of bas files improvement added by cicciocb
//JSON parsing routine added by cicciocb



// Comment or uncomment following lines to produce a custom build omitting features.








#include <WebSocketsServer.h>
#include <Hash.h>
#include <WebSockets.h>


//#include <ArduinoJson.h>
#include "spiffs/spiffs.h"
#include <FS.h>
#include <ESP8266mDNS.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
//#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
//#include <ESP8266httpUpdate.h>               // that file need to be copied into the folder for 2.0.0-rc1
#include <ESP8266HTTPUpdateServer.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Base64.h"

//#include <WiFiClientSecure.h>
#if defined(BASIC_TFT)
#include "ESP8266httpUpdate.h"
#endif
#include <time.h>
#include <ESP8266HTTPClient.h>              // that line needs to be added for the esp8266-2.0.0 and 2.1.0-rc2

//Ping stuff
#include <ESP8266Ping.h>



//LCD Stuff
 #if defined(BASIC_TFT)
#include <LiquidCrystal_SR.h>
#include <I2CIO.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_SR3W.h>
#include <LCD.h>
#include <LiquidCrystal_SR2W.h>
#endif
//#include <FastIO.h>
#include <LiquidCrystal_I2C.h>

//PS2 Key Board
//#include <PS2Keyboard.h>


#include <Time.h>

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel *pixels = NULL;

#include <SoftwareSerial.h>
SoftwareSerial *swSer = NULL;

#include "VERSION.h"



//ThingSpeak Stuff




//wifi mode exclusivity 
bool wifiApStaModeOn = 0;
//WIFI SCANN STUFF
int numberOfWifiScanResults;


// SPI STUFF
#include <SPI.h>

// Infrared Stuff
#include <IRremoteESP8266.h>
IRsend *irsend = NULL; //irsend(0); //an IR led is connected to GPIO pin 0
IRrecv *irrecv = NULL;
decode_results IRresults;
int IRBranchLine = 0;



#if defined(BASIC_TFT)
//ILI9341 Stuff
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
// For the Adafruit shield, these are the default.
//#define TFT_DC 4
//#define TFT_CS 5
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 *tft = NULL; //= Adafruit_ILI9341(TFT_CS, TFT_DC);
byte TFT_CS_pin, TFT_DC_pin;
#include "TFT_Objects.h"
TFT_Form form1(40);

// touch stuff
byte Touch_CS_pin;
int TouchBranchLine = 0;
int Touch_p = 0;
unsigned int Touch_millis = 0;
uint16_t touchX, touchY;
uint16_t touchX_raw, touchY_raw;
#endif

// The Math precision is defined, by default, inside expression_parser_string.h
// there is a definition PARSER_PREC that can be double or float
#include "expression_parser_string.h"
char* _parser_error_msg;

PARSER_PREC numeric_value;
String string_value ;
int parser_result;

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);



#include <DHT.h>   // adafruit library
//DHT dht(5, DHT21);   // 5 is GPIO5, DHT21 you may want change at DHT11 or DHT22
DHT *dht = NULL;


 #if defined(BASIC_TFT)
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address
#endif

// i2c OLED Display

#if defined(BASIC_TFT)
#include "SSD1306.h" 
SSD1306  display(0x3c);
#endif




//PS2Keyboard keyboard;

//WiFiClient EmailClient;
String EmailServer;
int Emailport;
String EmailSMTPuser;
String EmailSMTPpassword;

// udp client
WiFiUDP udp;

// WebSockets
WebSocketsServer *webSocket;
String WebSockMessage;
int WebSockEventBranchLine = 0;
int WebSockChangeBranchLine = 0;
String WebSockEventName;
String WebSockChangeName;
int WebSocketTimeOut[5];
int delaytime;

WiFiClient client;
WiFiClient clientTelnet;
int telnetBranch;

//MQTT Stuff
#include <PubSubClient.h>
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"
PubSubClient MQTTclient(client);
String MQTTlatestMsg;
bool MQTTnewMsgReceived ;
bool MQTTActivated;
String MQTTBranch;
String MQTTSubscribeTopic;
String MQTTPublishTopic;
String MQTTPublishMSG;
int MQTTTimeFromLastCheck;


ESP8266WebServer *server; //server(80);
ESP8266HTTPUpdateServer httpUpdater;
//Web Server Variables
byte WebGuiOff = 0;
String HTMLout;
PROGMEM const char InputFormText[] = R"=====( <input type="text" id="myid" name="input"><input type="submit" value="Submit" name="inputButton"><hr>)=====";
PROGMEM const char TextBox[] = R"=====(<input type="text" id="myid" name="variablenumber" value="variablevalue" onchange="objChange(this)">)=====";
PROGMEM const char passwordbox[] = R"=====( <input type="password" id="myid" name="variablenumber" value="variablevalue" onchange="objChange(this)">)=====";
PROGMEM const char Slider[] = R"=====( <input type="range" id="myid" name="variablenumber" min="minval" max="maxval" onchange="objChange(this)" value=variablevalue >)=====";
PROGMEM const char GOTObutton[] =  R"=====(<button id="myid" onclick="cmdClick(this)" name="gotonobranch">gotonotext</button>)=====";
PROGMEM const char GOTOimagebutton[] =  R"=====(<input type="image" onclick="cmdClick(this)" id="myid" src="/file?file=gotonotext" value="gotonotext" name="gotonobranch">)=====";
PROGMEM const char normalImage[] =  R"=====(<img src="/file?file=name">)=====";
PROGMEM const char javascript[] =  R"=====(<script src="/file?file=name"></script>)=====";
PROGMEM const char CSSscript[] =  R"=====(<link rel="stylesheet" type="text/css" href="/file?file=name">)=====";
PROGMEM const char CSSid[] =  R"=====(<style>#myid {*style*}</style>)=====";
PROGMEM const char CSSclass[] =  R"=====(<style>myid {*style*}</style>)=====";
PROGMEM const char DropDownList[] =  R"=====(<select name="variablenumber" id="myid" onchange="objChange(this)" size="theSize">options</select><script>document.getElementsByName("variablenumber")[0].value = "VARS|variablenumber";</script>)=====";
PROGMEM const char DropDownListOpptions[] =  R"=====(<option>item</option>)=====";
PROGMEM const char meter[] =  R"=====(<meter id="myid" name="variablenumber" value="variablevalue" min="minval" max="maxval"></meter>)=====";

String LastElimentIdTag;

PROGMEM const char MobileFreindlyWidth[] = R"=====(<meta name="viewport" content="width=device-width, initial-scale=1.0">)=====";

 #if defined(BASIC_TFT)
PROGMEM const char DebugPage[] =  R"=====(</script><script src='WebSockets.js'></script><script src='wsport.js'></script>
<div id="fooBar" style="float:left; width:20%;">Vars<hr></div>
<DIV style="float:left; width:35%;">
<button id="run" onclick="dcmdClick(this)">Run</button>
<button id="stop" onclick="dcmdClick(this)">Stop</button>
<button id="pause" onclick="dcmdClick(this)">Pause</button>
<button id="continue" onclick="dcmdClick(this)">Continue</button>
<input type="text" id="connection_status" value="Disconnected">
<button id="clear" onclick="logClear()">Clear</button>
Speed
<input type="text" id="speed" name="speed" value="*speed*"onchange="objChange(this)">
<br>Line No<input type="text" id="lno" value="line no">
<br><select id='code' size=35 style="width:100%"></select>
<textarea rows="20" style="width:100%;height:40%" id="log">log</textarea>
</DIV>
<div id="app" style="float:left; width:40%;">
)=====";
#else
PROGMEM const char DebugPage[] =  R"=====( )=====";
#endif



byte WaitForTheInterpertersResponse = 1;


 #if defined(BASIC_TFT)
PROGMEM const char AdminBarHTML[] = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" type="text/css" href="/file?file=skin.css">
<a href="./vars">[ VARS ]</a> 
<a href="./edit">[ EDIT ]</a>
<a href="./run">[ RUN ]</a>
<a href="./debug">[ DEBUG ]</a>
<a href="./settings">[ SETTINGS ]</a>
<a href="./filemng">[ FILE MANAGER ]</a>
<hr>)=====";
#else
PROGMEM const char AdminBarHTML[] = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<a href="./edit">[ EDIT ]</a>
<a href="./run">[ RUN ]</a>
<a href="./settings">[ SETTINGS ]</a>
<a href="./filemng">[ FILE MANAGER ]</a>
<hr>)=====";
#endif



 #if defined(BASIC_TFT)
PROGMEM const char UploadPage[] = R"=====(
<form method='POST' action='/filemng' enctype='multipart/form-data'>
<input type='file' name='Upload'>
<input type='submit' value='Upload'>
</form>
<form id="filelist">
<input type="submit" value="Delete" name="Delete">
<input type="submit" value="View" name="View">
<input type="submit" value="Edit" name="Edit">
<input type="submit" value="Rename" name="Rename">
</form>
<select name="fileName" size="25" form="filelist"></select>
</script><script src='filmanws.js'></script>
</script><script src='wsport.js'></script>
)=====";




PROGMEM const char FIleManWSJS[] =  R"=====(
function start(websocketServerLocation) {
	connection = new WebSocket(websocketServerLocation);
	connection.onopen = function() {
		connection.send("OK");
		connection.send('filelist')		
	};
	connection.onclose = function() {
		setTimeout(function() { start(websocketServerLocation)}, 1000);
	};

	connection.onmessage = function(e) {
		var res = e.data.split("~^`");

		
		if (res[0].toLowerCase() == "filename") {
			connection.send("OK");
			
			var x = document.getElementsByName("fileName")[0];
			var option = document.createElement("option");
			option.text = res[1];
			x.add(option);
			return;
		}
	};
}
var aliveme = setInterval(aliveTimer, 5000);
function aliveTimer() {
	connection.send("OK");
}
)=====";
#else
PROGMEM const char UploadPage[] = R"=====(
<form method='POST' action='/filemng' enctype='multipart/form-data'>
<input type='file' name='Upload'>
<input type='submit' value='Upload'>
</form>
<form id="filelist">
<input type="submit" value="Delete" name="Delete">
<input type="submit" value="View" name="View">
<input type="submit" value="Edit" name="Edit">
<input type="submit" value="Rename" name="Rename">
</form>
<select name="fileName" size="25" form="filelist">*table*</select>
<script>
function call() {
	var x = document.getElementsByName("fileName")[0];
	var optionVal = new Array();
	for (i = 0; i < x.length; i++) {
		optionVal.push(x.options[i].text);
	}
	for (i = x.length; i >= 0; i--) {
		x.remove(i);
	}
	optionVal.sort();
	for (var i = 0; i < optionVal.length; i++) {
		var opt = optionVal[i];
		var el = document.createElement("option");
		el.textContent = opt;
		el.value = opt;
		x.appendChild(el);
	}
}
</script>
<body onload="call()">
)=====";
#endif




//<a href="http://www.esp8266basic.com/help"  target="_blank">[ HELP ]</a>

PROGMEM const char EditorPageHTML[] =  R"=====(
<script src="editor.js"></script>
<script src="/file?file=codemirror.js.gz"></script>
<link rel="stylesheet" href="/file?file=codemirror.css.gz">
<form action='edit' id="usrform">
<input type="text" id="FileName" name="name" value="*program name*">
<input type="submit" value="Open" name="open">
</form>
<button onclick="SaveTheCode()">Save</button>
<br>
<textarea rows="30" style="width:100%" name="code" id="code">*program txt*</textarea><br>
<input type="text" id="Status" value="">
<script>
var editor = CodeMirror.fromTextArea(document.getElementById("code"), {
lineNumbers: true,
indentUnit: 4});
</script>
)=====";



PROGMEM const char WebSocketsJS[] =  R"=====(
function start(websocketServerLocation) {
	connection = new WebSocket(websocketServerLocation);
	connection.onopen = function() {
		connection.send('OK');
		connection.send("vars");
		document.getElementById("connection_status").value = "Connected";
	};
	connection.onclose = function() {
		setTimeout(function() {
			start(websocketServerLocation)
		}, 1000);
		document.getElementById("connection_status").value = "Disconnected";
	};

	connection.onmessage = function(e) {
		var res = e.data.split("~^`");
		if (res[0].toLowerCase() == "var") {
			connection.send("OK");
			for (i = 0; i < document.getElementsByName(res[1]).length; i++) {
				document.getElementsByName(res[1])[i].value = res[2];
			}

			return;
		}
		if (res[0].toLowerCase() == "varname") {
			connection.send("OK");
			document.getElementsByName("var" + res[1].toString())[0].value = res[2];
			return;
		}
		if (res[0].toLowerCase() == "code") {
			connection.send("OK");
			document.getElementById("lno").value = res[1];
			document.getElementById('code').value = res[1];
			return;
		}

		if (res[0].toLowerCase() == "print") {
			AddToBody('<hr>' + res[1]);
			connection.send("vars");
			return;
		}
		if (res[0].toLowerCase() == "gupdate") {
			document.getElementsByName('gra')[0].contentWindow.location.reload();
			return;
		}

		if (res[0].toLowerCase() == "guicls") {
			if (document.getElementById("app")) 
				{
				var bla = document.getElementById('app');
				bla.innerHTML = '';
				}
			else
				{
				document.body.innerHTML = '';
				}
			location.reload();
			return;
		}
		if (res[0].toLowerCase() == "wprint") {
			AddToBody(res[1]);
			connection.send("vars");
			return;
		} else if (res[0].toLowerCase() == "get") {
			var obj = document.getElementById(res[1]);
			if (obj == null) {
				connection.send("unknown object");
				return;
			}
			connection.send(obj.value);
			return;
		} else if (res[0].toLowerCase() == "log") {
			connection.send('OK');
			var log = document.getElementById("log");
			log.value = "\n" + res[1] + log.value;
			log.selectionStart = log.selectionEnd = log.value.length;
			return;
		} else if (res[0].toLowerCase() == "call") {
			connection.send('OK');
			eval(res[1] + '();');
			return;
		} else {
			// default 
			connection.send("KO");
		}
	};
}

function dcmdClick(e) {
	connection.send("cmd:" + e.id);
}

function cmdClick(e) {
	connection.send("guicmd:" + e.name);
}

function logClear() {
	document.getElementById('app').innerHTML ='';
	document.getElementById("log").value = '';
}

function objEvent(e) {
	connection.send("guievent:" + e.name + ":" + document.getElementById(e.id).value);
}

function objChange(e) {
	connection.send("guichange~" + e.name + "~" + document.getElementById(e.id).value);
}

var aliveme = setInterval(aliveTimer, 5000);

function aliveTimer() {
	connection.send("OK");
}

window.onload = function makeVarList() {
	for (xxx = 0; xxx < 101; xxx++) {
		add(xxx.toString(), "");
	}
	var arrayOfLines = localStorage.getItem("lastcode").split("\n");
	var sel = document.getElementById('code');
	for (i = 0; i < arrayOfLines.length; i++) 
	{ 
	  x = i + 1;
	  if (arrayOfLines[i] != "undefined")
	  {
		var opt = document.createElement('option');
		opt.innerHTML = pad('.....',i+1,true)+ ':    ' + arrayOfLines[i];
		opt.value = i+1;
		sel.appendChild(opt);
	  }
	}
}


function pad(pad, str, padLeft) {
  if (typeof str === 'undefined') 
	return pad;
  if (padLeft) {
	return (pad + str).slice(-pad.length);
  } else {
	return (str + pad).substring(0, pad.length);
  }
}

//Create text boxes for variables
function add(itemName, itemValue) {

	var element = document.createElement("input");
	element.setAttribute("type", "text");
	element.setAttribute("value", itemValue);
	element.setAttribute("name", "var" + itemName);
	element.setAttribute("style", "width:50%");

	var element1 = document.createElement("input");
	element1.setAttribute("type", "text");
	element1.setAttribute("value", itemValue);
	element1.setAttribute("name", itemName);
	element1.setAttribute("style", "width:50%");


	document.getElementById("fooBar").appendChild(element);
	document.getElementById("fooBar").appendChild(element1);
}


function removeSpecials(str) {
	var lower = str.toLowerCase();
	var upper = str.toUpperCase();

	var res = "";
	for(var i=0; i<lower.length; ++i) {
		if(lower[i] != upper[i] || lower[i].trim() === '')
			res += str[i];
	}
	return res;
}

function AddToBody(str){
if (document.getElementById("app")) 
	{
	var bla = document.getElementById('app');
	bla.innerHTML += str ;}
else
	{
	document.body.innerHTML +=str;
	}
}
)=====";






PROGMEM const char editCodeJavaScript[] =  R"=====(
function SaveTheCode() {
  if (typeof editor != "undefined") {editor.save();}
  var textArea = document.getElementById("code");
  localStorage.setItem("lastcode", textArea.value);
  var arrayOfLines = textArea.value.split("\n");
  httpGet("/codein?SaveTheCode=start&FileName="+document.getElementById("FileName").value);
  httpGet("/codein?SaveTheCode=yes");
  block = 0;
  document.getElementById("Status").value = "";
for (i = 0; i < arrayOfLines.length; i++) 
{ 
  x = i + 1;
  if (arrayOfLines[i] != "undefined")
  {
	stocca(encodeURIComponent(arrayOfLines[i]));
	
  }
}
stocca(">>-save_<<");
document.getElementById("Status").value = "Saved";
  httpGet("/codein?SaveTheCode=end");
alert("Saved");
}
var Sendy="";
var block = 0;
function stocca(s)
{
 Sendy += s;
 block++;
 if ((Sendy.length > 1024) || (s == ">>-save_<<"))
 {
  if (s == ">>-save_<<")
   Sendy = Sendy.substr(0,Sendy.length-10); 
   var WhatToSend = "/codein?line=" + block + "&code="+Sendy;
   httpGet(WhatToSend);
   Sendy = "";
 }
 else
 {
   Sendy += "%0D%0A";
 }
}
function httpGet(theUrl)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", theUrl, false ); // false for synchronous request
	xmlHttp.send( null );
	return xmlHttp.responseText;
}
function replaceAll(str, find, replace) {
  for (x = 0; x <= 10; x++) 
  {
	str = str.replace(find, replace);
  }
  return str;
}
)=====";


PROGMEM const char SettingsPageHTML[] =  R"=====(
<form action='settings' id="usrform">
*BasicVersion*
<style>div {width: 270px;text-align: right;}</style>
<div>
<b>Station Mode (Connect to router):</b>
<div>Name:<input type="text" name="staName" value="*sta name*"></div>
<div>Pass:<input type="password" name="staPass" value="*sta pass*"></div>
</div>
<div>
<b>Ap mode (brocast out its own ap):</b>
<div>Name:<input type="text" name="apName" value="*ap name*"></div>
<div>Pass (8 characters):<input type="password" name="apPass" value="*ap pass*"></div>
</div>
<div>
<b>IP (STA or AP mode):</b>
<div>IP address:<input type="text" name="ipaddress" value="*ipaddress*"></div>
<div>Subnet mask:<input type="text" name="subnetmask" value="*subnetmask*"></div>
<div>gateway:<input type="text" name="gateway" value="*gateway*"></div>
<div>HTTP port:<input type="text" name="listenport" value="*listenport*"></div>
<div>WS port:<input type="text" name="wsport" value="*wsport*"></div>
</div>
<div>
<div>Log In Key:<input type="password" name="LoginKey" value="*LoginKey*"></div>
<div>Menu bar Disable:<input type="checkbox" name="showMenueBar" value="off" **checked**></div>
<div>Run default.bas at startup:<input type="checkbox" name="autorun" value="on" **autorun**></div>
<div>OTA URL<input type="text" name="otaurl" value="*otaurl*"></div>
</div>
<div>
<input type="submit" value="Save" name="save">
<input type="submit" value="Format" name="format">
<input type="submit" value="Update" name="update">
<input type="submit" value="Restart" name="restart">
</div></form><br>)=====";







PROGMEM const char LogInPage[] =  R"=====(
<form action='settings' id="usrform">
Log In Key
<input type="password" name="key" value="">
<input type="submit" value="login" name="login">
</form>
)=====";


//Graphics HTML CODE
#if defined(BASIC_TFT)

PROGMEM const char GraphicsStartCode[] =  R"=====(<svg width="*wid*" height="*hei*">)=====";

PROGMEM const char GraphicsLineCode[] =  R"=====(<line x1="*x1*" y1="*y1*" x2="*x2*" y2="*y2*" stroke="*collor*"/>)=====";

PROGMEM const char GraphicsCircleCode[] =  R"=====(<circle cx="*x1*" cy="*y1*" r="*x2*" fill="*collor*"/>)=====";

PROGMEM const char GraphicsEllipseCode[] =  R"=====(<ellipse cx="*x1*" cy="*y1*" rx="*x2*" ry="*y2*" fill="*collor*"/>)=====";

PROGMEM const char GraphicsRectangleCode[] =  R"=====(<rect x="*x1*" y="*y1*" width="*x2*" height="*y2*" style="fill:*collor*"/>)=====";

PROGMEM const char GraphicsTextCode[] =  R"=====(<text x="*x1*" y="*y1*" style="fill:*collor*" transform="rotate(*x2* *x1*,*y1*)">*text*</text>)=====";
#endif


byte numberButtonInUse = 0;
//String ButtonsInUse[20];


bool NewGuiItemAddedSinceLastWait;
bool NewGraphicItemAddedSinceLastWait;

int  msgbranch;
String   MsgBranchRetrnData;

int UdpBranchLine = 0;
String  UdpBuffer = "";
IPAddress UdpRemoteIP;
int UdpRemotePort;


// place where the program will jump on serial data reception
int SerialBranchLine  = 0;
int Serial2BranchLine = 0;


// Buffer to store incoming commands from serial port
String inData;

const int TotalNumberOfLines = 5000;            //this is the maximum number of lines that can be saved/loaded; it can go until 65535
int  program_nb_lines = 0;                      //this is the number of program lines read from the file

int LastVarNumberLookedUp;                                 //Array to hold all of the basic variables
bool VariableLocated;

const int TotalNumberOfVariables = 100;
#define VariablesNameLength  30
#include "Classes.h"

bool RunningProgram = 1;                                //Will be set to 1 if the program is currently running
int RunningProgramCurrentLine = 0;                     //Keeps track of the currently running line of code
//byte NumberOfReturns;
bool BasicDebuggingOn;
int debugDelaySpeed;
//uint16_t ReturnLocations[254];

int TimerWaitTime;
int timerLastActiveTime;
int timercbLastActiveTime;
String TimerBranch;

String refreshBranch;

uint16_t GraphicsEliments[100][7];
String GraphicsText[100];

File fsUploadFile;

int noOfLinesForEdit;
String ProgramName = "/default.bas";

bool fileOpenFail;


bool inputPromptActive = 0;

int LoggedIn = 0;

int SerialTimeOut;


int TimerCBtime = 0;
int TimerCBBranchLine;

//uint16_t ForNextReturnLocations[255];



Servo servos[11];


String  PinListOfStatus[17];
int  PinListOfStatusValues[17];


//time Stff
int timezone = 3;
int dst = 0;



FSInfo fs_info;

void setup() {
  //  dht.begin();
  //  pixels.begin();
  SPIFFS.begin();
  Serial.begin(9600);
  // gets the listening port
  String listenport = LoadDataFromFile("listenport");
  String WebSocketslistenport = LoadDataFromFile("wsport");
  // listening port - by default goes to 80 -
  if (listenport.toInt() == 0) listenport = "80";
  if (WebSocketslistenport.toInt() == 0) {WebSocketslistenport = F("81"); SaveDataToFile(F("wsport"), WebSocketslistenport);}

  // print the listening port in the console
  Serial.println("HTTP Port: " + listenport);
  // create the instance of the web server
  server = new ESP8266WebServer(listenport.toInt());
  webSocket = new WebSocketsServer(WebSocketslistenport.toInt());
  
  

  //Serial.setDebugOutput(true);
  WiFi.setAutoConnect(false) ;
  WiFi.mode(WIFI_AP_STA);
  PrintAndWebOut(BasicVersion);
  PrintAndWebOut("MAC: " + WiFi.softAPmacAddress());
  //CheckWaitForRunningCode();
  
  MQTTclient.setCallback(MQTTcallback);


  server->on("/", []()
  {
	String WebOut;
	if (LoadDataFromFile("ShowMenueBar") != "off") WebOut =    AdminBarHTML;
	WebOut += RunningProgramGui();
	server->send(200, F("text/html"), WebOut);
  });


  server->on("/settings", []()
  {

	server->send(200, F("text/html"), SettingsPageHandeler());
  });


#if defined(BASIC_TFT)
  server->on("/vars", []()
  {
	String WebOut = AdminBarHTML;
	String FixSpaces;
	if ( CheckIfLoggedIn() )
	{
	  WebOut = LogInPage;
	}
	else
	{
	  WebOut += F("<div style='float: left;'>Variable Dump:");
	  for (int i = 0; i < TotalNumberOfVariables; i++)
	  {
		FixSpaces = AllMyVariables[i].getVar();
		FixSpaces.replace(' ' , char(160));
		if ( AllMyVariables[i].getName() != "") WebOut += String("<hr>" + AllMyVariables[i].getName() + " = " + (AllMyVariables[i].Format == PARSER_STRING ? "\"" : "") +
			  FixSpaces         + (AllMyVariables[i].Format == PARSER_STRING ? "\"" : "") );
	  }


	  WebOut += F("<hr></div><div style='float: right;'>Pin Stats");
	  for (byte i = 0; i <= 15; i++)
	  {
		if ( i < 6 | i > 11) WebOut += String("<hr>" + String(i) + " = " + PinListOfStatus[i] + "  , " + String(PinListOfStatusValues[i]));
	  }
	  WebOut += "</div>";
	}

	server->send(200, F("text/html"), WebOut);
  });
#endif

  server->on("/run", []()
  {
	clientTelnet.stop();
	BasicDebuggingOn = 0;
	telnetBranch = 0;
	String WebOut;
	RunningProgram = 1;
	RunningProgramCurrentLine = 0;
	WaitForTheInterpertersResponse = 0 ;
	numberButtonInUse = 0;
	HTMLout = "";
	TimerWaitTime = 0;
	TimerCBtime = 0;
	GraphicsEliments[0][0] = 0;
	WebOut = F(R"=====(  <meta http-equiv="refresh" content="0; url=./input?" />)=====");

	clear_stacks();
	server->send(200, F("text/html"), WebOut);
  });

  #if defined(BASIC_TFT)
  server->on("/debug", []()
  {
	clientTelnet.stop();
	BasicDebuggingOn = 1;
	telnetBranch = 0;
	String WebOut;
	RunningProgramCurrentLine = 0;
	WaitForTheInterpertersResponse = 0 ;
	numberButtonInUse = 0;
	HTMLout = "";
	TimerWaitTime = 0;
	TimerCBtime = 0;
	GraphicsEliments[0][0] = 0;
	WebOut = F(R"=====(  <meta http-equiv="refresh" content="0; url=./" />)=====");

	clear_stacks();
	server->send(200, F("text/html"), WebOut);

  });
  #endif
  
  

  server->on("/wsport.js", []()
  {
	server->send(200, F("text/html"), String("start('ws://' + location.hostname + ':" + LoadDataFromFile("wsport") + "/');"));
  });

  
  
  
  
#if defined(BASIC_TFT)
  server->on("/graphics.htm", []()
  {
	server->send(200, F("text/html"), BasicGraphics());
  });
#endif

  server->onFileUpload(handleFileUpdate);

  server->on("/filemng", []()
  {
	DoSomeFileManagerCode();
  });
  
  #if defined(BASIC_TFT)
  server->on("/filmanws.js", []()
  {
	server->send(200, F("text/html"), FIleManWSJS);
  });
  #endif

  server->on("/edit", []()
  {
	WebGuiOff = 2;
	String WebOut;
	if (CheckIfLoggedIn())
	{
	  WebOut = String(LogInPage);
	  server->send(200, F("text/html"), String(AdminBarHTML + WebOut ));
	  WebGuiOff = 0;return;
	}
	else
	{
	  String CRLF = F("\r\n");
	  WaitForTheInterpertersResponse = 1;

	  String TextboxProgramBeingEdited;
	  //String ProgramName;
	  //WebOut = String("<form action='input'>" + HTMLout + "</form>");
	  WebOut = String(EditorPageHTML);

	  if ( server->arg("open") == F("Open") )
	  {
		// really takes just the name for the new file otherwise it uses the previous one
		ProgramName = GetRidOfurlCharacters(server->arg("name"));

		ProgramName.trim();
		if (ProgramName == "")
		{
		  ProgramName = F("/default.bas");
		}
		ProgramName  = MakeSureFileNameStartsWithSlash(ProgramName );
		LoadBasicProgramFromFlash( ProgramName);
	  }
	  // the goal here is to replace the server send function by an equivalent that
	  // permit to handle big buffers; this is acheived using the "chunked transfer"
	  WebOut = String(EditorPageHTML);
	  WebOut = WebOut.substring(0, WebOut.indexOf(F("*program txt*")) );
	  WebOut.replace(F("*program name*"), ProgramName);

	  server->sendContent(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection:close\r\nTransfer-Encoding: chunked\r\nAccess-Control-Allow-Origin: *\r\n\r\n"));
	  delay(0);
	  Serial.println(F("start sending"));
	  // each "chunk" is composed of :
	  // the size of the block (in hex) terminated by \r\n
	  server->sendContent(String(String(AdminBarHTML).length(), 16) + CRLF);
	  // the block terminated by \r\n
	  server->sendContent(String(AdminBarHTML) + CRLF);
	  /////// end of first chunk ///////////
	  delay(0);
	  server->sendContent(String(WebOut.length(), 16) + CRLF);
	  server->sendContent(WebOut + CRLF);
	  delay(0);
	  int iii;
	  int i;
	  fileOpenFail = 0;
	  TextboxProgramBeingEdited = BasicProgram(1);
	  for (i = 2; (i < program_nb_lines); i++)
	  {
		TextboxProgramBeingEdited = TextboxProgramBeingEdited + "\n" + BasicProgram(i);

		if (TextboxProgramBeingEdited.length() > 2048)
		{
		  server->sendContent(String(TextboxProgramBeingEdited.length(), 16) + CRLF);
		  server->sendContent(TextboxProgramBeingEdited + CRLF);
		  TextboxProgramBeingEdited = "";
		  delay(0);
		}
	  }
	  if (TextboxProgramBeingEdited.length() > 0)
	  {
		server->sendContent(String(TextboxProgramBeingEdited.length(), 16) + CRLF);
		server->sendContent(TextboxProgramBeingEdited + CRLF);
		delay(0);
	  }

	  WebOut = String(EditorPageHTML);
	  WebOut = WebOut.substring(WebOut.indexOf(F("</textarea>")));
	  server->sendContent(String(WebOut.length(), 16) + CRLF);
	  server->sendContent(WebOut + CRLF);
	  // end of transmission
	  server->sendContent(F("0\r\n\r\n"));
	  server->sendContent(F("0\r\n\r\n"));
	  delay(0);
	  Serial.println(F("End of Open"));
	  WebGuiOff = 0;
	}

  });



  server->on("/editor.js", []() {
	server->send(200, F("text/html"), editCodeJavaScript);
  });



  server->on("/WebSockets.js", []() {
	server->send(200, F("text/html"), WebSocketsJS);
  });



  server->on("/codein", []() {
	//    ProgramName.trim();
	//    if (ProgramName == "")
	//    {
	//      ProgramName = F("/default.bas");
	//    }

	if (server->arg("SaveTheCode") == F("start"))
	{
	  inData = "end";
	  ExicuteTheCurrentLine();
	  HaltBasic("");
	  Serial.println(F("start save"));
	  ProgramName = GetRidOfurlCharacters(server->arg("FileName"));
	  if (ProgramName == "")ProgramName = F("/default.bas");
	  ProgramName.trim();
	  if (ProgramName[0] != '/') ProgramName = "/" + ProgramName;
	  OpenToWriteOnFlash( ProgramName );
	}

	if (server->arg("SaveTheCode") != F("yes") & server->arg("SaveTheCode") != F("start") & server->arg("SaveTheCode") != F("end"))
	{
	  String LineNoForWebEditorIn;
	  LineNoForWebEditorIn = server->arg("line");
	  int y = LineNoForWebEditorIn.toInt();
	  delay(0);
	  //Serial.println(server->arg("code"));
	  Serial.println(ProgramName + F("/") + String(y));
	  //BasicProgramWriteLine(y, GetRidOfurlCharacters(server->arg("code")));
	  WriteBasicLineOnFlash(GetRidOfurlCharacters(server->arg("code")));
	  delay(0);
	  noOfLinesForEdit = y;

	} 

	if (server->arg("SaveTheCode") == F("end"))
	{
	  // terminate the save
	  Serial.println(F("end of save!!"));
	  CloseWriteOnFlash();
	  HaltBasic("");
	  LoadBasicProgramFromFlash( ProgramName );
	}

	if (server->arg("SaveTheCode") == F("yes"))
	{
		HaltBasic("");
	}
	server->send(200, F("text/html"), F("good"));
  });





  server->on("/msg", []() {

	MsgBranchRetrnData = F("No MSG Branch Defined");

	if (msgbranch != 0)
	{
		byte oldWebGuiOff;
		oldWebGuiOff = WebGuiOff;
		Serial.println(msgbranch);
		//WebGuiOff = 1;
		RunningProgramCurrentLine = msgbranch;
		WaitForTheInterpertersResponse = 0;
		runTillWaitPart2();
		WebGuiOff = oldWebGuiOff;
	}

	server->send(200, F("text/html"), MsgBranchRetrnData);
  });





  server->on("/input", []() {
	server->send(200, F("text/html"), RunningProgramGui());
  });

  server->onNotFound ( []() {
	String fileNameToServeUp;
	fileNameToServeUp = GetRidOfurlCharacters(server->arg("file"));
	File mySuperFile = SPIFFS.open(String(F("/uploads/")) + fileNameToServeUp, "r");
	if (mySuperFile)
	{
	  server->streamFile(mySuperFile, getContentType(fileNameToServeUp));
	  //server->send(200, getContentType(fileNameToServeUp), mySuperFile.readString());

	}
	else
	{
	  server->send(200, F("text/html"), RunningProgramGui());
	}
	mySuperFile.close();
  });

  //LoadBasicProgramFromFlash("");


  if (  ConnectToTheWIFI(LoadDataFromFile("WIFIname"), LoadDataFromFile("WIFIpass"), LoadDataFromFile("ipaddress"), LoadDataFromFile("gateway"), LoadDataFromFile("subnetmask")) == 0)
  {
	if (LoadDataFromFile("APname") == "")
	{
	  CreateAP("", "", LoadDataFromFile("ipaddress"), LoadDataFromFile("gateway"), LoadDataFromFile("subnetmask"));
	}
	else
	{
	  CreateAP(LoadDataFromFile("APname"), LoadDataFromFile("APpass"), LoadDataFromFile("ipaddress"), LoadDataFromFile("gateway"), LoadDataFromFile("subnetmask"));
	}
  }


  Wire.begin(0, 2);

  //  keyboard.begin(14, 12); //For PS2 keyboard input
 #if defined(BASIC_TFT)
  StartUp_OLED();
  display.init();
  lcd.begin(16, 2); // initialize the lcd for 16 chars 2 lines and turn on backlight
#endif
  sensors.begin();

  if (!SPIFFS.exists(ProgramName))
  {
	OpenToWriteOnFlash(ProgramName);
	delay(0);
	WriteBasicLineOnFlash("  ");
	CloseWriteOnFlash();
  }
  
  
  LoadBasicProgramFromFlash( ProgramName);

  server->begin();
  httpUpdater.setup(server);
  RunningProgram = 0;
  WaitForTheInterpertersResponse = 1;
  StartUpProgramTimer();
  InitCommandParser(); // init the commands parser

  // start webSocket server
  webSocket->begin();
  webSocket->onEvent(webSocketEvent);
}


String SettingsPageHandeler()
{
  if ( server->arg("key") == LoadDataFromFile("LoginKey"))
  {
	LoggedIn = millis();
  }


  WaitForTheInterpertersResponse = 1;
  String WebOut = String(AdminBarHTML) + String(SettingsPageHTML);
  String staName = LoadDataFromFile(F("WIFIname"));
  String staPass = LoadDataFromFile(F("WIFIpass"));
  String apName = LoadDataFromFile(F("APname"));
  String apPass = LoadDataFromFile(F("APpass"));
  String LoginKey = LoadDataFromFile(F("LoginKey"));
  String ShowMenueBar = LoadDataFromFile(F("ShowMenueBar")); 
  String otaUrl = LoadDataFromFile(F("otaUrl"));
  String autorun = LoadDataFromFile(F("autorun"));
  String listenport = LoadDataFromFile(F("listenport"));
  String wsport = LoadDataFromFile(F("wsport"));
  String ipaddress = LoadDataFromFile(F("ipaddress"));
  String subnetmask = LoadDataFromFile(F("subnetmask"));
  String gateway = LoadDataFromFile(F("gateway"));
  
  
  

  // listening port - by default goes to 80 -
  if (listenport.toInt() == 0)
	listenport = F("80");

  //Serial.print("Loading Settings Files");

  if (millis() > LoggedIn + 600000 || LoggedIn == 0 )
  {
	WebOut = LogInPage;
  }
  else
  {

	if ( server->arg("restart") == F("Restart") ) ESP.restart();

    #if defined(BASIC_TFT)
	if ( server->arg("update") == F("Update") )
	{

	  //        Serial.println(BasicOTAupgrade());
	  if (LoadDataFromFile("otaUrl") == "")
	  {
		t_httpUpdate_return  ret = ESPhttpUpdate.update(F("esp8266basic.smbisoft.com"), 80, F("/4M/ESP8266Basic.cpp.bin"));
		if (ret == HTTP_UPDATE_FAILED ) Serial.println(F("Update failed"));
	  }
	  else
	  {
		String URLtoGet = LoadDataFromFile("otaUrl");
		String ServerToConnectTo;
		String PageToGet;
		ServerToConnectTo = URLtoGet.substring(0, URLtoGet.indexOf("/"));
		PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));
		t_httpUpdate_return  ret = ESPhttpUpdate.update(ServerToConnectTo, 80, PageToGet);
		if (ret == HTTP_UPDATE_FAILED ) Serial.println(F("Update failed"));
	  }
	  //t_httpUpdate_return  ret = ESPhttpUpdate.update("cdn.rawgit.com", 80, "/esp8266/Basic/master/Flasher/Build/4M/ESP8266Basic.cpp.bin");

	}
    #endif

	if ( server->arg("save") == F("Save") )
	{
	  staName      = GetRidOfurlCharacters(server->arg("staName"));
	  staPass      = GetRidOfurlCharacters(server->arg("staPass"));
	  apName       = GetRidOfurlCharacters(server->arg("apName"));
	  apPass       = GetRidOfurlCharacters(server->arg("apPass"));
	  LoginKey     = GetRidOfurlCharacters(server->arg("LoginKey"));
	  ShowMenueBar = GetRidOfurlCharacters(server->arg("showMenueBar"));
	  otaUrl       = GetRidOfurlCharacters(server->arg("otaurl"));
	  autorun      = GetRidOfurlCharacters(server->arg("autorun"));
	  ipaddress   = GetRidOfurlCharacters(server->arg("ipaddress"));
	  subnetmask   = GetRidOfurlCharacters(server->arg("subnetmask"));
	  gateway   = GetRidOfurlCharacters(server->arg("gateway"));
	  listenport   = GetRidOfurlCharacters(server->arg("listenport"));
	  wsport   = GetRidOfurlCharacters(server->arg("wsport"));

	  SaveDataToFile("WIFIname" , staName);
	  SaveDataToFile("WIFIpass" , staPass);
	  SaveDataToFile("APname" , apName);
	  SaveDataToFile("APpass" , apPass);
	  SaveDataToFile("LoginKey" , LoginKey);
	  SaveDataToFile("ShowMenueBar" , ShowMenueBar);
	  SaveDataToFile("otaUrl" , otaUrl);
	  SaveDataToFile("autorun" , autorun);
	  SaveDataToFile("ipaddress" , ipaddress);
	  SaveDataToFile("subnetmask" , subnetmask);
	  SaveDataToFile("gateway" , gateway);
	  SaveDataToFile("listenport" , listenport);
	  SaveDataToFile("wsport" , wsport);

	}

	if ( server->arg("format") == F("Format") )
	{
	  // BasicFileOpened.close();
	  Serial.println(F("Formating "));
	  Serial.print(SPIFFS.format());
	}

	WebOut.replace(F("*sta name*"), staName);
	WebOut.replace(F("*sta pass*"), staPass);
	WebOut.replace(F("*ap name*"),  apName);
	WebOut.replace(F("*ap pass*"),  apPass);
	WebOut.replace(F("*LoginKey*"), LoginKey);
	WebOut.replace(F("*BasicVersion*"), BasicVersion);
	WebOut.replace(F("*otaurl*"), otaUrl);

	WebOut.replace(F("*ipaddress*"), ipaddress);
	WebOut.replace(F("*subnetmask*"), subnetmask);
	WebOut.replace(F("*gateway*"), gateway);    
	WebOut.replace(F("*listenport*"), listenport);
	WebOut.replace(F("*wsport*"), wsport);

	if ( ShowMenueBar == F("off"))
	{
	  WebOut.replace(F("**checked**"), F("checked"));
	}
	else
	{
	  WebOut.replace(F("**checked**"), "");
	}

	if ( autorun == F("on"))
	{
	  WebOut.replace(F("**autorun**"), F("checked"));
	}
	else
	{
	  WebOut.replace(F("**autorun**"), "");
	}

  }
  
  return WebOut;
}



String getContentType(String filename) {
  String ret = F("text/plain");
  if (filename.endsWith(".gz"))
  {
	ret = F("application/x-gzip");
	filename = filename.substring(0, filename.length()-3);
	Serial.println("getcontenttype " + filename);
  }
  
	   if (filename.endsWith(".htm"))  return F("text/html");
  else if (filename.endsWith(".html")) return F("text/html");
  else if (filename.endsWith(".css"))  return F("text/css");
  else if (filename.endsWith(".js"))   return F("application/javascript");
  else if (filename.endsWith(".png"))  return F("image/png");
  else if (filename.endsWith(".gif"))  return F("image/gif");
  else if (filename.endsWith(".jpg"))  return F("image/jpeg");
  else if (filename.endsWith(".ico"))  return F("image/x-icon");
  else if (filename.endsWith(".xml"))  return F("text/xml");
  else if (filename.endsWith(".pdf"))  return F("application/x-pdf");
  else if (filename.endsWith(".zip"))  return F("application/x-zip");
  else if (filename.endsWith(".gz"))   return F("application/x-gzip");
  return ret;
}

void StartUpProgramTimer()
{
  int Startuptimer  = LoadDataFromFile("starttimer").toInt();
  if (Startuptimer == 0) Startuptimer = 30;
  pinMode(0, INPUT_PULLUP);  // set GPIO0 to input with pullup; so if float should read 1, 0 on ground
  while  (millis() < Startuptimer * 1000)
  {
	delay(0);
	//Serial.println(millis());
	server->handleClient();
	if (WaitForTheInterpertersResponse == 0) return;

	// MOD cicciocb April 2016
	// if the pin GPIO0 is taken to GND, the program will not start
	if (digitalRead(0) == 0)  return; // if the pin GPIO0 is at GND , stop the autorun
  }
  if (LoadDataFromFile("autorun") != "on")  return; // if the autorun option is disabled, returns
  Serial.println(F("Starting Default Program"));
  RunningProgram = 1;
  RunningProgramCurrentLine = 0;
  WaitForTheInterpertersResponse = 0 ;
  numberButtonInUse = 0;
  HTMLout = "";

  clear_stacks();
  return;
}



void DoSomeFileManagerCode()
{
  String WholeUploadPage = UploadPage;
  String FileListForPage ;

  if (CheckIfLoggedIn())
  {
	WholeUploadPage = LogInPage;
  }
  else
  {
	if (server->arg("Delete") != "")
	{
	  String FIleNameForDelete = server->arg("fileName");
	  FIleNameForDelete = GetRidOfurlCharacters(FIleNameForDelete);
	  Serial.println(FIleNameForDelete);
	  Serial.println(SPIFFS.remove(FIleNameForDelete));
	  //Serial.println(SPIFFS.remove("uploads/settings.png"));
	}

#if !defined(BASIC_TFT)
	Dir dir = SPIFFS.openDir(String(F("/") ));
	while (dir.next()) {
	  FileListForPage += String(F("<option>")) + dir.fileName() + String(F("</option>"));
	  delay(0);
	}

	WholeUploadPage.replace("*table*", FileListForPage);
#endif

	if (server->arg("View") != "")
	{
	  String FileNameToView = server->arg("fileName");
	  FileNameToView = GetRidOfurlCharacters(FileNameToView);
	  FileNameToView.replace("/uploads/", "");
	  WholeUploadPage = F(R"=====(  <meta http-equiv="refresh" content="0; url=./file?file=item" />)=====");
	  WholeUploadPage.replace("item", FileNameToView);
	}


	if (server->arg("Edit") != "")
	{
	  String FileNameToView = server->arg("fileName");
	  FileNameToView = GetRidOfurlCharacters(FileNameToView);
	  //FileNameToView.replace("/uploads/", "");
	  WholeUploadPage = F(R"=====(  <meta http-equiv="refresh" content="1; url=./edit?name=item&open=Open" />)=====");
	  WholeUploadPage.replace("item", FileNameToView);
	}

	if (server->arg("Rename") != "")
	{
	  String FileNameToView = server->arg("fileName");
	  FileNameToView = GetRidOfurlCharacters(FileNameToView);
	  String newfileName = server->arg("newfileName");
	  newfileName  = GetRidOfurlCharacters(newfileName );
	  WholeUploadPage = F(R"=====(<form id="filelist">Old Name<br><input type="text" name="fileName" value="*item name*"><br>New Name<br><input type="text" name="newfileName" value="*item name*"><input type="submit" value="NewName" name="Rename"></form>)=====");
	  WholeUploadPage.replace("*item name*", FileNameToView);
	  if (newfileName != "" )
	  {
		newfileName = MakeSureFileNameStartsWithSlash(newfileName);
		WholeUploadPage = F(R"=====(  <meta http-equiv="refresh" content="0; url=./filemng" />)=====");
		SPIFFS.rename(FileNameToView , newfileName);
	  }
	}
  }
  server->send(200, F("text/html"),  String( AdminBarHTML + WholeUploadPage ));
}



void handleFileUpdate()
{
  //if (server->uri() != "/edit") return;
  HTTPUpload& upload = server->upload();
  if (upload.status == UPLOAD_FILE_START) {
	String filename = upload.filename;
	//DBG_OUTPUT_PORT.print("Upload Name: "); DBG_OUTPUT_PORT.println(filename);
	fsUploadFile = SPIFFS.open(String("/uploads/" + filename), "w");
	filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
	//DBG_OUTPUT_PORT.print("Upload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
	if (fsUploadFile)
	  fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
	if (fsUploadFile)
	  fsUploadFile.close();
	//DBG_OUTPUT_PORT.print("Upload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}








String  getSerialInput()
{
  unsigned long  serialTimeOutStart = millis();
  bool donereceivinginfo = 0;
  Serial.println(">");

  String someInput;
  while (donereceivinginfo == 0)
  {
	if (serialTimeOutStart + SerialTimeOut < millis() & SerialTimeOut != 0) return someInput;
	delay(0);
	while (Serial.available() > 0)
	{
	  char recieved = Serial.read();
	  // Process message when new line character is recieved
	  if (recieved == '\n')
	  {
		Serial.println(someInput);
		donereceivinginfo = 1;
		return someInput;
	  }
	  someInput += recieved;
	}
  }
}






//String GetPS2input()
//{
//  String PS2inputString;
//
//
//  bool donereceivinginfo = 0;
//
//  while (donereceivinginfo == 0)
//  {
//    delay(0);
//    while (keyboard.available())
//    {
//      delay(0);
//      // read the next key
//      char c = keyboard.read();
//
//      // check for some of the special keys
//      if (c == PS2_ENTER) {
//        return PS2inputString;
//      } else if (c == PS2_TAB) {
//        //Serial.print("[Tab]");
//      } else if (c == PS2_ESC) {
//        return "";
//      } else if (c == PS2_PAGEDOWN) {
//        //Serial.print("[PgDn]");
//      } else if (c == PS2_PAGEUP) {
//        //Serial.print("[PgUp]");
//      } else if (c == PS2_LEFTARROW) {
//        //Serial.print("[Left]");
//      } else if (c == PS2_RIGHTARROW) {
//        //Serial.print("[Right]");
//      } else if (c == PS2_UPARROW) {
//        //Serial.print("[Up]");
//      } else if (c == PS2_DOWNARROW) {
//        //Serial.print("[Down]");
//      } else if (c == PS2_DELETE) {
//        //Serial.print("[Del]");
//      } else {
//        if (BasicDebuggingOn == 1) Serial.print(c);
//        PS2inputString += c;
//      }
//    }
//  }
//}






bool CheckIfLoggedIn()
{
  if (LoadDataFromFile("LoginKey") != "")
  {
	if ( millis() > LoggedIn + 600000 || LoggedIn == 0 )     return 1;
  }
  return 0;
}










void loop()
{

  RunBasicTillWait();
  delay(0);
  if (delaytime == 0)server->handleClient();
  webSocket->loop();
  
  if (MQTTActivated)
  { 
	  if (!MQTTclient.connected()) {
	  	MQTTreconnect();
	  }
	  MQTTclient.loop();
	  //Serial.println("Checking mqtt");
	  //MQTTclient.loop();
	  if (MQTTnewMsgReceived == 1 & MQTTBranch != "")
	  {     
			MQTTnewMsgReceived = 0;
		    inData = String(" goto " + MQTTBranch + " ");
			WaitForTheInterpertersResponse = 0;
			ExicuteTheCurrentLine();
			runTillWaitPart2();
	  } 
  }
  
  
  
}



void RunBasicTillWait()
{
  webSocket->loop();
  if (delaytime > millis() && delaytime != 0) {return;} else {delaytime = 0;}
  runTillWaitPart2();
  if (RunningProgramCurrentLine > TotalNumberOfLines)
  {
	inData = String(" end");
	WaitForTheInterpertersResponse = 0;
	ExicuteTheCurrentLine();
	return;
  }



  
  
  if (TimerWaitTime + timerLastActiveTime <= millis() &&  TimerWaitTime != 0  & WaitForTheInterpertersResponse )
  {
	inData = String(" goto " + TimerBranch + " ");
	WaitForTheInterpertersResponse = 0;
	timerLastActiveTime = millis() ;
	ExicuteTheCurrentLine();
	runTillWaitPart2();
  }
  if (telnetBranch != 0 &&  clientTelnet.available())
  {
	RunningProgramCurrentLine = telnetBranch;
	WaitForTheInterpertersResponse = 0;
	runTillWaitPart2();
  }

  
  
  
  
  delay(0);
  for (int pinnn = 0; pinnn <= 15 ; pinnn++)
  {
	delay(0);
	//Serial.println(pinnn);
	if ((PinListOfStatus[pinnn] != "po") & ( PinListOfStatus[pinnn] != "pi") & (PinListOfStatus[pinnn] != "pwi") & (PinListOfStatus[pinnn] != "pwo") & (PinListOfStatus[pinnn] != "servo") & ( PinListOfStatus[pinnn] != ""))
	{
	  //Serial.println("Foud interupt pin");
	  if ( PinListOfStatusValues[pinnn] != UniversalPinIO("pi", String(pinnn), 0))
	  {
		inData = String(" goto " + PinListOfStatus[pinnn] + " ");
		WaitForTheInterpertersResponse = 0;
		//Serial.println(PinListOfStatus[pinnn]);
		ExicuteTheCurrentLine();
		runTillWaitPart2();
	  }
	}
  }
}


void runTillWaitPart2()
{
  
  if (NewGuiItemAddedSinceLastWait) WebSocketSend(  "guicls");NewGuiItemAddedSinceLastWait = 0;
  if (NewGraphicItemAddedSinceLastWait) WebSocketSend("gupdate");NewGraphicItemAddedSinceLastWait = 0;

  //Serial.println("Delay time" + String(delaytime));
  //Serial.println("CUrent Line" + String(RunningProgramCurrentLine));
  //Serial.println("Wait = " + String(WaitForTheInterpertersResponse));
  while (RunningProgram == 1 && RunningProgramCurrentLine < TotalNumberOfLines && WaitForTheInterpertersResponse == 0 && delaytime == 0)
  {
	//Serial.println(inData);

	delay(0);
	RunningProgramCurrentLine++;
	inData = BasicProgram(RunningProgramCurrentLine);
	inData = StripCommentsFromLine(inData);
	if (fileOpenFail == 1) inData  = "end";
	ExicuteTheCurrentLine();
	delay(0);
	CheckForUdpData();
	webSocket->loop();
  }
  if (RunningProgram == 1 && RunningProgramCurrentLine < TotalNumberOfLines && WaitForTheInterpertersResponse == 1 )
  {
	//Serial.print("sto in wait ");
	//Serial.println(RunningProgramCurrentLine);
	CheckForUdpData();
	webSocket->loop();
  }


}

String StripCommentsFromLine(String ret)
{
  // comment handling stuff
  // we try to remove all the comment from the line
  // the comment is defined as all the text following the ' (single quote) until the end of the line
  // the problem is that the ' can be also inside a string (text included between quotes " )
  // so we need to understand if the ' is inside quotes or not
  // let's go
  bool quotes = false;
  for (int i = 0; i < ret.length(); i++)
  {
	if (ret[i] == '"')
	  quotes = !quotes;

	if ( (ret[i] == '\'') && (quotes == false) )
	{
	  ret = ret.substring(0, i);  // cut the line at the current position (removes all the comments from the line
	  break;
	}
  }
  return ret;
}

void CheckForUdpData()
{
  int numBytes = udp.parsePacket();
  if ( numBytes)
  {
	//    Serial.print("Packet received ");
	//    Serial.print(RunningProgramCurrentLine);
	//    Serial.print("  ");
	//    Serial.println(udp.available());

	if (numBytes > 0)
	{
	  char Buffer[numBytes + 1];
	  UdpRemoteIP = udp.remoteIP();
	  UdpRemotePort = udp.remotePort();
	  delay(0);
	  udp.read(Buffer, numBytes);
	  Buffer[numBytes] = '\0'; // terminate the string with '\0'
	  UdpBuffer = String(Buffer);
	}

	//// test of gosub ///////
	if (UdpBranchLine > 0)
	{
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = UdpBranchLine + 1; // gosub after the udpbranch label
	  UdpBranchLine = - UdpBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  ////
  if (Serial.available() > 0)
  {
	delay(50); // insure that the data can be received
	//// put like a gosub ///////
	if (SerialBranchLine > 0)
	{
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = SerialBranchLine + 1; // gosub after the SerialBranch label
	  SerialBranchLine = - SerialBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  if ((swSer != NULL))
  {
	if (swSer->available() > 0)
	{
	  delay(50); // insure that the data can be received
	  //// put like a gosub ///////
	  if (Serial2BranchLine > 0)
	  {
		// if the program is in wait, it returns to the previous line to wait again
		return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
		WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
		RunningProgramCurrentLine = Serial2BranchLine + 1; // gosub after the SerialBranch label
		Serial2BranchLine = - Serial2BranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	  }
	}
  }
  if (irrecv->decode(&IRresults)) {
	//Serial.println(IRresults.value, HEX);
	//dump(&IRresults);
	//irrecv->resume(); // Receive the next value
	if (IRBranchLine > 0)
	{
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = IRBranchLine + 1; // gosub after the IRBranch label
	  IRBranchLine = - IRBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  if (TimerCBtime > 0)
  {
	if (millis() > (timercbLastActiveTime + TimerCBtime) )
	{
	  timercbLastActiveTime = millis();
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = TimerCBBranchLine + 1; // gosub after the IRBranch label
	  TimerCBBranchLine = - TimerCBBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  #if defined(BASIC_TFT)
  if (TouchBranchLine > 0)
  {
	if ( millis() > (Touch_millis + 100) ) 
	{
	  int raw;
	  int tt = ReadTouchXY(0, &raw);
	  if ((tt != -1) && (Touch_p == -1))
	  {
//        Serial.print(tt & 0xffff);
//        Serial.print(":");
//        Serial.print(tt >> 16);
//        Serial.print(" ");
//        Serial.print(raw & 0xffff);
//        Serial.print(":");
//        Serial.print(raw >> 16);
//        Serial.print(" ");
//        Serial.println(Touch_p);
		touchX = tt & 0xffff;
		touchY = tt >> 16;
		touchY_raw = raw & 0xffff;
		touchY_raw = raw >> 16;
		// if the program is in wait, it returns to the previous line to wait again
		return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
		WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
		RunningProgramCurrentLine = TouchBranchLine + 1; // gosub after the IRBranch label
		TouchBranchLine = - TouchBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command  
	  }
	  delay(0);
	  Touch_millis = millis();
	  Touch_p = tt;
	}
  }  
  #endif
}

String getValueforPrograming(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
	0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
	if (data.charAt(i) == separator || i == maxIndex) {
	  found++;
	  strIndex[0] = strIndex[1] + 1;
	  strIndex[1] = (i == maxIndex) ? i + 1 : i;
	}
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}




String getValue(String data, char separator, int index)
{
  data = String(data + String(separator)+ String(separator));
  int maxIndex = data.length() - 1;
  int j = 0;
  byte WaitingForQuote;
  String chunkVal = "";
  String ChunkReturnVal;
  for (int i = 0; i <= maxIndex && j <= index; i++)
  {
	if (data[i] == '\"' )
	{
	  i++;
	  while (i <= maxIndex && data[i] != '\"' ) {
		chunkVal.concat(data[i]);
		i++;
		delay(0);
	  }
	}
	else if (data[i] == '|' )
	{
	  i++;
	  while (i <= maxIndex && data[i] != '|' ) {
		chunkVal.concat(data[i]);
		i++;
		delay(0);
	  }
	}
	else
	{
	  if (data[i] != separator) chunkVal.concat(data[i]);
	}

	if (data[i] == separator & data[i - 1] != separator)
	{
	  j++;
	  if (j > index)
	  {
		//chunkVal.trim();
		if (chunkVal != String(separator))
		{
		  ChunkReturnVal = chunkVal;
		  break;
		}
	  }
	  chunkVal = "";
	}
  }
  //    Serial.println("index");
  //    Serial.println(index);
  //    Serial.println(j);

  if (j == index + 1)
  {

	return ChunkReturnVal;
  }
}




String FetchOpenWeatherMapApi(String URLtoGet, String index)
{
  String ServerToConnectTo = URLtoGet.substring(0, URLtoGet.indexOf("/"));
  String PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));

  String s = "";
  char c;
  int cnt = 0;
  int phase = 0;
  int graffe = 0;

  const char lookforLIST[] = "\"list\":[";
  const char lookforBEGIN[] = "{\"";
  char ptr = 0;

  int list = index.toInt();

  if (list == 0)  // if the index is 0, it takes the first part, the root
	phase = 3;

  if (client.connect(ServerToConnectTo.c_str() , 80))
  {
	client.print(String("GET " + PageToGet + " HTTP/1.1\r\nHost: " +  ServerToConnectTo + "\r\n\r\n"));
	delay(300);

	while (client.available())
	{
	  delay(0);
	  //delay(1);
	  c = client.read();
	  delay(0);
	  //Serial.print(c);
	  switch (phase)
	  {
		case 0:
		  if (c == lookforLIST[ptr])
			ptr++;
		  else
			ptr = 0;

		  if (ptr == strlen(lookforLIST))
		  {
			phase = 1;
			list = list - 1;
			if (list == 0)
			{
			  phase = 2;
			}
			//Serial.println("phase 0 OK");
		  }
		  break;

		case 1:
		  if (c == '{')
			graffe++;
		  else if (c == '}')
			graffe--;

		  if (graffe == 0)
		  {
			if ( (c == ',') || (c == ']') )
			  list--;
		  }
		  if (list == 0)
		  {
			phase = 2;
			//Serial.println("phase 1 OK");
		  }
		  break;

		case 2:
		  s.concat(c);
		  cnt++;
		  if (c == '{')
			graffe++;
		  else if (c == '}')
			graffe--;

		  if ((graffe == 0) || (cnt > 600)) // max 600 chars
		  {
			//Serial.println("phase 2 OK");
			client.stop();
			return s;
		  }

		  break;

		case 3:    // search the beginning of the message starting with  {"
		  if (c == lookforBEGIN[ptr])
			ptr++;
		  else
			ptr = 0;

		  if (ptr == strlen(lookforBEGIN))
		  {
			cnt = 2;
			phase = 4;
			//Serial.println("phase 3 OK");
			s = F("{\"");
		  }
		  break;

		case 4:
		  s.concat(c);
		  cnt++;
		  if (c == lookforLIST[ptr])
			ptr++;
		  else
			ptr = 0;

		  if ( (ptr == strlen(lookforLIST)) || (cnt > 1000)) // max 1000 chars
		  {
			//Serial.println("phase 4 OK");
			client.stop();
			s.concat("]}");
			return s;
		  }
		  break;
	  }

	  if (client.available() == false)
	  {
		// if no data, wait for 300ms hoping that new data arrive
		delay(300);
	  }

	}//while
	client.stop();
	return F("END OF DATA REACHED");

  }
  client.stop();
  return "";
}








String FetchWebUrl(String URLtoGet, int PortNoForPage)
{
  if (PortNoForPage == 0) PortNoForPage = 80;
  String str = "             ";
  String ServerToConnectTo = URLtoGet.substring(0, URLtoGet.indexOf("/"));
  String PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));
  byte numberOwebTries = 0;
  // ServerToConnectTo ;
  //PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));

  //  Serial.println(ServerToConnectTo);
  //  Serial.println(PageToGet);


  if (client.connect(ServerToConnectTo.c_str() , PortNoForPage))
  {
	client.print(String("GET " + PageToGet + " HTTP/1.1\r\nHost: " +  ServerToConnectTo + "\r\n\r\n"));
	// wait for maximum 12 x 300msec = 3.6 seconds
	while ( ! client.available() && numberOwebTries < 12 ) {
	  numberOwebTries++;
	  delay(300);   // 300ms
	}

	while (client.available())
	{
	  delay(0);
	  if (str.endsWith(String("\r\n\r\n")))  str = "";

	  str.concat( (const char)client.read());
	  delay(0);
	  if (client.available() == false)
	  {
		// if no data, wait for 300ms hoping that new data arrive
		delay(300);
	  }

	}



	client.stop();
	return str.substring(0, str.indexOf(String(String(char(10)) + "0" )  ));
  }
  client.stop();
  return "";
}



void serialFlush()
{

  while (Serial.available() > 0)
  {
	delay(0);
	char t = Serial.read();
  }
}


void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
	String MQTTNewMSg;
	//Serial.println("msg received");

  for (int i = 0; i < length; i++) 
  {
	MQTTNewMSg = String(MQTTNewMSg + (char)payload[i]);
	delay(0);
  }

  if (MQTTNewMSg != MQTTlatestMsg )
  {
	MQTTnewMsgReceived  = 1;  
	MQTTlatestMsg = MQTTNewMSg;
  }  
}


void MQTTreconnect() {
  // Loop until we're reconnected
  byte reconnectAttempts = 0;
  while (!MQTTclient.connected()) {
	reconnectAttempts++;
	if (reconnectAttempts > 3) return;
	if (MQTTclient.connect("ESP8266Client")) {
	  MQTTclient.subscribe(MQTTSubscribeTopic.c_str());
	  if (MQTTPublishTopic != "") 
	  {
		  bool MQTTSendSucess = 0;
		  while (!MQTTSendSucess)
		  {
			//Serial.print("Atempting to send");
			Serial.println(MQTTPublishMSG);
			MQTTSendSucess =  MQTTclient.publish(MQTTPublishTopic.c_str(), MQTTPublishMSG.c_str(),1);
		    if (MQTTSendSucess) 
			{
				MQTTPublishTopic = "";
				//Serial.println(MQTTSendSucess);
				return;
			}
			if (reconnectAttempts > 10) return;
			delay(0);
			reconnectAttempts++;
		  }
		  
		  
	  }
	  //MQTTclient.loop();
	  //Serial.println("connected");
	} else {
	  Serial.print("failed, rc=");
	  Serial.println(MQTTclient.state());
	  delay(0);
	}
  } 
}
