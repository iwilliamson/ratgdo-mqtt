#include "common.h"
#include "rolling_code.h"
#include "Helpers.h"

void readCounterFromFlash(const char *type, unsigned int &counter){

	File file = LittleFS.open(type, "r");

	//Check if the file exists
	if(!file){
		Serial.print(type);
		Serial.println(" doesn't exist. creating...");

		AddDebug( "!RCF:" + String(type) + ":" + String(counter) + "\n");

		writeCounterToFlash(type,counter);
		return;
	}

	counter = file.parseInt();

	AddDebug( "RCF:" + String(type) + ":" + String(counter) + "\n");

	//Close the file
	file.close();
}

void writeCounterToFlash(const char *type, unsigned int &counter){
	//Open the file 
	File file = LittleFS.open(type, "w");
	
	//Write to the file
	file.print(counter);
	delay(1);
	//Close the file
	file.close();
	
	AddDebug( "WCF:" + String(type) + ":" + String(counter) + "\n");

	Serial.print(type);
	Serial.println(" write successful");
}

void readRollingCode(byte rxSP2RollingCode[SECPLUS2_CODE_LEN], uint8_t &door, uint8_t &light, uint8_t &lock, uint8_t &motion, uint8_t &obstruction){
	uint32_t rolling = 0;
	uint64_t fixed = 0;
	uint32_t data = 0;

	uint16_t cmd = 0;
	uint8_t nibble = 0;
	uint8_t byte1 = 0;
	uint8_t byte2 = 0;

	decode_wireline(rxSP2RollingCode, &rolling, &fixed, &data);

	cmd = ((fixed >> 24) & 0xf00) | (data & 0xff);

	nibble = (data >> 8) & 0xf;
	byte1 = (data >> 16) & 0xff;
	byte2 = (data >> 24) & 0xff;

	printRollingCode(rxSP2RollingCode);

	AddDebug("vvvvvvvvvvvvvvvvvvvvvvvvv\n");
	AddDebug("IN: [" + String(rolling) + "]: 0x"+ String(cmd, 16) + "\n");

	if(cmd == 0x81){
		door = nibble;
		light = (byte2 >> 1) & 1;
		lock = byte2 & 1;
		motion = 0; // when the status message is read, reset motion state to 0|clear
		// obstruction = (byte1 >> 6) & 1; // unreliable due to the time it takes to register an obstruction

		Serial.print(" | STATUS:");
		Serial.print(" door:");
		Serial.print(nibble);
		Serial.print(" light:");
		Serial.print((byte2 >> 1) & 1);
		Serial.print(" lock:");
		Serial.print((byte2 & 1));
		Serial.print(" obs:");
		Serial.print((byte1 >> 6) & 1);
		AddDebug("IN81: ");
		AddDebug("door:" + String(door) + " ");
		AddDebug("light:" + String(light) + " ");
		AddDebug("lock:" + String(lock) + " ");
		AddDebug("obs:" + String((byte1 >> 6) & 1) + "\n");

	}else if(cmd == 0x281){
		light ^= 1; // toggle bit

		Serial.print(" | LIGHT:");
		Serial.print(light);
		AddDebug("IN281: ");
		AddDebug("light toggle to:" + String(light) + "\n");
	}else if(cmd == 0x84){
	}else if(cmd == 0x285){
		motion = 1; // Motion detected
		light = 1;
		Serial.print(" | MOTION:");
		Serial.print(motion);
		AddDebug("IN285: ");
		AddDebug("Motion Detect\n");
	}

	AddDebug("^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	Serial.println("");
}

void getRollingCode(const char *command){
	Serial.print("rolling code for ");
	Serial.print(idCode, HEX);
	Serial.print(" ");
	Serial.print(rollingCodeCounter);
	Serial.print("|");
	Serial.print(command);
	Serial.print(" : ");

	uint64_t id = idCode;
	uint64_t fixed = 0;
	uint32_t data = 0;

	if(strcmp(command,"reboot1") == 0){
		fixed = 0x400000000;
		data = 0x0000008b;
	}else if(strcmp(command,"reboot2") == 0){
		fixed = 0;
		data = 0x00000080;
	}else if(strcmp(command,"reboot3") == 0){
		fixed = 0;
		data = 0x000000a0;
	}else if(strcmp(command,"reboot4") == 0){
		fixed = 0;
		data = 0x00000080;
	}else if(strcmp(command,"reboot5") == 0){
		fixed = 0x300000000;
		data = 0x00000092;
	}else if(strcmp(command,"reboot6") == 0){
		fixed = 0x300000000;
		data = 0x00000092;
	}else if(strcmp(command,"door1") == 0){
		fixed = 0x200000000;
		data = 0x01010280;
	}else if(strcmp(command,"door2") == 0){
		fixed = 0x200000000;
		data = 0x01000280;
	}else if(strcmp(command,"light") == 0){
		fixed = 0x200000000;
		data = 0x00000281;
	}else if(strcmp(command,"lock") == 0){
		fixed = 0x0100000000;
		data = 0x0000028c;
	}else{
		Serial.println("ERROR: Invalid command");
		return;
	}

	AddDebug("OUT: [" + String(rollingCodeCounter) + "]: 0x"+ String(data, 16) + "\n");

	fixed = fixed | id;

	encode_wireline(rollingCodeCounter, fixed, data, txSP2RollingCode);

	printRollingCode(txSP2RollingCode);
	Serial.println("");

	if(strcmp(command,"door1") != 0){ // door2 is created with same counter and should always be called after door1
		AddDebug( "INC:" + String(rollingCodeCounter) + " --> ");
		rollingCodeCounter = (rollingCodeCounter + 1) & 0xfffffff;
		AddDebug( String(rollingCodeCounter) + "\n");
	}
	return;
}

void printRollingCode(byte code[SECPLUS2_CODE_LEN]){
	for(int i = 0; i < SECPLUS2_CODE_LEN; i++){
		if(code[i] <= 0x0f) Serial.print("0");
		Serial.print(code[i],HEX);
	}
}