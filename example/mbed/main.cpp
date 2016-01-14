#include "mbed.h"
#include "ring_buffer.h"
//#include "BufferedSerial.h"

#include "us_ticker_api.h"

#include "EventDriven.h"
#include "TimerEv.h"

Serial pc(USBTX, USBRX); // tx, rx
//BufferedSerial pc(USBTX, USBRX, 128, 1024); // tx, rx

// define events
#define ADC_START     1
#define BT            2
#define LED           3
#define DEBUG         4
#define ADC_END       5

EventDriven eventloop;
TimerEv timerev(eventloop);

int16_t ch1 = 512;
int16_t ch2 = 512;
int16_t ch3 = 512;

ring_buffer RX_buf(64);
void _RX_ISR(void);
char readInt(int16_t &output);

/*uint32_t us_ticker_read(){
	return micros();
}*/

int main(void) {
    //int i;
    char LED = 0;

    pc.baud(115200);


    pc.attach(&_RX_ISR, pc.RxIrq);
    NVIC_EnableIRQ(UART0_IRQn);

    timerev.add(LED, 62.5*1000, 1, 1);  // add repeat LED timer
    timerev.add(ADC_START, 5*1000, 1, 1);  // add repeat ADC timer
    wait_ms(2);
    timerev.add(DEBUG, 65*1000, 1, 1);  // add repeat DEBUG timer


    pc.printf("init end...\n");

    while (true) {

        /*if(pc.readable()){
            _RX_ISR();
        }*/

        timerev.tick();
        switch(eventloop.get()){

            // do when BT happened
            case BT:
                test = readInt(ch1);
                test += readInt(ch2);
                test += readInt(ch3);
                RX_buf.reset();
                //pc.printf("%d\t%d\t%d\n", ch1, ch2, ch3);
            break;

            // do every 5 ms
            case ADC_START:
                // ...
                timerev.add(ADC_END, 10*1000, 0, 1); // emit ADC_END after 10 ms
            break;

            // do when ADC_END emited
            case ADC_END:
                dosome();
            break;

            // do every 65 ms
            case DEBUG:
                pc.printf("%d\t%d\t%d\n", ch1, ch2, ch3);
            break;

            //LED do every 62.5 ms
            case LED:
                LED = 1 - LED;
            break;

        }

    }
}



void _RX_ISR(void) {
    unsigned char c;

    //pc.putc(c+1);
    //if(eventloop.check_map(BT)){
    //    return;
    //}else{
        c = pc.getc();
        if(c == '\n'){
            eventloop.push(BT);
        }else{
            if(c == '!'){
                RX_buf.reset();
            }else{
                RX_buf.put(c);
            }
        }
    //}
}

char readInt(int16_t &output) {
    int16_t out = 0x00;

    int r = RX_buf.get();
    if(r != -1){
        out |= r - '0';
        out <<= 4;
    }else{
        return 0;
    }

    r = RX_buf.get();
    if(r != -1){
        out |= r - '0';
        out <<= 4;
    }else{
        return 0;
    }

    r = RX_buf.get();
    if(r != -1){
        out |= r - '0';
    }else{
        return 0;
    }

    output = out;

    return 1;
}
