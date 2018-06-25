#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 256			// ASCII 코드 표의 Dec 수
#define MAX_STR 300		// 문자열의 최대 길이
#define bi_max_code 14	//허프만 트리를 구성하여 코드를 구해보면 그 길이가 대략 14를 넘지 않는다.

int freq[MAX];			// 빈도수 저장 배열

typedef struct sortnode	
{
	int alpa;	// 알파벳 (ASCII)
	int freq;	// 빈도수
	int len;	// 코드의 길이
	int scode[bi_max_code];
} sortnode;

struct sortnode s_node[MAX];

typedef struct _huf
{
	int count;  // 빈도
	char data;  // 문자
	struct _huf *left,*right;
} huf;

huf *head[MAX];
int nhead;
huf *huf_head;
unsigned code[256];
int len[256];

char sen[MAX_STR];   // 입력받을 문자열 선언


// 문자별 빈도를 구해서 freq 배열에 저장하는 함수
void get_freq(void)
{
	int i;
	for(i=0;i<MAX;i++)				// ASCII 코드에 있는 문자열과 저장된 문자열을 비교하기 위해
								// MAX의 크기(256: ASCII 코드의 Dec 수) 만큼 반복
	freq[i]=0;					// 문자열 빈도수를 저장하는 freq 배열 초기화

	for(i=0;i<strlen(sen);i++)     // 파일 입력을 통해 저장된 문자들의 수 만큼 반복
	if(sen[i]!=' ')				// 저장된 문자 중 띄어쓰기를 제외
	freq[sen[i]]++;				// 빈도수를 저장하는 freq 배열에 각 알파벳의 사용빈도를 저장
}

// 최소 빈도수를 찾는 함수 
int find_minimum(void)
{
	int mindex,i;
	mindex=0;
	for(i=1;i<nhead;i++)
	{
		if(head[i]->count < head[mindex]->count)
		mindex=i;
	}
	return mindex;   // struct내에 있는 빈도를 갖고 최소의 빈도를 찾는다.
}

// freq 배열로 허프만 트리를 구성하는 함수
void construct_tree(void)
{
	int i,m;
	huf *h,*h1,*h2;

	// 초기 단계의 노드를 생성
	for(i=nhead=0;i<MAX;i++)
	{
		if(freq[i]!=0)
		{
			if((h=(huf *)malloc(sizeof(huf))) == NULL)
			{
				printf("\nError : out of memory.");
				exit(1);
			}
			h->count=freq[i];
			h->data=i;
			h->left=h->right=NULL;
			head[nhead++]=h;
		}
	}

 //생성된 노드들을 허프만 트리로 만드는 단계
	 while(nhead>1)
	 {
		m=find_minimum();  // 이 최소값으로 트리를 차례차례 구성한다.
		h1=head[m];
		head[m]=head[--nhead];
		m=find_minimum();
		h2=head[m];
		if((h=(huf*)malloc(sizeof(huf))) == NULL)
		{
			 printf("\nError : out of memory.");
			exit(1);
		}
	  /* 위에서 구한 두개의 최소빈도를 갖는 노드를 갖고 두개의 빈도의 합을 갖는 노드를 생성하여 이 노드의 좌, 우에 최소빈도를 갖는 노드를 붙여 트리를 만든다.*/
		h->count=h1->count+h2->count;
		h->data=0;
		h->left=h1;
		h->right=h2;
		head[m]=h;
	}
	huf_head=head[0];
}

/* 허프만 트리 제거 */
void destruct_tree(huf *h)
{
	if(h!=NULL)
	{
		destruct_tree(h->left);
		destruct_tree(h->right);
		free(h);
	}
}

/* 주어진 문자열의 사용빈도를 화면에 출력하는 함수 */
void show_freq(void)
{
	int i;
	for(i=0 ; i<MAX ; i++)
	{
	if(freq[i]==0)
		continue;
	printf("\t %c \t %d\n",i,freq[i]);
	}
}

// 허프만트리에서 코드를 얻어냄. code배열과 len배열 설정
void _make_code(huf *h,unsigned c, int l)
{
	if(h->left != NULL || h->right !=NULL)		// 허프만 트리의 헤더가 자식이 있는 경우
	{
		c <<= 1;								// 비트를 왼쪽으로 1번 이동 후 다시 할당
		l++;
		// 재귀호출을 사용하여 각 노드들의 코드값과 코드의 길이를 10진수의 형태로 저장
		_make_code(h->left,c,l);				// 왼쪽 자식일 경우
		c|=1u;									
		_make_code(h->right,c,l);				// 오른쪽 자식일 경우
		c >>= 1;								// 비트를 오른쪽으로 1번 이동 후 다시 할당 
		l--;
	}
	else										// 허프만 트리의 헤더가 자식이 없는 경우
	{
		code[h->data]=c;						// 코트 값을 10진수 형태로 저장하는 배열에, 연산된 코드 c 저장
		len[h->data]=l;							// 허프만 코드의 길이를 저장하는 len 배열에 계산된 코드의 길이 저장 
	}
}

// _make_code()를 사용하기 위한 함수
void make_code(void)
{
	int i;
	for(i=0;i<256;i++)			// 상수로 설정한 배열 code 와 len 의 크기만큼 반복
	code[i]=len[i]=0;			// 10진수형태로 코드를 저장할 code 배열과 해당 코드 길이를 저장할 len 배열 초기화
	_make_code(huf_head,0u,0);	// _make_code 함수에 huf_head 포인터와 unsigned int 0과 0을 매개변수로 넣어준다
}




/* 코드의 길이 중 가장 긴 길이를 찾는 함수 */
int code_leng(void)
{
	int i,max=0;
	for(i=0 ; i<MAX ; i++)
		if(max<len[i])
			max=len[i];
	return max;
}
// 정렬
void range()
{
	int i, j, temp, bi_code[bi_max_code];
	struct sortnode tempnode = {0,0,0,0};

	for(i=0 ; i<MAX ; i++)					// 상수 MAX(256) 만큼 반복
	{
		if(len[i]!=0)						// len(256) 배열의 값이 0이 아닐 경우  
		{									// (허프만 코드의 길이가 저장되어있을 시 실행)
			temp=code[i];					// 코드를 10진수로 저장한 code 배열의 값들을 2진수로 풀기 위해 temp 값에 임시 저장
			for(j=1 ; j<=len[i]; j++)		// 코드의 길이만큼 반복
			{
				bi_code[len[i]-j]=temp%2;	// 10진수로 저장된 코드의 값을 0과 1을 사용한 2진수로 변환하며
				temp/=2;					// 코드의 길이만큼 배열에 저장
			}
			// 구조체를 이용하여 빈도수기준으로 정렬을 하기위해 구조체에 빈도수 / 알파벳 / 생성된 허프만 코드 저장
			s_node[i].alpa = i;						// 알파벳 저장
			s_node[i].freq = freq[i];				// 빈도수 저장
			for(j=0 ; j<MAX;j++)
				s_node[i].len=len[i];				// 코드의 길이 저장 (저장된 코드를 출력하기 위해 필요)
			for(j=0 ; j<len[i];j++)
				s_node[i].scode[j] = bi_code[j];	// 허프만코드 저장
		}
	}
	// 버블정렬을 사용한 빈도수 기준 내림차순 정렬
	for(i=MAX-1; i>0; i--){
		for(j=0; j<i; j++)
			if(s_node[j].freq < s_node[j+1].freq){	// 구조체에 저장된 빈도수들을 비교
				tempnode = s_node[j];				// 임시로 구조체를 저장해놓기 위한 tempnode
				s_node[j] = s_node[j+1];			// 구조체 복사를 이용해 빈도수 기준으로 각 구조체들의 순서를
				s_node[j+1] = tempnode;				// 내림차순으로 재배치함
			}
	}
	// 버블정렬을 사용한 코드 길이 기준 내림차순 정렬
	for(i=MAX-1; i>0; i--){
		for(j=0; j<i; j++)
			if(s_node[j].len > s_node[j+1].len){	// 구조체에 저장된 코드의 길이들을 비교
				tempnode = s_node[j];				// 임시로 구조체를 저장해놓기 위한 tempnode
				s_node[j] = s_node[j+1];			// 구조체 복사를 이용해 빈도수 기준으로 각 구조체들의 순서를
				s_node[j+1] = tempnode;				// 내림차순으로 재배치함
			}
	}
}
/* 허프만 압축함수 */
void huffman_comp()
{
	char ch;
	int i,j,bi_code[bi_max_code],temp,max;


	get_freq();
	construct_tree();
	make_code();
	max=code_leng();
	range();

	printf("\n");
	for(i=0 ; i<MAX ; i++)							// MAX(256) 만큼 반복
	{
		if(s_node[i].freq != 0)						// 빈도수가 0인 경우 제외
		{	// 각 구조체의 멤버들을 출력
			printf("  weight=%2.d  ", s_node[i].freq);		// 빈도수 출력
			printf("depth=%2.d  ", s_node[i].len);			// 코드 길이 출력
			printf("char = %c  ",s_node[i].alpa);			// 알파벳 출력
			printf("code= ");
			for(j=0 ; j<s_node[i].len;j++)			// 코드의 길이만큼 반복
				printf("%d",s_node[i].scode[j]);	// 저장된 코드 출력
			printf("\n");
		}
	}
	printf("\n\n");
	destruct_tree(huf_head);						// 허프만 트리 제거 (메모리 해제)
}

/* 메인 함수 */
void main(void)
{
	FILE* text;
	int input = 0;
	int i=0;

	text = fopen("word_list.txt", "r");
	if(text == NULL)
		puts("File open erorr");

	while(input != EOF){
		input = fgetc(text);
		if((input>=65 && input<=90) || (input>=97 && input<=122))
			sen[i++] = input;
	}

	huffman_comp();
}

