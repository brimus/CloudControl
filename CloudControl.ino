// This #include statement was automatically added by the Particle IDE.
#include <NCD4Relay.h>

// This #include statement was automatically added by the Particle IDE.
#include <NCD2Relay.h>

/* Includes ------------------------------------------------------------------*/
#include <application.h>
#include <spark_wiring_i2c.h>

// HCPA-5V-U3 I2C address is 0x28(40)
#define Addr 0x28

NCD2Relay relay2a;
NCD2Relay relay2b;
NCD2Relay relay2c;
NCD4Relay relay4a;

SYSTEM_MODE(AUTOMATIC);

int triggerRelay(String command);
int triggerRelay1(String command);
int triggerRelay2(String command);
int triggerRelay4(String command);

bool tripped[6];
bool tripped1[6];
bool tripped2[6];
bool tripped4[6];

int debugTrips[6];
int debugTrips1[6];
int debugTrips2[6];
int debugTrips4[6];

int minTrips = 5;
int minTrips1 = 5;
int minTrips2 = 5;
int minTrips4 = 5;

int status;
int status1;
int status2;
int status4;

double cTemp = 0.0;
double fTemp = 0.0;
double humidity = 0.0;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	// Functions
	Particle.function("relay2a", triggerRelay);
	Particle.function("relay2b", triggerRelay1);
	Particle.function("relay2c", triggerRelay2);
	Particle.function("relay4a", triggerRelay4);
	// Variables
	Particle.variable("r2aStatus", status1);
	Particle.variable("r2bStatus", status);
	Particle.variable("r2cStatus", status2);
	Particle.variable("r4aStatys", status4);
	Particle.variable("i2cdevice", "HCPA-5V-U3");
	Particle.variable("humidity", humidity);
	Particle.variable("cTemp", cTemp);
	Particle.variable("fTemp", fTemp);
	// Starting
	Serial.begin(115200);
	Wire.begin();
	// I2C Addresses
	relay2a.setAddress(0,0,0);
	relay2b.setAddress(1,0,0);
	relay4a.setAddress(0,1,0);
	relay2c.setAddress(0,0,1);
	Wire.beginTransmission(Addr);
	
	Wire.write(0x80);
	Wire.endTransmission();
	delay(300);
	
	// End of Setup
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	// HCPA Temp sensor
	unsigned int data[4];
	Wire.beginTransmission(Addr);
	Wire.endTransmission();
	Wire.requestFrom(Addr, 4);
	if (Wire.available() == 4)
	{
		data[0] = Wire.read();
		data[1] = Wire.read();
		data[2] = Wire.read();
		data[3] = Wire.read();
		
		humidity = (((data[0] & 0x3F) * 256) + data[1]) / 16384.0 * 100.0;
		cTemp = (((data[2] * 256) + (data[3] & 0xFC)) / 4) / 16384.0 * 165.0 - 40.0;
		fTemp = (cTemp * 1.8) + 32;
		
		Particle.publish("Relative humidity : ", String(humidity));
		Particle.publish("Temperature in Celsius : ", String(cTemp));
		Particle.publish("Temperature in Fahrenheit : ", String(fTemp));
	}
	// Relay 2
	int status = relay2b.readAllInputs();
	int a = 0;
	for(int i = 1; i < 33; i*=2){
		if(status & i){
			debugTrips[a]++;
			if(debugTrips[a] >= minTrips){
				if(!tripped[a]){
					tripped[a] = true;
					//set input trip event to true
					String eventName = "Input_";
					eventName+=(a+1);
					Particle.publish(eventName, "ON");
					Serial.print("eventName: ");
					Serial.println(eventName);
					Serial.print("eventContents: ");
					Serial.println("ON");
				}
			}
		}else{
			debugTrips[a] = 0;
			if(tripped[a]){
				tripped[a] = false;
				//set input trip event to false
				String eventName = "Input_";
				eventName+=(a+1);
				Particle.publish(eventName, "OFF");
				Serial.print("eventName: ");
				Serial.println(eventName);
				Serial.print("eventContents: ");
				Serial.println("OFF");
			}
		}
		a++;
	}
	// Relay 1
	int status1 = relay2a.readAllInputs();
	int b = 0;
	for(int i = 1; i < 33; i*=2){
		if(status1 & i){
			debugTrips1[b]++;
			if(debugTrips1[b] >= minTrips1){
				if(!tripped1[b]){
					tripped1[b] = true;
					//set input trip event to true
					String eventName = "Input1_";
					eventName+=(b+1);
					Particle.publish(eventName, "ON");
					Serial.print("eventName: ");
					Serial.println(eventName);
					Serial.print("eventContents: ");
					Serial.println("ON");
				}
			}
		}else{
			debugTrips1[b] = 0;
			if(tripped1[b]){
				tripped1[b] = false;
				//set input trip event to false
				String eventName = "Input1_";
				eventName+=(b+1);
				Particle.publish(eventName, "OFF");
				Serial.print("eventName: ");
				Serial.println(eventName);
				Serial.print("eventContents: ");
				Serial.println("OFF");
			}
		}
		b++;
	}
	// Relay 4
	int status4 = relay4a.readAllInputs();
	int c = 0;
	for(int i = 1; i < 33; i*=2){
		if(status4 & i){
			debugTrips4[c]++;
			if(debugTrips4[c] >= minTrips4){
				if(!tripped4[c]){
					tripped4[c] = true;
					//set input trip event to true
					String eventName = "Input4_";
					eventName+=(c+1);
					Particle.publish(eventName, "ON");
					Serial.print("eventName: ");
					Serial.println(eventName);
					Serial.print("eventContents: ");
					Serial.println("ON");
				}
			}
		}else{
			debugTrips4[c] = 0;
			if(tripped4[c]){
				tripped4[c] = false;
				//set input trip event to false
				String eventName = "Input4_";
				eventName+=(c+1);
				Particle.publish(eventName, "OFF");
				Serial.print("eventName: ");
				Serial.println(eventName);
				Serial.print("eventContents: ");
				Serial.println("OFF");
			}
		}
		c++;
	}
	// Relay 3
	int status2 = relay2c.readAllInputs();
	int d = 0;
	for(int i = 1; i < 33; i*=2){
		if(status2 & i){
			debugTrips2[d]++;
			if(debugTrips2[d] >= minTrips2){
				if(!tripped2[d]){
					tripped2[d] = true;
					//set input trip event to true
					String eventName = "Input2_";
					eventName+=(d+1);
					Particle.publish(eventName, "ON");
					Serial.print("eventName: ");
					Serial.println(eventName);
					Serial.print("eventContents: ");
					Serial.println("ON");
				}
			}
		}else{
			debugTrips2[d] = 0;
			if(tripped2[d]){
				tripped2[d] = false;
				//set input trip event to false
				String eventName = "Input2_";
				eventName+=(d+1);
				Particle.publish(eventName, "OFF");
				Serial.print("eventName: ");
				Serial.println(eventName);
				Serial.print("eventContents: ");
				Serial.println("OFF");
			}
		}
		c++;
	}
	
	
	// End of Loop
}

int triggerRelay(String command){
	if(command.equalsIgnoreCase("turnonallrelays")){
		relay2a.turnOnAllRelays();
		return 1;
	}
	if(command.equalsIgnoreCase("turnoffallrelays")){
		relay2a.turnOffAllRelays();
		return 1;
	}
	if(command.startsWith("setBankStatus:")){
		int status = command.substring(14).toInt();
		if(status < 0 || status > 255){
			return 0;
		}
		Serial.print("Setting bank status to: ");
		Serial.println(status);
		relay2a.setBankStatus(status);
		Serial.println("done");
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	Serial.print("relayNumber: ");
	Serial.println(relayNumber);
	String relayCommand = command.substring(1);
	Serial.print("relayCommand:");
	Serial.print(relayCommand);
	Serial.println(".");
	if(relayCommand.equalsIgnoreCase("on")){
		Serial.println("Turning on relay");
		relay2a.turnOnRelay(relayNumber);
		Serial.println("returning");
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("off")){
		relay2a.turnOffRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("toggle")){
		relay2a.toggleRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("momentary")){
		relay2a.turnOnRelay(relayNumber);
		delay(300);
		relay2a.turnOffRelay(relayNumber);
		return 1;
	}
	return 0;
}
int triggerRelay1(String command){
	if(command.equalsIgnoreCase("turnonallrelays")){
		relay2b.turnOnAllRelays();
		return 1;
	}
	if(command.equalsIgnoreCase("turnoffallrelays")){
		relay2b.turnOffAllRelays();
		return 1;
	}
	if(command.startsWith("setBankStatus:")){
		int status1 = command.substring(14).toInt();
		if(status1 < 0 || status1 > 255){
			return 0;
		}
		Serial.print("Setting bank status to: ");
		Serial.println(status1);
		relay2b.setBankStatus(status1);
		Serial.println("done");
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	Serial.print("relayNumber: ");
	Serial.println(relayNumber);
	String relayCommand = command.substring(1);
	Serial.print("relayCommand:");
	Serial.print(relayCommand);
	Serial.println(".");
	if(relayCommand.equalsIgnoreCase("on")){
		Serial.println("Turning on relay");
		relay2b.turnOnRelay(relayNumber);
		Serial.println("returning");
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("off")){
		relay2b.turnOffRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("toggle")){
		relay2b.toggleRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("momentary")){
		relay2b.turnOnRelay(relayNumber);
		delay(300);
		relay2b.turnOffRelay(relayNumber);
		return 1;
	}
	return 0;
}

int triggerRelay4(String command){
	if(command.equalsIgnoreCase("turnonallrelays")){
		relay4a.turnOnAllRelays();
		return 1;
	}
	if(command.equalsIgnoreCase("turnoffallrelays")){
		relay4a.turnOffAllRelays();
		return 1;
	}
	if(command.startsWith("setBankStatus:")){
		int status4 = command.substring(14).toInt();
		if(status4 < 0 || status4 > 255){
			return 0;
		}
		Serial.print("Setting bank status to: ");
		Serial.println(status4);
		relay4a.setBankStatus(status4);
		Serial.println("done");
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	Serial.print("relayNumber: ");
	Serial.println(relayNumber);
	String relayCommand = command.substring(1);
	Serial.print("relayCommand:");
	Serial.print(relayCommand);
	Serial.println(".");
	if(relayCommand.equalsIgnoreCase("on")){
		Serial.println("Turning on relay");
		relay4a.turnOnRelay(relayNumber);
		Serial.println("returning");
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("off")){
		relay4a.turnOffRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("toggle")){
		relay4a.toggleRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("momentary")){
		relay4a.turnOnRelay(relayNumber);
		delay(300);
		relay4a.turnOffRelay(relayNumber);
		return 1;
	}
	return 0;
}

int triggerRelay2(String command){
	if(command.equalsIgnoreCase("turnonallrelays")){
		relay2c.turnOnAllRelays();
		return 1;
	}
	if(command.equalsIgnoreCase("turnoffallrelays")){
		relay2c.turnOffAllRelays();
		return 1;
	}
	if(command.startsWith("setBankStatus:")){
		int status2 = command.substring(14).toInt();
		if(status2 < 0 || status2 > 255){
			return 0;
		}
		Serial.print("Setting bank status to: ");
		Serial.println(status2);
		relay2b.setBankStatus(status2);
		Serial.println("done");
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	Serial.print("relayNumber: ");
	Serial.println(relayNumber);
	String relayCommand = command.substring(1);
	Serial.print("relayCommand:");
	Serial.print(relayCommand);
	Serial.println(".");
	if(relayCommand.equalsIgnoreCase("on")){
		Serial.println("Turning on relay");
		relay2c.turnOnRelay(relayNumber);
		Serial.println("returning");
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("off")){
		relay2c.turnOffRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("toggle")){
		relay2c.toggleRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("momentary")){
		relay2c.turnOnRelay(relayNumber);
		delay(300);
		relay2c.turnOffRelay(relayNumber);
		return 1;
	}
	return 0;
}







