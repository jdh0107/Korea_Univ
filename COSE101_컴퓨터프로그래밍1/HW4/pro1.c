#include <stdio.h>

/*function prototype*/
int ulcase(char v);

int main()
{
	char c;
	int result;

	printf("Enter the Character : ");
	scanf_s("%c", &c);

	result = ulcase(c);

	if (result == 0)
		printf("%c: Lowercase", c);
	else if (result == 1)				//���⼭ ����
		printf("%c: Uppercase", c);
	else if (result == 2)
		printf("INPUT IS NOT AN ALPHABET");


	return 0;
}

int ulcase(char v) {

	int ascii = (int)v;

	if (ascii >= 'a' && ascii <= 'z')
		return 0;
	else if (ascii >= 'A' && ascii <= 'Z')
		return 1;
	else
		return 2;
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
