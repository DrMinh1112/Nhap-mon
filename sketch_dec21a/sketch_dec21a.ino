#include <LiquidCrystal_I2C.h>
#include <Servo.h>
LiquidCrystal_I2C lcd(___,16,2)

int angle1;
int angle2;
int angle3;

typedef struct{
  char name[10];
  int check; //bằng 0 nếu ko có xe đỗ và ngược lại
}p_lot; //kieu du lieu gom cac ô đỗ xe
p_lot p[3]={{"a1",0},{"a2",0},{"a3",0}};// mảng gồm các chỗ đỗ xe

class usonic_ss{
  public:
  Servo sv; // servo ứng với cảm biến đó
  int id_p_lot[3];// mảng chứa chỉ số của mảng p[] tức là những ô mà cảm biến đấy sẽ detect
  int echopin;
  int trigpin;
  usonic_ss(): id_p_lot({0,1,2}), echopin(7), trigpin(8) {} // không truyền tham số thì mặc định là như này
  usonic_ss(int id[],int trig, int echo): echopin(echo),trigpin(trig) {
    for(int i=0;i<3;i++){
      id_p_lot[i]=id[i];
    }
  }
  ~usonic_ss(){}
  void detect(int x); //check xem xe đấy đỗ chx (khoảng cách <= bn đấy thì p[x].check=1 và ngược lại)
  void rotate(){
    // Quay servo đến góc 60 độ
    sv.write(60);
    delay(1000); // Đợi 1 giây để servo ổn định
    detect(0);
    // Quay servo đến góc 120 độ
    sv.write(120);
    delay(1000);
    detect(1);
    // Quay servo đến góc 180 độ
    sv.write(180);
    delay(1000);
    detect(2);    
};

usonic_ss ss; //cam bien tên u1 (vì mình có một cảm biến nên khao báo 1 biến thôi nhiều thì sẽ là mảng)

void setup() {
  ss.sv.attach(9); //pin cua servo ung voi u1
  pinMode(ss.echopin, OUTPUT);
  pinMode(ss.trigpin, INPUT);
  lcd.init();
  lcd.backlight();
}

void loop() {
  ss.rotate();
  displayonlcd(p);
}

void usonic_ss:: rotate(); //quay 3 góc và detect() sử dụng hàm detect ở trên

void usonic_ss::detect();

void displayonlcd(p_lot pl[]) {
  bool con_slot = false;
  for (int i = 0; i < 3; i++) {
    if (pl[i].check == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print ("Hay do xe vao o: ");
      lcd.setCursor(0,1);
      lcd.print (pl[i].name);
      con_slot = true;
      break;
    }
  }
  if (con_slot == false) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print ("Da het cho de xe");
    lcd.setCursor(0,1);
    lcd.print ("           ");
  }
