#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int flip();

void main()
{
	srand(time(NULL)); //�� Ÿ�ֺ̹��� �õ��Ѵ�!

	int count = 1;
	int Head = 0, Tail = 0;
	while (count <= 100) {

		if (count % 10 == 0) {
			if (flip()) {			// rand���� Ȧ���� ������
				printf("Heads \n");
				Head++;
			}
			else {
				printf("Tails \n"); // rand���� ¦���� ������
				Tail++;
			}
		}

		else {
			if (flip()) {			// rand���� Ȧ���� ������
				printf("Heads ");
				Head++;
			}
			else {
				printf("Tails "); // rand���� ¦���� ������
				Tail++;
			}
		}
		count++;
	}

	printf("\n");
	printf("The total number of Heads was %d\n", Head);
	printf("The total number of Tails was %d", Tail);
}

int flip() {


	int i = rand();


	if (i % 2 == 0)
		return 0;
	else
		return 1;
}
// ���α׷� ����: <Ctrl+F5> �Ǵ� [�����] > [��������� �ʰ� ����] �޴�
// ���α׷� �����: <F5> Ű �Ǵ� [�����] > [����� ����] �޴�

// ������ ���� ��: 
//   1. [�ַ�� Ž����] â�� ����Ͽ� ������ �߰�/�����մϴ�.
//   2. [�� Ž����] â�� ����Ͽ� �ҽ� ��� �����մϴ�.
//   3. [���] â�� ����Ͽ� ���� ��� �� ��Ÿ �޽����� Ȯ���մϴ�.
//   4. [���� ���] â�� ����Ͽ� ������ ���ϴ�.
//   5. [������Ʈ] > [�� �׸� �߰�]�� �̵��Ͽ� �� �ڵ� ������ ����ų�, [������Ʈ] > [���� �׸� �߰�]�� �̵��Ͽ� ���� �ڵ� ������ ������Ʈ�� �߰��մϴ�.
//   6. ���߿� �� ������Ʈ�� �ٽ� ������ [����] > [����] > [������Ʈ]�� �̵��ϰ� .sln ������ �����մϴ�.
