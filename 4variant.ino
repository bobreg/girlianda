int pinBlue = 7;            //вывод для синего диода
int pinYellow = 8;          //вывод для жёлтого диода
int pinRed = 9;             //вывод для красного диода
int pinButton = A1;         //контакт кнопки
int ledLine = 3;            //контакт оптопары для управления гирляндой
int clickButton = 0;        //счётчик нажатий
byte numbers[7] = {0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111};  //массив вариантов выбора из трёх разных режимов
boolean flag = false;       //переменная для учёта отпускания кнопки, а не дребезга контактов
byte mask = 0b111;          //переменная которая принимает выбранный режим работы из массива number[]
int ledMax = 255;           //максимальная яркость гирлянды (ШИМ)
int ledMin = 20;            //минимальная яркость гирлянды (ШИМ)
const int pause = 20;       //контанта паузы
boolean onLed;              //переменная которая принимает значение 1 или 0 из переменной mask
void setup() {
  attachInterrupt (0, selectionMode, LOW);  //прерывание по тумпблеру. (0 соответствует контакту 2 на ардуино, 
  pinMode(pinBlue, OUTPUT);                 //                          selectionMode - функция к которой следует обратиться,
  pinMode(pinYellow, OUTPUT);               //                          LOW - прерывание будет выполняться до тех пор пока
  pinMode(pinRed, OUTPUT);                  //                          на контакте 2 низкий потенциал)
  pinMode(ledLine, OUTPUT);
  pinMode(pinButton, INPUT);
  for(int i = 0; i < 3; i++){ //цикл включения трез светодиодов зависящий от переменной mask
    onLed = bitRead(mask,i);  //побайтное считывание данных из переменной mask
    digitalWrite(i + 7, onLed);//включение определённого диода i+7 при условии что onLed примет значение 1
  }
}
void loop() {
  for(int i = 0; i < 3; i++){ //цикл включения различных режимов мигания гирлянды
    onLed = bitRead(mask,i);  //побайтное считывание данных из переменной mask
    if(onLed == 1 && i == 0){ //режим включится только если i будет равно 0, а onLed равно 1
      lift1();
    }
    if(onLed == 1 && i == 1){ //режим включится только если i будет равно 1, а onLed равно 1
      lift2();
    }
    if(onLed == 1 && i == 2){ //режим включится только если i будет равно 2, а onLed равно 1
      lift3();
    }
  }
}
void selectionMode(){
  // код который фиксирует нажатие и увеличивает счётчик ------------------------------------------------------------
  boolean buttonState = digitalRead(pinButton);  //фиксируем нажатие кнопки в этот момент
  if(buttonState == HIGH && flag == false){      //если кнопка нажата, а флаг false фиксируем нажатие
    flag = true;                                 //сразу переводим flag в значение true
    clickButton = (clickButton + 1) % 7;         //счётчик нажатий увеличиваем на единицу и берём остаток от деления
  }                                              //остаток от деления нужен для того, чтобы счетчик всегда был в диапазоне 1 - 7
  delay(100);                                    //задержка на дребезг
  buttonState = digitalRead(pinButton);          //проверяем, отпущена ли кнопка
  if (buttonState == LOW) {                      //если кнопка отпущена, то сбразываем flag в false
      flag = false;
    }
  //-----------------------------------------------------------------------------------------------------------------
  mask = numbers[clickButton];                   //mask принемает двоичное значение из массива по адресу clickButton (сколько нажали)
  //тут собственно мы выбрали режим мигания гирлянды, теперь после окончания прерывания по значению mask будут выбраны режимы мигания
  //-----------------------------------------------------------------------------------------------------------------
  for(int i = 0; i < 3; i++){                    //зажгём светодиоды по mask через цикл
    onLed = bitRead(mask,i);
    digitalWrite(i + 7, onLed);
  }
}
void lift1(){  //режим мигания (включение и выключение светодиодов)
  int cycle = random(3,7); //переменная количества циклов включения 
  for(int a = 0; a < cycle; a++) {
    analogWrite(ledLine, 255);
    delay(random(100, 1000));  //пауза включения. Пауза будет длинной от 100 до 1000 мс
    analogWrite(ledLine, 0);
    delay(random(100, 1000));  //пауза выключения
  }
}
void lift2(){  //режим мигания (плавное включение и правное выключение)
  int cycle = random(3,7);
  for(int a = 0; a < cycle; a++){
    ledMax = random(100, 254);
    for(int i = ledMin; i < 70; i++) { 
      analogWrite(ledLine, i);
      delay(pause*2);
    }
    for(int i = 70; i < ledMax; i+=7) { 
      analogWrite(ledLine, i);
      delay(pause);
    }
    delay(pause*100);
    int b = random(20, 69);
    for(int i = ledMax; i > 70; i-=7) {
      analogWrite(ledLine, i);
      delay(pause);
    }
    for(int i = 70; i > ledMin; i--) {
      analogWrite(ledLine, i);
      delay(pause*2);
    }
    delay(pause*20);  
  }
}
void lift3(){  //режим мигания
  int cycle = random(3,7);
  for(int a = 0; a < cycle; a++){
    for(int i = 40; i < 70; i++) { 
      analogWrite(ledLine, i);
      delay(pause*2);
    }
    for(int i = 70; i < 255; i+=7) { 
      analogWrite(ledLine, i);
      delay(pause);
    }
    delay(pause*100);
    analogWrite(ledLine, 20);
    delay(500);
  }
}


