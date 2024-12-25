#include <LiquidCrystal.h>
#include <Servo.h>

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
  void rotate(); //quay 3 góc và detect() sử dụng hàm detect ở trên 
};

usonic_ss ss; //cam bien tên u1 (vì mình có một cảm biến nên khao báo 1 biến thôi nhiều thì sẽ là mảng)

void setup() {
  ss.sv.attach(9); //pin cua servo ung voi u1
  pinMode(ss.echopin, OUTPUT);
  pinMode(ss.trigpin, INPUT);
}
void loop() {
  ss.rotate();
  displayonlcd(p);
}

void usonic_ss:: rotate(p_lot lot, int trigPIn, int echoPin){
digitalWrite(trigPin,LOW);//setup lại, không truyền điện cho chân trig
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);//bắt đầu truyền điện cho chân trig
delayMicroseconds(20);//chân trig sẽ phát ra sóng siêu âm trong vòng 20 micro giây// NOTE: phát sóng bao lâu là tối ưu?
digitalWrite(trigPin, LOW);//Tắt chân echo


khoang_tg=pulseIn(echoPin, HIGH,1000);//Hàm pulsein đo thời gian micro giây từ lúc chân echo nhận sóng phản hồi(HIGH) đến lúc kết thúc
quang_dg=ceil(khoang_tg*0.034/2);// công thức tính quãng đường

Serial.println(khoang_tg);
Serial.println(quang_dg);
if (quang_dg <= 2){ 
  lot.check=2;
  Serial.println(" Da có vat chan cam bien, can di chuyen vat can");
}
else if(2<quang_dg&&quang_dg<10){
  lot.check=1;
  Serial.println(" Da co xe do o day");
}
else{
 lot.check=0;
  Serial.println("Bai do con trong");
};
}; //quay 3 góc và detect() sử dụng hàm detect ở trên

void usonic_ss::detect();

void displayonlcd(p_lot pl[]);

