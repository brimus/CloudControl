// This #include statement was automatically added by the Particle IDE.
#include <NCD4Relay.h>

// This #include statement was automatically added by the Particle IDE.
#include <NCD2Relay.h>

/* Includes ------------------------------------------------------------------*/
#include <application.h>
#include <spark_wiring_i2c.h>

// HCPA-5V-U3 I2C address is 0x28(40)
#define HCPAddr 0x28
// TCN75A I2C address is 0x48(72)
#define TCNAddr 0x48


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

int ttemp = 0;
int count = 0;

double hcTemp = 0.0;
double hfTemp = 0.0;
double humidity = 0.0;
double tcTemp = 0.0;
double tfTemp = 0.0;

String version = "1.0.8";

/* This function is called once at start up ----------------------------------*/
void setup()
{
    //Particle.process();
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
	Particle.variable("Hi2cdevice", "HCPA-5V-U3");
	Particle.variable("humidity", humidity);
	Particle.variable("hcTemp", hcTemp);
	Particle.variable("hfTemp", hfTemp);
	Particle.variable("Ti2cdevice", "TCN75A");
	Particle.variable("tcTemp", tcTemp);
	Particle.variable("tfTemp", tfTemp);
	Particle.variable("Count", count);
	Particle.variable("Version", version);
	//Particle.variable("Cell IP", Cellular.localIP());
	//Particle.variable("WIFI IP", WiFi.localIP());
	//Particle.variable("WIFI IP", WiFi.gatewayIP());
	// Starting
	Serial.begin(115200);
	Wire.begin();
	// I2C Addresses
	relay2a.setAddress(0,0,0);
	relay2b.setAddress(1,0,0);
	relay4a.setAddress(0,1,0);
	relay2c.setAddress(0,0,1);

	Wire.beginTransmission(HCPAddr);
	Wire.write(0x80);
	Wire.endTransmission();
	delay(100);

	Wire.beginTransmission(TCNAddr);
	Wire.write(0x01);
	Wire.write(0x60);
	Wire.endTransmission();
	delay(100);

	// End of Setup
}

/* This function loops forever --------------------------------------------*/
void loop()
{
    
	// HCPA Temp sensor
	unsigned int hdata[4];
	Wire.beginTransmission(HCPAddr);
	Wire.endTransmission();
	Wire.requestFrom(HCPAddr, 4);
	if (Wire.available() == 4)
	{
		hdata[0] = Wire.read();
		hdata[1] = Wire.read();
		hdata[2] = Wire.read();
		hdata[3] = Wire.read();
		
		humidity = (((hdata[0] & 0x3F) * 256) + hdata[1]) / 16384.0 * 100.0;
		hcTemp = (((hdata[2] * 256) + (hdata[3] & 0xFC)) / 4) / 16384.0 * 165.0 - 40.0;
		hfTemp = (hcTemp * 1.8) + 32;
		
		if (count = 0)
		{
		    Particle.publish("HCPA Relative humidity : ", String(humidity));
		    Particle.publish("HCPA Temperature in Fahrenheit : ", String(hfTemp));
		    //Particle.publish("HCPA Temperature in Celsius : ", String(hcTemp));
		}
		if (hfTemp > 80)
		{
		    triggerRelay("turnonallrelays");
		}else{
		    triggerRelay("turnoffallrelays");
		}
	}
	// TCN75A Temp sensor
	unsigned int tdata[2];
	Wire.beginTransmission(TCNAddr);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(TCNAddr, 2);
	if (Wire.available() == 2)
	{
	    tdata[0] = Wire.read();
	    tdata[1] = Wire.read();
	    ttemp = (((tdata[0] * 256) + (tdata[1] & 0xF0)) / 16);
	    if(ttemp > 2047)
	    {
	        ttemp -= 4096;	
	    }
	    tcTemp = ttemp * 0.0625;
	    tfTemp = (tcTemp * 1.8) + 32;
	    if (count = 0)
	    {
	        //Particle.publish("TCN75A Temperature in Celsius : ", String(tcTemp));
	        Particle.publish("TCN75A Temperature in Fahrenheit : ", String(tfTemp));
	    }
		if (tfTemp > 80)
		{
		    triggerRelay1("turnonallrelays");
		}else{
		    triggerRelay1("turnoffallrelays");
		}

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
			}
		}
		c++;
	}
	count++;
	if (count > 300)
	{
	    count = 0;
	}
	delay(300);
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
		relay2a.setBankStatus(status);
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	String relayCommand = command.substring(1);
	if(relayCommand.equalsIgnoreCase("on")){
		relay2a.turnOnRelay(relayNumber);
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
		relay2b.setBankStatus(status1);
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	String relayCommand = command.substring(1);
	if(relayCommand.equalsIgnoreCase("on")){
		relay2b.turnOnRelay(relayNumber);
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
		relay4a.setBankStatus(status4);
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	String relayCommand = command.substring(1);
	if(relayCommand.equalsIgnoreCase("on")){
		relay4a.turnOnRelay(relayNumber);
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
		relay2b.setBankStatus(status2);
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	String relayCommand = command.substring(1);
	if(relayCommand.equalsIgnoreCase("on")){
		relay2c.turnOnRelay(relayNumber);
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







