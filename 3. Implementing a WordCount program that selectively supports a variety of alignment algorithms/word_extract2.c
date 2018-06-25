#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//	#define DEBUG		
#define MAX_LINE	200
#define	TRUE		1
#define	FALSE		0

int main(int argc, char * argv[])
{
	FILE *fp;
	char buffer[MAX_LINE];
	char seps[] = "[]'`_*?0123456789()\",.! -:;/\n";
	char *token;
	char *cp;
	int d_flag;
	int c_count, w_count, l_count;
	c_count = w_count = l_count = 0;

#ifdef DEBUG
		printf("argc = %d\n", argc);
#endif

	if ( argc != 3 ) {
		printf("�μ� ������ 3�� �ƴմϴ�.\n");
		printf("����: word_extract2 -d read_file_name\n");
		exit(1);
	}

	if ( *argv[1] != '-') {
		printf("�ɼ��� �Էµ��� �ʾ����ϴ�\n");
		exit(2);
	}

	cp = argv[1];

	switch (*(++cp)) {
	case 'd':
#ifdef DEBUG
		printf("d �ɼ��� �����Ǿ����ϴ�.\n");
#endif
		d_flag = TRUE;
		break;
	default:
		printf("������ �ɼ��� �������� �ʾ����ϴ�.\n");
		break;
	}

/*
#ifdef DEBUG
	system("ls");
#endif
*/

	if ((fp = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "���� %s�� �� �� �����ϴ�.\n", argv[2]);
		exit(2);
	}

	
	while (fgets(buffer, MAX_LINE, fp) != NULL) {
		l_count++;
		c_count += strlen(buffer);

#ifdef DEBUG
		printf("\n�Է� ����[%d] : %s\n",l_count, buffer);
		printf("\t��ū�� : ");
#endif

		token = strtok(buffer, seps);

		while (token != NULL) {
			printf("%s\n", token);
			token = strtok(NULL, seps);
			w_count++;
		}
#ifdef DEBUG
		printf("w_count = %d", w_count);
#endif
	}

#ifdef DEBUG
	printf("����[%s]���� ���� %d ��, �ܾ� %d ��, ���� %d ���� ���ԵǾ� �����ϴ�.\n", 
		argv[2], l_count, w_count, c_count);
#endif

	exit(0);

}
