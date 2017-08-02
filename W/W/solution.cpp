#include <stdio.h>
#include <stdlib.h>
#include "blowfish.h"
#include <unordered_map>



int get_lr( unsigned long *l, unsigned long *r) {
	unsigned char buf[8];
	int ret = fread(buf, 1, 8, stdin);
	printf("READ %08x %08x\n", ((unsigned long* )buf)[0], ((unsigned long*)buf)[1]);
	*l = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	*r = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
	
	printf("STORED %08x %08x\n", *l, *r);
	
	return ret;
}

void dump_lr(unsigned long l, unsigned long r)
{
	printf("%c%c%c%c", (l >> 24) & 0xff, (l >> 16) & 0xff, (l >> 8) & 0xff, l & 0xff);
	printf("%c%c%c%c", (r >> 24) & 0xff, (r >> 16) & 0xff, (r >> 8) & 0xff, r & 0xff);
}




int main(int argc, char *argv[]) {
	printf("Running...\n");
	BLOWFISH_CTX ctx1, ctx2;

	unsigned long iv_l = 0xb1d8dcd5;
	unsigned long iv_r = 0xe92923cd;

	unsigned long l, r;
	unsigned long ll, rr;
	unsigned long l0, r0;

	unsigned char k1[] = { 0x00, 0x00, 0x00, 0x00 };
	unsigned char k2[] = { 0x00, 0x00, 0x00, 0x00 };

	//PNG Header
	unsigned long png_l = 0x89504e47;
	unsigned long png_r = 0x0d0a1a0a;

	//unsigned long * L;
	unsigned long * R;

	//std::list<unsigned long> * tmplist;

	//std::unordered_map<unsigned long, std::list<unsigned long> * > mL;
	std::unordered_map<unsigned long, unsigned long> mL;

	//std::unordered_map<unsigned long, std::list<unsigned long>> mR;

	std::list<unsigned long>::const_iterator itr1;

	//L = (unsigned long *) malloc(256 * 256 * 256 * sizeof(unsigned long)); 
	R = (unsigned long *) malloc(256 * 256 * 256 * sizeof(unsigned long));


	get_lr( &l, &r);
	get_lr( &l0, &r0);

	printf("Read values from file: %08x %08x %08x %08x\n", l, r, l0, r0);

	l ^= png_l;
	r ^= png_r;

	printf("Computing BF1 Encryption space from known plaintext PNG header\n");
	unsigned long idx = 0;
	for (unsigned char i = 0; i < 256; i++) {
		printf("Key1 %08x\n", idx);
		k1[1] = i;
		for (unsigned char j = 0; j < 256; j++) {
			k1[2] = j;
			for (unsigned char k = 0; k < 256; k++) {
				k1[3] = k;
				Blowfish_Init(&ctx1, k1, 4);
				ll = l; rr = r;
				Blowfish_Encrypt(&ctx1, &ll, &rr);
				//printf("DEC: %08x %08x\n",ll,rr);
				//L[idx] = ll;
				R[idx] = rr;
				
				//if (mL[ll]) {
				//	printf("Collision! %p :: %08x\n", mL[ll], ll);
				//	mL[ll]->push_back(idx);
				//}
				//else {
				//	
				//	tmplist = new std::list<unsigned long>;
				//	tmplist->push_back(ll);
				//	mL.insert(std::make_pair(ll, tmplist ));
				//}
				mL.insert(std::make_pair(ll, idx));
				idx++;
				if (k == 255) { break; }
			}
			if (j == 255) { break; }
		}
		if (i == 255) { break; }
	}
	printf("Computing BF2 Decryption space from ciphertext\n");
	idx = 0;
	for (unsigned char i = 0; i < 256; i++) {
		printf("Key2 %08x\n", idx);
		k2[1] = i;
		for (unsigned char j = 0; j < 256; j++) {
			k2[2] = j;
			for (unsigned char k = 0; k < 256; k++) {
				k2[3] = k;
				Blowfish_Init(&ctx2, k2, 4);
				ll = l0; rr = r0;
				Blowfish_Decrypt(&ctx2, &ll, &rr);
				
				//if (mL[ll]) {
				//	printf("Check 1\n");
				//	for (itr1 = mL[ll]->begin(); itr1 != mL[ll]->end(); ++itr1) {
				//		if (R[*itr1] == rr) {
				//			printf("KEY: %08x %08x\n", *itr1, idx);
				//			return 0;
				//		}
				//	}
				//}

				if (mL[ll]) {
					//printf("Hit on first part! %08x at idx %08x\n", ll,mL[ll]);
					if (R[mL[ll]] == rr) {
						printf("KEY: %08x %08x\n", mL[ll], idx);
						return 0;
					}
				}

				/*for (unsigned long n = 0; n < 256 * 256 * 256; n++) {
					if (L[idx] == ll && R[idx] == rr) {
						printf("KEY: %08x %08x\n", n, idx);
						return 0;
					}
				}*/
				idx++;
				if (k == 255) { break; }
			}
			if (j == 255) { break; }
		}
		if (i == 255) { break; }
	}



}