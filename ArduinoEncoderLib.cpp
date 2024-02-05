#include "ArduinoEncoderLib.h"

Encoder::Encoder(uint8_t s1Pin, uint8_t s2Pin, uint8_t btnPin, EncoderType type) {
  this->s1Pin = s1Pin;
  this->s2Pin = s2Pin;
  this->button = Button(btnPin);
  this->type = type;
}

void Encoder::setType(EncoderType type) {
  this->type = type;
}

void Encoder::begin() {
  pinMode(s1Pin, INPUT);
  pinMode(s2Pin, INPUT);
  button.begin();

  CW_step[4] = { 1, 1 };
  CW_step[3] = { 1, 0 };
  CW_step[2] = { 0, 0 };
  CW_step[1] = { 0, 1 };
  CW_step[0] = { 1, 1 };

  ACW_step[4] = { 1, 1 };
  ACW_step[3] = { 0, 1 };
  ACW_step[2] = { 0, 0 };
  ACW_step[1] = { 1, 0 };
  ACW_step[0] = { 1, 1 };

  for (uint8_t i = 0; i < 5; i++) {
    encRecords[i].s1 = 1;
    encRecords[i].s2 = 1;
  }
}

void Encoder::updateRecords(ENCrecord record) {
  for (uint8_t i = 4; i >= 1; i--) {
    encRecords[i] = encRecords[i - 1];
  }
  encRecords[0] = record;
}

bool Encoder::compareRecords(ENCrecord record1, ENCrecord record2) {
  return (record1.s1 == record2.s1) && (record1.s2 == record2.s2);
}

bool Encoder::clockwiseRotation() {
  for (uint8_t i = 0; i < 5; i++) {
    if (!compareRecords(CW_step[i], encRecords[i])) {
      return false;
    }
  }
  return true;
}

bool Encoder::anticlockwiseRotation() {
  for (uint8_t i = 0; i < 5; i++) {
    if (!compareRecords(ACW_step[i], encRecords[i])) {
      return false;
    }
  }
  return true;
}

RotationType Encoder::combineRotationAndBtnPosition(RotationType rotation, ClickType click) {
  if (rotation == NO_ROTATION) return rotation;
  if (click == NO_CLICK) return rotation;
  //---------------------------------------------------------
  if (rotation == CLOCKWISE) {
    if (click == HOLD) return CLOCKWISE_HOLD;
    if (click == CLICK_AND_HOLD) return CLOCKWISE_CLICK_AND_HOLD;
  }
  //---------------------------------------------------------
  if (rotation == ANTICLOCKWISE) {
    if (click == HOLD) return ANTICLOCKWISE_HOLD;
    if (click == CLICK_AND_HOLD) return ANTICLOCKWISE_CLICK_AND_HOLD;
  }
}

void Encoder::sout() {
  Serial.println("New rec");
  Serial.println("-----------");
  for (uint8_t i = 4; i > -1; i--) {
    Serial.print("|");
    Serial.print(encRecords[i].s1);
  }
  Serial.println("|");
  Serial.println("-----------");
  for (uint8_t i = 4; i > -1; i--) {
    Serial.print("|");
    Serial.print(encRecords[i].s2);
  }
  Serial.println("|");
  Serial.println("-----------");
}

RotationType Encoder::getRotation() {
  RotationType rotation = NO_ROTATION;
  ClickType click = NO_CLICK;

  ENCrecord newRec{ digitalRead(s1Pin), digitalRead(s2Pin) };
  if (!compareRecords(newRec, encRecords[0])) {
    updateRecords(newRec);
    update = 1;
    // sout();
  }

  switch (type) {
    case TYPE1:
      break;
    case TYPE2:
      if (clockwiseRotation()) {
        rotation = CLOCKWISE;
      }
      if (anticlockwiseRotation()) {
        rotation = ANTICLOCKWISE;
      }
      click = button.getState();
      break;
  }

  if (update) {
    update = 0;
    return combineRotationAndBtnPosition(rotation, click);
  } else {
    return NO_ROTATION;
  }
}

bool Encoder::clockwise() {
  return getRotation() == CLOCKWISE;
}
bool Encoder::anticlockwise() {
  return getRotation() == ANTICLOCKWISE;
}
bool Encoder::clockwiseHold() {
  return getRotation() == CLOCKWISE_HOLD;
}
bool Encoder::anticlockwiseHold() {
  return getRotation() == ANTICLOCKWISE_HOLD;
}
bool Encoder::clockwiseClickAndHold() {
  return getRotation() == CLOCKWISE_CLICK_AND_HOLD;
}
bool Encoder::anticlockwiseClickAndHold() {
  return getRotation() == ANTICLOCKWISE_CLICK_AND_HOLD;
}

Button Encoder::getBtn() {
  return button;
}
ClickType Encoder::getBtnState() {
  return button.getState();
}
