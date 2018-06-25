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
		printf("인수 갯수가 3이 아닙니다.\n");
		printf("사용법: word_extract2 -d read_file_name\n");
		exit(1);
	}

	if ( *argv[1] != '-') {
		printf("옵션이 입력되지 않았읍니다\n");
		exit(2);
	}

	cp = argv[1];

	switch (*(++cp)) {
	case 'd':
#ifdef DEBUG
		printf("d 옵션이 설정되었읍니다.\n");
#endif
		d_flag = TRUE;
		break;
	default:
		printf("적합한 옵션이 설정되지 않았읍니다.\n");
		break;
	}

/*
#ifdef DEBUG
	system("ls");
#endif
*/

	if ((fp = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "파일 %s를 열 수 없습니다.\n", argv[2]);
		exit(2);
	}

	
	while (fgets(buffer, MAX_LINE, fp) != NULL) {
		l_count++;
		c_count += strlen(buffer);

#ifdef DEBUG
		printf("\n입력 라인[%d] : %s\n",l_count, buffer);
		printf("\t토큰들 : ");
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
	printf("파일[%s]에는 라인 %d 줄, 단어 %d 개, 글자 %d 개가 포함되어 있읍니다.\n", 
		argv[2], l_count, w_count, c_count);
#endif

	exit(0);

}
