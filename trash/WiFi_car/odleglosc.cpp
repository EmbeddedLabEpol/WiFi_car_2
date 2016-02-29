int Trig = 2;  //pin 2 Arduino po³¹czony z pinem Trigger czujnika
int Echo = 3;  //pin 3 Arduino po³¹czony z pinem Echo czujnika
int CM;        //odleg³oœæ w cm
long CZAS;     //d³ugoœæ powrotnego impulsu w uS
 
void setup()
{
  Serial.begin(9600);                        //inicjalizaja monitora szeregowego
  pinMode(Trig, OUTPUT);                     //ustawienie pinu 2 w Arduino jako wyjœcie
  pinMode(Echo, INPUT);                      //ustawienie pinu 3 w Arduino jako wejœcie
   
  Serial.println("Test czujnika odleglosci");
}
  
void loop()
{
  pomiar_odleglosci();              //pomiar odleg³oœci
  Serial.print("Odleglosc: ");      //wyœwietlanie wyników na ekranie w pêtli co 200 ms
  Serial.print(CM);
  Serial.println(" cm");
  delay(200);
}
  
void pomiar_odleglosci ()
{
  digitalWrite(Trig, HIGH);       //ustawienie stanu wysokiego na 10 uS - impuls inicjalizujacy - patrz dokumentacja
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  CZAS = pulseIn(Echo, HIGH);
  CM = CZAS / 58;                //szerokoœæ odbitego impulsu w uS podzielone przez 58 to odleglosc w cm - patrz dokumentacja
}