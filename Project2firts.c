/*
 * T2_RTC.c
 *
 * Created: 05-Dec-20 4:31:52 PM
 * Author: Lost Heaven
 */

#include <mega16.h>
#include <alcd.h>
#include <delay.h>

//variable the calculate the clock
int sec = 0 ;
int min = 0;
int hour = 0;
int ADC_read(unsigned char ch);
//float ADC(void);
char str_sec[3] = {'\0'}, str_min[3] = {'\0'}, str_hour[3] = {'\0'}; //declared to make clock in base hh:mm:ss
void convert(void);
void display(void);

int temp = 0;//to get temprature of sensor
int std = 0; //to get number of students
void main(void)
{
	//initialize ADC 3
	DDRA.3 = 0;
	PORTA.3 = 1;
	ADMUX = 0b00000011; // choose adc 3
	ADCSRA = 0b10100011;

	lcd_init(16); //16 char per line in lcd

	DDRD.0 = 1;
	//initialize timmer and choose the mode
	TCCR2 = 0b00000101;  //Normal mode, Output pin disconnected, pre = 128
	ASSR |= 1 << 3;      // Async mode enabled
	TIMSK |= 1 << 6;    //Overflow interrupt enable

	lcd_gotoxy(4, 0); //to put the clock in good position in our opinion
	lcd_puts("00:00:00");//could be 08:00:00 if the library opens 8am
	temp =  ADC_read(3);
	lcd_gotoxy(0, 1);
	lcd_printf("%iC", temp);
	DDRD.3 = 0; PORTD.3 = 1;
#asm("sei")//general interrupt enable
	MCUCR = 0b00001000; //falling edge for interrupt 1
	GICR |= 1 << 7;     //Specific interrupt enable
	DDRB = 0xff;
	PORTB.2 = 1;

	MCUCR = 0b00000000;//falling edge for interupt2
	GICR |= 1 << 5;     //Specific interrupt enable
	/////////////////////////////////////////////////////////////////////////////////
	while (1)
		{
		if (0 < temp && temp < 10)
			{
			PORTB.4 = 0; PORTB.5 = 0;   //Two Heaters ON
			PORTB.6 = 1; PORTB.7 = 1;
			break;
			}
		if (10 <= temp && temp < 20)
			{
			PORTB.4 = 0; PORTB.5 = 0;   //One Heater ON
			PORTB.6 = 0; PORTB.7 = 1;
			break;
			}
		if (20 <= temp && temp < 30)
			{
			PORTB.4 = 0; PORTB.5 = 0;   //All are OFF
			PORTB.6 = 0; PORTB.7 = 0;
			break;
			}
		if (30 <= temp && temp < 40)
			{
			PORTB.4 = 1; PORTB.5 = 0;   //One air-conditioner ON
			PORTB.6 = 0; PORTB.7 = 0;
			break;
			}
		if (40 <= temp && temp < 50)
			{
			PORTB.4 = 1; PORTB.5 = 1;   //Two air-conditioners ON
			PORTB.6 = 0; PORTB.7 = 0;
			break;
			}
		}
}
/////////////////////////////////////////////////////////////////////////////////////
interrupt [5] void T2Ov (void)
{

	sec++;
	if(sec >= 60)
		{
		sec = 0;
		min++;
		}
	if (min >= 60)
		{
		min = 0;
		hour++;
		}
	convert();
	display();
	lcd_gotoxy(2, 1);  //to print C in specific location
	lcd_printf("C");
	if (sec % 5 == 0)    //to display temprature every 5 sec
		{
		temp =  ADC_read(3);
		lcd_gotoxy(0, 1);
		lcd_printf("%i", temp);
		}


}
void convert(void)    //to convert time in base hh:mm:ss
{
	str_sec[1] = (sec % 10) + '0';
	str_sec[0] = (sec / 10) + '0';

	str_min[1] = (min % 10) + '0';
	str_min[0] = (min / 10) + '0';


	str_hour[1] = (hour % 10) + '0';
	str_hour[0] = (hour / 10) + '0';
	return;
}
void display(void)  //display the clock in the middle of LCD to be clear
{
	lcd_gotoxy(10, 0);
	lcd_puts(str_sec);

	lcd_gotoxy(7, 0);
	lcd_puts(str_min);
	lcd_gotoxy(4, 0);
	lcd_puts(str_hour);
	delay_ms(100);
	return;
}


/////////////////////////
int ADC_read(unsigned char ch)  //analog to digital converter
{
	ADMUX = (ADMUX & 0b11100000) | ch; //Adjust channel
	ADCSRA.6 = 1 ;          //Start ADC conversion
	while (ADCSRA.6 == 1);  //Wait for conversion to end.

	return ((ADCW / 255) * 13);  //Full scale is 50C (5 volts)


}
/////////////////////////////////////////////////
interrupt [3] void ext1 (void)  //increse number of students as the enter door
{
	std++;
	lcd_gotoxy(6, 1);
	lcd_printf("%iSTD", std);
	if (std >= 25)
		{
		lcd_gotoxy(12, 1);
		lcd_printf("FULL");
		}

	if(std < 10)
	{PORTB.0 = 1; PORTB.1 = 0; PORTB.7 = 0;}
	if(std >= 10 && std <= 20)
	{PORTB.0 = 0; PORTB.1 = 1; PORTB.7 = 0;}
	if(std > 20 )
	{PORTB.0 = 0; PORTB.1 = 0; PORTB.7 = 1;}

}
/////////////////////////////////////////////////////////
interrupt [19] void ext2 (void) //decrases number of students as exit door
{
	std--;
	lcd_gotoxy(6, 1);
	lcd_printf("%iSTD", std);
	if(std < 25)
		{
		lcd_gotoxy(12, 1);
		lcd_printf("    ");
		}
	if(std < 10)
	{PORTB.0 = 1; PORTB.1 = 0; PORTB.7 = 0;}
	if(std >= 10 && std <= 20)
	{PORTB.0 = 0; PORTB.1 = 1; PORTB.7 = 0;}
	if(std > 20 )
	{PORTB.0 = 0; PORTB.1 = 0; PORTB.7 = 1;}
}