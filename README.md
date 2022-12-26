## 🇬🇧 Christmas decoration WS2811/WS2812 LED panel

![front](front.png?raw=true "front")

https://www.youtube.com/shorts/c0Lq98C2jhU

### Features

+ Based on [FastLED](https://github.com/FastLED/FastLED) library
+ Can be used to create animated rectangular panel from cheap (~20$ for 100 LEDs) RGB strips
+ Provides functions to print text (includes Latin and Cyrillic 5x7 pixels font), draw lines and circles (Bresenham's algorithm)
+ Includes 13 scene examples and 9 transition effects, including:
  + text marquee (ticker)
  + x-mas tree, snowman, Santa
  + animated gradient
  + falling snow
  + ping-pong

### Installation

#### Hardware

+ Arduino UNO, Nano, or analog (even DigiSpark will do)
+ 500k resistor 1/4W
+ LED strip(s) with 100 or more LEDs
  + FastLED supports IC WS2811, WS2812, TM1809 and others
  + Personally, I would recommend buying **at least two** _50pcs WS2811 Full Color LED Pixel IP68_ available on Aliexpress.com
+ Power supply for LED strip matching its voltage and current.
  > Note: if you have several strips connected in series, usually you can use a separate power supply for each strip
  > 
  > Note 2: for strip I've bought, 5V 5А power supply is needed to display all white LEDs at full brightness, but amps required could be lowered by adjusting `BRIGHTNESS` and `STRIP_MILLIAMPS` macros
+ Optional: DC power adapter to screw terminal


+ Connect strips in series
+ Mount strips on some surface - I used a cupboard. This sketch expects pixels placed in columns. Install the first LED in the left top corner, then go down to the last row. The second column starts from the bottom row and goes up and so on. You can provide your custom LEDs order by changing `getCoord()` function
  > Note: if you buy strip from the pic: it starts from the female 3-pin connector end
+ Connect LED strip to the power supply `+` and `-`. Mind the polarity!
+ Connect the strip data line to the Arduino pin (6 by default) thru 500k resistor
+ Connect strip GND to Arduion GND with wire

#### Software

+ Open Arduino IDE
+ Install (`Ctrl+Shift+I`) FastLED library (this sketch tested on 3.2.9)
+ Open sketch, edit macros settings - pin, number of LEDs, speed, etc
+ Upload sketch, enjoy!

### ToDo

+ Hardware button to switch modes
+ Control from a smartphone or IR remote
+ Implement flood fill (paint bucket) function

![back](back.png?raw=true "back")
![ws2811 led strip](ws2811.png?raw=true "ws2811 led strip")

***

## 🇷🇺 Новогодняя LED-панель из гирлянды WS2811/WS2812

### Возможности

+ Испольует библиотеку [FastLED](https://github.com/FastLED/FastLED)
+ Предназначена для создания прямоугольных анимированных светодиодных панелей из недорогих RGB лент (~20$ за 100 LED)
+ Содержит функции печати русского и английского текста (шрифт 5x7), рисования линий и окружностей (алгоритм Брезенхема)
+ Реализованы 13 анимаций и 9 переходных эффектов, в т.ч.:
  + бегущая строка
  + елочка, снеговик, Дед Мороз
  + анимированный градиент
  + падающий снег
  + пинг-понг

### Установка

#### Аппаратная часть

+ Arduino UNO, Nano или аналог (подойдет и DigiSpark)
+ 500 кОм резистор 1/4 Вт
+ Светодиодная лента (или несколько) - суммарно от 100 пикселей и больше
  + FastLED поддерживает чипы WS2811, WS2812, TM1809 и другие
  + Рекомендуется к покупке **минимум две** _50pcs WS2811 Full Color LED Pixel IP68_ на Aliexpress.com
+ Блок питания соответствующего напряжения и мощности
  > Примечание: при использовании нескольких лент, соединенных последовательно, как правило, можно подключить нескольких блоков питания - отдельный к каждой ленте
  >
  > Примечание 2: для вышеуказанной ленты нужен блок питания 5 Вольт 5 Ампер - если зажечь все светодиоды белым на полной яркости, но потребление тока можно уменьшить с помощью макросов `BRIGHTNESS` и `STRIP_MILLIAMPS` в коде
+ Опционально: переходник с круглого штекера на терминал


+ Соедините ленты последовательно
+ Закрепите ленты на какой-нибудь поверхности - я использую картонку. Скетч предполагает, что светодиоды расположены колонкам. Первый пиксель устанавливается в верхний левый угол, второй по ним и так до последней "строки". Вторая колонка начиается с последней строки и идет снизу-вверх и так далее. Если вы хотите изменить порядок пикселей, поправьте код в функции `getCoord()`
  > Примечание: если вы купили ленту как у меня, то она начинается с разъема "мама"
+ Подключите ленту к контактам `+` и `-` адаптера питания. Соблюдайте полярность!
+ Подключите управляющий контакт ленты через резистор к Arduino (по умолчанию пин 6)
+ GND ленты соедините с GND Arduino проводом

#### Программное обеспечение

+ Откройте Arduino IDE
+ Установите (`Ctrl+Shift+I`) библиотеку FastLED (скетч протестирован под версию 3.2.9)
+ Откройте скетч и отредактируйте макросы, которые задают чип ленты, количество светодиодов, мощность, пин и т.д., если потребуется
+ Загрузите скетч

### ToDo

+ Добавить аппаратную кнопку управления режимами
+ Добавить управление со смартфона или ИК-пульта
+ Реализовать функцию заливки цветом