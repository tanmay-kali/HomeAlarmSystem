#include <Keypad.h>
char key_pressed = 0;
const byte columns = 4; 
const byte rows = 1;
char fourKeys[rows][columns] = {{'1','2','3','4'}};
byte column_pins[columns] = {9,8,11,10};
byte row_pins[rows] = {6};
Keypad keypad_key = Keypad(makeKeymap(fourKeys), row_pins, column_pins, rows, columns);
char password[4] = {'1','2','3','4'};
char enteredPassword[4] = {};
int i =0;
const int buttonPin = 7;
int buttonState = 0;
int lastButtonState = 0;


//buzzer + magnetic sensor 
int reed_switch = 4; // digital pin for reed switch
int buzzer = 5;
int reed_status; //status
float f = 500; 

//0 means unarmed
int alarm_state = 0;
bool doorOpened = false;
bool noise = false;
void setup() {
  pinMode (13, OUTPUT); // define the digital output interface 13 feet
  pinMode(buzzer, OUTPUT); // buzzer
  pinMode(reed_switch, INPUT);
   pinMode(buttonPin, INPUT);
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  reed_status = digitalRead(reed_switch);
  bool valid = takeKeypadInput();
  if(valid && alarm_state==0){
    //changing it to armed 
     Serial.println("Arming");
    alarm_state = 1;
    ActivateAlarm();
  }
  else if(valid && alarm_state==1){
    //changing it to unarmed 
    alarm_state = 0;
    Serial.println("Unarming");
    deActivateAlarm();
  }

  if(alarm_state==1){
    ActivateAlarm();
  }
  else{
    deActivateAlarm();
  }

  
  delay(200);
  
}


void ActivateAlarm(){
  //reed_status -1 -> means far apart 
  // reed status -0 -> means together
  Serial.println("Activate Alarm");
  Serial.println("Hello");
//  if (reed_status==1 && doorOpened == false || reed_status == 0 && doorOpened == true || reed_status==1 && doorOpened == true ){
//    doorOpened = true;
//    startNoise();
//  }


    //it means the door is getting opened the first time
    if(reed_status == 1  && doorOpened == false){
      doorOpened=true;
      startNoise();
    }

    //closed the door but still havent types in the code to deactivate
    if(reed_status == 0 && doorOpened == true){
      startNoise();
    }

   if(reed_status==0 && doorOpened == false){
    stopNoise();
    }

}

void deActivateAlarm(){
  doorOpened = false;
  Serial.println("Deactivate Alarm");
  stopNoise();
}


void startNoise(){
  Serial.println("start Noise");
//  tone(buzzer,500);
  tone(buzzer,440);
}

void stopNoise(){
 // noTone(buzzer);
  noTone(buzzer); 
}


bool takeKeypadInput(){
  buttonState = digitalRead(buttonPin);
  key_pressed  = keypad_key.getKey();
  bool passCheck=true;
  
  if(key_pressed != NO_KEY){
    enteredPassword[i]=key_pressed;
    Serial.print(enteredPassword[i]);
    i++;
  }
  if(buttonState != lastButtonState){
    if(buttonState==1){
    for(int j = 0 ; j <4;j++){
      char temp1 = enteredPassword[j];
      char temp2 = password[j];
      enteredPassword[j]='0';
      if(temp1!=temp2){
        passCheck = false;
      };
    }
    if(passCheck){
      Serial.print("PERFECT PASS");
      Serial.println();
      return true;
    }

    else{
      Serial.print("WRONG PASS");
      Serial.println();
    }
    
    i=0;
    passCheck=true;
  }
    
 }
lastButtonState = buttonState;
return false;
}
