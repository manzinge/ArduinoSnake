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

//Initializing the array
int leds[rows][columns] = {{blue1,blue2,blue3,blue4}, {green1,green2,green3,green4},{red1,red2,red3,red4}, {white1,white2,white3,white4}};
int historysnake[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Initializing useful variables
char direction = 'r';
int headx = 0, heady=0;
int foodx = 100, foody=100;
unsigned long joystickmillis = millis();
unsigned long movemillis = millis();
int tail = 0;

void setup () {
  Serial.begin(38400);  
  clearleds(true);
  direction = 'r';
  tail = 0;
  headx = 0;
  heady = 0;
  digitalWrite(leds[headx][heady],HIGH);
  spawnfood();
}

void loop () {
  //Getting current time
  unsigned long currentMillis = millis();

  //Reading direction of the joystick every 10th of a second for better controls
  if((unsigned long)(currentMillis - joystickmillis) >= 100) {
    direction = readinput(direction);
    joystickmillis = millis();
  }

  //Moving the snake every second and checking if it is in range of the playarea
  if((unsigned long)(currentMillis - movemillis) >=1000) {
      //In case the head touches the food, the snake will eat the food and grow larger
      if(headx == foodx && heady == foody) {
        tail++;
        spawnfood();
      }
      checkifinrange(); //Checks if player is out of bounds
      movesnake(readinput(direction));
      movemillis = millis();
  }
}

//Function to move the snake in the direction given by the joystick
void movesnake(char direction) {
  for(int g = 37;g>=0;g--) {
    historysnake[g+2] = historysnake[g];
  }
  historysnake[0] = headx;
  historysnake[1] = heady;
  
  switch(direction) {
    case 'l' : headx = headx-1;break;
    case 'r' : headx = headx+1;break;
    case 'u' : heady = heady-1;break;
    case 'd' : heady = heady+1;break;
    default: break;
  }
  //Disabling all LEDs (except for food)
  clearleds(false);

  //Activating LEDs with tail
  digitalWrite(leds[headx][heady],HIGH);
  for(int i=0;i<tail*2-1;i+=2) {
    digitalWrite(leds[historysnake[i]][historysnake[i+1]],HIGH);
  }
}

//Function to read the Input of the joystick and converting it into a direction
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
  return direction; //Returning char for direction (l = left, r = right, u = up, d = down)
}

//function to spawn food (Checks if the randomly generated spot is already used by the snake)
void spawnfood() {
  foodx = random(0,rows);
  foody = random(0,columns);
  while(digitalRead(leds[foodx][foody]) == HIGH) {
    foodx = random(0,rows);
    foody = random(0,columns);
  }
  digitalWrite(leds[foodx][foody],HIGH);
}

//Function to check if the player is inside of bounds
void checkifinrange() {
  if(headx < 0 || headx > 3 || heady < 0 || heady > 3) {
    gameover();
  }
}

//Function to restart the game (flashes LEDs beforehand)
void gameover() {
  for(int i=0;i<3;i++) {
    activateleds();
    delay(500);
    clearleds(true);
    delay(500);
  }
  setup();
}

//Function to lighten up all LEDs
void activateleds() {
  for(int i=0;i<columns;i++) {
    for(int j=0;j<rows;j++) {
      digitalWrite(leds[i][j],HIGH);
      }
   }
}

//Function to power off all LEDs
void clearleds(bool clearfood) {
  for(int i=0;i<columns;i++) {
    for(int j=0;j<rows;j++) {
      //Bool paramter in function head in order to determine if food should be cleared as well
      if(i == foodx && j == foody && clearfood == false) { }
      else {
        digitalWrite(leds[i][j],LOW);
      }
    }
  }
}