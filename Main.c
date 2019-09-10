/* ========================================
 *
 * Copyright HUIKELECTRONICS, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF HUIKELECTRONICS.
 *          Boris Huikeshoven
 * ========================================
*/
#include <project.h>

/* Vaste waarden definieren */
#define AANTAL_P_STRIP 30   //30 leds per strip
#define AANTAL_STRIPS 13    //13 treden met een strip eronder

/* kleuren */
#define GROEN   0x0000FF
#define LGROEN  0x000022
#define ROOD    0x00FF00
#define BLAUW   0xFF0000
#define UIT     0x000000
#define WIT     0xFFFFFF
#define LWIT    0x222222

/* Voids aanroepen en kenbaar maken     */

void Init();
void SwitchCase(int effect);
void SingleLED(uint32_t delay, uint32_t color, int aantalAAN);
void Stripdown(uint32_t snelheid, uint32_t color, uint32_t colorchange, uint32_t bgcolor);
void Stripdown2(uint32_t snelheid, uint32_t color, uint32_t colorchange, uint32_t bgcolor);
void Stripup2(uint32_t snelheid, uint32_t color, uint32_t bgcolor);
void Arrowdown(uint32_t delay, uint32 color, uint32_t colorchange, uint32 bgcolor, int16 dikte);
void Arrowup(uint32_t delay, uint32 color, uint32_t colorchange, uint32 bgcolor, int16 dikte);
int DATAVerwerk(int dataPin0, int dataPin1, int dataPin2, int dataPin3, int dataPin4, int dataPin5);
void DataInlezen();
void Wokkel(uint32_t delay, uint32 color, uint32 bgcolor, int16 dikte);
void Gordijn(uint32_t snelheid, uint32_t color, uint32_t colorchange);
void NachtRijder(uint32_t snelheid, uint32_t color);

/* Variabelen declaratie */

int dataPin0 = 0;
int dataPin1 = 0;
int dataPin2 = 0;
int dataPin3 = 0;
int dataPin4 = 0;
int dataPin5 = 0;
int togglePin = 0;
int Status = 0;
int DataIn = 0;



/************************************************
 *                    Interrupt-routine
 *
 *  Als er data van Home Assistant via ESP8266 binnenkomt
 *  Komt er een trigger/interrupt binnen. 
 *
 ************************************************/

CY_ISR_PROTO( ResetISR_Handler );

CY_ISR( ResetISR_Handler)
{
    Bootloadable_1_Load();  /*Force a bootloader restart*/
}

/***************************************************************************/
/****************************     START    *********************************/
/***************************************************************************/
int main()
{
    ResetTimer_Start();
    Reset_ISR_ClearPending();
    Reset_ISR_StartEx(ResetISR_Handler);
    CyGlobalIntEnable; 
    
    Init();

    for(;;)
    {
        DataInlezen();
        Status = DATAVerwerk(dataPin0, dataPin1, dataPin2, dataPin3, dataPin4, dataPin5);
        SwitchCase(Status);
    }
    
}

/************************************************
 *                    Init()
 *
 *  First start at the program Initializing
 *
 ************************************************/
void Init()
{

    //togglePin_interrupt_StartEx(togglePinHandler);
    LEDStrip_Start(); 
    LEDStrip_Dim(4); // 0 is vol licht, 4 is max gedimt.
    LEDStrip_MemClear(UIT); 
}

/************************************************
 *                    DATAIN()
 *
 *  Hier worden de 6 inkomende bits verwerkt tot 1 getal
 *
 ************************************************/
int DATAVerwerk(int dataPin0, int dataPin1, int dataPin2, int dataPin3, int dataPin4, int dataPin5)
{
    int DataIn = 0;
    if (dataPin0 == 1)DataIn = DataIn + 1;
    else DataIn = DataIn;
    if (dataPin1 == 1)DataIn = DataIn + 2;
    else DataIn = DataIn;
    if (dataPin2 == 1)DataIn = DataIn + 4;
    else DataIn = DataIn;
    if (dataPin3 == 1)DataIn = DataIn + 8;
    else DataIn = DataIn;
    if (dataPin4 == 1)DataIn = DataIn + 16;
    else DataIn = DataIn;
    if (dataPin5 == 1)DataIn = DataIn + 32;
    else DataIn = DataIn;
    return DataIn;
}
/************************************************/
void DataInlezen()
{
 dataPin0 = (Bit0_Read());
 dataPin1 = (Bit1_Read());
 dataPin2 = (Bit2_Read());
 dataPin3 = (Bit3_Read());
 dataPin4 = (Bit4_Read());
 dataPin5 = (Bit5_Read());
}

/************************************************
 *                    SwitchCase()
 *
 *  Switch case functie.
 *
 ************************************************/
void SwitchCase(int effect)
{    
    switch(effect) 
    {
        case 1 : //effect 1
            Gordijn(130, 0x880088, 2);
            return;
        case 2 : //effect 2
            NachtRijder(40, ROOD);
            return;
        case 3 : //effect 3
            Stripdown2(500, LEDStrip_ColorInc(17), 3, BLAUW);
            break;
        case 4 : //nacht
            Arrowup(50, LWIT, 0, LGROEN, 10);
            break;
        case 5 : //op
            Stripdown2(500, LEDStrip_ColorInc(17), 0, UIT);
            break;
        case 6 : //neer
            Stripup2(500, BLAUW, UIT);
            return;
        case 7 : // twinkel
            SingleLED(300, LGROEN, 20);
            return;
        case 8 : // disco
            Arrowdown(50, GROEN, 0, WIT, 8);
            break;
        case 9 : // rainbow_op
            Stripdown(150, GROEN, 3, UIT);
            break; 
        case 10 : //rainbow_neer
            Stripup2(50, LEDStrip_ColorInc(16), LEDStrip_ColorInc(3));
            break;
        case 11 : // effect 11
            Arrowdown(150, GROEN, 0, ROOD, 20);
            break;
        case 12 :// effect 12
            Arrowdown(20, BLAUW, 0, ROOD, 4);
            break; 
        case 13 :// effect 13
            Arrowup(30, ROOD, 3, UIT, 15);
            break;
        case 14 :// effect 14
            Arrowdown(150, GROEN, 3, UIT, 8);
            break;
        case 15 :// effect 15
            Arrowdown(50, GROEN, 0, WIT, 8);
            break;   
        case 16 :// effect 16
            Arrowdown(50, GROEN, 0, WIT, 8);
            break;
        case 17 :// effect 17
            Arrowdown(50, GROEN, 0, WIT, 8);
            break; 
        case 18 :// effect 18
            Arrowup(50, WIT, 0, GROEN, 8);
            break;
        case 19 :// effect 19
            Arrowdown(50, GROEN, 0, WIT, 8);
            break;
        case 20 :// effect 20
            Arrowdown(50, GROEN, 0, WIT, 8);
            break; 
        case 21 :// DEMO (Er is plaats voor 64 cases)
            Arrowdown(50, GROEN, 0, WIT, 8);
            break; 
        case 0 :// uit
            LEDStrip_MemClear(UIT);
            LEDStrip_Trigger(1);
            CyDelay(50);
            break;
        default :
            LEDStrip_MemClear(UIT);
            LEDStrip_Trigger(1);
            CyDelay(50);
    }
 }           


/************************************************
 *                    SingleLED()
 *
 *   Aantal losse leds die aan en weer uit gaan.
 *
 ************************************************/
void SingleLED(uint32_t delay, uint32_t color, int aantalAAN)
{
    uint32_t kleur = color;
    LEDStrip_MemClear(UIT);
    int i; //loop counter
    int x; // x pos voor aan
    int ux; // x pos voor uit
    int ix = 0 + aantalAAN; //index x voor aan
    int iux = 0; // index x voor uit
    int y;
    int uy;
    int iy = 0 + aantalAAN;
    int iuy = 0;
    int myX_Array[30] = {8,13,29,4,5,26,15,7,1,12,16,22,3,2,27,10,6,11,28,9,25,0,14,23,18,24,17,19};
    int myY_Array[31] = {5,10,1,3,7,11,0,6,2,12,8,4,9,11,0,3,5,10,1,9,7,2,12,4,6,2,12,8,4,9,7};
    
    for(;;)
    {
        for(i = 1; i < 200; i++)  
        {
		    // Wait for last update to finish
	        while( LEDStrip_Ready() == 0);

    		x = myX_Array[ix];
            ix ++;
    		if (ix > 30   ) ix = 0;
            y = myY_Array[iy];
            iy ++;
            if (iy > 31) iy = 0;
    	    ux = myX_Array[iux];
            iux ++;
    		if (iux > 30   ) iux = 0;
            uy = myY_Array[iuy];
            iuy ++;
            if (iuy > 31) iuy = 0;
    		// Set the color of a single LED
    	    LEDStrip_Pixel(x , y, kleur); 
            LEDStrip_Pixel(ux, uy, UIT);

    		// Trigger update of all LEDs at once
            LEDStrip_Trigger(1);    
    		
    		// Loop delay
            CyDelay(delay);    
                
    		// If new effect is ready to start, stop this effect.
    		if(togglePin_Read() == 1) break;
        }	
    if(togglePin_Read() == 1) break;
	}
while(togglePin_Read() == 1);
CyDelay(50);
}



/************************************************
 *                    Stripdown()
 *
 *  Per trede naar bendeden.
 *
 ************************************************/
void Stripdown(uint32_t snelheid, uint32_t color, uint32_t colorchange, uint32_t bgcolor)
{
    int16 i = 0;
    int STRIP; // De stap die gemaakt wordt.
    uint32_t kleur = color;
    LEDStrip_MemClear(bgcolor);
    for(;;)
    {
        for(STRIP = 0; STRIP < AANTAL_STRIPS; STRIP ++)  		
            { 
                for (i = 0; i < AANTAL_P_STRIP; i++)
                {
                    while( LEDStrip_Ready() == 0);
                    LEDStrip_Pixel(i, STRIP, kleur);
                }
            kleur = LEDStrip_ColorInc(colorchange);   
            LEDStrip_Trigger(1);
            CyDelay(snelheid);
            if(togglePin_Read() == 1) break;
            }
        if(togglePin_Read() == 1) break;
    }    
    while(togglePin_Read() == 1);
    CyDelay(50);    	
}


/************************************************
 *                    Stripdown2()
 *
 *  Per twee treden naar bendeden.
 *
 ************************************************/
void Stripdown2(uint32_t snelheid, uint32_t color, uint32_t colorchange, uint32_t bgcolor)
{
    int16 i = 0;
    int STRIP; 
    uint32_t kleur = color;
    LEDStrip_MemClear(bgcolor);
    for(;;)
    {
        for(STRIP = 0; STRIP < AANTAL_STRIPS; STRIP = (STRIP+2))  		
            { 
                for (i = 0; i < AANTAL_P_STRIP; i++)
                {
                    while( LEDStrip_Ready() == 0);
                    LEDStrip_Pixel(i, STRIP, kleur);
                }
            kleur = LEDStrip_ColorInc(colorchange);   
            LEDStrip_Trigger(1);
            CyDelay(snelheid);
            if(togglePin_Read() == 1) break;
            }
        if(togglePin_Read() == 1) break;
    }
    while(togglePin_Read() == 1);
    CyDelay(50);     
}

/************************************************
 *                    Stripup2()
 *
 *  Per twee treden naar bendeden.
 *
 ************************************************/
void Stripup2(uint32_t snelheid, uint32_t color, uint32_t bgcolor)
{
    int16 i = 0;
    int STRIP; // De stap die gemaakt wordt.
    uint32_t kleur = color;
    LEDStrip_MemClear(bgcolor);
    for(;;)
    {
        for(STRIP = 0; STRIP < AANTAL_STRIPS; STRIP = (STRIP+2))  		
            { 
                for (i = 0; i < AANTAL_P_STRIP; i++)
                {
                    while( LEDStrip_Ready() == 0);
                    LEDStrip_Pixel(i, (AANTAL_STRIPS - STRIP), kleur);
                }
            kleur = LEDStrip_ColorInc(3);   
            LEDStrip_Trigger(1);
            CyDelay(snelheid);
            if(togglePin_Read() == 1) break;
            }
        if(togglePin_Read() == 1) break;    
    }        
    while(togglePin_Read() == 1);
    CyDelay(50);            
}


/************************************************
 *                    Arrowdown()
 *
 *  Pijlpunt die omlaag beweegt
 *
 ************************************************/
void Arrowdown(uint32_t delay, uint32 color, uint32_t colorchange, uint32 bgcolor, int16 dikte)
{
    uint32_t kleur = color;
    int16 ledPosL = 14;  // On LED position Linkerzijde
    int16 ledPosR = 15;  // On LED position Rechterzijde
    int16 strip = 0;     // start op onderste strip
    dikte = dikte + 1;
    int16 i;
    int STAPPEN; 
    int STAP; // De stap die gemaakt wordt.
    for(;;)
    {
        for(STAPPEN = 1; STAPPEN < 33+dikte; STAPPEN ++)  
        {		
		// Set the color of a single LED
            LEDStrip_MemClear(bgcolor);
            STAP = STAPPEN;
            while (STAP > 0)
            { 
                for (i = 1; i < dikte; i++)
                {
                    while( LEDStrip_Ready() == 0);
        	        LEDStrip_Pixel(ledPosL+STAP, strip-i, kleur); 
                    LEDStrip_Pixel(ledPosR-STAP, strip-i, kleur);          
                }
                STAP--;
                strip++;
                  
            }
            kleur = LEDStrip_ColorInc(colorchange); 
		    // Trigger update of all LEDs at once
            LEDStrip_Trigger(1);    
		    strip = 0;
		    // Loop delay
            CyDelay(delay);  
            if(togglePin_Read() == 1) break;
        }
        if(togglePin_Read() == 1) break;    
    }	
    while(togglePin_Read() == 1);
    CyDelay(50);
}


/************************************************
 *                    Arrowup()
 *
 *  Pijlpunt die omhoog beweegt
 *
 ************************************************/
void Arrowup(uint32_t delay, uint32 color, uint32_t colorchange, uint32 bgcolor, int16 dikte)
{ 
    uint32_t kleur = color;
    int16 ledPosL = 14;  // On LED position Linkerzijde
    int16 ledPosR = 15;  // On LED position Rechterzijde
    int16 strip = 12;     // start op onderste strip
    dikte = dikte + 1;
    int16 i;
    int STAPPEN; 
    int STAP; // De stap die gemaakt wordt.
    for(;;)
    {
        for(STAPPEN = 1; STAPPEN < 33+dikte; STAPPEN ++)  
        {		
		    // Set the color of a single LED
            LEDStrip_MemClear(bgcolor);
            STAP = STAPPEN;
            while (STAP > 0)
            { 
                for (i = 1; i < dikte; i++)
                {
                    // while( LEDStrip_Ready() == 0);
	                LEDStrip_Pixel(ledPosL+STAP, strip+i, kleur); 
                    LEDStrip_Pixel(ledPosR-STAP, strip+i, kleur);          
                }
                STAP--;
                strip--;  
            }
            kleur = LEDStrip_ColorInc(colorchange); 
		    // Trigger update of all LEDs at once
            LEDStrip_Trigger(1);    
		    strip = 12;
		    // Loop delay
            CyDelay(delay);  
            if(togglePin_Read() == 1) break; 
        }
        if(togglePin_Read() == 1) break;         
    }	
    while(togglePin_Read() == 1);
    CyDelay(50);	
}



/************************************************
 *                    Wokkel()
 *
 *  Kronkel streep die heen en weer slingert
 *
 ************************************************/
void Wokkel(uint32_t delay, uint32 color, uint32 bgcolor, int16 dikte)
{
    int16 ledPosL = 14;  // On LED position Linkerzijde
    int16 ledPosR = 15;  // On LED position Rechterzijde
    int16 strip = 12;     // start op onderste strip
    dikte = dikte + 1;
    int16 i;
    int STAPPEN; 
    int STAP; // De stap die gemaakt wordt.
    for(;;)
    {
        for(STAPPEN = 1; STAPPEN < 33+dikte; STAPPEN ++)  
        {		
		    // Set the color of a single LED
            LEDStrip_MemClear(bgcolor);
            STAP = STAPPEN;
            while (STAP > 0)
            { 
                for (i = 1; i < dikte; i++)
                {
                    while( LEDStrip_Ready() == 0);
        	        LEDStrip_Pixel(ledPosL+STAP, strip+i, color); 
                    LEDStrip_Pixel(ledPosR-STAP, strip+i, color);          
                }
                STAP--;
                strip--;   
            }
    		// Trigger update of all LEDs at once
            LEDStrip_Trigger(1);    
    		strip = 12;
    		// Loop delay
            CyDelay(delay);    
            if(togglePin_Read() == 1) break; 
        }
		if(togglePin_Read() == 1) break; 
    }
    while(togglePin_Read() == 1);
    CyDelay(50);
}


/************************************************
 *                    Gordijn()
 *
 *  Van midden naar buiten kanten Aan/Uit
 *
 ************************************************/
void Gordijn(uint32_t snelheid, uint32_t color, uint32_t colorchange)
{
    int16 i = 0;
    int STAP; // De stap die gemaakt wordt.
    uint32_t kleur = color;
    LEDStrip_MemClear(UIT);
    for(;;)
    {
        for(STAP = 0; STAP < AANTAL_P_STRIP/2; STAP ++)  		
            { 
                for (i = 0; i < AANTAL_STRIPS; i++)
                {
                    while( LEDStrip_Ready() == 0);
                    LEDStrip_Pixel(15 + STAP, i, kleur);
                    LEDStrip_Pixel(14 - STAP, i, kleur);                    
                }
               
            LEDStrip_Trigger(1);
            CyDelay(snelheid);
            if(togglePin_Read() == 1) break;
            }
        kleur = LEDStrip_ColorInc(colorchange);    
        if(togglePin_Read() == 1) break;
    }    
    while(togglePin_Read() == 1);
    CyDelay(50);    	
}

/************************************************
 *                    NachtRijder()
 *
 *  De Rijder in de Nacht
 *
 ************************************************/
void NachtRijder(uint32_t snelheid, uint32_t color)
{
    int16 i = 0;
    int STAP; // De stap die gemaakt wordt.
    uint32_t kleur = color;
    LEDStrip_MemClear(UIT);
    for(;;)
    {
        for(STAP = 0; STAP < AANTAL_P_STRIP; STAP ++)  		
            { 
                for (i = 0; i < AANTAL_STRIPS; i++)
                {
                    while( LEDStrip_Ready() == 0);
                    LEDStrip_Pixel(STAP, i, kleur);
                    LEDStrip_Pixel(STAP-1, i, 0x007F00);
                    LEDStrip_Pixel(STAP-2, i, 0x003F00);
                    LEDStrip_Pixel(STAP-3, i, 0x002F00);
                    LEDStrip_Pixel(STAP-4, i, 0x001F00);
                    LEDStrip_Pixel(STAP-5, i, 0x001000);
                    LEDStrip_Pixel(STAP-6, i, UIT);
                }

            LEDStrip_Trigger(1);
            CyDelay(snelheid);
            if(togglePin_Read() == 1) break;
            }
        for(STAP = AANTAL_P_STRIP; STAP > -1; STAP --)  		
            { 
                for (i = 0; i < AANTAL_STRIPS; i++)
                {
                    while( LEDStrip_Ready() == 0);
                    LEDStrip_Pixel(STAP, i, kleur);
                    LEDStrip_Pixel(STAP+1, i, 0x007F00);
                    LEDStrip_Pixel(STAP+2, i, 0x003F00);
                    LEDStrip_Pixel(STAP+3, i, 0x002F00);
                    LEDStrip_Pixel(STAP+4, i, 0x001F00);
                    LEDStrip_Pixel(STAP+5, i, 0x001000);
                    LEDStrip_Pixel(STAP+6, i, UIT);
                }
            LEDStrip_Trigger(1);
            CyDelay(snelheid);
            if(togglePin_Read() == 1) break;
            }    
        if(togglePin_Read() == 1) break;
    }    
    while(togglePin_Read() == 1);
    CyDelay(50);    	
}


/* [] END OF FILE */
