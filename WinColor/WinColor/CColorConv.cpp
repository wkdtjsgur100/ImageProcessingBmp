// ColorConv.cpp: implementation of the CColorConv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinColor.h"
#include "ColorConv.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorConv::CColorConv()
{
}

CColorConv::~CColorConv()
{
}

//////////////////////////////////////////////////////////////////////////////////
// RGB_To_Gray - RGB���� Gray�� �ٲ۴�.
//
// Gray �� ���� [0,1]���� ���� ���´�.
// R,G,B�� ���� [0,1]���� ���� ���´�.

void CColorConv::RGB_To_Gray(float r, float g, float b, float *gray)
{
	*gray = r * 0.30F + g * 0.59F + b * 0.11F;
}

//////////////////////////////////////////////////////////////////////////////////
// Gray_To_RGB - Gray ���� RGB�� �ٲ۴�. 
//               �̰����� promotion�� �ش��ϴ� �������� ��ǻ� ���ǹ̰� ����.
// Gray �� ���� [0,1]���� ���� ���´�.
// R,G,B�� ���� [0,1]���� ���� ���´�.

void CColorConv::Gray_To_RGB(float gray, float *r, float *g, float *b)
{
	*r = *g = *b = gray;
}


//////////////////////////////////////////////////////////////////////////////////
// CMY_To_RGB - CMY���� RGB�� �ٲ۴�.
//
// R,G,B�� ���� [0,1]���� ���� ���´�.
// C,M,Y�� ���� [0,1]���� ���� ���´�.

void CColorConv::RGB_To_CMY(float r, float g, float b, float *c, float *m, float *y)
{
	*c = 1.0F - r;
	*m = 1.0F - g;
	*y = 1.0F - b;
}


//////////////////////////////////////////////////////////////////////////////////
// CMY_To_RGB - CMY���� RGB�� �ٲ۴�.
//
// R,G,B�� ���� [0,1]���� ���� ���´�.
// C,M,Y�� ���� [0,1]���� ���� ���´�.

void CColorConv::CMY_To_RGB(float c, float m, float y, float *r, float *g, float *b)
{
	*r = 1.0F - c;
	*g = 1.0F - m;
	*b = 1.0F - y;
}


//////////////////////////////////////////////////////////////////////////////////
// RGB_To_HSV - RGB���� HSV�� �ٲ۴�.
//
// R,G,B�� ���� [0,1]���� ���� ���´�.
// H�� [0,360) ����, S,V���� ���� [0,1]������ ���´�.

void CColorConv::RGB_To_HSV(float r, float g, float b, float *h, float *s, float *v)
{
	float max, min;

	max = MAX(r, g);
	max = MAX(max, b);

	min = MIN(r, g);
	min = MIN(min, b);

	*v = max;                                   // ����(V) = max(r,g,b)
	*s = (max != 0.0F) ? (max - min) / max : 0.0F;    // ä��(S)�� ���, S=0�̸� R=G=B=0

	if (*s == 0.0F)	*h = UNDEFINED;
	else                                       // ä��(S)�� != 0
	{
		float delta = max - min;                 // ����(H)�� ���Ѵ�.
		if (r == max) *h = (g - b) / delta;     // ������ Yello�� Magenta���� 
		else if (g == max) *h = 2.0F + (b - r) / delta; // ������ Cyan�� Yellow���� 
		else if (b == max) *h = 4.0F + (r - g) / delta; // ������ Magenta�� Cyan����

		*h *= 60.0F;

		if (*h < 0.0F) *h += 360.0F;           // ������ ������ �ٲ۴�.
	}
}


//////////////////////////////////////////////////////////////////////////////////
// HSV_To_RGB - HSV���� RGB�� �ٲ۴�.
//
// H�� [0,360) ����, S,V���� ���� [0,1]������ ���´�.
// R,G,B�� ���� [0,1]���� ���´�.

void CColorConv::HSV_To_RGB(float h, float s, float v, float *r, float *g, float *b)
{
	if (s == 0.0F)                                // �÷� ���� ��� ���� ��.
	{
		if (h == UNDEFINED) *r = *g = *b = v;     // �̰�� ���� ���� ����.
		else			*r = *g = *b = UNDEFINED;
	}
	else
	{                                           // S != 0 �̹Ƿ� ����(H)�� �ִ�.
		float f, p, q, t;
		int i;

		if (h == 360.0F) h = 0.0F;
		h /= 60.0F;                             // H ���� [0,360)���� ��
		i = (int)h;                             // <=H �� ���� ū ������
		f = h - i;                              // F �� H �� �Ǽ� �κ�
		p = v * (1.0F - s);
		q = v * (1.0F - (s * f));
		t = v * (1.0F - (s * (1.0F - f)));

		switch (i)
		{
		case 0: *r = v; *g = t; *b = p; break;
		case 1: *r = q; *g = v; *b = p; break;
		case 2: *r = p; *g = v; *b = t; break;
		case 3: *r = p; *g = q; *b = v; break;
		case 4: *r = t; *g = p; *b = v; break;
		case 5: *r = v; *g = p; *b = q; break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////
// RGB_To_HLS - RGB���� HLS�� �ٲ۴�.
//
// R,G,B�� ���� [0,1]���� ���´�.
// H�� [0,360) ����, S,V���� ���� [0,1]������ ���´�.

void CColorConv::RGB_To_HSL(float	r, float g, float b, float *h, float *s, float *l)
{
	float v, m, vm;
	float r2, g2, b2;

	v = MAX(r, g); 	v = MAX(v, b);
	m = MIN(r, g);	m = MIN(m, b);

	if ((*l = (m + v) / 2.0F) <= 0.0F) return;
	if ((*s = vm = v - m) > 0.0F) *s /= (*l <= 0.5F) ? (v + m) : (2.0F - v - m);
	else return;

	r2 = (v - r) / vm;
	g2 = (v - g) / vm;
	b2 = (v - b) / vm;

	if (r == v)		 *h = (g == m ? 5.0F + b2 : 1.0F - g2);
	else if (g == v) *h = (b == m ? 1.0F + r2 : 3.0F - b2);
	else			 *h = (r == m ? 3.0F + g2 : 5.0F - r2);
	*h *= 60.0F;
}


//////////////////////////////////////////////////////////////////////////////////
// HLS_To_RGB - HLS���� RGB�� �ٲ۴�.
//
// H�� [0,360) ����, S,V���� ���� [0,1]������ ���´�.
// R,G,B�� ���� [0,1]���� ���´�.


void CColorConv::HSL_To_RGB(float h, float sl, float l, float *r, float *g, float *b)
{
	float v;

	v = (l <= 0.5F) ? (l * (1.0F + sl)) : (l + sl - l * sl);
	if (v <= 0) *r = *g = *b = 0.0F;
	else
	{
		int sextant;
		float m, sv, fract, vsf, mid1, mid2;

		m = l + l - v;
		sv = (v - m) / v;
		h /= 60.0F;
		sextant = (int)h;
		fract = h - sextant;
		vsf = v * sv * fract;
		mid1 = m + vsf;
		mid2 = v - vsf;

		switch (sextant)
		{
		case 0: *r = v;    *g = mid1; *b = m; break;
		case 1: *r = mid2; *g = v;    *b = m; break;
		case 2: *r = m;    *g = v;    *b = mid1; break;
		case 3: *r = m;    *g = mid2; *b = v; break;
		case 4: *r = mid1; *g = m;    *b = v; break;
		case 5: *r = v;    *g = m;    *b = mid2; break;
		}
	}
}



void CColorConv::RGB_To_YIQ(float r, float g, float b, float *y, float *i, float *q)
{
	*y = 0.299f * r + 0.587f * g + 0.114f * b;
	*i = 0.596f * r - 0.275f * g - 0.321f * b;
	*q = 0.212f * r - 0.528f * g + 0.311f * b;
}


//////////////////////////////////////////////////////////////////////////////////
// RGB_To_HSI - RGB���� HSI�� �ٲ۴�.
//
// R,G,B�� ���� [0,1]���� ���´�.
// H�� [0,360) ����, S,V���� ���� [0,1]������ ���´�.
void CColorConv::RGB_To_HSI(float r, float g, float b, float *h, float *s, float *i)
{
	float minc;              /// minimum and maximum RGB values 
	float angle;             /// temp variable used to compute Hue 

	minc = MIN(r, g);
	minc = MIN(minc, b);

	/// compute intensity 
	*i = (r + g + b) / 3.0f;

	/// compute hue and saturation 
	if ((r == g) && (g == b))  /// gray-scale 
	{
		*s = 0.0f;
		*h = 0.0f;
		return;
	}
	else
	{
		*s = 1.0f - (3.0f / (r + g + b)) * minc;
		angle = (r - 0.5f * g - 0.5f * b) / (float)sqrt((r - g) * (r - g) + (r - b) * (g - b));
		*h = (float)acos(angle);
		*h *= 57.29577951f;          /// convert to degrees 
	}

	if (b>g)	*h = 360.0f - *h;
}


//////////////////////////////////////////////////////////////////////////////////
// HSI_To_RGB - HSI���� RGB�� �ٲ۴�.
//
// H�� [0,360) ����, S,V���� ���� [0,1]������ ���´�.
// R,G,B�� ���� [0,1]���� ���´�.
void CColorConv::HSI_To_RGB(float h, float s, float i, float *r, float *g, float *b)
{
	float angle1, angle2, scale;   /// temp variables 

	if (i == 0.0)    /// BLACK 
	{
		*r = 0.0f;
		*g = 0.0f;
		*b = 0.0f;
		return;
	}
	if (s == 0.0)     /// gray-scale  H is undefined
	{
		*r = i;
		*g = i;
		*b = i;
		return;
	}
	if (h<0.0)	h += 360.0f;

	scale = 3.0f * i;
	if (h <= 120.0)
	{
		angle1 = h*0.017453293f;    /// convert to radians - mul by pi/180 
		angle2 = (60.0f - h)*0.017453293f;

		*b = (1.0f - s) / 3.0f;
		*r = (float)(1.0f + (s*cos(angle1) / cos(angle2))) / 3.0f;
		*g = 1.0f - *r - *b;
		*b *= scale;
		*r *= scale;
		*g *= scale;
	}
	else if ((h>120.0) && (h <= 240.0))
	{
		h -= 120.0f;
		angle1 = h*0.017453293f;    /// convert to radians - mul by pi/180 
		angle2 = (60.0f - h)*0.017453293f;

		*r = (1.0f - s) / 3.0f;
		*g = (float)(1.0f + (s*cos(angle1) / cos(angle2))) / 3.0f;
		*b = 1.0f - *r - *g;
		*r *= scale;
		*g *= scale;
		*b *= scale;
	}
	else
	{
		h -= 240.0f;
		angle1 = h*0.017453293f;    /// convert to radians - mul by pi/180 
		angle2 = (60.0f - h)*0.017453293f;

		*g = (1.0f - s) / 3.0f;
		*b = (float)(1.0 + (s*cos(angle1) / cos(angle2))) / 3.0f;
		*r = 1.0f - *g - *b;
		*r *= scale;
		*g *= scale;
		*b *= scale;
	}
}