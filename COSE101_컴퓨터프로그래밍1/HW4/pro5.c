#include <stdio.h>

/*function prototype*/
void i_to_a(int n);

int main()
{
	int a;

	while (1) {
		printf("Enter the inputs : ");
		scanf_s("%d", &a);

		if (100000 <= a && a <= 999999) {
			break;
		}

		else {
			printf("Input Error!\n");
		}
	}

	i_to_a(a);

	return 0;

}

void i_to_a(int n) {

	//�� �ڸ� �̻��� ���
	if (n / 100 >= 1) {
		i_to_a(n / 100);
	}

	//�� �ڸ� ������ ���
	printf("%c ", n % 26 + 'A');
	return;
}