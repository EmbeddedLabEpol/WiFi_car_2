int Trig = 2;  //pin 2 Arduino po��czony z pinem Trigger czujnika
int Echo = 3;  //pin 3 Arduino po��czony z pinem Echo czujnika
int CM;        //odleg�o�� w cm
long CZAS;     //d�ugo�� powrotnego impulsu w uS
 
void setup()
{
  Serial.begin(9600);                        //inicjalizaja monitora szeregowego
  pinMode(Trig, OUTPUT);                     //ustawienie pinu 2 w Arduino jako wyj�cie
  pinMode(Echo, INPUT);                      //ustawienie pinu 3 w Arduino jako wej�cie
   
  Serial.println("Test czujnika odleglosci");
}
  
void loop()
{
  pomiar_odleglosci();              //pomiar odleg�o�ci
  Serial.print("Odleglosc: ");      //wy�wietlanie wynik�w na ekranie w p�tli co 200 ms
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
  CM = CZAS / 58;                //szeroko�� odbitego impulsu w uS podzielone przez 58 to odleglosc w cm - patrz dokumentacja
}