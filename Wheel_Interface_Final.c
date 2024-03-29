#include <ASTCanLib.h>
#include "Switch.h"
uint8_t sendData[8];

// For all the debounce checks
unsigned long current_time = 0; // updated every loop (in void loop())

// For CAN sending
unsigned long timeSendCan = 100; // set time between CAN sends to 100ms
unsigned long lastSendCan = 0; // Initialize last send to 0

#define MESSAGE_ID        0x700       // Message ID
#define MESSAGE_PROTOCOL  1         // CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
#define MESSAGE_LENGTH    8         // Data length: 8 bytes
#define MESSAGE_RTR       0         // rtr bit

st_cmd_t txMsg;

//Define constants for debouncing and declare variables for debouncing
#define pressed LOW
#define notPressed HIGH
unsigned long debounce_delay_button = 50; // Initialize debounce delay to 10ms
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
unsigned long lastSend[4] = {0, 0, 0, 0};
int lastBut[4] = {0, 0, 0, 0};
int stateOfBut[4] = {notPressed, notPressed, notPressed, notPressed};
unsigned long highTime[4] = {0, 0, 0, 0};

// check which input low, does task associated to input
void rotary1() { // 6 input rotary: 2-7
  sendData[0] = 1;
  if (digitalRead(2) == LOW) {
    sendData[0] = 1;
  } else if (digitalRead(3) == LOW) {
    sendData[0] = 2;
  } else if (digitalRead(4) == LOW) {
    sendData[0] = 3;
  } else if (digitalRead(5) == LOW) {
    sendData[0] = 4;
  } else if (digitalRead(6) == LOW) {
    sendData[0] = 5;
  } else if (digitalRead(7) == LOW) {
    sendData[0] = 6;
  }
}

// check which input low, does task associated to input
void rotary2() { // 4 input rotary: 8->11
  sendData[1] = 1;
  if (digitalRead(8) == LOW) {
    sendData[1] = 1;
  } else if (digitalRead(9) == LOW) {
    sendData[1] = 2;
  } else if (digitalRead(10) == LOW) {
    sendData[1] = 3;
  } else if (digitalRead(11) == LOW) {
    sendData[1] = 4; 
  }
}

// check which input low, does task associated to input
// LEAVE OUT PIN 13: Doesn't react like expected due to LED
void rotary3() { // 5 input rotary: 12->17 (leave out 13)
  sendData[2] = 1;
  if (digitalRead(12) == LOW) {
    sendData[2] = 1;
  } if (digitalRead(14) == LOW) {
    sendData[2] = 2;
  } if (digitalRead(15) == LOW) {
    sendData[2] = 3;
  } if (digitalRead(16) == LOW) {
    sendData[2] = 4;
}

void button1() { // 18
  
  sendData[3] = !stateOfBut[0];
  
  if (stateOfBut[0] == pressed && (current_time - highTime[0]) < 1000) {
    // Do nothing
  }
  else {
    int readBut1 = digitalRead(18);
    
    if (readBut1 != lastBut[0]) {
      lastDebounceTime[0] = current_time; // reset the debouncing timer
    }
    
    if ((current_time - lastDebounceTime[0]) > debounce_delay_button) {
      if (readBut1 != stateOfBut[0]) {
        stateOfBut[0] = readBut1;
        if (stateOfBut[0] == LOW) {
          highTime[0] = current_time;
        }
      }
    }
    
    lastBut[0] = readBut1;
    
  }
}

void button2() { // 19

  sendData[4] = !stateOfBut[1];

  if (stateOfBut[1] == pressed && (current_time - highTime[1]) < 1000) {
    // Do nothing
  }
  else {
    int readBut1 = digitalRead(19);

    if (readBut1 != lastBut[1]) {
      lastDebounceTime[1] = current_time; // reset the debouncing timer
    }

    if ((current_time - lastDebounceTime[1]) > debounce_delay_button) {
      if (readBut1 != stateOfBut[1]) {
        stateOfBut[1] = readBut1;
        if (stateOfBut[1] == LOW) {
          highTime[1] = current_time;
        }
      }
    }
    lastBut[1] = readBut1;
  }

}

void button3() { // 120

  sendData[5] = !stateOfBut[2];

  if (stateOfBut[2] == pressed && (current_time - highTime[2]) < 1000) {
    // Do nothing
  }
  else {
    int readBut1 = digitalRead(20);

    if (readBut1 != lastBut[2]) {
      lastDebounceTime[2] = current_time; // reset the debouncing timer
    }

    if ((current_time - lastDebounceTime[2]) > debounce_delay_button) {
      if (readBut1 != stateOfBut[2]) {
        stateOfBut[2] = readBut1;
        if (stateOfBut[2] == LOW) {
          highTime[2] = current_time;
        }
      }
    }
    lastBut[2] = readBut1;
  }

}

void button4() { // 21


  sendData[6] = !stateOfBut[3];

  if (stateOfBut[3] == pressed && (current_time - highTime[3]) < 1000) {
    // Do nothing
  }
  else {
    int readBut1 = digitalRead(21);

    if (readBut1 != lastBut[3]) {
      lastDebounceTime[3] = current_time; // reset the debouncing timer
    }

    if ((current_time - lastDebounceTime[3]) > debounce_delay_button) {
      if (readBut1 != stateOfBut[3]) {
        stateOfBut[3] = readBut1;
        if (stateOfBut[3] == LOW) {
          highTime[3] = current_time;
        }
      }
    }
    lastBut[3] = readBut1;
  }
}

void setup() {

  // Overwrite the JTAG Pins
  MCUCR = (1 << JTD);
  MCUCR = (1 << JTD);

  canInit(1000000); // init CAN port
  Serial.begin(9600);
  txMsg.pt_data = &sendData[0]; // reference message data to sendData

  // initialize the pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  //  pinMode(13, INPUT_PULLUP); // THIS DOESN"T WORK AS INTENDED
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  //  pinMode(17, INPUT_PULLUP); // Not being used
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);

}


void loop() {

  // update current_time
  current_time = millis();

  // call all the functions
  rotary1();
  rotary2();
  rotary3();
  button1();
  button2();
  button3();
  button4();

  // Setup CAN packet.
  txMsg.ctrl.ide = MESSAGE_PROTOCOL;  // Set CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
  txMsg.id.ext   = MESSAGE_ID;        // Set message ID
  txMsg.dlc      = MESSAGE_LENGTH;    // Data length: 8 bytes
  txMsg.ctrl.rtr = MESSAGE_RTR;       // Set rtr bit
  txMsg.ctrl.ide = 0;

  if (lastSendCan - timeSendCan >= 0)
  {
    // Send command to the CAN port controller
    txMsg.cmd = CMD_TX_DATA;       // send message
    // Wait for the command to be accepted by the controller
    while (can_cmd(&txMsg) != CAN_CMD_ACCEPTED);
    // Wait for command to finish executing
    while (can_get_status(&txMsg) == CAN_STATUS_NOT_COMPLETED);
    // Transmit is now complete. Wait a bit and loop
    //delay(100); // delay for stability
    lastSendCan = current_time;
  }

}
