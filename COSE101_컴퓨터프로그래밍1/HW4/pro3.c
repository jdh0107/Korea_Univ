#include <stdio.h>

int gcd(int n, int m);

int main()
{
	int a = 0, b = 0, t = 0;

	do {
		printf("Enter two positive intergers: ");
		scanf_s("%d %d", &a, &b);

		if (a <= 0 || b <= 0) {
			printf("Wrong Input. Try again!\n\n");
		}
	} while (a <= 0 || b <= 0); //a �Ǵ� b�� ������ ���� ���

	/*a �� b ��� ���� ����*/

	if (a <= b) {
		t = a;
		a = b;
		b = t;
	}

	/* a�� b���� ŭ*/

	printf("GCD of %d and %d is %d", a, b, gcd(a, b));

	return 0;
}

int gcd(int n, int m) {
	if (m == 0) {
		return n;
	}

	else {
		gcd(m, n%m);
	}
}