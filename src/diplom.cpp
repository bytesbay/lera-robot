#include <Arduino.h>

const uint8_t ACTION_DELAY = 50;
bool AUTO = false; // Положение автопилота (ВКЛ\ВЫКЛ)
bool ROTATING = false;

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
void fullStop() {
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
}

/* ДИСТАНЦИЯ */
float getDist() {

	digitalWrite(11, LOW);
	delayMicroseconds(2);
	digitalWrite(11, HIGH);
	delayMicroseconds(10);
	digitalWrite(11, LOW);

	return pulseIn(12, HIGH) / 29 / 2;
}

/* ОСВЕЩЕНИЕ */
int getLightness() {
	return analogRead(0);
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

		switch (input) {
			case 'F': forward(); break;
			case 'B': back(); break;
			case 'L': left(); break;
			case 'R': right(); break;

			case 'I': forwardLeft(); break;
			case 'G': forwardRight(); break;
			case 'J': backLeft(); break;
			case 'H': backRight(); break;

			case 'A': AUTO = true; break;
			case 'O': AUTO = false; break;
		}
	}

	if(getLightness() > 600) {
		digitalWrite(13, HIGH);
	} else {
		digitalWrite(13, LOW);
	}

	/* Если включен автопилот */
	if(AUTO) {

		/* Если в пределах 30см есть препятствие, то начинает крутиться */
		if(getDist() < 30) {
			/* Крутиться до тех пор, пока не станет стенки */
			left();
			left();
			left();
			uint8_t left_side = getDist();
			right();
			right();
			right();
			right();
			right();
			right();
			uint8_t right_side = getDist();

			if(right_side > left_side) {
				right();
				right();
				right();
			}
			else if(left_side > right_side) {
				left();
				left();
				left();
			}
			else {
				back();
				back();
				back();
				uint8_t rand_side = random(0, 2);
				if(rand_side) {
					left();
					left();
					left();
					left();
					left();
					left();
				} else {
					right();
					right();
					right();
					right();
					right();
					right();
				}
			}

		} else {
			forward();
		}
	}

	fullStop();
}
