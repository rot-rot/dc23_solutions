#include <stdio.h>
#include "blowfish.h"

/*

BLOWFISH_CTX ctx1, ctx2;

unsigned long iv_l = 0xb1d8dcd5;
unsigned long iv_r = 0xe92923cd;

unsigned char k1[] = { 0x00, 0x00 , 0x00  , 0x00};
unsigned char k2[] = { 0x00, 0x00 , 0x00 , 0x00 };

void dump_lr(unsigned long l, unsigned long r)
{
	printf("%c%c%c%c", (l >> 24) & 0xff, (l >> 16) & 0xff, (l >> 8) & 0xff, l & 0xff);
	printf("%c%c%c%c", (r >> 24) & 0xff, (r >> 16) & 0xff, (r >> 8) & 0xff, r & 0xff);
}

int main1(int argc, char *argv[])
{
	unsigned long l, r;
	unsigned char buf[8];

	Blowfish_Init(&ctx1, k1, 4);
	Blowfish_Init(&ctx2, k2, 4);

	l = iv_l;
	r = iv_r;

	dump_lr(l, r);

	while (fread(buf, 1, 8, stdin) > 0) {
		l ^= (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
		r ^= (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
		Blowfish_Encrypt(&ctx1, &l, &r);
		Blowfish_Encrypt(&ctx2, &l, &r);
		dump_lr(l, r);
	}

}
*/
