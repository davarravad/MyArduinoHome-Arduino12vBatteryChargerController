//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Setup the switch cases
typedef enum
{
  CHARGER_MODE_MENU,
  CHARGER_MODE_SUB_MENU,
  CHARGER_MODE_RUN,
  CHARGER_MODE_VOLTAGE_TEST_ALL,
  CHARGER_MODE_PERCENT_TEST_ALL,
  CHARGER_MODE_OFF
} d_modes;
d_modes chargerMode;

// Setup Button
const int buttonPin = 8;
bool buttonPress = false;
bool buttonLongPress = false;
long buttonTimer = 0;
long buttonLongTimer = 3000;

// Setup Door Pin
const int doorPin = 7;
bool doorState = 0;
bool prevDoorState = 0;

// Set The Relay Pins
const int outPutPin1 =  3;
const int outPutPin2 =  4;
const int outPutPin3 =  5;
const int outPutPin4 =  6;
const int outPutPin5 =  10;
const int outPutPin6 =  11;
const int outPutPin7 =  12;
const int outPutPin8 =  13;

// Setup menu
int menuOpt = 0;
int prevMenuOpt = 0;
int subMenuOpt = 0;
int prevSubMenuOpt = 0;
String menuOptTitle = "";
String run_mode = "";
String selectMode = "";
int prevChargerMode = 0;
int chargeCycle = 0;
int chargeCycleStatus = 0; // 0=off 1=on

// Set pin for Voltage Meters
int voltage1 = A0;
int voltage2 = A1;
int voltage3 = A2;
int voltage4 = A3;

// Set Values for Voltage Data
float vout1 = 0.0;
float vin1 = 0.0;
float vin1b = 0.0;
float R1 = 30000.0; //  
float R2 = 7500.0; // 
float fullCharge = 13.80;  // 13.80 for this project
int value1 = 0;

void setup()
{

  // Setup inputs/outputs
  pinMode(buttonPin, OUTPUT);

  // Setup Voltage Meters
  pinMode(voltage1, INPUT);
  pinMode(voltage2, INPUT);
  pinMode(voltage3, INPUT);
  pinMode(voltage4, INPUT);

  // initialize the relay pins as outputs:
  pinMode(outPutPin1, OUTPUT);
  pinMode(outPutPin2, OUTPUT);
  pinMode(outPutPin3, OUTPUT);
  pinMode(outPutPin4, OUTPUT);
  pinMode(outPutPin5, OUTPUT);
  pinMode(outPutPin6, OUTPUT);
  pinMode(outPutPin7, OUTPUT);
  pinMode(outPutPin8, OUTPUT);

  // Set the relays to test mode
  digitalWrite(outPutPin1, HIGH);
  digitalWrite(outPutPin2, HIGH);
  digitalWrite(outPutPin3, HIGH);
  digitalWrite(outPutPin4, HIGH);
  digitalWrite(outPutPin5, HIGH);
  digitalWrite(outPutPin6, HIGH);
  digitalWrite(outPutPin7, HIGH);
  digitalWrite(outPutPin8, HIGH);

  // Initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("My Arduino Home");
  lcd.setCursor(0,1);
  lcd.print("12v Batt Charger");
  delay(5000);
  lcd.clear();


  //Set default charger mode
  chargerMode = CHARGER_MODE_MENU;
}


void loop()
{
  // Check to see if charger is requested off
  // If Door is open - turn off
  // If Button Long Press while on - turn off
  if (digitalRead(buttonPin) == HIGH) {
		if (buttonPress == false) {
			buttonPress = true;
			buttonTimer = millis();
		}
		if ((millis() - buttonTimer > buttonLongTimer) && (buttonLongPress == false)) {
			buttonLongPress = true;
		}
	} else {
		if (buttonPress == true) {
			if (buttonLongPress == true) {
				buttonLongPress = false;
			} else {
        // Nothing
			}
			buttonPress = false;
		}
	}

  // Check to see which mode the charger is running
  switch (chargerMode) {
    case CHARGER_MODE_MENU:
      // Display Charger Menu
      runChargerMenu();
      break;
    case CHARGER_MODE_SUB_MENU:
      // Display Charger Menu
      runChargerSubMenu();
      break;
    case CHARGER_MODE_VOLTAGE_TEST_ALL:
      // Display Voltage for All Batteries
      runVoltageTestAll();
      break;
    case CHARGER_MODE_PERCENT_TEST_ALL:
      // Display Voltage for All Batteries
      runPercentageTestAll();
      break;
    case CHARGER_MODE_RUN:
      // Run Charger Based on Menu Selection
      runCharger();
      break;
    case CHARGER_MODE_OFF:
      // Turn Charger Off
      runChargerOff();
      break;
    default:
      break;
  }
}

// Charger Menu Function
// Displays charger menu options
void runChargerMenu(){
  // Check button state for menus selection
  if(buttonLongPress == true){
      // Run charger if button long press
      chargerMode = CHARGER_MODE_SUB_MENU;
      buttonLongPress = false;
  }else if(buttonPress == true){
      // Go to next menu option
      menuOpt = prevMenuOpt+1;
  }else{
    // Display Menu Options
    if(menuOpt == 0){
      if(menuOpt != prevMenuOpt){ lcd.clear(); }
      lcd.setCursor(0,0);
      lcd.print("Auto Charge All");
      lcd.setCursor(0,1);
      lcd.print("Hold for Info");
      delay(100);
      menuOptTitle = "Auto Charge All";
      prevMenuOpt = 0;
    }else if(menuOpt == 1){
      if(menuOpt != prevMenuOpt){ lcd.clear(); }
      lcd.setCursor(0,0);
      lcd.print("Auto Charge Bat1");
      lcd.setCursor(0,1);
      lcd.print("Hold for Info");
      delay(100);
      menuOptTitle = "Auto Charge Bat1";
      prevMenuOpt = 1;
    }else if(menuOpt == 2){
      if(menuOpt != prevMenuOpt){ lcd.clear(); }
      lcd.setCursor(0,0);
      lcd.print("Auto Charge Bat2");
      lcd.setCursor(0,1);
      lcd.print("Hold for Info");
      delay(100);
      menuOptTitle = "Auto Charge Bat2";
      prevMenuOpt = 2;
    }else if(menuOpt == 3){
      if(menuOpt != prevMenuOpt){ lcd.clear(); }
      lcd.setCursor(0,0);
      lcd.print("Auto Charge Bat3");
      lcd.setCursor(0,1);
      lcd.print("Hold for Info");
      delay(100);
      menuOptTitle = "Auto Charge Bat3";
      prevMenuOpt = 3;
    }else if(menuOpt == 4){
      if(menuOpt != prevMenuOpt){ lcd.clear(); }
      lcd.setCursor(0,0);
      lcd.print("Auto Charge Bat4");
      lcd.setCursor(0,1);
      lcd.print("Hold for Info");
      delay(100);
      menuOptTitle = "Auto Charge Bat4";
      prevMenuOpt = 4;
    }else{
      // End of menu return to first option
      menuOpt = 0;
      menuOptTitle = "";
      lcd.clear();
    }
  }
  // Setup Prev Charger Mode
  prevChargerMode = CHARGER_MODE_MENU;
}

// Charger Sub Menu Function
// Displays charger sub menu options
void runChargerSubMenu(){
  // Let user know they are moving on to sub menu
  if(prevChargerMode != CHARGER_MODE_SUB_MENU){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Getting More");
    lcd.setCursor(0,1);
    lcd.print("Charger Options");
    buttonLongPress = false;
    // Delay for button press
    delay(3000);
  }
  // Check button state for menus selection
  if(buttonLongPress == true){
      // Run charger if button long press
      if(selectMode == "CHARGER_MODE_VOLTAGE_TEST_ALL"){
        chargerMode = CHARGER_MODE_VOLTAGE_TEST_ALL;
      }else if(selectMode == "CHARGER_MODE_PERCENT_TEST_ALL"){
        chargerMode = CHARGER_MODE_PERCENT_TEST_ALL;
      }else if(selectMode == "CHARGER_MODE_RUN"){
        chargerMode = CHARGER_MODE_RUN;
      }else{
        chargerMode = CHARGER_MODE_MENU;
      }
      buttonLongPress = false;
  }else if(buttonPress == true){
      // Go to next menu option
      subMenuOpt = prevSubMenuOpt+1;
  }else{

    // Display Sub Menu Options Based on Menu Options
    if(prevMenuOpt == 0 || prevMenuOpt == 1){
      // Auto Charge All Menu
      // Display Sub Menu Options
      if(subMenuOpt == 0){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "View Voltage";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        selectMode = "CHARGER_MODE_VOLTAGE_TEST_ALL";
        prevSubMenuOpt = 0;
      }else if(subMenuOpt == 1){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "View Percentage";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        selectMode = "CHARGER_MODE_PERCENT_TEST_ALL";
        prevSubMenuOpt = 1;
      }else if(subMenuOpt == 2){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "Start Auto Charge";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        selectMode = "CHARGER_MODE_RUN";
        prevSubMenuOpt = 2;
      }else{
        // End of menu return to first option
        subMenuOpt = 0;
        run_mode = "";
        lcd.clear();
      }
    }else if(prevMenuOpt == 2 || prevMenuOpt == 3){
      // Auto Charge View Voltage Options
      // Display Sub Menu Options
      if(subMenuOpt == 0){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "60 Minutes";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        prevSubMenuOpt = 0;
      }else if(subMenuOpt == 1){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "40 Minutes";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        prevSubMenuOpt = 1;
      }else if(subMenuOpt == 2){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "20 Minutes";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        prevSubMenuOpt = 2;
      }else if(subMenuOpt == 3){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "Cool Down";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        prevSubMenuOpt = 3;
      }else{
        // End of menu return to first option
        subMenuOpt = 0;
        run_mode = "";
        lcd.clear();
      }
    }else if(prevMenuOpt == 4){
      // Air Dry No Heat Options
      // Display Sub Menu Options
      if(subMenuOpt == 0){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "20 Minutes";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        prevSubMenuOpt = 0;
      }else if(subMenuOpt == 1){
        if(subMenuOpt != prevSubMenuOpt){ lcd.clear(); }
        run_mode = "10 Minutes";
        lcd.setCursor(0,0);
        lcd.print(menuOptTitle);
        lcd.setCursor(0,1);
        lcd.print(run_mode);
        delay(100);
        prevSubMenuOpt = 1;
      }else{
        // End of menu return to first option
        subMenuOpt = 0;
        run_mode = "";
        lcd.clear();
      }
    }

  }
  // Setup Prev Charger Mode
  prevChargerMode = CHARGER_MODE_SUB_MENU;
}

// Set relays status based on options
void setChargerRelays(char* relay_modes){
  if(relay_modes == "ON"){
    digitalWrite(outPutPin1, LOW);
    digitalWrite(outPutPin2, LOW);
    digitalWrite(outPutPin3, LOW);
    digitalWrite(outPutPin4, LOW);
    digitalWrite(outPutPin5, LOW);
    digitalWrite(outPutPin6, LOW);
    digitalWrite(outPutPin7, LOW);
    digitalWrite(outPutPin8, LOW);
  }else if(relay_modes == "PAUSE"){
    digitalWrite(outPutPin1, HIGH);
    digitalWrite(outPutPin2, HIGH);
    digitalWrite(outPutPin3, HIGH);
    digitalWrite(outPutPin4, HIGH);
    digitalWrite(outPutPin5, HIGH);
    digitalWrite(outPutPin6, HIGH);
    digitalWrite(outPutPin7, HIGH);
    digitalWrite(outPutPin8, HIGH);
  }else if(relay_modes == "OFF"){
    digitalWrite(outPutPin1, HIGH);
    digitalWrite(outPutPin2, HIGH);
    digitalWrite(outPutPin3, HIGH);
    digitalWrite(outPutPin4, HIGH);
    digitalWrite(outPutPin5, HIGH);
    digitalWrite(outPutPin6, HIGH);
    digitalWrite(outPutPin7, HIGH);
    digitalWrite(outPutPin8, HIGH);
  }
}

// Charger Run Function
// Starts the charger based on menu selection
void runCharger(){
  if(prevChargerMode != CHARGER_MODE_RUN){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Starting Charger");
    lcd.setCursor(0,1);
    lcd.print("Smile :)");
    buttonLongPress = false;
    // Delay for button press
    delay(3000);
  }
  // Check to see if user is turning off charger with button long press
  if(buttonLongPress == true){
    chargerMode = CHARGER_MODE_OFF;
    buttonLongPress = false;
  }
  // Start the Charge Cycle
  if(chargeCycle == 1){
    // Turn the charger off to get voltage
    digitalWrite(outPutPin1, HIGH);
	// Add delay for reset
    delay(500);
    // Get data from Voltage Meters
    value1 = analogRead(voltage1);
    vout1 = (value1 * 5.0) / 1024.0; // see text
    vin1 = vout1 / (R2/(R1+R2)); 
    vin1b = ((vout1 / (R2/(R1+R2))) / fullCharge) * 100; 
    // Run Battery 1 Cycle
    chargeCycle = 1;
	// Display the data
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Charging Batt 1");
    lcd.setCursor(0,1);
    lcd.print(vin1,2);
    lcd.print("v - ");
    lcd.print(vin1b,2);
    lcd.print("%");
    buttonLongPress = false;
    // Delay for button press
    delay(3000);
    // Set Current Charge Cycle Status
    // Base weather on or off by the amount of voltage to battery
    if(chargeCycleStatus == 1){
      // Check to see if battery is charged.
      if(vin1 >= fullCharge){
        // Battery is charged turn off charger
        digitalWrite(outPutPin1, HIGH);
      }else{
        // Battery is not charged, turn on charger
        digitalWrite(outPutPin1, LOW);
        // Set delay for amount of time to turn charger on.
        delay(60000);
      }
    }else{
      // Start the battery charge cycle
      chargeCycleStatus = 1; // 0=off 1=on
    }
    
    
  }else{
    // No cycle count at this time, start over
    chargeCycle = 1;
  }
  // Setup Prev Charger Mode
  prevChargerMode = CHARGER_MODE_RUN;
}

// Charger Voltage Test All Function
// Starts the charger based on menu selection
void runVoltageTestAll(){
  if(prevChargerMode != CHARGER_MODE_VOLTAGE_TEST_ALL){
    // Welcome the user to the voltage reader
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Load Batt Voltage");
    lcd.setCursor(0,1);
    lcd.print("Please Wait...");
    buttonLongPress = false;
    // Delay for button press
    delay(3000);
  }

  // Get data from Voltage Meters
  value1 = analogRead(voltage1);
  vout1 = (value1 * 5.0) / 1024.0; // see text
  vin1 = vout1 / (R2/(R1+R2)); 
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Battery 1 Voltage");
  lcd.setCursor(0,1);
  lcd.print(vin1,2);
  lcd.print(" volts");
  // Delay for button press and next reading
  delay(4000);
  
  // Check to see if user is turning off charger with button long press
  if(buttonLongPress == true){
    chargerMode = CHARGER_MODE_OFF;
    buttonLongPress = false;
  }
  // Setup Prev Charger Mode
  prevChargerMode = CHARGER_MODE_VOLTAGE_TEST_ALL;
}

// Charger Percentage Test All Function
// Starts the charger based on menu selection
void runPercentageTestAll(){
  if(prevChargerMode != CHARGER_MODE_PERCENT_TEST_ALL){
    // Welcome the user to the voltage reader
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Load Batt Percentage");
    lcd.setCursor(0,1);
    lcd.print("Please Wait...");
    buttonLongPress = false;
    // Delay for button press
    delay(3000);
  }

  // Get data from Voltage Meters
  value1 = analogRead(voltage1);
  vout1 = (value1 * 5.0) / 1024.0; // see text
  vin1 = ((vout1 / (R2/(R1+R2))) / fullCharge) * 100; 
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Battery 1 Percentage");
  lcd.setCursor(0,1);
  lcd.print(vin1,2);
  lcd.print(" %");
  // Delay for button press and next reading
  delay(4000);
  
  // Check to see if user is turning off charger with button long press
  if(buttonLongPress == true){
    chargerMode = CHARGER_MODE_OFF;
    buttonLongPress = false;
  }
  // Setup Prev Charger Mode
  prevChargerMode = CHARGER_MODE_PERCENT_TEST_ALL;
}

// Charger Off Function
// Turns everything in charger off
void runChargerOff(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Turning Charger");
  lcd.setCursor(0,1);
  lcd.print("OFF");
  // Shut down charger
  setChargerRelays("OFF");
  delay(5000);
  // Let menu know to start from first menu option
  prevMenuOpt = 0;
  // After Charger off delay send to menu
  chargerMode = CHARGER_MODE_MENU;
  // Setup Prev Charger Mode
  prevChargerMode = CHARGER_MODE_OFF;
}
