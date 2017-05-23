#include <Arduino.h>

const uint8_t ACTION_DELAY = 50;
uint8_t AUTO = false; // Положение автопилота (ВКЛ\ВЫКЛ)

/* ДВИЖЕНИЕ */
void forward() {
	digitalWrite(3, HIGH);
	digitalWrite(4, HIGH);
	delay(ACTION_DELAY);
}
void back() {
	digitalWrite(2, HIGH);
	digitalWrite(5, HIGH);
	delay(ACTION_DELAY);
}
void left() {
	digitalWrite(2, HIGH);
	digitalWrite(4, HIGH);
	delay(ACTION_DELAY);
}
void right() {
	digitalWrite(5, HIGH);
	digitalWrite(3, HIGH);
	delay(ACTION_DELAY);
}
void forwardLeft() {
	digitalWrite(3, HIGH);
	delay(ACTION_DELAY);
}
void forwardRight() {
	digitalWrite(5, HIGH);
	delay(ACTION_DELAY);
}
void backLeft() {
	digitalWrite(2, HIGH);
	delay(ACTION_DELAY);
}
void backRight() {
	digitalWrite(4, HIGH);
	delay(ACTION_DELAY);
}

/* ДИСТАНЦИЯ */
int getDist() {
	// Clears the trigPin
	digitalWrite(11, LOW);
	delayMicroseconds(2);
	// Sets the trigPin on HIGH state for 10 micro seconds
	digitalWrite(11, HIGH);
	delayMicroseconds(10);
	digitalWrite(11, LOW);
	return pulseIn(12, HIGH) * (0.034 / 2);
}

void setup() {
	Serial.begin(9600);
	pinMode(2, OUTPUT); // ЛЕВО-Назад
	pinMode(3, OUTPUT); // ЛЕВО-Вперед
	pinMode(4, OUTPUT); // ПРАВО-Назад
	pinMode(5, OUTPUT); // ПРАВО-Вперед

	pinMode(12, INPUT);
	pinMode(11, OUTPUT);

	pinMode(13, OUTPUT); // Фары
}
void loop() {
	if(Serial.available() > 0) {

		uint8_t input = Serial.read();

		if(input == 'F') forward();
		else if(input == 'B') back();
		else if(input == 'L') left();
		else if(input == 'R') right();
        else if(input == 'I') forwardLeft();
    	else if(input == 'G') forwardRight();
        else if(input == 'J') backLeft();
    	else if(input == 'H') backRight();

		else if(input == 'A') AUTO = true; // Вкл автопилот
		else if(input == 'R') AUTO = false; // Выкл автопилот

		digitalWrite(2, LOW);
		digitalWrite(3, LOW);
		digitalWrite(4, LOW);
		digitalWrite(5, LOW);
	}

	if(analogRead(0) > 600) {
		digitalWrite(13, HIGH);
	} else {
		digitalWrite(13, LOW);
	}

	/* Если включен автопилот */
	if(AUTO) {
		/* Тупо едет вперед */
		forward();
		int tmp_dist = getDist();
		uint8_t side = random(0, 1);

		if(tmp_dist < 20) {
			/* Крутиться до тех пор, пока не станет стенки */
			for(uint8_t i = 0; getDist() < 20; i++) {
				/*
					0 - значит повернуть на лево
					1 - на право
				*/

				if(side == 0) left();
				else right();
			}
		}
	}
}
