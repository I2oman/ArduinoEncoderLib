#pragma once
#include <Arduino.h>
#include <ArduinoButtonLib.h>

enum EncoderType {
  TYPE1,
  TYPE2
};

enum RotationType {
  CLOCKWISE,
  ANTICLOCKWISE,
  CLOCKWISE_HOLD,
  ANTICLOCKWISE_HOLD,
  CLOCKWISE_CLICK_AND_HOLD,
  ANTICLOCKWISE_CLICK_AND_HOLD,
  NO_ROTATION
};

struct ENCrecord {
  bool s1;
  bool s2;
};

class Encoder {
public:
  // Encoder(uint8_t s1Pin, uint8_t s2Pin, uint8_t btnPin, EncoderType type);
  Encoder(uint8_t s1Pin, uint8_t s2Pin, uint8_t btnPin, EncoderType type = TYPE2);

  void setType(EncoderType type);
  void begin();

  RotationType getRotation();

  bool clockwise();
  bool anticlockwise();

  bool clockwiseHold();
  bool anticlockwiseHold();

  bool clockwiseClickAndHold();
  bool anticlockwiseClickAndHold();

  Button getBtn();
  ClickType getBtnState();

private:
  uint8_t s1Pin;
  uint8_t s2Pin;
  Button button;
  EncoderType type;

  bool update;

  ENCrecord encRecords[5];

  ENCrecord CW_step[5];
  ENCrecord ACW_step[5];

  void updateRecords(ENCrecord record);
  bool compareRecords(ENCrecord record1, ENCrecord record2);

  bool clockwiseRotation();
  bool anticlockwiseRotation();

  RotationType combineRotationAndBtnPosition(RotationType rotation, ClickType click);

  void sout();
};
