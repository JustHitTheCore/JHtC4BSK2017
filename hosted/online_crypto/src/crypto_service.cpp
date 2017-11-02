#include <cstdio>
#include <cassert>
#include <stdint.h>
#include <unistd.h>

using namespace std;

const uint32_t KEY_LEN = 50;
const uint32_t FLAG_LEN = 26;

//  Zakodowana flaga:
//  B4 25 2B 73 CE 74 C7 D4 3D 1F 16 03 E2 3E 70 E7 0F F5 EE B5 62 DB 35

char flag[FLAG_LEN+1] = "JHtC4BSK{S1mp1e_Crypt0_12}";

unsigned char key[KEY_LEN] = {0xfe, 0x6d, 0x5f, 0x30, 0xfa, 0x36, 0x94, 0x9f, 0x46, 0x4c, 0x27, 0x6e, 0x92,
					 0x0f, 0x15, 0xb8, 0x4c, 0x87, 0x97, 0xc5, 0x16, 0xeb, 0x48, 0x95, 0x93, 0xb1,
					 0x63, 0x13, 0x70, 0x14, 0xe5, 0x6d, 0x71, 0x69, 0x57, 0xcd, 0xce, 0x17, 0x9f,
					 0x33, 0x94, 0x7d, 0xf1, 0xba, 0x65, 0x2e, 0x3f, 0x11, 0xfd, 0x0c};
char msg[KEY_LEN];

int main(){

	assert(FLAG_LEN <= KEY_LEN);

	uint32_t input_len = 0;
   for(uint32_t i=0;i<KEY_LEN;++i){
		read(STDIN_FILENO, &msg[i], 1);
		if(msg[i] == '\n'|| msg[i] == '\r')
			break;

		if(i < FLAG_LEN && (msg[i] ^ key[i]) == flag[i]){
			printf("No.\r\n");
			return 0;
		}

		++input_len;
   }

   assert(input_len <= KEY_LEN);

   for(uint32_t i=0;i<input_len;++i)
		printf("%02X ", (unsigned char)(msg[i] ^ key[i]));

   return 0;
}
