//ahttps://blog.naver.com/shumin/220831049037

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>	

#define SWAP(x, y, t) ( (t)=(x), (x)=(y), (y)=(t) )
#define MAX_LINE	5000
#define MAX_WORD	100
#define	TRUE		1
#define	FALSE		0

typedef struct WordInfo
{
	char str[MAX_WORD];	// 단어
	int freq;			// 빈도 수
}Word;

Word word[MAX_LINE];	// 단어 저장 구조체
Word tempnode;			// 임시의 구조체 정보 저장소

void ShellSort();		// 쉘 정렬_1 (시나리오 1)
void InsertionSort2(struct WordInfo word[], int size, int step);	// 쉘 정렬_2 (시나리오 1)
void bubble_sort();		// 버블 정렬 (시나리오 2)
void quick_sort(Word *word, int low, int high);		// 퀵 정렬_1 (시나리오 3)
void s_swap(Word * a, Word * b);					// 퀵 정렬_2 (시나리오 3)

//void selection_sort();	// 선택 정렬
void Sort();			// test

int i, j, count=0;

int main(int argc, char * argv[])
{
	FILE *fp;
	char buffer[MAX_LINE];
	char seps[] = "[]'`_*?0123456789()\",.! -:;/\n\r\t";
	char *token;

#ifdef DEBUG
		printf("argc = %d\n", argc);
#endif

	if(argc > 4 || argc < 3 ||  *argv[1] != '-1' || *argv[1] != '-2' || *argv[1] != '-3') { 
		// 인수 개수가 3 또는 2아니면 오류
		printf("사용법 : wordcount -1 파일명\n");
		printf("사용법 : wordcount -2 파일명\n");
		printf("사용법 : wordcount -3 파일명 찾을문자열\n");
		exit(1);
	}

	if ( *argv[1] != '-') {
		printf("옵션이 입력되지 않았습니다\n");
		exit(2);
	}

	if ((fp = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "파일 %s를 열 수 없습니다.\n", argv[2]);
		exit(2);
	}


	// 구조체에 단어와 빈도수 저장
	while (fgets(buffer, MAX_LINE-1, fp) != NULL)	// 파일로부터 문자열을 입력받음
	{	
		token = strtok(buffer, seps);				// 문자열을 토큰별로 자르기 위해 strtok 함수 사용
													// strtok(자를 대상 문자열, 잘라내기 위한 토큰)
		while (token != NULL) 
		{
			for(i=0; i<count; i++)					// 단어의 개수만큼 반복 (count는 초기 전역변수 0으로 설정)
			{
				if(strcmp(token, word[i].str) == 0)	// 구조체에 이미 존재하는 단어일 경우
				{
					word[i].freq++;					// 그 단어의 빈도수 증가
					break;
				}
			}

			if(i==count)							// 존재하는 단어가 없을 경우
			{
				strcpy(word[count].str, token);		// 구조체에 새로운 단어 추가
				word[count].freq = 1;				// 해당 단어 빈도수 1로 설정
				count++;							// 전체 단어 개수 증가
			}
			//printf("%s\n", token);
			token = strtok(NULL, seps);				
		}
	}

/////////////////// 1번 시나리오
	if(strcmp(argv[1], "-1")==0)
	{
		int line=1;		// 라인 수를 세기 위한 변수
		clock_t time;	// 수행 시간 측정을 위한 선언
		time=clock();

		// 쉘소트 사용
		//Sort();
		ShellSort();	
		time = clock() -time;

		printf("<<Scenario 1>>\n");
		printf("       Word / Freq\n");

		for(i=0;i<count; i++, line++)
			printf("L:%4.d)  %s  %d\n", line, word[i].str, word[i].freq);

		printf ("\n-> %f seconds progress (used ShellSort)\n",((float)time)/CLOCKS_PER_SEC);
	}

/////////////////// 2번 시나리오
	if(strcmp(argv[1], "-2")==0)
	{
		int line=1;		// 라인 수를 세기 위한 변수
		clock_t time;	// 수행 시간 측정을 위한 선언
		time=clock();

		printf("<<Scenario 2>>\n");
		printf("      Freq / Word\n");

		// 버블소트 사용
		bubble_sort();
		time = clock() -time;

		for(i=0;i<50; i++, line++)
		{
			if(word[i].freq == 0)
				break;
			printf("L:%2.d)  %d  %s\n", line, word[i].freq, word[i].str);
		}

		printf ("\n-> %f seconds progress (used BubbleSort)\n",((float)time)/CLOCKS_PER_SEC);
	}

/////////////////// 3번 시나리오
	if(strcmp(argv[1], "-3")==0)
	{
		int line=1;		// 라인 수를 세기 위한 변수
		clock_t time;	// 수행 시간 측정을 위한 선언
		time=clock();

		printf("Scenario 3\n");
		printf("       Word / Freq\n");

		// 퀵 소트 사용
		quick_sort(word, 0, count-1);
		time = clock() -time;

		for(i=0;i<count; i++, line++)
			printf("L:%4.d)  %s  %d\n", line, word[i].str, word[i].freq);

		printf("\n <<Search results>>\n");
		for(i=0;i<count; i++)
		{
			if(strcmp(argv[3], word[i].str) == 0)
				printf(" %s  %d", word[i].str, word[i].freq);
		}
		printf ("\n-> %f seconds progress (used QuickSort)\n",((float)time)/CLOCKS_PER_SEC);
	}

	exit(0);
}


							///////////////////////////////함 수/////////////////////////////////
/*
strcmp(str1, str2) > 0)
str1이 더 크면 0보다 큰 값 반환(양수)
str2가 더 크면 0보다 작은 값 반환(음수)
내용이 같으면 0 반환
*/
// 버블 소트 함수
void bubble_sort()
{
	for(i=count-1; i>0; i--)
	{
		for(j=0; j<i; j++)
		{
			if(word[j].freq < word[j+1].freq)	// 구조체에 저장된 빈도수들을 비교
			{	
				tempnode = word[j];				// 임시로 구조체를 저장해놓기 위한 tempnode
				word[j] = word[j+1];			// 구조체 복사를 이용해 빈도수 기준으로 각 구조체들의 순서를
				word[j+1] = tempnode;			// 내림차순으로 재배치함
			}
		}
	}
}

/*
// 선택정렬 함수
void selection_sort()
{ 
	int min;
	for(i=0; i<count-1; i++) {
		min = (int)word[i].str;
		for(j=i+1; j<count; j++)	// 최솟값 탐색
		{
			if(word[j].str < word[min].str) 
			{
				min = (int)word[j].str;
				SWAP(word[i], word[min], tempnode);
			}
		}
	}
}
*/

// test sort
void Sort()
{
    int min;
 
    for(i=0; i<count-1; i++)
    {
        min = i;
        for(j= i+1; j<count; j++)
        {
			if(strcmp(word[min].str, word[j].str) > 0)
            {
                min = j;
            }
        }
		tempnode = word[i];
		word[i] = word[min];
		word[min] = tempnode;
    }
}

// 쉘 정렬 함수
void ShellSort()
{
    int step;

    for (step = count / 2; step>0; step /= 2)		// step의 폭을 1/2로 줄여감
    {
        for (i = 0; i<step; i++)					// step 간격에 있는 요소들을 삽입정렬
        {
            InsertionSort2(word, count - i, step);	
        }
    }
}

void InsertionSort2(struct WordInfo word[], int size, int step)
{ 
    for (i = step; i<size; i += step)				// 정렬 대상 원소는 step 간격으로 이동
    {
        for (j = i; j>0; j -= step)					// step 간격으로 앞으로 이동
        {
			if(strcmp(word[j - step].str, word[j].str) > 0)
            {
                SWAP(word[j - step], word[j], tempnode);	//교환
            }
            else
            {
                break;
            }
        }
    }
}

// 퀵 정렬에 사용되는 스왑 함수
void s_swap(Word * a, Word * b)
{
	Word temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
// 퀵 정렬 함수
void quick_sort(Word *word, int low, int high)
{
	int pivot;
	if(low < high)
	{
		pivot = low;
		i = low;
		j = high;
		while(i < j)
		{
			while(strcmp(word[i].str, word[pivot].str) <= 0 && i <= high)
			{
				i++;
			}
			while(strcmp(word[j].str, word[pivot].str) > 0 && j >= low)
			{
				j--;
			}
			if(i < j)
			{
				s_swap(&word[i], &word[j]);
			}
		}
		s_swap(&word[j], &word[pivot]);
		quick_sort(word, low, j - 1);
		quick_sort(word, j + 1, high);
	}
}