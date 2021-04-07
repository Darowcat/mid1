#include "mbed.h"
#include "uLCD_4DGL.h"

using namespace std::chrono;

InterruptIn up_button(A0);
InterruptIn down_button(A2);
InterruptIn confirm_button(A1);
AnalogOut Aout(PA_5);
AnalogIn Ain(A5);
uLCD_4DGL uLCD(D1, D0, D2);

int freq = 4;
double ADCdata[160];
bool conf = false;
int idx = 0;
float i = 0.0f;
int j = 0;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

void info(){
    uLCD.locate(1,2);
    if (conf){
        uLCD.color(RED);
    } else {
        uLCD.color(WHITE);
    }
    if (idx == 0){
        uLCD.printf(" 1 ");
    } else if (idx == 1){
        uLCD.printf("1/2");
    } else if (idx == 2){
        uLCD.printf("1/4");
    } else if (idx == 3){
        uLCD.printf("1/8");
    }
    uLCD.printf("\n");
}

void rise_fun(){
    if (idx == 0){
        idx = 0;
    } else {
        idx--;
    }
    conf = false;
    //uLCD.color(WHITE);
    queue.call(info);
}

void down_fun(){
    if (idx == 3){
        idx = 3;
    } else {
        idx++;
    }
    conf = false;
    //uLCD.color(WHITE);
    queue.call(info);
}

void confirm_fun(){
    //uLCD.color(RED);
    conf = true;
    queue.call(info);
}

void go(){
    if (i <= 80.0f){
        Aout = (i / 88.0f);
        //Aout = 1.0f;
        //printf("%lf\r\n", Aout.read());
    } else if (i <= 160.0f){
        Aout = 0.909f;
        //Aout = 1.0f;
        //printf("%lf\r\n", Aout.read());
    } else {
        Aout = (240.0f - i) / 88.0f;
        //Aout = 1.0f;
        //printf("%lf\r\n", Aout.read());
    }
    i+=3.0f;
    printf("%lf\r\n", i);
    if (i > 240.0f){
        conf = false;
        i = 0.0f;
    }
}

int main()
{       
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.color(WHITE);
    info();
    t.start(callback(&queue, &EventQueue::dispatch_forever));
   
    while (1)
    {
        up_button.rise(&rise_fun);
        down_button.rise(&down_fun);
        confirm_button.rise(&confirm_fun);

        if (i >= 240.0f){
            i = 0.0f;
        }

        if (idx == 0){
            if (i <= 80.0f){
                Aout = (i / 88.0f);
            } else if (i <= 160.0f){
                Aout = 0.909f;
            } else {
                Aout = (240.0f - i) / 88.0f;
            }
        } else if (idx == 1){
            if (i <= 40.0f){
                Aout = (i / 44.0f);
            } else if (i <= 200.0f){
                Aout = 0.909f;
            } else {
                Aout = (240.0f - i) / 44.0f;
            }
        } else if (idx == 2){
            if (i <= 20.0f){
                Aout = (i / 22.0f);
            } else if (i <= 220.0f){
                Aout = 0.909f;
            } else {
                Aout = (240.0f - i) / 22.0f;
            }
        } else if (idx == 3){
            if (i <= 10.0f){
                Aout = (i / 11.0f);
            } else if (i <= 230.0f){
                Aout = 0.909f;
            } else {
                Aout = (240.0f - i) / 11.0f;
            }
        }

        i += 3.0f;

        wait_us(3000);

        if (conf){
            ADCdata[j] = Ain;
            if (j == 159){
                //printf("%d\r\n", freq);
                for (int k = 0; k < 160; k++){
                    printf("%lf\r\n", ADCdata[k]);
                }
                j = 0;
                conf = 0;
            } else {
                //printf("%d\r\n", idx);
                j++;
            }
        }
    }
    
}