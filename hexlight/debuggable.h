#ifndef DEBUGGABLE_H
#define DEBUGGABLE_H
class Debuggable {
	public:
		inline Debuggable() {
			Serial.begin(115200);
		}
		inline void debug(char* debugString) { 
			Serial.println(debugString); 
		}
};
#endif