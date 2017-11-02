#include <iostream>
#include <random>
#include <stdio.h>

typedef uint64_t ppos_t;
const unsigned STACK_SIZE = 16;
const unsigned FLAG_SIZE = 31;

char  mainStack[FLAG_SIZE+1+STACK_SIZE];
char *mainStackPointer;

void stackPush(uint8_t x){
	ppos_t ppos = mainStackPointer - mainStack;
	if(ppos >= STACK_SIZE+FLAG_SIZE)
		return;
	++mainStackPointer;
	*mainStackPointer = x;
}

void stackPop(){
	--mainStackPointer;
}

int main(){

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);

	FILE *f = fopen("flag.txt", "r");
	if(f == NULL)
		return -1;

	for(uint8_t i=0;i<FLAG_SIZE;++i)
		mainStack[i] = '\0';
	fread(mainStack,1,FLAG_SIZE,f);

	mainStackPointer = mainStack+FLAG_SIZE;

	char c;
	while(std::cin >> c){
		if(c == 'g'){
			stackPush(dis(gen));
		} else if(c == 'l'){
			uint32_t x;
			std::cin >> x;
			stackPush(x%256);
		} else if(c == 'p'){
			stackPop();
		} else if(c == 'r'){
			printf("0x%02X ", (unsigned char)(*mainStackPointer));
		} else if(c == 'c'){
			for(uint8_t i=0;i<STACK_SIZE;++i)
				if(mainStack[i+FLAG_SIZE+1] != dis(gen)){
					std::cout << "Not correct!\r\n";
					return 0;
				}
			std::cout << mainStack;
			std::cout << "\r\n";
		}
	}

   return 0;
}
