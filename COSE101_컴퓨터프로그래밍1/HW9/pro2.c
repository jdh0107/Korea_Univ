#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{	//����ü
	char name[10];
	char student_number[9];
	int grade;
	char major[20];
	int count = 0;
	char search_number[9];

	FILE *fPtr;
	char buffer[20];

	if((fPtr = fopen("student_file.txt", "w")) == NULL) {
		printf("Can't open that file\n");
	}
	else {
		while (count < 5) {
			printf("Name Student_number Grade Major: ");
			scanf("%s %s %d %s", name, student_number, &grade, major);
			fprintf(fPtr, "%s %s %d %s\n", name, student_number, grade, major);
			count++;
		}
	}
	fclose(fPtr);

	printf("Insert the student number to find: ");
	scanf("%s", search_number);

	//�ٽÿ��� �� ������ �޾Ƽ� ã���� ���
	if ((fPtr = fopen("student_file.txt", "r")) == NULL) {
		printf("Can't read that file\n");
	}
	else { //���ڿ� ��
		do {
			fscanf(fPtr, "%s", buffer);
			if (!strcmp(buffer, search_number)) { //ã�� ������ fPtr�� ������ ��������
				break;
			}
		} while (1);

		fscanf(fPtr, "%s", buffer);
		fscanf(fPtr, "%s", buffer);
		printf("The major of student with ID %s is %s", search_number, buffer);
	}
	
	fclose(fPtr);
	return 0;
}
