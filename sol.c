#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define BOOL short

//FIXED_FLOAT은 short범위에서 고정소수점으로 소수를 표현하기 위한 데이터 타입(7bit SHIFT)
typedef unsigned short FIXED_FLOAT;

//FIXED_FLOAT에서의 곱셉을 연산한다. 7bit shift 수를 두번 곱한 뒤 지나치게 shift된 7bit를 되돌림
FIXED_FLOAT mulFixedFloat(FIXED_FLOAT a, FIXED_FLOAT b)
{
    return ((FIXED_FLOAT)a * b) >> 7;
}

//y = x^3 + 4x^2 – 10 = 0 간단하게 하면
//y = x^2*(x+4) - 10 = 0 이 된다.
//y의 부호가 -(여기서는 FALSE로 표현)이면 x^2*(x+4) - 10 < 0 이다.
//x^2*(x+4) < 10 일때에 따라 부호를 판단할 수 있다.
//이때 7 bit shift를 유의한다.
BOOL getFunctionYOfFixedFloat(FIXED_FLOAT x) {
	FIXED_FLOAT squared = mulFixedFloat(x,x);
	return mulFixedFloat(squared, x+(4<<7)) < (10<<7)? FALSE : TRUE;
}
//7bit shift된 데이터 타입으로 구성한 bisection method를 통해 y=0인 x를 찾는 함수
FIXED_FLOAT getFunctionYofZeroFixedFloat(FIXED_FLOAT x1, FIXED_FLOAT x2, FIXED_FLOAT tolerance) {
	FIXED_FLOAT x3 = 0;
	x1 = x1<<7;	
	x2 = x2<<7;
	int i = 1;
	while (0.5 * abs(x1 - x2) > tolerance * 0.5) { 	
		x3 = (x1 + x2) / 2; 														//신규 경계 값
		if (getFunctionYOfFixedFloat(x3) == getFunctionYOfFixedFloat(x1)) x1 = x3; 	//f(x1)의 부호와 f(x2)의 부호가 같은지 비교, 같으면 x1은 신규 경계값 할당
		else x2 = x3;																//다르면 x2에 신규 경계값 할당
		i++;
	}
    printf("총 iter 수: %d\n", i);
	return x3;
}

//위 알고리즘을 일반적인 데이터 타입 float을 통하여 구성
BOOL getFunctionYOf(float x) {
	float squared = x*x;
	return squared*(x+4.0) < 10.0 ? FALSE : TRUE;
}
float getFunctionYofZero(float x1, float x2, float tolerance) {
	float x3 = 0;
	int i = 1;
 	while (0.5 * fabsf(x1 - x2) >= tolerance) {
		x3 = (x1 + x2) / 2;
		if (getFunctionYOf(x3) == getFunctionYOf(x1)) x1 = x3;
		else x2 = x3;
		i++;
	}
    printf("총 iter 수: %d\n", i);
	return x3;
}

int main() {
    printf("\nUnsigned Short(16bits)\n" );
	FIXED_FLOAT x = getFunctionYofZeroFixedFloat(1, 2, 1);
	float fixedx = (float)x / 128.0;
    printf("funtion(x)=0의 x의 근사치: %.20lf\n", fixedx);
    printf("funtion(x)=0의 x를 근사했을때 y 근사치: %.20lf\n", (fixedx*fixedx) * (fixedx + 4.0) - 10 );
	
    printf("\nFloating point(32bit)\n" );
	float floatingx = getFunctionYofZero(1.0, 2.0, 0.0000001);

    printf("funtion(x)=0의 x의 근사치: %.20lf\n", floatingx);
    printf("funtion(x)=0의 x를 근사했을때 y 근사치: %.20lf\n\n", (floatingx*floatingx) * (floatingx + 4.0) - 10 );
    return 0;
}