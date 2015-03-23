#include "U8glib.h"
//указание пинов для использования дисплея, не обязательно брать пины аппаратного SPI
U8GLIB_ST7920_128X64_1X u8g(36, 38, 39);

//переменные для работы с клавиатурой
//пин к которому подключена панель управления
int KeyButton1Pin=0;
//значение с панели управления
int KeyButton1Value=0;
//последнее время когда на панели не было нажатых кнопок
long KeyButton1TimePress=0;
//задержка перед считыванием состояния панели управления после нажатия
long KeyButton1Latency=100000;
//метка означающая что нажатие кнопки на панели было обработано
int KeyButton1WasChecked=0;
//время после которого удерживание кнопки начинает засчитываться как многократные быстрые нажатия
long KeyButton1RepeatLatency=1500000;
//вспомогательная переменная для обработки повторных нажатий, после каждого повторного срабатывания переменная увеличивается
//сдвигая тем самым време следующего повторного срабатывания зажатой кнопки
long KeyButton1RepeatTimePress=0;
//переменная для хранения времени между временем когда не было зажатых кнопок и временем проверки
long KeyButton1TimeFromPress=0;
//Переменные используются для периодической обработки нажатий клавиатуры
//время прошлой обработки клавиатуры
long KeyBoardTime1=0;
//текущее время
long KeyBoardTime2=0;
//период обработки клавиатуры
long KeyBoardTimeInterval=25000;

//Переменные отвечающие за меню
//Массив с названиями меню
char* MenuNames[50];
//Тип элемента меню
//0-родительское меню
//1-целое число
//2-временной интервал (h:m:s, целое число но отображается как время)
//3-Вкл/Выкл (целое число, но отображается как On/Off)
//4-Расстояние (cm, целое число, но отображается как 0.хх метров)
int MenuTypeCode[50];
//Значение элемента меню
int MenuValue[50];
//Текущая позиция в меню, вложенность не важна т.к. меню представляет из себя список с ссылками на родительские 
//и дочерние пункты
int MenuNowPos=0;
//Режим редактирования (0-нет, просто выделен определенный параметр или пункт меню, 1-редактируем значение параметра)
int MenuEdit=0;
//номер элемента меню который является для данного родительским
//0-нет родительского элемента
int MenuParent[50];
//номер элемента меню который является для данного дочерним
//0-нет дочернего элемента
int MenuChild[50];
//Номер элемента дочернего меню который является первым
int MenuChildFirst[50];
//номер элемента дочернего меню который является последним
int MenuChildEnd[50];
//позиция меню в которой находится выделенный пункт на экране (например на экране отображается 3 пункта при этом выделен второй)
int MenuDrawPos=0;
//максимальное количество отображаемых на экране пунктов
int MenuDrawCount=4;


//переменные для таймера перерисовки
//время последней перерисовки
long DrawTime1=0;
//текущее время
long DrawTime2=0;
//интервал для перерисовки экрана
long DrawTimeInterval=200000;


void MenuSetup()
{
	//Настройка меню
	//задаем начальное положение в меню
	MenuNowPos=1;
	//Массив с названиями меню, индексами родительских элементов меню, начальными и конечными индексами дочерних элементов меню
	//также задаем начальные значения параметров элементов и их тип
	MenuNames[0]="Main Menu";
	MenuTypeCode[0]=0;
	MenuValue[0]=0;
	MenuParent[0]=0;
	MenuChildFirst[0]=1;
	MenuChildEnd[0]=3;

	MenuNames[1]="Menu1";
	MenuTypeCode[1]=0;
	MenuValue[1]=0;
	MenuParent[1]=0;
	MenuChildFirst[1]=4;
	MenuChildEnd[1]=9;

	MenuNames[2]="Menu2";
	MenuTypeCode[2]=0;
	MenuValue[2]=0;
	MenuParent[2]=0;
	MenuChildFirst[2]=10;
	MenuChildEnd[2]=11;

	MenuNames[3]="Menu3";
	MenuTypeCode[3]=0;
	MenuValue[3]=0;
	MenuParent[3]=0;
	MenuChildFirst[3]=12;
	MenuChildEnd[3]=15;

	MenuNames[4]="Param1_1";
	MenuTypeCode[4]=1;
	MenuValue[4]=0;
	MenuParent[4]=1;
	MenuChildFirst[4]=0;
	MenuChildEnd[4]=0;

	MenuNames[5]="Param1_2";
	MenuTypeCode[5]=2;
	MenuValue[5]=0;
	MenuParent[5]=1;
	MenuChildFirst[5]=0;
	MenuChildEnd[5]=0;

	MenuNames[6]="Param1_3";
	MenuTypeCode[6]=3;
	MenuValue[6]=0;
	MenuParent[6]=1;
	MenuChildFirst[6]=0;
	MenuChildEnd[6]=0;

	MenuNames[7]="Param1_4";
	MenuTypeCode[7]=1;
	MenuValue[7]=0;
	MenuParent[7]=1;
	MenuChildFirst[7]=0;
	MenuChildEnd[7]=0;

	MenuNames[8]="Param1_5";
	MenuTypeCode[8]=2;
	MenuValue[8]=0;
	MenuParent[8]=1;
	MenuChildFirst[8]=0;
	MenuChildEnd[8]=0;

	MenuNames[9]="Param1_6";
	MenuTypeCode[9]=3;
	MenuValue[9]=0;
	MenuParent[9]=1;
	MenuChildFirst[9]=0;
	MenuChildEnd[9]=0;

	MenuNames[10]="Param2_1";
	MenuTypeCode[10]=3;
	MenuValue[10]=0;
	MenuParent[10]=2;
	MenuChildFirst[10]=0;
	MenuChildEnd[10]=0;

	MenuNames[11]="Param2_2";
	MenuTypeCode[11]=3;
	MenuValue[11]=0;
	MenuParent[11]=2;
	MenuChildFirst[11]=0;
	MenuChildEnd[11]=0;

	MenuNames[12]="Param3_1";
	MenuTypeCode[12]=1;
	MenuValue[12]=0;
	MenuParent[12]=3;
	MenuChildFirst[12]=0;
	MenuChildEnd[12]=0;

	MenuNames[13]="Param3_2";
	MenuTypeCode[13]=3;
	MenuValue[13]=0;
	MenuParent[13]=3;
	MenuChildFirst[13]=0;
	MenuChildEnd[13]=0;

	MenuNames[14]="Param3_3";
	MenuTypeCode[14]=3;
	MenuValue[14]=0;
	MenuParent[14]=3;
	MenuChildFirst[14]=0;
	MenuChildEnd[14]=0;

	MenuNames[15]="Menu3_4";
	MenuTypeCode[15]=0;
	MenuValue[15]=0;
	MenuParent[15]=3;
	MenuChildFirst[15]=16;
	MenuChildEnd[15]=17;

	MenuNames[16]="Param3_4_1";
	MenuTypeCode[16]=2;
	MenuValue[16]=0;
	MenuParent[16]=15;
	MenuChildFirst[16]=0;
	MenuChildEnd[16]=0;

	MenuNames[17]="Param3_4_2";
	MenuTypeCode[17]=3;
	MenuValue[17]=0;
	MenuParent[17]=15;
	MenuChildFirst[17]=0;
	MenuChildEnd[17]=0;
}


void setup()
{
	//настройка порта для клавиатуры
	pinMode(KeyButton1Pin, INPUT);
	//настройка дисплея
	u8g.setHardwareBackup(u8g_backup_avr_spi);
	if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
		u8g.setColorIndex(255);     
	}
	else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
		u8g.setColorIndex(3);       
	}
	else if ( u8g.getMode() == U8G_MODE_BW ) {
		u8g.setColorIndex(1);       
	}
	else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
		u8g.setHiColorByRGB(255,255,255);
	}
	//формирование меню
	MenuSetup();
}


void DrawMenu()
{
	//временные переменные для отображения временных параметров  
	int DrawHours=0;
	int DrawMinutes=0;
	int DrawSeconds=0;

	u8g.setFont(u8g_font_fixed_v0);  
	//вывод названия родительского меню вверху экрана
	u8g.setPrintPos(5, 9);
	u8g.print(MenuNames[MenuParent[MenuNowPos-MenuDrawPos]]);
	u8g.drawLine( 0, 10, 123,10);
	//переменная для вывода пунктов меню на экран
	int DrawI=0;
	//цикл для вывода пунктов меню на экран
	for(DrawI=0; DrawI<MenuDrawCount;DrawI++)  
	{
		u8g.setPrintPos(5, 21+10*DrawI);
		if ((MenuChildFirst[MenuParent[MenuNowPos]]<=(MenuNowPos-MenuDrawPos+DrawI)) and 
				(MenuChildEnd[MenuParent[MenuNowPos]]>=(MenuNowPos-MenuDrawPos+DrawI)))
		{ 
			u8g.print(MenuNames[MenuNowPos-MenuDrawPos+DrawI]);  


			u8g.setPrintPos(80, 21+10*DrawI);
			//Если целое число
			if (MenuTypeCode[MenuNowPos-MenuDrawPos+DrawI]==1) 
			{
				u8g.print(MenuValue[MenuNowPos-MenuDrawPos+DrawI]);  
			}

			//Если тип временной интервал  
			if (MenuTypeCode[MenuNowPos-MenuDrawPos+DrawI]==2) 
			{
				DrawHours=MenuValue[MenuNowPos-MenuDrawPos+DrawI] / 3600;
				DrawMinutes=(MenuValue[MenuNowPos-MenuDrawPos+DrawI] % 3600) / 60;
				DrawSeconds=(MenuValue[MenuNowPos-MenuDrawPos+DrawI] % 3600) % 60;
				u8g.print((String)DrawHours+":"+(String)DrawMinutes+":"+(String)DrawSeconds);
			}  
			//Если пункт меню бинарный
			if (MenuTypeCode[MenuNowPos-MenuDrawPos+DrawI]==3) 
			{ 
				if (MenuValue[MenuNowPos-MenuDrawPos+DrawI]==0) {u8g.print("Off");}
				else {u8g.print("On");}  
			}     
		}
	}

	//если параметр сейчас не редактируется то отображение рамки вокруг выделенного пункта меню
	if (MenuEdit==0)
	{
		u8g.drawLine( 3, 12+10*MenuDrawPos, 70, 12+10*MenuDrawPos);
		u8g.drawLine(70, 12+10*MenuDrawPos, 70,22+10*MenuDrawPos);
		u8g.drawLine( 3,22+10*MenuDrawPos, 70,22+10*MenuDrawPos);
		u8g.drawLine( 3,  22+10*MenuDrawPos, 3,12+10*MenuDrawPos);
	}  

	//если параметр сейчас редактируется то отображение рамки вокруг значения параметра
	if (MenuEdit==1)
	{
		u8g.drawLine( 75, 12+10*MenuDrawPos, 122, 12+10*MenuDrawPos);
		u8g.drawLine(122, 12+10*MenuDrawPos, 122,22+10*MenuDrawPos);
		u8g.drawLine( 75,22+10*MenuDrawPos, 122,22+10*MenuDrawPos);
		u8g.drawLine( 75,  22+10*MenuDrawPos, 75,12+10*MenuDrawPos);
	}  
}


void Draw()
{
	u8g.firstPage();   
	do 
	{ 
		//прорисовка статуса калибровки
		DrawMenu();
	} while( u8g.nextPage() );
}


//при завершении редактирования пункта меню происходит обновление настроек
void UpdateSettings()
{
	//здесь происходит обновление настроек
	//допустим мы имеем программу которая мигает лампочкой с частотой speed
	//допустим этот параметр speed задается в элементе с индексом 4 тогда нам надо написать такой код:
	/*if (MenuNowPos==4) {
	  Speed=MenuValue[MenuNowPos]
	  }
	 */
}


//Процедура для обработки нажатия кнопки "вверх"
void UpPress()
{
	//если не находимся в режиме редактирования то кнопка используется для передвижения по меню
	if (MenuEdit==0)
	{
		//если текущая позиция в меню больше чем позиция первого элемента в этом меню то можно осуществлять передвижение.
		if (MenuChildFirst[MenuParent[MenuNowPos]]<MenuNowPos)
		{
			//осуществляем передвижение по меню на 1 пункт
			MenuNowPos=MenuNowPos-1;
			//при движении вверх проверяем где расположен выделенный пункт меню на дисплее
			//если выделенный пункт не упирается в край дисплея то также смещаем его на дисплее на 1 позицию
			if (MenuDrawPos>0)
			{
				MenuDrawPos=MenuDrawPos-1;  
			}
		}
	}

	//Если находимся в режиме редактирования
	if (MenuEdit==1)
	{
		//проверяем какого типа меню и проверяем соответствующие ограничения, также контроллируем в зависимости от значения приращение
		//или уменьшение значения
		//Если тип целое число то максимального ограничения нет (добавить потом чтоб бралось максимальное значение из меню)
		if (MenuTypeCode[MenuNowPos]==1) 
		{
			MenuValue[MenuNowPos]=MenuValue[MenuNowPos]+1;
		}
		//Если тип временной интервал  
		if (MenuTypeCode[MenuNowPos]==2) 
		{
			MenuValue[MenuNowPos]=MenuValue[MenuNowPos]+1;
		}
		//Если пункт меню бинарный то инвертируем значение
		if (MenuTypeCode[MenuNowPos]==3) 
		{
			MenuValue[MenuNowPos]=(MenuValue[MenuNowPos]+1) % 2;  
		}
	}
}


//Процедура для обработки нажатия кнопки "вниз"
void DownPress()
{
	//если не находимся в режиме редактирования
	if (MenuEdit==0)
	{
		//проверяем не является ли текущий пункт последним дочерним элементом
		if (MenuChildEnd[MenuParent[MenuNowPos]]>MenuNowPos)
		{
			//если не является то двигаемся на 1 пункт вниз
			MenuNowPos=MenuNowPos+1;
			//проверяем упираемся ли мы в край экрана. максимальное число элементов меню на экране задано переменной MenuDrawCount
			if ((MenuDrawPos<MenuDrawCount-1) and (MenuDrawPos<MenuChildEnd[MenuParent[MenuNowPos]]-MenuChildFirst[MenuParent[MenuNowPos]]))
			{
				//если в край экрана не упираемся то также сдвигаем позицию на экране на 1 пункт вниз
				MenuDrawPos=MenuDrawPos+1;  
			}
		}
	}
	//если находимся в режиме редактирования 
	if (MenuEdit==1)
	{
		//проверяем какого типа меню и проверяем соответствующие ограничения, также контроллируем в зависимости от значения приращение
		//или уменьшение значения
		if (MenuTypeCode[MenuNowPos]==1) 
		{
			if (MenuValue[MenuNowPos]>0)
			{
				MenuValue[MenuNowPos]=MenuValue[MenuNowPos]-1;
			}
		}
		//Если тип временной интервал  
		if (MenuTypeCode[MenuNowPos]==2) 
		{
			if (MenuValue[MenuNowPos]>0)
			{
				MenuValue[MenuNowPos]=MenuValue[MenuNowPos]-1;
			}
		}
		//Если пункт меню бинарный то инвертируем значение
		if (MenuTypeCode[MenuNowPos]==3) 
		{
			MenuValue[MenuNowPos]=(MenuValue[MenuNowPos]+1) % 2;  
		}
	}  
}  


//Процедура для обработки нажатия кнопки "влево"  
void LeftPress()
{
	//если не находимся в режиме редактирования
	if (MenuEdit==0)
	{ 
		//если пункт меню содержит ненулевой индекс родителя (т.е. мы находимся внутри другого меню)
		if (MenuParent[MenuNowPos]>0)
		{ 
			//то переходим на индекс родительского пункта меню
			MenuNowPos=MenuParent[MenuNowPos];
			//установка позиции на экарне, если количество пунктов меньше чем влезает на экране то выделенный пункт будет в самом низу но не в конце
			//иначе будет в самом конце
			if (MenuChildEnd[MenuParent[MenuNowPos]]-MenuChildFirst[MenuParent[MenuNowPos]]<MenuDrawCount)
			{
				MenuDrawPos=MenuNowPos-MenuChildFirst[MenuParent[MenuNowPos]];
			}
			else 
			{
				MenuDrawPos=MenuDrawCount-1;
			}
		}
	}
	//если находимся в режиме редактирования то выключаем режим редактирования  
	if (MenuEdit==1)
	{  
		MenuEdit=0;
		//запускаем процедуру для обновления настроек. это необходимо для того чтобы параметры меню которые мы поменяли начали действовать
		//в программе для которой мы используем это меню
		UpdateSettings()
	}
}


//Процедура для обработки нажатия кнопки "вправо"   
void RightPress()
{
	//если код типа элемента отличается от нуля (т.е. выделенный элемент является параметром) то включаем режим редактирования
	if (MenuTypeCode[MenuNowPos]>0)
	{
		MenuEdit=1;  
	}
	//если код типа элемента равен нулю значит в данный момент выделен пункт меню и мы можем войти в него  
	if (MenuTypeCode[MenuNowPos]==0)
	{
		//обнуляем позицию выделенного пункта на экране
		MenuDrawPos=0;  
		//переходим на первый дочерний элемент для текущего элемента
		MenuNowPos=MenuChildFirst[MenuNowPos];
	}  
}


void ButtonPress()
{
	//130
	if ((KeyButton1Value>125) and (KeyButton1Value<135))
	{
		UpPress(); 
	}
	//258  
	if ((KeyButton1Value>255) and (KeyButton1Value<262))
	{
		DownPress();
	}
	//65  
	if ((KeyButton1Value>60) and (KeyButton1Value<70))
	{
		LeftPress();  
	}
	//195  
	if ((KeyButton1Value>190) and (KeyButton1Value<200))
	{
		RightPress();
	}
}


void KeyBoardCalculate()
{
	//Часть отработки нажатия клавиши
	KeyButton1Value=analogRead(KeyButton1Pin); 
	//если сигнал с кнопки нулевой то обнуляем метку обработки нажатия
	if ((KeyButton1Value<=50) or (KeyButton1Value>=1000))
	{
		//Сохраняем время последнего сигнала без нажатой кнопки
		KeyButton1TimePress=micros(); 
		KeyButton1WasChecked=0;
		KeyButton1RepeatTimePress=0;
	} 

	KeyButton1TimeFromPress=micros()-KeyButton1TimePress;
	//исключаем шумы
	if ((KeyButton1Value>50) and (KeyButton1Value<1000))
	{
		//отработка первого нажатия
		if ( ((KeyButton1TimeFromPress)>KeyButton1Latency) and (KeyButton1WasChecked==0))
		{
			ButtonPress();
			KeyButton1WasChecked=1;
			KeyButton1RepeatTimePress=0;
		}
		//отработка повторных нажатий  
		if ( ((KeyButton1TimeFromPress)>(KeyButton1RepeatLatency+KeyButton1RepeatTimePress)) and (KeyButton1WasChecked==1))
		{
			ButtonPress();
			//различная скорость обработки повторений, если держим кнопку меньше 5 секунд то повторная обработка кнопки раз в 0.4с, 
			//если кнопка удерживается дольше 5 секунд то время следующего повторного срабатывания не увеличивается и происходит максимально быстро
			if (KeyButton1TimeFromPress<(KeyButton1RepeatLatency+5000000)) {
				KeyButton1RepeatTimePress=KeyButton1RepeatTimePress+400000;
			}
		}
	}
}


void loop()
{
	//проверка таймера для обработки графики, аналогично с обработкой клавиатуры 
	//обновление графики происходит не чаще заданного интервала DrawTimeInterval
	DrawTime2=micros();
	if ((DrawTime2-DrawTime1)>DrawTimeInterval) 
	{
		DrawTime1=DrawTime2;
		Draw();
	}
	//проверка таймера для обработки нажатий клавиатуры
	//обработка нажатий происходит не чаще заданного интервала KeyBoardTimeInterval
	KeyBoardTime2=micros();
	if ((KeyBoardTime2-KeyBoardTime1)>KeyBoardTimeInterval) 
	{
		KeyBoardTime1=KeyBoardTime2;
		KeyBoardCalculate();
	}
}
