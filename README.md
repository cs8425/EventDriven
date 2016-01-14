# EventDriven
A simple event driven system for microcontroller in C++.

## Setup
  1. define your event id
```
#define ADC_START      1
#define ADC_END        2
#define LED            3
#define DEBUG_PRINT    4
```

  2. implement `uint32_t us_ticker_read()` return microseconds
```
uint32_t us_ticker_read(){
	return micros();
}
```

## Usage

```
#include "EventDriven.h"
#include "TimerEv.h"

EventDriven eventloop;
TimerEv timerev(eventloop);

// define your events
#define A       1
#define B       2
#define C       3

// ...

int main(){

	timerev.add(C, 50000, 1, 1);  // add repeat C timer (50 ms)
	eventloop.push(A); // first call A (emit A event)

	while(1){
		timerev.tick();
		switch(eventloop.get()){

			case A:
				// do somthing
				// ...
				timerev.add(B, 10*1000, 0, 1); // emit B event 10 ms later
			break;

			case B:
				// do somthing
				// ...
				eventloop.push(A); // emit A event right now
			break;

			// this do every 50 ms
			case C:
				led = !led;
			break;
		}
	}
	return 0;
}


```


## API

[TOC]

### event_loop EventDriven()
creat a event loop system.

#### void EventDriven.push(event_id)
push a event into event loop.

#### event_id EventDriven.get()
return next event in event loop.

#### void EventDriven.reset()
clear all event in event loop.

#### bool EventDriven.check_map(event_id)
return is event in event loop or not.

#### int EventDriven.length()
return the event count in event loop.

#### bool EventDriven.available()
return is event loop full or not.


### timer_slot TimerEv(event_loop)
creat a timer system base on event loop.

#### void TimerEv.add(event_id, time, repeat = 0, enable = 1)
add a timer event to timer slots.

#### void TimerEv.enable(event_id)
enable a timer event in timer slots.

#### void TimerEv.disable(event_id)
disable a timer event in timer slots.

#### void TimerEv.clear(event_id)
remove timer event in timer slots.

#### void TimerEv.reset()
clear all timer event in timer slots.

#### void TimerEv.tick()
call this for emit event.

#### int TimerEv.length()
return the timer event count in timer slots.

#### bool TimerEv.available()
return is timer slots full or not.



