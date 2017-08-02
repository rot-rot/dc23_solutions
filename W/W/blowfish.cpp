/* blowfish.c */

#include "blowfish.h"


unsigned long F(BLOWFISH_CTX *ctx, unsigned long x)
{
	unsigned short a;
	unsigned short b;
	unsigned short c;
	unsigned short d;
	unsigned long y;
	d = (x & 255);
	x >>= 8;
	c = (x & 255);
	x >>= 8;
	b = (x & 255);
	x >>= 8;
	a = (x & 255);
	y = ((ctx->S)[0][a] + (ctx->S)[1][b]);
	y = (y ^ (ctx->S)[2][c]);
	y = (y + (ctx->S)[3][d]);
	return y;
}
void Blowfish_Encrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr)
{
	unsigned long Xl;
	unsigned long Xr;
	unsigned long temp;
	short i;
	Xl = (*xl);
	Xr = (*xr);
	for (i = 0; i<16; i++)
	{
		Xl = (Xl ^ (ctx->P)[i]);
		Xr = (F(ctx, Xl) ^ Xr);
		temp = Xl;
		Xl = Xr;
		Xr = temp;
	}
	temp = Xl;
	Xl = Xr;
	Xr = temp;
	Xr = (Xr ^ (ctx->P)[16]);
	Xl = (Xl ^ (ctx->P)[16 + 1]);
	*xl = Xl;
	*xr = Xr;
}
void Blowfish_Decrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr)
{
	unsigned long Xl;
	unsigned long Xr;
	unsigned long temp;
	short i;
	Xl = (*xl);
	Xr = (*xr);
	for (i = (16 + 1); i>1; i--)
	{
		Xl = (Xl ^ (ctx->P)[i]);
		Xr = (F(ctx, Xl) ^ Xr);
		temp = Xl;
		Xl = Xr;
		Xr = temp;
	}
	temp = Xl;
	Xl = Xr;
	Xr = temp;
	Xr = (Xr ^ (ctx->P)[1]);
	Xl = (Xl ^ (ctx->P)[0]);
	*xl = Xl;
	*xr = Xr;
}
void Blowfish_Init(BLOWFISH_CTX *ctx, unsigned char *key, int keyLen)
{
	int i;
	int j;
	int k;
	unsigned long data;
	unsigned long datal;
	unsigned long datar;
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<256; j++)
			ctx->S[i][j] = ORIG_S[i][j];
	}
	j = 0;
	for (i = 0; i<(16 + 2); i++)
	{
		data = 0;
		for (k = 0; k<4; k++)
		{
			data = ((data << 8) | key[j]);
			j = (j + 1);
			if (j >= keyLen)
				j = 0;
		}
		ctx->P[i] = (ORIG_P[i] ^ data);
	}
	datal = 0;
	datar = 0;
	for (i = 0; i<(16 + 2); i += 2)
	{
		Blowfish_Encrypt(ctx, &datal, &datar);
		ctx->P[i] = datal;
		ctx->P[i + 1] = datar;
	}
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<256; j += 2)
		{
			Blowfish_Encrypt(ctx, &datal, &datar);
			ctx->S[i][j] = datal;
			ctx->S[i][j + 1] = datar;
		}
	}
}
int Blowfish_Test(BLOWFISH_CTX *ctx)
{
	unsigned long L = 1;
	unsigned long R = 2;
	Blowfish_Init(ctx, (unsigned char *) "TESTKEY", 7);
	Blowfish_Encrypt(ctx, &L, &R);
	if ((L != 3744677842u) || (R != 816257972))
		return -1;
	Blowfish_Decrypt(ctx, &L, &R);
	if ((L != 1) || (R != 2))
		return -1;
	return 0;
}