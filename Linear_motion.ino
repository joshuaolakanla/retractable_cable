//D is out on the remote
#include <EEPROM.h>
int linear_left = 2;
int linear_right = 3;
int left_switch = 4;
int right_switch = 6;
int empty = 13;
int in_signal= A1;
int out_signal = A0;
int left;
int right;
int state;
int state1;
int state2;
int inn;
int outt; 
int in_address = 1;
int out_address = 2;
int in_dir;
int out_dir;
unsigned long t = 0;
unsigned long ct;
int interval;
int x;
int y;
void setup() {
 
  pinMode(linear_left, OUTPUT);
  pinMode(linear_right, OUTPUT);
  pinMode(left_switch,INPUT_PULLUP);
  pinMode(right_switch, INPUT_PULLUP);
  pinMode(in_signal, INPUT);
  pinMode(out_signal, INPUT);
  Serial.begin(9600);
  in_dir = EEPROM.read(in_address);
  out_dir = EEPROM.read(out_address);
  x = 1;
}


//Loop section
void loop() {
    // put your main code here, to run repeatedly:  
  inn = analogRead(in_signal);
  outt= analogRead(out_signal);
  in_dir = EEPROM.read(in_address);
  out_dir = EEPROM.read(out_address);
  

  ct = millis();

  while (inn>=600){

    inn = analogRead(in_signal);
    if (out_dir == linear_left) {
      Serial.println("out_dir is equal to linear left");
      Serial.print("out_dir data is ");
      Serial.println(out_dir);

      state1 = linear_right;
      Serial.print("State 1 is now: ");
      Serial.println(state1);
      EEPROM.update(in_address, state1);
      state2 = false;
      EEPROM.update(out_address, false); 
      Serial.print("Input for in was pressed and now changed to:");
      in_dir = EEPROM.read(in_address);
      Serial.println(in_dir);
      Serial.print("The other state was changed to:");
      out_dir = EEPROM.read(out_address);
      Serial.println(out_dir);
  
    }
    if (out_dir == linear_right){
      Serial.println("out_dir is equal to linear right");
      state1= linear_left;
      Serial.print("State 1 is now: ");
      Serial.println(state1);
      EEPROM.update(in_address, state1);
      state2 = false;
      EEPROM.update(out_address, false);
      Serial.print("Input for in was pressed and now changed to:");
      in_dir = EEPROM.read(in_address);
      Serial.println(in_dir);
      Serial.print("The other state was changed to:");
      out_dir = EEPROM.read(out_address);
      Serial.println(out_dir);
    } 
    Serial.print("now executing in data is:");
    in_dir = EEPROM.read(in_address);
    Serial.println(in_dir);
    
    in();
    
    }
    ct = millis();
    t = ct;


  if (outt>=600) {
    outt= analogRead(out_signal);
   if (in_dir == linear_left) {
      state2 = linear_right;
      EEPROM.update(out_address, state2);
      state1 = false;
      EEPROM.update(in_address, state1);
    }
    if (in_dir == linear_right){
      state2= linear_left;
      EEPROM.update(out_address, state2);
      state1 = false;
      EEPROM.update(in_address, state1);
    }  
    Serial.println("now executing out");
    
    out();
    
    }
    ct = millis();
    t = ct;

}



void in() {
  left = digitalRead(left_switch);
  right= digitalRead(right_switch);
  in_dir = EEPROM.read(in_address); //check what was saved to the memory initially
  state1 = in_dir; //the saved data is equal to the state
  Serial.print("In loop state1 is: ");
  Serial.println(state1);

  
  //if the left switch was pressed, change the direction
  if (left==0) {
    state1 = linear_left;
    EEPROM.update(in_address, state1);
    }

  //if the right switch was pressed, change the direction
  if (right==0) {
    state1 = linear_right;
    EEPROM.update(in_address, state1);
    }



  //now let's set the digital outputs. After setting it has high or low , it has to check in case the stae has been changed to avoid errors
  in_dir = EEPROM.read(in_address); //check what was saved to the memory 
  state1 = in_dir;
  Serial.print("I am about to set it as output and the state is: ");
  Serial.println(state1);
  ct = millis();
  if (x == 1) {
    if ((ct - t) >= 17500) {
      t = ct;
      x = 0;
      y = 1;
      inn = analogRead(in_signal);
      
        if (left==0) {
           state1 = linear_left;
           EEPROM.update(in_address, state1);
      }

        if (right==0) {
            state1 = linear_right;
            EEPROM.update(in_address, state1);
    }

       if (inn>=600){
         digitalWrite(state1, 1);
         delay(400);
         digitalWrite(state1, 0);
  }
    }

  }


  if (y == 1) {
    if ((ct - t) >= 400) {
      t = ct;
      x = 1;
      y = 0;
      digitalWrite(state1, 0);
  }
    }

  }


  //delay(11000);
  //inn = analogRead(in_signal);
  //if (inn>=600){
  //  digitalWrite(state1, 1);
  //  delay(400);
  //}
  //digitalWrite(state1, 0);
  //delay(8000);
  



 void out() {
  left = digitalRead(left_switch);
  right= digitalRead(right_switch);
  out_dir = EEPROM.read(out_address); //check what was saved to the memory initially
  state2 = out_dir; //the saved data is equal to the state


 
  //if the left switch was pressed, change the direction
  if (left==0) {
    state2=linear_left;
    EEPROM.update(out_address, state2);
    }

  //if the right switch was pressed, change the direction
  if (right == 0) {
    state2 = linear_right;
    EEPROM.update(out_address, state2);
    }



  //now let's set the digital outputs. After setting it has high or low , it has to check in case the stae has been changed to avoid errors
  out_dir = EEPROM.read(out_address); //check what was saved to the memory 
  state2 = out_dir;
  delay(1000);
  outt= analogRead(out_signal);
  if (outt>=600) {
    digitalWrite(state2, 1);
    delay(400);
  }

  digitalWrite(state2, 0);
  delay(2
  600);
  
  }
