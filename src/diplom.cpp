#include <Arduino.h>

const uint8_t ACTION_DELAY = 100;
bool AUTO = false; // Положение автопилота (ВКЛ\ВЫКЛ)

/* ДВИЖЕНИЕ */
void forward() {
	digitalWrite(3, HIGH);
	digitalWrite(4, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
}
void back() {
	digitalWrite(2, HIGH);
	digitalWrite(5, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(2, LOW);
	digitalWrite(5, LOW);
}
void left() {
	digitalWrite(2, HIGH);
	digitalWrite(4, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(2, LOW);
	digitalWrite(4, LOW);
}
void right() {
	digitalWrite(5, HIGH);
	digitalWrite(3, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(5, LOW);
	digitalWrite(3, LOW);
}
void forwardLeft() {
	digitalWrite(3, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(3, LOW);
}
void forwardRight() {
	digitalWrite(5, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(5, LOW);
}
void backLeft() {
	digitalWrite(2, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(2, LOW);
}
void backRight() {
	digitalWrite(4, HIGH);
	delay(ACTION_DELAY);
	digitalWrite(4, LOW);
}
void fullStop() {
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
}

/* ДИСТАНЦИЯ */
int getDist() {

	digitalWrite(11, LOW);
	delayMicroseconds(2);
	digitalWrite(11, HIGH);
	delayMicroseconds(10);
	digitalWrite(11, LOW);
	int distance = pulseIn(12, HIGH) / 29 / 2;
	if(distance > 2000) distance = 2;
	else if(distance > 500) distance = 500;
	return distance;
}

/* ОСВЕЩЕНИЕ */
int getLightness() {
	return analogRead(0);
}

/* ВИБРАЦИЯ */
void vibrate() {
	left();
	right();
	left();
	right();
	left();
	right();
}

void setup() {
	randomSeed(analogRead(3));
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

			case 'A':
				if(!AUTO) AUTO = true;
				else AUTO = false;
				break;
			case 'D':
				Serial.print("Distance: ");
				Serial.print(getDist());
				Serial.print(", Ligtness: ");
				Serial.print(getLightness());
				Serial.print(", IR-sensor: ");
				Serial.println(digitalRead(10));
				break;

			/* Вибрация */
			case 'V': vibrate(); break;
		}
	}

	if(getLightness() > 600) {
		digitalWrite(13, HIGH);
	} else {
		digitalWrite(13, LOW);
	}

	/* Если включен автопилот */
	if(AUTO) {

		/* Если в пределах 20см есть препятствие, то начинает крутиться */
		if(getDist() < 15) {

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
			left();
			left();
			left();
			if(left_side > right_side) {
				left();
				left();
				left();
				left();
				left();
				left();
			}
			else if(right_side > left_side) {
				right();
				right();
				right();
				right();
				right();
				right();
			}
			else {
				uint8_t rand_side = random(0, 2);
				back();
				back();
				back();
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
		}
		/* Если ничего нет, то ехать вперед и обновлять сторону */
		else {
			forward();
		}
	}
	fullStop();
}
