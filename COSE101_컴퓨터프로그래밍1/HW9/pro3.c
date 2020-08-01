#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//�Լ�, �迭
void open_print(FILE *fp);
void summation(FILE *fp1, FILE *fp2, FILE *fp3);
int main(void)
{
	FILE *fPtr = fopen("input1.txt", "r");
	FILE *f2Ptr = fopen("input2.txt", "r");
	FILE *f3Ptr = fopen("result.txt", "w"); 

	printf("input1 matrix:\n");
	open_print(fPtr);

	printf("input2 matrix:\n");
	open_print(f2Ptr);

	printf("result matrix:\n");
	summation(fPtr, f2Ptr, f3Ptr);
	
	fclose(fPtr);
	fclose(f2Ptr);
	fclose(f3Ptr);
	return 0;
}

void open_print(FILE *fp)
{
	char buffer[4] = { 0 };
	int count = 0;

	while (!feof(fp)) {						//eof�� �ƴϸ�
		fgets(buffer, sizeof(buffer), fp);	//�� �� �о���̰�
		count++;
		
		if (!(count % 3))					//count�� 3�� �����
			printf("%-8d\n", atoi(buffer));
		else								//count�� 3�� ����� �ƴϸ�
			printf("%-8d", atoi(buffer));
	}
	printf("\n\n");
}

void summation(FILE *fp1, FILE *fp2, FILE *fp3)
{
	char buff1[4] = { 0 }, buff2[4] = { 0 };	//���� ����
	
	rewind(fp1);
	rewind(fp2);

	for (int i = 0; i < 6; i++) {
		fgets(buff1, sizeof(buff1), fp1);	
		fgets(buff2, sizeof(buff2), fp2);		//���� �� ����
		fprintf(fp3, "%-8d", atoi(buff1) + atoi(buff2));	//result.txt�� ���
		printf("%-8d", atoi(buff1) + atoi(buff2));			//ȭ�鿡 ���

		if (i % 3 == 2) {
			fprintf(fp3, "\n");
			printf("\n");
		}
	}
}
