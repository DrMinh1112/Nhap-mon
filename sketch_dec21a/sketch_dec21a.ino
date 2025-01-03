#include <LiquidCrystal_I2C.h>
#include <Servo.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int angle1;
int angle2;
int angle3;

typedef struct {
  char name[10];
  int check;                                             //bằng 0 nếu ko có xe đỗ và ngược lại
} p_lot;                                                 //kieu du lieu gom cac ô đỗ xe
p_lot p[3] = { { "a1", 0 }, { "a2", 0 }, { "a3", 0 } };  // mảng gồm các chỗ đỗ xe

class usonic_ss {
public:
  Servo sv;         // servo ứng với cảm biến đó
  int id_p_lot[3];  // mảng chứa chỉ số của mảng p[] tức là những ô mà cảm biến đấy sẽ detect
  int echopin;
  int trigpin;
  usonic_ss()
    : id_p_lot{ 0, 1, 2 }, echopin(7), trigpin(8) {}  // không truyền tham số thì mặc định là như này
  usonic_ss(int id[], int trig, int echo)
    : echopin(echo), trigpin(trig) {
    for (int i = 0; i < 3; i++) {
      id_p_lot[i] = id[i];
    }
  }
  ~usonic_ss() {}
  void detect(int x);
  void rotate();
  void attachServo(int pin) {
    sv.attach(pin);
  }
};
usonic_ss ss;  //cam bien tên u1 (vì mình có một cảm biến nên khao báo 1 biến thôi nhiều thì sẽ là mảng)

void setup() {
  Serial.begin(9600);
  ss.attachServo(9);  //pin cua servo ung voi u1
  pinMode(ss.echopin, INPUT);
  pinMode(ss.trigpin, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop() {
  ss.rotate();
  displayonlcd(p);
}

void usonic_ss::detect(int x) {
  digitalWrite(trigpin, LOW);  //setup lại, không truyền điện cho chân trig
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);  //bắt đầu truyền điện cho chân trig
  delayMicroseconds(10);        //chân trig sẽ phát ra sóng siêu âm trong vòng 20 micro giây// NOTE: phát sóng bao lâu là tối ưu?
  digitalWrite(trigpin, LOW);   //Tắt chân echo

  long khoang_tg = pulseIn(echopin, HIGH);     //Hàm pulsein đo thời gian micro giây từ lúc chân echo nhận sóng phản hồi(HIGH) đến lúc kết thúc
  int quang_dg = ceil(khoang_tg * 0.034 / 2);  // công thức tính quãng đường
  Serial.println(quang_dg);
  if (quang_dg <= 2) {
    p[x].check = 2;
    Serial.println(" Da có vat chan cam bien, can di chuyen vat can");
  } else if (2 < quang_dg && quang_dg < 10) {
    p[x].check = 1;
    Serial.println(" Da co xe do o day");
  } else {
    p[x].check = 0;
    Serial.println("Bai do con trong");
  }
};

//quay 3 góc và detect() sử dụng hàm detect ở trên
void usonic_ss::rotate() {
  // Quay servo đến góc 60 độ
  sv.write(60);
  detect(id_p_lot[0]);
  delay(2000);  // Đợi 1 giây để servo ổn định
  // Quay servo đến góc 120 độ
  sv.write(120);
  detect(id_p_lot[1]);
  delay(2000);
  // Quay servo đến góc 180 độ
  sv.write(180);
  detect(id_p_lot[2]);
  delay(2000);
};

void displayonlcd(p_lot pl[]) {
  bool con_slot = false;
  for (int i = 0; i < 3; i++) {
    if (pl[i].check == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hay do xe vao o: ");
      lcd.setCursor(0, 1);
      lcd.print(pl[i].name);
      con_slot = true;
      break;
    }
  }
  if (con_slot == false) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Da het cho de xe");
  }
}
