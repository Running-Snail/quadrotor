#define QD_HELLO 0x48
#define QD_SET_MOTOR 0x53
#define QD_FEEDBACK 0x41
#define QD_ERROR 0x45

#define QD_PKG_LENGTH 32

#define QD_BUFFER_OVERFLOW_ERROR "\x45\x01\x01\x45"
#define QD_OK_FEEDBACK "\x41\x02\x6f\x6b\x47"
#define QD_HELLO_FEEDBACK "\x41\x05HELLO\x06"

#define QD_STATE_CMD 0
#define QD_STATE_LEN 1
#define QD_STATE_DAT 2
#define QD_STATE_CHECK 3

#define QD_MOTOR_0 OCR0A
#define QD_MOTOR_1 OCR0B
#define QD_MOTOR_2 OCR2A
#define QD_MOTOR_3 OCR2B

volatile unsigned char qd_pkg[32];
volatile unsigned int qd_pkg_top = 0;
volatile unsigned char qd_pkg_state = QD_STATE_CMD;
volatile unsigned int qd_pkg_data_len_now = 0;
volatile unsigned int qd_pkg_data_len = 0;

void qdSetMotorSpeed(unsigned char motor, unsigned char spd) {
  switch(motor) {
    case 0:
      QD_MOTOR_0 = spd;
      break;
    case 1:
      QD_MOTOR_1 = spd;
      break;
    case 2:
      QD_MOTOR_2 = spd;
      break;
    case 3:
      QD_MOTOR_3 = spd;
      break;
  }
}

void qdExecPkg() {
  switch (qd_pkg[0]) {
    case QD_HELLO:
      Serial.write(QD_HELLO_FEEDBACK);
      break;
    case QD_SET_MOTOR:
      qdSetMotorSpeed(qd_pkg[2], qd_pkg[3]);
      Serial.write(qd_pkg[3]);
      break;
  }
  qd_pkg_data_len = 0;
  qd_pkg_data_len_now = 0;
  qd_pkg_top = 0;
  qd_pkg_state = QD_STATE_CMD;
}

byte byteRead;

void setup() {
  DDRD = _BV(PD5)|_BV(PD6); // all output
  TCCR0A = 0xA3; // fast pwm
  TCCR0B = 0x2; // feq/8
  OCR0A = 0; // PD6 D6
  OCR0B = 0; // PD5 D5

  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    byteRead = Serial.read();
    qd_pkg[qd_pkg_top] = byteRead;
    qd_pkg_top += 1;
    if (qd_pkg_top >= QD_PKG_LENGTH) {
      Serial.write(QD_BUFFER_OVERFLOW_ERROR);
    }

    switch (qd_pkg_state) {
      case QD_STATE_CMD:
        if (!(byteRead & 0x80)) qd_pkg_state = QD_STATE_LEN;
        break;
      case QD_STATE_LEN:
        qd_pkg_data_len = (qd_pkg_data_len << 8) | byteRead;
        if (!(byteRead & 0x80)) {
          if (qd_pkg_data_len == 0) {
            qd_pkg_state = QD_STATE_CHECK;
          } else {
            qd_pkg_state = QD_STATE_DAT;
          }
        }
        break;
      case QD_STATE_DAT:
        qd_pkg_data_len_now += 1;
        if (qd_pkg_data_len_now >= qd_pkg_data_len) {
          qd_pkg_state = QD_STATE_CHECK;
        }
        break;
      case QD_STATE_CHECK:
        qdExecPkg();
        break;
    }
  }
}
