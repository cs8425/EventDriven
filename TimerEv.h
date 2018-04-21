#ifndef __TimerEv_H
#define __TimerEv_H

#include "EventDriven.h"

#define TIMER_SIZE 8

#define isEN  0x01
#define isREP 0x02
#define isRm  0x04

extern uint32_t us_ticker_read();

class TimerEv {
 
public:
    EventDriven *evlp;

    uint32_t last[TIMER_SIZE];
    uint32_t period[TIMER_SIZE];

    uint16_t count;

    unsigned char slot[TIMER_SIZE];
    unsigned char status_map[TIMER_SIZE];


    TimerEv(EventDriven &_evlp) {
        evlp = &_evlp;
        
        count = 0;

        memset(status_map, 0, TIMER_SIZE);
        memset(slot, 0, TIMER_SIZE);
        
        memset(last, 0, TIMER_SIZE * sizeof(uint32_t));
        memset(period, 0, TIMER_SIZE * sizeof(uint32_t));
    }
 
    void add(unsigned char ev, uint32_t time, char repeat = 0, char enable = 1) {
        if (count < TIMER_SIZE) {
            slot[count] = ev;
            
            period[count] = time;
            last[count] = us_ticker_read();
            
            status_map[count] = ((repeat << 1)|enable) & 0x03;
            
            count++;
        }
    }

    void enable(unsigned char ev) {
        uint16_t i;
        for(i=0; i< count; i++){
            if(slot[i] == ev){
                status_map[i] |= isEN;
                return;
            }
        }
    }

    void disable(unsigned char ev) {
        uint16_t i;
        for(i=0; i< count; i++){
            if(slot[i] == ev){
                status_map[i] &= ~isEN;
                return;
            }
        }
    }

    void clear(unsigned char ev) {
        uint16_t i;
        for(i=0; i< count; i++){
            if(slot[i] == ev){
                status_map[i] |= isRm;
                status_map[i] &= ~isEN;
                merge();
                return;
            }
        }
    }

    void reset(void) {
        count = 0;
        memset(status_map, 0, TIMER_SIZE);
        memset(slot, 0, TIMER_SIZE);
        
        memset(last, 0, TIMER_SIZE * sizeof(uint32_t));
        memset(period, 0, TIMER_SIZE * sizeof(uint32_t));
    }
    
    int length(void) {
        return count;
    }
    
    inline int available(void){
        return (count < TIMER_SIZE) ? 1 : 0;
    }
    
    void tick(void) {
        char mg = 0;
        uint32_t now = us_ticker_read();
        uint16_t i;
        uint32_t delta;
        for(i = 0; i<count; i++){
            if(status_map[i] & isEN){
                delta = now - last[i];
                if(delta > period[i]){
                    evlp->push(slot[i]);
                    last[i] = now;
                    if(!(status_map[i] & isREP)){
                        status_map[i] |= isRm;
                        status_map[i] ^= isEN;
                        mg = 1;
                    }
                }
            }
        }
        if(mg) merge();
    }


protected:
    void merge(void) {
        uint16_t i;
        uint16_t dec = 0;
        for(i = 0; i<count; i++){
            if(status_map[i] & isRm){
                if(i+1 < count){
                    status_map[i] = status_map[i+1];
                    last[i] = last[i+1];
                    slot[i] = slot[i+1];
                    period[i] = period[i+1];
                    status_map[i+1] |= isRm;
                }
                dec++;
            }
        }
        count -= dec;
    }

};

#endif
