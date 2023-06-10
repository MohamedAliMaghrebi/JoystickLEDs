/*----------------------------------------------------------------
LAB EXERCISE: DIGITAL INPUT AND OUTPUT
---------------------------------------
Digital input: joystick on the application board
Digital output: 4 LEDs on the mbed board
Function: Use the GPIO peripheral to control LEDs according to 
the status of the joystick
-----------------------------------------------------------------*/

//Define the structure of the pin multiplexor

typedef struct
{
  volatile  unsigned int    PINSEL0;
  volatile  unsigned int    PINSEL1;
  volatile  unsigned int    PINSEL2;
  volatile  unsigned int    PINSEL3;
  volatile  unsigned int    PINSEL4;
  volatile  unsigned int    PINSEL5;
  volatile  unsigned int    PINSEL6;
  volatile  unsigned int    PINSEL7;
  volatile  unsigned int    PINSEL8;
  volatile  unsigned int    PINSEL9;
  volatile  unsigned int    PINSEL10;
  volatile  unsigned int    RESERVED0[5];
  volatile  unsigned int    PINMODE0;
  volatile  unsigned int    PINMODE1;
  volatile  unsigned int    PINMODE2;
  volatile  unsigned int    PINMODE3;
  volatile  unsigned int    PINMODE4;
  volatile  unsigned int    PINMODE5;
  volatile  unsigned int    PINMODE6;
  volatile  unsigned int    PINMODE7;
  volatile  unsigned int    PINMODE8;
  volatile  unsigned int    PINMODE9;
  volatile  unsigned int    PINMODE_OD0;
  volatile  unsigned int    PINMODE_OD1;
  volatile  unsigned int    PINMODE_OD2;
  volatile  unsigned int    PINMODE_OD3;
  volatile  unsigned int    PINMODE_OD4;
  volatile  unsigned int    I2CPADCFG;
} My_PINCON_TypeDef;


#define My_PINCON_BASE       (0x40000000UL + 0x2C000)
#define My_PINCON            ((My_PINCON_TypeDef    *) My_PINCON_BASE   )


//Define the structure of the GPIO peripheral

typedef struct
{

  volatile  unsigned int   FIODIR;
		
  volatile  unsigned int   RESERVED[3];

  volatile  unsigned int   FIOMASK;

  volatile  unsigned int   FIOPIN;

  volatile  unsigned int   FIOSET;

  volatile  unsigned int   FIOCLR;

} My_GPIO_TypeDef;

#define My_GPIO_BASE         (0x2009C000UL)

#define My_GPIO0_BASE        (My_GPIO_BASE + 0x00000)
#define My_GPIO1_BASE        (My_GPIO_BASE + 0x00020)


#define My_GPIO0             ((My_GPIO_TypeDef      *) My_GPIO0_BASE    )	//GPIO0 for joystick
#define My_GPIO1             ((My_GPIO_TypeDef      *) My_GPIO1_BASE    ) //GPIO1 for LEDs


int main()
{
	
	//Status of the joystick
	int joystick_up;
	int joystick_down;
	int joystick_left;
	int joystick_right;
	int joystick_centre;
	
	/*------------------------------------------------------
	Configure the pin multiplexor and GPIO pull-up mode
	PINSEL: enable the external pins to be connected to the GPIO peripheral 
	PINMODE: pin internal pull-up setting
	10: tri-state mode
	11: on-chip pull-down resistor enabled
	-------------------------------------------------------*/
	
	//P0.23 connected to joystick-up -- GPIO, pull-down resistor
	My_PINCON->PINSEL1 &=~((1 <<15)|(1<<14));
	My_PINCON->PINMODE1 |=((1 <<15)|(1<<14));

	//P0.17 connected to joystick-down -- GPIO, pull-down resistor
	My_PINCON->PINSEL1 &=~((1 <<3)|(1<<2));
	My_PINCON->PINMODE1 |=((1 <<3)|(1<<2));

	//P0.15 connected to joystick-left -- GPIO, pull-down resistor
	My_PINCON->PINSEL0 &=~((1 <<31)|(1<<30));
	My_PINCON->PINMODE0 |=((1 <<31)|(1<<30));

	//P0.24 connected to joystick-right -- GPIO, pull-down resistor
	My_PINCON->PINSEL1 &=~((1 <<17)|(1<<16));
	My_PINCON->PINMODE1 |=((1 <<17)|(1<<16));
	
	//P0.16 connected to joystick-centre -- GPIO, pull-down resistor
	My_PINCON->PINSEL1 &=~((1 <<1)|(1<<0));
	My_PINCON->PINMODE1 |=((1 <<1)|(1<<0));

	//P1.18 connected to LED1 -- GPIO, tri-state mode
	My_PINCON->PINSEL3 &=~((1 <<5)|(1<<4));
	My_PINCON->PINMODE3 |=(1 <<4);
	My_PINCON->PINMODE3 &=~(1<<5);

	//P1.20 connected to LED2 -- GPIO, tri-state mode
	My_PINCON->PINSEL3 &=~((1 <<9)|(1<<8));
	My_PINCON->PINMODE3 |=(1 <<9);
	My_PINCON->PINMODE3 &=~(1<<8);

	//P1.21 connected to LED3 -- GPIO, tri-state mode
	My_PINCON->PINSEL3 &=~((1 <<11)|(1<<10));
	My_PINCON->PINMODE3 |=(1 <<11);
	My_PINCON->PINMODE3 &=~(1<<10);

	//P1.23 connected to LED4 -- GPIO, tri-state mode
	My_PINCON->PINSEL3 &=~((1 <<15)|(1<<14));
	My_PINCON->PINMODE3 |=(1 <<15);
	My_PINCON->PINMODE3 &=~(1<<14);
	
	//Turn off all 4 LEDs
	My_GPIO1->FIODIR |=((1<<18)|(1<<20)|(1<<21)|(1<<23));			//direction: input 
	My_GPIO1->FIOMASK &=~((1<<18)|(1<<20)|(1<<21)|(1<<23));		//mask out undesired bits
	My_GPIO1->FIOCLR |=((1<<18)|(1<<20)|(1<<21)|(1<<23));			//write to clear register to clear all four bits

	//Infinite loop
	while(1){
			
			
			//Read joystick-up bit
			My_GPIO0->FIOMASK &=~(1<<23);										//mask register, clear bit[23]
			My_GPIO0->FIODIR &=~(1<<23);										//direction register, clear bit[23] for input
			joystick_up=(((My_GPIO0->FIOPIN)&(1<<23))>>23);	//read bit[23]
			
			//Write to LED1
			My_GPIO1->FIODIR |=(1<<18);				//direction register, set bit[18] for output
			My_GPIO1->FIOMASK &=~(1<<18);			//mask register, clear bit[18]
			if(joystick_up==1)
				My_GPIO1->FIOSET |=(1<<18);			//write to set bit[18]
			else
				My_GPIO1->FIOCLR |=(1<<18);			//write to clear bit[18]

			//Do the same for other bits
			
			//Read joystick-down bit and write to LED2
			My_GPIO0->FIOMASK &=~(1<<17);
			My_GPIO0->FIODIR &=~(1<<17);
			joystick_down=(((My_GPIO0->FIOPIN)&(1<<17))>>17);
			
			My_GPIO1->FIODIR |=(1<<20);
			My_GPIO1->FIOMASK &=~(1<<20);
			if(joystick_down==1)
				My_GPIO1->FIOSET |=(1<<20);
			else
				My_GPIO1->FIOCLR |=(1<<20);			
				
			//Read joystick-left bit and write to LED3
			My_GPIO0->FIOMASK &=~(1<<15);
			My_GPIO0->FIODIR &=~(1<<15);
			joystick_left=(((My_GPIO0->FIOPIN)&(1<<15))>>15);
			
			My_GPIO1->FIODIR |=(1<<21);
			My_GPIO1->FIOMASK &=~(1<<21);
			if(joystick_left==1)
				My_GPIO1->FIOSET |=(1<<21);
			else
				My_GPIO1->FIOCLR |=(1<<21);		


			//Read joystick-right bit and write to LED4
			My_GPIO0->FIOMASK &=~(1<<24);
			My_GPIO0->FIODIR &=~(1<<24);
			joystick_right=(((My_GPIO0->FIOPIN)&(1<<24))>>24);
			
			My_GPIO1->FIODIR |=(1<<23);
			My_GPIO1->FIOMASK &=~(1<<23);
			if(joystick_right==1)
				My_GPIO1->FIOSET |=(1<<23);
			else
				My_GPIO1->FIOCLR |=(1<<23);			
				
			//Read joystick-centre bit and write to LED1-4
			My_GPIO0->FIOMASK &=~(1<<16);
			My_GPIO0->FIODIR &=~(1<<16);
			joystick_centre=(((My_GPIO0->FIOPIN)&(1<<16))>>16);
			
			My_GPIO1->FIODIR |=((1<<18)|(1<<20)|(1<<21)|(1<<23));
			My_GPIO1->FIOMASK &=~((1<<18)|(1<<20)|(1<<21)|(1<<23));
			if(joystick_centre==1)
				My_GPIO1->FIOSET |=((1<<18)|(1<<20)|(1<<21)|(1<<23));
			else
				My_GPIO1->FIOCLR |=((1<<18)|(1<<20)|(1<<21)|(1<<23));			
					
		}		

}

