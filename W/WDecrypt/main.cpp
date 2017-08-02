#include <stdio.h>
#include "blowfish.h"



BLOWFISH_CTX ctx1, ctx2;

unsigned long iv_l = 0xb1d8dcd5;
unsigned long iv_r = 0xe92923cd;


unsigned char k1[] = { 0x00, 0xc4 , 0xf2 , 0x3f };
unsigned char k2[] = { 0x00, 0x5f , 0x6d , 0x8f };

void dump_lr(FILE * f, unsigned long l, unsigned long r)
{
	
	fprintf(f, "%c%c%c%c", (l >> 24) & 0xff, (l >> 16) & 0xff, (l >> 8) & 0xff, l & 0xff);
	fprintf(f, "%c%c%c%c", (r >> 24) & 0xff, (r >> 16) & 0xff, (r >> 8) & 0xff, r & 0xff);
}

int get_lr(FILE * f, unsigned long *l, unsigned long *r) {
	unsigned char buf[8];
	int ret = fread(buf, 1, 8, f);
	//printf("READ %08x %08x\n", ((unsigned long*)buf)[0], ((unsigned long*)buf)[1]);
	*l = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	*r = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];

	//printf("STORED %08x %08x\n", *l, *r);

	return ret;
}

int main(int argc, char *argv[])
{
	unsigned long l, r;
	unsigned long ll, rr;
	unsigned char buf[8];

	Blowfish_Init(&ctx1, k1, 4);
	Blowfish_Init(&ctx2, k2, 4);

	FILE * out = fopen("C:\\Users\\Kevin\\Downloads\\inception.txt (2)\\out.png", "wb");
	FILE * f = fopen("C:\\Users\\Kevin\\Downloads\\inception.txt (2)\\flag.png.enc.dbl-bfsh-32-cbc", "rb");


	//PNG Header
	unsigned long png_l = 0x89504e47;
	unsigned long png_r = 0x0d0a1a0a;

	get_lr(f, &iv_l, &iv_r);

	l = iv_l;
	r = iv_r;

	unsigned long pl, pr;

	


	int idx = 0;
	while (fread(buf, 1, 8, f) > 0) {
		//printf("%08x %08x\n", ((unsigned long *) buf)[0], ((unsigned long * )buf)[1]);
		pl = ll = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
		pr = rr = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
		
		

		//printf("%08x %08x\n", ll, rr);
		Blowfish_Decrypt(&ctx2, &ll, &rr);
		//printf("%08x %08x\n", ll, rr);
		Blowfish_Decrypt(&ctx1, &ll, &rr);
		//printf("%08x %08x\n", ll, rr);
		

		dump_lr(out, ll ^ l, rr ^ r);

		l = pl;
		r = pr;

		//printf("%08x %08x\n", l, r);
		idx += 1;
		//printf("Iteration: %d\n", idx);
		
	}

}
