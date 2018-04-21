#ifndef __EventDriven_H
#define __EventDriven_H

#define QUEUE_SIZE 32

class EventDriven {

public:
	unsigned char buffer[QUEUE_SIZE];
	unsigned char head;
	unsigned char tail;

	unsigned char union_map[QUEUE_SIZE/8];

	unsigned char index;
	unsigned char offset;

	EventDriven() {
		head = 0;
		tail = 0;
		memset(union_map, 0, QUEUE_SIZE/8);
	}

	void push(unsigned char c) {
		int i = (unsigned char)(head + 1);

		if(i == QUEUE_SIZE) i = 0;

		if (i != tail) {
			buffer[head] = c;
			head = i;

			set_map(c);
		}
	}

	int get(void) {
		// if the head isn't ahead of the tail, we don't have any characters
		if (head == tail) {
			return -1;
		} else {
			unsigned char c = buffer[tail];

			clear_map(c);

			tail = (unsigned char)(tail + 1);
			if(tail == QUEUE_SIZE) tail = 0;
			return c;
		}
	}

	void reset(void) {
		head = tail;
		memset(union_map, 0, QUEUE_SIZE/8);
	}

	int length(void) {
		if(head == tail) return 0;

		if(head > tail){
			return head - tail + 1;
		}else{
			return head - tail + QUEUE_SIZE;
		}
	}

	inline int available(void){
		return (head == tail) ? 0 : 1;
	}

	unsigned char check_map(unsigned char c) {
		getbit(c);

		unsigned char byte = union_map[index];

		return (byte & (1 << offset));
	}

protected:
	inline void getbit(unsigned char c) {
		unsigned char i = 0;

		while(c >= 8) {
			i++;
			c -= 8;
		}
		index = i;
		offset = c;
	}

	inline void set_map(unsigned char c) {
		getbit(c);
		union_map[index] |= (1 << offset);
	}
	inline void clear_map(unsigned char c) {
		getbit(c);
		union_map[index] &= ~(1 << offset);
	}

};

#endif
