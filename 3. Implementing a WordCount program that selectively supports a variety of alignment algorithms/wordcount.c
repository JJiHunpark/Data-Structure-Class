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
	char str[MAX_WORD];	// �ܾ�
	int freq;			// �� ��
}Word;

Word word[MAX_LINE];	// �ܾ� ���� ����ü
Word tempnode;			// �ӽ��� ����ü ���� �����

void ShellSort();		// �� ����_1 (�ó����� 1)
void InsertionSort2(struct WordInfo word[], int size, int step);	// �� ����_2 (�ó����� 1)
void bubble_sort();		// ���� ���� (�ó����� 2)
void quick_sort(Word *word, int low, int high);		// �� ����_1 (�ó����� 3)
void s_swap(Word * a, Word * b);					// �� ����_2 (�ó����� 3)

//void selection_sort();	// ���� ����
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
		// �μ� ������ 3 �Ǵ� 2�ƴϸ� ����
		printf("���� : wordcount -1 ���ϸ�\n");
		printf("���� : wordcount -2 ���ϸ�\n");
		printf("���� : wordcount -3 ���ϸ� ã�����ڿ�\n");
		exit(1);
	}

	if ( *argv[1] != '-') {
		printf("�ɼ��� �Էµ��� �ʾҽ��ϴ�\n");
		exit(2);
	}

	if ((fp = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "���� %s�� �� �� �����ϴ�.\n", argv[2]);
		exit(2);
	}


	// ����ü�� �ܾ�� �󵵼� ����
	while (fgets(buffer, MAX_LINE-1, fp) != NULL)	// ���Ϸκ��� ���ڿ��� �Է¹���
	{	
		token = strtok(buffer, seps);				// ���ڿ��� ��ū���� �ڸ��� ���� strtok �Լ� ���
													// strtok(�ڸ� ��� ���ڿ�, �߶󳻱� ���� ��ū)
		while (token != NULL) 
		{
			for(i=0; i<count; i++)					// �ܾ��� ������ŭ �ݺ� (count�� �ʱ� �������� 0���� ����)
			{
				if(strcmp(token, word[i].str) == 0)	// ����ü�� �̹� �����ϴ� �ܾ��� ���
				{
					word[i].freq++;					// �� �ܾ��� �󵵼� ����
					break;
				}
			}

			if(i==count)							// �����ϴ� �ܾ ���� ���
			{
				strcpy(word[count].str, token);		// ����ü�� ���ο� �ܾ� �߰�
				word[count].freq = 1;				// �ش� �ܾ� �󵵼� 1�� ����
				count++;							// ��ü �ܾ� ���� ����
			}
			//printf("%s\n", token);
			token = strtok(NULL, seps);				
		}
	}

/////////////////// 1�� �ó�����
	if(strcmp(argv[1], "-1")==0)
	{
		int line=1;		// ���� ���� ���� ���� ����
		clock_t time;	// ���� �ð� ������ ���� ����
		time=clock();

		// ����Ʈ ���
		//Sort();
		ShellSort();	
		time = clock() -time;

		printf("<<Scenario 1>>\n");
		printf("       Word / Freq\n");

		for(i=0;i<count; i++, line++)
			printf("L:%4.d)  %s  %d\n", line, word[i].str, word[i].freq);

		printf ("\n-> %f seconds progress (used ShellSort)\n",((float)time)/CLOCKS_PER_SEC);
	}

/////////////////// 2�� �ó�����
	if(strcmp(argv[1], "-2")==0)
	{
		int line=1;		// ���� ���� ���� ���� ����
		clock_t time;	// ���� �ð� ������ ���� ����
		time=clock();

		printf("<<Scenario 2>>\n");
		printf("      Freq / Word\n");

		// �����Ʈ ���
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

/////////////////// 3�� �ó�����
	if(strcmp(argv[1], "-3")==0)
	{
		int line=1;		// ���� ���� ���� ���� ����
		clock_t time;	// ���� �ð� ������ ���� ����
		time=clock();

		printf("Scenario 3\n");
		printf("       Word / Freq\n");

		// �� ��Ʈ ���
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


							///////////////////////////////�� ��/////////////////////////////////
/*
strcmp(str1, str2) > 0)
str1�� �� ũ�� 0���� ū �� ��ȯ(���)
str2�� �� ũ�� 0���� ���� �� ��ȯ(����)
������ ������ 0 ��ȯ
*/
// ���� ��Ʈ �Լ�
void bubble_sort()
{
	for(i=count-1; i>0; i--)
	{
		for(j=0; j<i; j++)
		{
			if(word[j].freq < word[j+1].freq)	// ����ü�� ����� �󵵼����� ��
			{	
				tempnode = word[j];				// �ӽ÷� ����ü�� �����س��� ���� tempnode
				word[j] = word[j+1];			// ����ü ���縦 �̿��� �󵵼� �������� �� ����ü���� ������
				word[j+1] = tempnode;			// ������������ ���ġ��
			}
		}
	}
}

/*
// �������� �Լ�
void selection_sort()
{ 
	int min;
	for(i=0; i<count-1; i++) {
		min = (int)word[i].str;
		for(j=i+1; j<count; j++)	// �ּڰ� Ž��
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

// �� ���� �Լ�
void ShellSort()
{
    int step;

    for (step = count / 2; step>0; step /= 2)		// step�� ���� 1/2�� �ٿ���
    {
        for (i = 0; i<step; i++)					// step ���ݿ� �ִ� ��ҵ��� ��������
        {
            InsertionSort2(word, count - i, step);	
        }
    }
}

void InsertionSort2(struct WordInfo word[], int size, int step)
{ 
    for (i = step; i<size; i += step)				// ���� ��� ���Ҵ� step �������� �̵�
    {
        for (j = i; j>0; j -= step)					// step �������� ������ �̵�
        {
			if(strcmp(word[j - step].str, word[j].str) > 0)
            {
                SWAP(word[j - step], word[j], tempnode);	//��ȯ
            }
            else
            {
                break;
            }
        }
    }
}

// �� ���Ŀ� ���Ǵ� ���� �Լ�
void s_swap(Word * a, Word * b)
{
	Word temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
// �� ���� �Լ�
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