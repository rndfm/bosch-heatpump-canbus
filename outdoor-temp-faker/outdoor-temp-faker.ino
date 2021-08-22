int duty = 0;
float target_voltage = 2.5;
int every = 0;
const int voltage_max = 5;
const float compensation = .26;
const float voltage_divider = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(D7, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

const byte numChars = 32;

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    static char command = 'u';
    static char receivedChars[numChars];
   
    while (Serial.available() > 0) {
        rc = Serial.read();
        
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0';
            target_voltage = atof(receivedChars);
            Serial.println(target_voltage);
            ndx = 0;
        }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  recvWithEndMarker();
  every++;
  int read = analogRead(A0);
  read += analogRead(A0);
  read += analogRead(A0);
  read += analogRead(A0);
  read += analogRead(A0);
  read = read / 5;
  float voltage = (read / 1024.0) * 3.3;
  voltage += voltage * compensation;
  voltage = voltage * voltage_divider;
  
  if (voltage > target_voltage && duty < 1024)
  {
    duty++;
  }
  if (voltage < target_voltage && duty > 0)
  {
    duty--;
  }

  if (every > 1000)
  {
    Serial.print(voltage);
    Serial.print("v - ");
    Serial.print(read);
    Serial.print(" - ");
    Serial.println(duty); 
    every = 0; 
  }
  
  analogWrite(D7, duty);
  delay(1);
}
