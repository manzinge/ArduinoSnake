#define joystickX A0
#define joystickY A1

int joystick = 8;

//White LEDs
const int white1 = 10;
const int white2 = 12;
const int white3 = 11;
const int white4 = 13;

//Red LEDs
const int red1 = 7;
const int red2 = 4;
const int red3 = 6;
const int red4 = 5;

//Bue LEDs
const int blue1 = 47;
const int blue2 = 49;
const int blue3 = 51;
const int blue4 = 53;

//Green LEDs
const int green1 = 38;
const int green2 = 41;
const int green3 = 34;
const int green4 = 37;

const int rows = 4;
const int columns = 4;
int leds[rows][columns] = {{blue1,blue2,blue3,blue4}, {green1,green2,green3,green4},{red1,red2,red3,red4}, {white1,white2,white3,white4}};
int size = sizeof(leds) / sizeof(int);
char direction = 'r';
int headx = 100, heady=100;
int foodx = 100, foody=100;
unsigned long joystickmillis = millis();
unsigned long movemillis = millis();
int snakelength = 1;

void setup () {
  Serial.begin(38400);  
  clearleds();
  direction = 'r';
  snakelength = 1;
  headx = random(0,rows);
  heady = random(0,columns);
  headx = 0;
  heady = 0;
  foodx = random(0,rows);
  foody = random(0, columns);
  digitalWrite(leds[headx][heady],HIGH);
  digitalWrite(leds[foodx][foody],HIGH);
}

void loop () {
  unsigned long currentMillis = millis();

  //Reading direction of the joystick every 10th of a second
  if((unsigned long)(currentMillis - joystickmillis) >= 100) {

    direction = readinput(direction);
    joystickmillis = millis();
  }

  //Moving the snake every second and checking if it is in range of the playarea
  if((unsigned long)(currentMillis - movemillis) >=1000) {
      if(headx == foodx && heady == foody) {
        Serial.print("Food eaten!");
        snakelength++;
        spawnfood();
      }
      Serial.println(snakelength);
      checkifinrange();
      movesnake(readinput(direction));
      movemillis = millis();
  }
}
void movesnake(char direction) {
  switch(direction) {
    case 'l' : digitalWrite(leds[headx][heady],LOW);headx = headx-1;digitalWrite(leds[headx][heady],HIGH);break;
    case 'r' : digitalWrite(leds[headx][heady],LOW);headx = headx+1;digitalWrite(leds[headx][heady],HIGH);break;
    case 'u' : digitalWrite(leds[headx][heady],LOW);heady = heady-1;digitalWrite(leds[headx][heady],HIGH);break;
    case 'd' : digitalWrite(leds[headx][heady],LOW);heady = heady+1;digitalWrite(leds[headx][heady],HIGH);break;
    default: break;
  }
}
char readinput(char last) {
  int x = analogRead(joystickX);
  int y = analogRead(joystickY);
  char direction = last;
  if(x < 100) {
    direction = 'l';
  }
  else if (x > 1000) 
  {
    direction = 'r';
  }
  else if(y < 100) {
    direction = 'u';
  }
  else if(y > 1000) {
    direction = 'd';
  }
  return direction;
}

void spawnfood() {
  foodx = random(0,rows);
  foody = random(0,columns);
  while(digitalRead(leds[random(0,columns)][random(0,rows)]) == HIGH) {
    foodx = random(0,rows);
    foody = random(0,columns);
  }
  pinMode(leds[foodx][foody],HIGH);
}

void checkifinrange() {
  if(headx < 0 || headx > 3 || heady < 0 || heady > 3) {
    gameover();
  }
}
void gameover() {
  for(int i=0;i<3;i++) {
    activateleds();
    delay(500);
    clearleds();
    delay(500);
  }
  setup();
}
void activateleds() {
  for(int i=0;i<columns;i++) {
    for(int j=0;j<rows;j++) {
      digitalWrite(leds[i][j],HIGH);
      }
   }
}
void clearleds() {
  for(int i=0;i<columns;i++) {
    for(int j=0;j<rows;j++) {
      digitalWrite(leds[i][j],LOW);
      }
   }
}