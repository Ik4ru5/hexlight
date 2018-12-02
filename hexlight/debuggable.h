#ifndef DEBUGGABLE_H
#define DEBUGGABLE_H
class Debuggable {
	public:
		int8_t level = 0;
		inline Debuggable() {
			Serial.begin(115200);
		}
		inline void debug(char* debugString) { 
			if(this->level > 5)
				Serial.println(debugString); 
		}
};
#endif