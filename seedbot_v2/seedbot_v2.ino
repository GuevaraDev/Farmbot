#include <Servo.h>
Servo myservo;
int x_input = 0, y_input = 0;
int x_actual = 1, y_actual = 1;
int mov_x = 0, mov_y = 0;
int count;
char serIn;

int pasos_mayor = 0;

int dir_x, dir_y;

//(ENA+ , DIR+ ,PUL+) entradas del driver a 5V del arduino
int mov_dir;
int n_pasos = 1000;
int xPUL = 9; //define Pulse pin negativo
int xDIR = 8; //define Direction pin negativo
int xENA = 7; //define Enable Pin negativo

int home_grid = 0;

int yPUL = 6; //define Pulse pin negativo
int yDIR = 5; //define Direction pin negativo
int yENA = 4; //define Enable Pin negativo

int zPUL = 2; //define Pulse pin negativo
int zDIR = 3; //define Direction pin negativo
int zENA = 10; //define Enable Pin negativo

int photo = 13;

int pos_init = 85, pos_fin = 10;
int seed = 1;

void setup() {
  digitalWrite(11, HIGH);
  myservo.attach(12);
  delay(125);
  myservo.write(pos_fin);
  pinMode(xPUL, OUTPUT);
  pinMode(xDIR, OUTPUT);
  pinMode(xENA, OUTPUT);
  pinMode(yPUL, OUTPUT);
  pinMode(yDIR, OUTPUT);
  pinMode(yENA, OUTPUT);
  pinMode(zPUL, OUTPUT);
  pinMode(zDIR, OUTPUT);
  pinMode(zENA, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(photo, OUTPUT);
  
  Serial.begin(9600);
}

void take_seed() {
  Serial.println(seed);
  switch(seed) {
    case 1:
      place(1, 1);
      break;
    case 2:
      place(2, 1);
      break;
    case 3:
      place(3, 1);
      break;
    case 4:
      place(4, 1);
      break;    
    case 5:
      place(5, 1);
      break;
  }
}

void riego() {
  //Rociador
  digitalWrite(11, LOW);
  delay(1000);
  digitalWrite(11, HIGH);
}

void axis_z(int dir, int dM, int eM, int pM, int pasos) {
  for (int j=0; j<pasos; j++) {
    j = j + 1;
    if (dir == 1) {
      digitalWrite(dM,LOW);
      Serial.println('D');
    } else if (dir == 2) {
      digitalWrite(dM,HIGH);
      Serial.println('U');
    }  
    digitalWrite(eM,HIGH);//Habilitas el driver, puedes jugar con este enable para ahorrar energia pero debes de conocer la estructura fisica para que no pierda posicion (despues te lo enseño)
    digitalWrite(pM,HIGH);//aqui los chingos de pulsos para que avance en pasitos
    delayMicroseconds(125);//con los delay les mueves a la velocidad del nema busca una velocidad de acuerdo a tu nema en la cual no pierda pasos (considera el peso de la estructura)
    digitalWrite(pM,LOW);//mas pinches pulsitos
    delayMicroseconds(125);
  }
  serIn = '{';
}

void open_grip() {
  for (int pos = 70; pos >= 0; pos -= 1) {
    // in steps of 1 degree
    myservo.write(pos);
    delay(15);
  }
  delay(100);
}

void close_grip() {
  for (int pos = 0; pos <= 70; pos += 1) {
    // in steps of 1 degree
    myservo.write(pos);
    delay(15);
  }
  delay(500);
}

void axis(int x_dir, int x_dM, int x_eM, int x_pM, int x_pasos, int y_dir, int y_dM, int y_eM, int y_pM, int y_pasos, int pasos_stop) {
  for (int i=0; i < pasos_stop; i++) {
    if (x_dir == 1) {
      digitalWrite(x_dM,LOW);
      Serial.println("F");
    } else if (x_dir == 2) {
      digitalWrite(x_dM,HIGH);
      Serial.println("B");
    }

    if (y_dir == 1) {
      digitalWrite(y_dM,LOW);
    } else if (y_dir == 2) {
      digitalWrite(y_dM,HIGH);
    }

    if (i <= x_pasos) {
      digitalWrite(x_eM,HIGH);
      digitalWrite(x_pM,HIGH);
      delayMicroseconds(125);
      digitalWrite(x_pM,LOW);
      delayMicroseconds(125);  
    }

    if (i <= y_pasos) {
      digitalWrite(y_eM,HIGH);
      digitalWrite(y_pM,HIGH);
      delayMicroseconds(125);
      digitalWrite(y_pM,LOW);
      delayMicroseconds(125);  
    }
    
  }
}

void place(int x, int y) {

  x_input = x;
  y_input = y;
  
  if (x_actual > x_input) {
    mov_x = (x_actual - x_input)*-1;
    dir_x = 2;
  } else if (x_input > x_actual) {
    mov_x = abs(x_actual - x_input);
    dir_x = 1;
  } else {
    mov_x = 0;
    dir_x = 1;
  }

  if (y_actual > y_input) {
    mov_y = (y_actual - y_input)*-1;
    dir_y = 1;
  } else if (y_input > y_actual) {
    mov_y = abs(y_actual - y_input);
    dir_y = 2;
  } else {
    mov_y = 0;
  }
  
  if (abs(mov_x*1700) > abs(mov_y*3580)) {
    pasos_mayor = abs(mov_x*1700); 
  } else {
    pasos_mayor = abs(mov_y*3580);   
  }

  axis(dir_x, xDIR, xENA, xPUL, abs(mov_x*1700), dir_y, yDIR, yENA, yPUL, abs(mov_y*3580), pasos_mayor);
  
  x_actual = x_input;
  y_actual = y_input;
  
  Serial.print("X: ");
  Serial.print(mov_x);
  
  Serial.print(" Y: ");
  Serial.println(mov_y);

  if(y_actual > 1) {
    /*digitalWrite(photo, HIGH);
    delay(800);
    digitalWrite(photo, LOW);*/
    axis_z(1, zDIR, zENA, zPUL, 2800);
    delay(500);
    //myservo.write(pos_init);
    //delay(1000);
    myservo.write(pos_fin);
    delay(500);
    axis_z(2, zDIR, zENA, zPUL, 2800);
    delay(100);
    riego();  
  } else if (home_grid == 5) {
    home_grid = 0;
  } else {
    axis_z(1, zDIR, zENA, zPUL, 2800);
    delay(500);
    myservo.write(pos_init);
    delay(1000);
    axis_z(2, zDIR, zENA, zPUL, 2800);
    delay(100);
  }
  
}

void plant(int a, int b) {
  home_grid = 0;
  take_seed();
  delay(300);
  place(a, b);
  
  home_grid = 5;
  place(1,1);
  
  digitalWrite(xPUL, LOW);
  digitalWrite(xDIR, LOW);
  digitalWrite(xENA, LOW);

  digitalWrite(yPUL, LOW);
  digitalWrite(yDIR, LOW);
  digitalWrite(yENA, LOW);
  
  digitalWrite(zPUL, LOW);
  digitalWrite(zDIR, LOW);
  digitalWrite(zENA, LOW);
}

void grid() {

  Serial.flush();
  
  if (Serial.available()) {
    serIn = Serial.read();
  }
  //Grid A
  if (serIn == '!') {
    seed = 1;
    //place(1, 1);
  } else if (serIn == '#') {
    seed = 2;
    //place(2, 1);
  } else if (serIn == '$') {
    seed = 3;
    //place(3, 1);
  } else if (serIn == '%') {
    seed = 4;
    //place(4, 1);
  } else if (serIn == '&') {
    seed = 5;
    //place(5, 1);
  //Grid A
  } else if (serIn == '1') {
    plant(1, 2);
  } else if (serIn == '2') {
    plant(2, 2);
  } else if (serIn == '3') {
    plant(3, 2);
  } else if (serIn == '4') {
    plant(4, 2);
  } else if (serIn == '5') {
    plant(5, 2);
  //Grid B
  } else if (serIn == '6') {
    plant(1, 3);
  } else if (serIn == '7') {
    plant(2, 3);
  } else if (serIn == '8') {
    plant(3, 3);
  } else if (serIn == '9') {
    plant(4, 3);
  } else if (serIn == 'M') {
    plant(5, 3);
  //Grid C
  } else if (serIn == 'N') {
    plant(1, 4);
  } else if (serIn == 'O') {
    plant(2, 4);
  } else if (serIn == 'P') {
    plant(3, 4);
  } else if (serIn == 'Q') {
    plant(4, 4);
  } else if (serIn == 'R') {
    plant(5, 4);
  //Grid d
  } else if (serIn == 'S') {
    plant(1, 5);
  } else if (serIn == 'T') {
    plant(2, 5);
  } else if (serIn == 'U') {
    plant(3, 5);
  } else if (serIn == 'V') {
    plant(4, 5);
  } else if (serIn == 'W') {
    plant(5, 5);
  //Grid E
  } else if (serIn == 'X') {
    plant(1, 6);
  } else if (serIn == 'Y') {
    plant(2, 6);
  } else if (serIn == 'Z') {
    plant(3, 6);
  } else if (serIn == '-') {
    plant(4, 6);
  } else if (serIn == '+') {
    plant(5, 6);
  }
  //serIn = '{';
}

void loop() {
  grid();
}
