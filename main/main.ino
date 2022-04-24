#include <LiquidCrystal.h>
#include "DHT.h"
#include "pitches.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(6, DHT11);

int anneal_temp = 82.22, heat_up = 7200, hold = 2700, cool_down = 7200, note_Dur = 1000, state_timer = 100; //in ms * 10
int selector = 0, value = 0, potent = 0, last_potent = 0, general = 0, last_general = 0, timer = 0, percent = 0, heater = 0;
float room_temp = 22.0, cur_temp = room_temp, lin_temp = room_temp;
bool state = 1, flag = false;
String line = "", line_two = "";

int melody_one[4] = {NOTE_B4, NOTE_C5, NOTE_G5, NOTE_E5};
int duration_one[4] = {8, 8, 8, 2};

int melody_two[3] = {NOTE_GS3, NOTE_C4, NOTE_G4};
int duration_two[3] = {16, 16, 8};

int melody_three[3] = {NOTE_A4, NOTE_B4, NOTE_CS5};
int duration_three[3] = {8, 8, 4};

byte circle[8] = {0b00000, 0b00000, 0b01110, 0b11111, 0b11111, 0b11111, 0b01110, 0b00000};
byte up_arrow[8] = {0b00000, 0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b00000};
byte down_arrow[8] ={0b00000, 0b00100, 0b00100, 0b00100, 0b10101, 0b01110, 0b00100, 0b00000};

int ref[4][3] = {
  {0, 10, 12},
  {1, 1, 3},
  {1, 6, 8},
  {1, 11, 13}
};

void setup() {
  Serial.begin(115200);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  lcd.begin(16, 2);
  lcd.createChar(0, circle);
  lcd.createChar(1, up_arrow);
  lcd.createChar(2, down_arrow);
  lcd.setCursor(0, 0);
  lcd.print("Set Temp: " + String(anneal_temp));
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.setCursor(1, 1);
  String statement = String(heat_up / 60) + " =" + String(hold / 60);
  lcd.print(statement);
  lcd.setCursor(3 + statement.length(), 1);
  lcd.write(byte(2));
  lcd.setCursor(4 + statement.length(), 1);
  lcd.print(cool_down / 60);
  dht.begin();
   
  for (int Note = 0; Note < 3; Note++) {
    note_Dur = 1000 / duration_three[Note];
    tone(10, melody_three[Note], note_Dur);
    delay(1.3 * note_Dur);
    noTone(10);
  }
}

void loop() {

  // SETUP PHASE
  if (! flag) {
    room_temp = dht.readTemperature();

    // Hit START
    if (digitalRead(9) == LOW) {
       switch (selector) {
        case 0:
          anneal_temp = value;
          break;
        case 1:
          heat_up = value * 60;
          break;  
        case 2:
          hold = value * 60;
          break;
        case 3:
          cool_down = value * 60;
          break;
      }
      for (int Note = 0; Note < 4; Note++) {
        note_Dur = 1000 / duration_one[Note];
        tone(10, melody_one[Note], note_Dur);
        delay(1.3 * note_Dur);
        noTone(10);
      }
      
      digitalWrite(7, HIGH);
      flag = true; 

    // Hit NEXT
    } else if (digitalRead(13) == LOW) {
      Serial.println(ref[selector][1], ref[selector][0]);
      if (! state) {
        lcd.setCursor(ref[selector][1], ref[selector][0]);
        lcd.print(value);
      }
      switch (selector) {
        case 0:
          anneal_temp = value;
          break;
        case 1:
          heat_up = value * 60;
          break;  
        case 2:
          hold = value * 60;
          break;
        case 3:
          cool_down = value * 60;
          break;
      }
      (selector < 3) ? selector++ : selector = 0;
      for (int Note = 0; Note < 3; Note++) {
        note_Dur = 1000 / duration_two[Note];
        tone(10, melody_two[Note], note_Dur);
        delay(1.3 * note_Dur);
        noTone(10);
      }

    // Update Frame and Check Potentiometer
    } else {
      lcd.setCursor(ref[selector][1], ref[selector][0]);
      if (timer >= state_timer) {
        if (state) {
          lcd.print("   ");
          state = false;
        } else {
          lcd.print(value);
          state = true;
        }
        timer = 0;
      }   
      
      // Potentiometer goes from 0 - ~1024 (1021 usually)
      potent = analogRead(A0);
      general = round(potent / 30); // Determines the sensitivity of the blinking
      if (general != last_general) {
        switch(selector) {
          case 0:
            value = round(0.0935 * potent) + 30;
            break;
          case 1:
          case 2:
          case 3:
            value = round(0.4712 * potent);
            break;
        }
        lcd.setCursor(ref[selector][1], ref[selector][0]);
        lcd.print(String(value) + " ");
      }
    last_general = general;
    }
    delay(10);
    ++timer;
  
  // CYCLE PHASE
  } else {
    lcd.clear();
    Serial.println("Temp: " + String(anneal_temp));
    Serial.println("HU: " + String(heat_up));
    Serial.println("H: " + String(hold));
    Serial.println("CD: " + String(cool_down));

    // Heat-up Procedure
    timer = 0;
    lcd.clear();
    while (timer < heat_up) {
      ++timer;
      percent = (timer * 100) / heat_up;
      cur_temp = dht.readTemperature();
      lin_temp = room_temp + ((anneal_temp - room_temp) / heat_up) * timer;
      if (cur_temp < lin_temp) {
        digitalWrite(8, HIGH);
        heater = 1;
      } else {
        digitalWrite(8, LOW);
        heater = 0;
      }
      Serial.print("Current temp: " + String(cur_temp, 2) + " ");
      Serial.print("Expected temp: " + String(lin_temp, 2) + " ");
      Serial.println("Heater: " + String(heater));
      line = "Heating to " + String(anneal_temp);
      line_two = "Cur: " + String(cur_temp, 2) + " " + String(percent) + "%";
      lcd.setCursor(0, 0);
      lcd.print(line);
      lcd.setCursor(line.length() + 1, 0);
      (heater) ? lcd.write(byte(0)) : lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(line_two);
      delay(1000);
    }

    // Hold Procedure
    timer = 0;
    lcd.clear();
    while (timer < hold) {
      ++timer;
      percent = (timer * 100) / hold;
      cur_temp = dht.readTemperature();
      if (cur_temp < anneal_temp) {
        digitalWrite(8, HIGH);
        heater = 1;
      } else {
        digitalWrite(8, LOW);
        heater = 0;
      }
      Serial.print("Current temp: " + String(cur_temp, 2) + " ");
      Serial.print("Expected temp: " + String(anneal_temp, 2) + " ");
      Serial.println("Heater: " + String(heater));
      line = "Holding at " + String(anneal_temp);
      line_two = "Cur: " + String(cur_temp, 2) + " " + String(percent) + "%";
      lcd.setCursor(0, 0);
      lcd.print(line);
      lcd.setCursor(line.length() + 1, 0);
      (heater) ? lcd.write(byte(0)) : lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(line_two);
      delay(1000);  
    }

    // Cooldown Procedure
    timer = 0;
    lcd.clear();
    while (timer < cool_down) {
      ++timer;
      percent = (timer * 100) / cool_down;
      cur_temp = dht.readTemperature();
      lin_temp = anneal_temp + ((room_temp - anneal_temp) / cool_down) * timer;
      if (cur_temp < lin_temp) {
        digitalWrite(8, HIGH);
        heater = 1;
      } else {
        digitalWrite(8, LOW);
        heater = 0;
      }
      Serial.print("Current temp: " + String(cur_temp, 2) + " ");
      Serial.print("Expected temp: " + String(anneal_temp, 2) + " ");
      Serial.println("Heater: " + String(heater));
      line = "Cooling to " + String(room_temp);
      line_two = "Cur: " + String(cur_temp, 2) + " " + String(percent) + "%";
      lcd.setCursor(0, 0);
      lcd.print(line);
      lcd.setCursor(line.length() + 1, 0);
      (heater) ? lcd.write(byte(0)) : lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(line_two);
      delay(1000);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready for use...");
    digitalWrite(7, LOW);
    flag = false;
    delay(3000);
  }
}
