#include <8051.h>

#define T0_DAT 65535-1152
#define TL_0 T0_DAT%256
#define TH_0 T0_DAT/256

#define UP 0b00001000
#define DOWN 0b00010000
#define LEFT 0b00100000
#define RIGHT 0b00000100
#define ESC 0b00000010
#define ENTER 0b00000001

#define FLASH_VAL 192

#define OK 0x10
#define LED 0b01000000
#define F1 0x01

unsigned int flash_counter = 0;

__xdata unsigned char* CSDS =(__xdata unsigned char *)0xFF30;  //disp
__xdata unsigned char* CSDB =(__xdata unsigned char *)0xFF38;  //seg

unsigned char led_idx = 0;//idx disp
unsigned char led_b = 1;//disp bitowo

__code unsigned char WZORY[10] = {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111};

unsigned int timer = 960;
unsigned char flash = 0;
unsigned char edit = 0;

unsigned char SS = 55;
unsigned char MM = 58;
unsigned char HH = 23;
unsigned char CZAS[6] = {0, 0, 0, 0, 0, 0};
unsigned char KEY[4] = {0, 0, 0, 0};

void t0_int(void) __interrupt(1){
	TH0 = 226;   //226 = 256-30 -> 921600/(30*32) = 960
	F0 = 1;
}

void przelicz(){
    unsigned char B = 10;
    CZAS[5] = HH/B;
    CZAS[4] = HH%B;
    CZAS[3] = MM/B;
    CZAS[2] = MM%B;
    CZAS[1] = SS/B;
    CZAS[0] = SS%B;
}

void incCzas(){
    SS++;
    if(SS > 59){
        SS = 0;
        MM++;
        if(MM > 59){
            MM = 0;
            HH++;
            if(HH > 23){
                HH = 0;
                SS = 0;
                MM = 0;
            }
        }
    }
    przelicz();
}

unsigned int running = 1;

void disp(){
    P1_6 = 1;
    if(led_idx == 6){
        *CSDS = LED;
        *CSDB = OK;
    }else{
    	*CSDS = led_b;
    	*CSDB = WZORY[CZAS[led_idx]];
    }
    P1_6 = 0;
}

unsigned char wybor = 0;

void dispDB(){
    P1_6 = 1;
    if(led_idx == 6){
        *CSDS = LED;
        *CSDB = F1;
    }else{
	    *CSDS = led_b;
	    if(led_idx == wybor*2 || led_idx == wybor*2+1){
	        *CSDB = (WZORY[CZAS[led_idx]] | 0b10000000);
	    }else{
	    	if(flash){
			*CSDB = WZORY[CZAS[led_idx]];
		}else{
		     *CSDB = 0;
		}
	    }
	 }
    P1_6 = 0;
}

unsigned char up_pressed = 0;
unsigned char down_pressed = 0;
unsigned char left_pressed = 0;
unsigned char right_pressed = 0;

void kbd(){
    if(KEY[0] != 0){
        if((KEY[0] & UP) && !running && !right_pressed && !left_pressed && !up_pressed && !down_pressed){
            up_pressed = 1;
            if(wybor == 0){
                if(SS < 59){
                    SS++;
                }else{
                    SS=0;
                }
                przelicz();
            }else if(wybor == 1){
                if(MM < 59){
                    MM++;
                }else{
                    MM=0;
                }
                przelicz();
            }else if(wybor == 2){
                if(HH < 23){
                    HH++;
                }else{
                    HH=0;
                }
                przelicz();
            }
        }
        if((KEY[0] & DOWN) && !running && !right_pressed && !left_pressed && !up_pressed && !down_pressed){
            down_pressed = 1;
            if(wybor == 0){
                if(SS > 0){
                    SS--;
                }else{
                    SS=59;
                }
                przelicz();
            }else if(wybor == 1){
                if(MM > 0){
                    MM--;
                }else{
                    MM=59;
                }
                przelicz();
            }else if(wybor == 2){
                if(HH > 0){
                    HH--;
                }else{
                    HH=23;
                }
                przelicz();
            }
        }
        if((KEY[0] & LEFT) && !running && !right_pressed && !left_pressed && !up_pressed && !down_pressed){
		left_pressed = 1;
                if(wybor < 2){
                    wybor++;
                }
        }
        if((KEY[0] & RIGHT) && !running && !right_pressed && !left_pressed && !up_pressed && !down_pressed){
		right_pressed = 1;
                if(wybor > 0){
                    wybor--;
                }
        }
        if(KEY[0] != KEY[1] && KEY[0] != KEY[2] && KEY[0] != KEY[3]){
            if((KEY[0] & ENTER)){
	    	timer = 0;
                running = 0;
                flash = 0;
            }
            if((KEY[0] & ESC)){
                running = 1;
            }
        }
        KEY[3] = KEY[2];
        KEY[2] = KEY[1];
        KEY[1] = KEY[0];
        KEY[0] = 0;
    }else{
        up_pressed = 0;
        down_pressed = 0;
        left_pressed = 0;
        right_pressed = 0;
    }
}

int main(){
    TMOD = 0b01010000;
    ET0 = 1;
    EA = 1;
    TH0 = TH_0;
    TL0 = TL_0;
    TR0 = 1;
    F0 = 1;
    ES = 1;
    przelicz();
    timer = 960;
    while(1){
        if(F0 == 1){
            F0 = 0;
            flash_counter++;
            if(flash_counter >= FLASH_VAL){
	    	flash_counter = 0;
	    	flash = !flash;
	    }
            if(running == 0){
                dispDB();
            }else if(running == 1){
                disp();
            }
            if(P3_5 == 1){
                KEY[0] = (KEY[0] | led_b);
            }
            if(led_idx < 6){
                led_idx++;
                led_b += led_b;
            }
            else{
                led_idx = 0;
                led_b = 1;
                kbd();
            }
            if(running == 1){
                timer--;
                if(timer == 0){
                    timer = 960;
                    incCzas();
                    P1_7 = !P1_7;
                }
            }
        }

    }
}



