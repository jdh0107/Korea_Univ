#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
	int occur = 0, wordcount = 0;
	char filename[30] = { 0 }, target[30] = { 0 }, buffer[30] = { 0 };
	FILE *fp;

	printf("Enter the file name and word: ");
	scanf("%s %s", filename, target);

	if ( (fp = fopen(filename, "r")) == NULL ) {	//�� ����
		printf("Can't open the file.");
	}
	else {											//����
		while (!feof(fp)) {							//���� ������
			fscanf(fp, "%s", buffer);
			if (!(strcmp(target, strtok(buffer, " \".,\n")))) { //Ÿ�� ã����
				occur++;							//occur 1 ����
			}
			wordcount++;
		}

		printf("The finding ratio : %d/%d", occur, wordcount - 1); //�������� crtl+z ������ �� �� �� �۵�
	}
	
	fclose(fp);
	return 0;
}
