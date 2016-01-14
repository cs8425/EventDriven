#ifndef __RING_BUF_H
#define __RING_BUF_H

//#define SERIAL_BUFFER_SIZE 4096

class ring_buffer {
 
public:
    //char buffer[SERIAL_BUFFER_SIZE];
    char *buffer;
    unsigned int head;
    unsigned int tail;
    uint32_t SERIAL_BUFFER_SIZE;
    
    ring_buffer(uint32_t size = 0x100) {
        head = 0;
        tail = 0;
        buffer = new char [size];
        SERIAL_BUFFER_SIZE = size;
    }
    
    ~ring_buffer(){
        delete [] buffer;
    
        return;
    }
 
    void put(char c) {
        unsigned int i = head + 1;
        
        if(i >= SERIAL_BUFFER_SIZE){
            i = 0;
        }

        if (i != tail) {
            buffer[head] = c;
            head = i;
        }
    }
 
    int get(void) {
        // if the head isn't ahead of the tail, we don't have any characters
        if (head == tail) {
            return -1;
        } else {
            char c = buffer[tail];
            tail = tail + 1;
            if(tail >= SERIAL_BUFFER_SIZE){
                tail = 0;
            }
            return c;
        }
    }
    
    void reset(void) {
        head = tail;
    }
    
    inline uint32_t available(void){
        return (head == tail) ? 0 : 1;
    }

 
protected:

};

#endif
