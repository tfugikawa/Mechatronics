const int prox1 = A4;
const int prox2 = A5;
const int switchPin = 13;

const int AIN1 = 7;
const int AIN2 = 6;
const int PWMA = 5;

const int BIN1 = 10;
const int BIN2 = 11;
const int PWMB = 9;

float distance[2];
float ref;

const int nominal = 100;
const int factor = 40;

void setup(){
  pinMode(prox1,INPUT); 
  pinMode(prox2,INPUT);
  pinMode(switchPin,INPUT_PULLUP);
  
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMB,OUTPUT);
  
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN2,HIGH);
  
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  
  Serial.begin(9600);
}

void loop(){
  //Initialize reference
  if (digitalRead(switchPin) == HIGH){
    analogWrite(PWMA,0);
    analogWrite(PWMB,0);
    getDistance();
    ref = distance[0];
  }
  else{
    getDistance(); // Sensor 0 is on the left side of the vehicle
    
    float diff = ref-distance[0];
    //motor B faster if diff>0
    
    int spA = min(max(nominal-factor*diff,50),255);
    int spB = min(max(nominal+factor*diff,50),255);
    
    Serial.print("A: ");
    Serial.print(spA);
    Serial.print("   B: ");
    Serial.println(spB);

    Serial.print("Ref: ");
    Serial.print(ref);
    Serial.print("   DistLeft: ");
    Serial.println(distance[0]);
//    Serial.print("   DistRight: ");
//    Serial.println(distance[1]);
    Serial.println();
    
    analogWrite(PWMA,spA);
    analogWrite(PWMB,spB);
    
    
//    Serial.print("Distance0:   ");
//    Serial.print(distance[0]);
//    Serial.print("   Distance1:   ");
//    Serial.println(distance[1]);
  }
}

void getDistance(){
  int numSamples = 5;
  float totDist[2] = {0,0};
  
  for(int i = 0;i<numSamples;i++){
    totDist[0] += analogRead(prox1);
    totDist[1] += analogRead(prox2);
    delay(40);
  }
  distance[0] = convertDist(totDist[0]/numSamples);
  distance[1] = convertDist(totDist[1]/numSamples);
}

float convertDist(float x){
  return ((6792/(x-9)) - 4);
}
