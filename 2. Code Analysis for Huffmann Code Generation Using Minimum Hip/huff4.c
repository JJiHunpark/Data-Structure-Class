#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 256			// ASCII �ڵ� ǥ�� Dec ��
#define MAX_STR 300		// ���ڿ��� �ִ� ����
#define bi_max_code 14	//������ Ʈ���� �����Ͽ� �ڵ带 ���غ��� �� ���̰� �뷫 14�� ���� �ʴ´�.

int freq[MAX];			// �󵵼� ���� �迭

typedef struct sortnode	
{
	int alpa;	// ���ĺ� (ASCII)
	int freq;	// �󵵼�
	int len;	// �ڵ��� ����
	int scode[bi_max_code];
} sortnode;

struct sortnode s_node[MAX];

typedef struct _huf
{
	int count;  // ��
	char data;  // ����
	struct _huf *left,*right;
} huf;

huf *head[MAX];
int nhead;
huf *huf_head;
unsigned code[256];
int len[256];

char sen[MAX_STR];   // �Է¹��� ���ڿ� ����


// ���ں� �󵵸� ���ؼ� freq �迭�� �����ϴ� �Լ�
void get_freq(void)
{
	int i;
	for(i=0;i<MAX;i++)				// ASCII �ڵ忡 �ִ� ���ڿ��� ����� ���ڿ��� ���ϱ� ����
								// MAX�� ũ��(256: ASCII �ڵ��� Dec ��) ��ŭ �ݺ�
	freq[i]=0;					// ���ڿ� �󵵼��� �����ϴ� freq �迭 �ʱ�ȭ

	for(i=0;i<strlen(sen);i++)     // ���� �Է��� ���� ����� ���ڵ��� �� ��ŭ �ݺ�
	if(sen[i]!=' ')				// ����� ���� �� ���⸦ ����
	freq[sen[i]]++;				// �󵵼��� �����ϴ� freq �迭�� �� ���ĺ��� ���󵵸� ����
}

// �ּ� �󵵼��� ã�� �Լ� 
int find_minimum(void)
{
	int mindex,i;
	mindex=0;
	for(i=1;i<nhead;i++)
	{
		if(head[i]->count < head[mindex]->count)
		mindex=i;
	}
	return mindex;   // struct���� �ִ� �󵵸� ���� �ּ��� �󵵸� ã�´�.
}

// freq �迭�� ������ Ʈ���� �����ϴ� �Լ�
void construct_tree(void)
{
	int i,m;
	huf *h,*h1,*h2;

	// �ʱ� �ܰ��� ��带 ����
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

 //������ ������ ������ Ʈ���� ����� �ܰ�
	 while(nhead>1)
	 {
		m=find_minimum();  // �� �ּҰ����� Ʈ���� �������� �����Ѵ�.
		h1=head[m];
		head[m]=head[--nhead];
		m=find_minimum();
		h2=head[m];
		if((h=(huf*)malloc(sizeof(huf))) == NULL)
		{
			 printf("\nError : out of memory.");
			exit(1);
		}
	  /* ������ ���� �ΰ��� �ּҺ󵵸� ���� ��带 ���� �ΰ��� ���� ���� ���� ��带 �����Ͽ� �� ����� ��, �쿡 �ּҺ󵵸� ���� ��带 �ٿ� Ʈ���� �����.*/
		h->count=h1->count+h2->count;
		h->data=0;
		h->left=h1;
		h->right=h2;
		head[m]=h;
	}
	huf_head=head[0];
}

/* ������ Ʈ�� ���� */
void destruct_tree(huf *h)
{
	if(h!=NULL)
	{
		destruct_tree(h->left);
		destruct_tree(h->right);
		free(h);
	}
}

/* �־��� ���ڿ��� ���󵵸� ȭ�鿡 ����ϴ� �Լ� */
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

// ������Ʈ������ �ڵ带 ��. code�迭�� len�迭 ����
void _make_code(huf *h,unsigned c, int l)
{
	if(h->left != NULL || h->right !=NULL)		// ������ Ʈ���� ����� �ڽ��� �ִ� ���
	{
		c <<= 1;								// ��Ʈ�� �������� 1�� �̵� �� �ٽ� �Ҵ�
		l++;
		// ���ȣ���� ����Ͽ� �� ������ �ڵ尪�� �ڵ��� ���̸� 10������ ���·� ����
		_make_code(h->left,c,l);				// ���� �ڽ��� ���
		c|=1u;									
		_make_code(h->right,c,l);				// ������ �ڽ��� ���
		c >>= 1;								// ��Ʈ�� ���������� 1�� �̵� �� �ٽ� �Ҵ� 
		l--;
	}
	else										// ������ Ʈ���� ����� �ڽ��� ���� ���
	{
		code[h->data]=c;						// ��Ʈ ���� 10���� ���·� �����ϴ� �迭��, ����� �ڵ� c ����
		len[h->data]=l;							// ������ �ڵ��� ���̸� �����ϴ� len �迭�� ���� �ڵ��� ���� ���� 
	}
}

// _make_code()�� ����ϱ� ���� �Լ�
void make_code(void)
{
	int i;
	for(i=0;i<256;i++)			// ����� ������ �迭 code �� len �� ũ�⸸ŭ �ݺ�
	code[i]=len[i]=0;			// 10�������·� �ڵ带 ������ code �迭�� �ش� �ڵ� ���̸� ������ len �迭 �ʱ�ȭ
	_make_code(huf_head,0u,0);	// _make_code �Լ��� huf_head �����Ϳ� unsigned int 0�� 0�� �Ű������� �־��ش�
}




/* �ڵ��� ���� �� ���� �� ���̸� ã�� �Լ� */
int code_leng(void)
{
	int i,max=0;
	for(i=0 ; i<MAX ; i++)
		if(max<len[i])
			max=len[i];
	return max;
}
// ����
void range()
{
	int i, j, temp, bi_code[bi_max_code];
	struct sortnode tempnode = {0,0,0,0};

	for(i=0 ; i<MAX ; i++)					// ��� MAX(256) ��ŭ �ݺ�
	{
		if(len[i]!=0)						// len(256) �迭�� ���� 0�� �ƴ� ���  
		{									// (������ �ڵ��� ���̰� ����Ǿ����� �� ����)
			temp=code[i];					// �ڵ带 10������ ������ code �迭�� ������ 2������ Ǯ�� ���� temp ���� �ӽ� ����
			for(j=1 ; j<=len[i]; j++)		// �ڵ��� ���̸�ŭ �ݺ�
			{
				bi_code[len[i]-j]=temp%2;	// 10������ ����� �ڵ��� ���� 0�� 1�� ����� 2������ ��ȯ�ϸ�
				temp/=2;					// �ڵ��� ���̸�ŭ �迭�� ����
			}
			// ����ü�� �̿��Ͽ� �󵵼��������� ������ �ϱ����� ����ü�� �󵵼� / ���ĺ� / ������ ������ �ڵ� ����
			s_node[i].alpa = i;						// ���ĺ� ����
			s_node[i].freq = freq[i];				// �󵵼� ����
			for(j=0 ; j<MAX;j++)
				s_node[i].len=len[i];				// �ڵ��� ���� ���� (����� �ڵ带 ����ϱ� ���� �ʿ�)
			for(j=0 ; j<len[i];j++)
				s_node[i].scode[j] = bi_code[j];	// �������ڵ� ����
		}
	}
	// ���������� ����� �󵵼� ���� �������� ����
	for(i=MAX-1; i>0; i--){
		for(j=0; j<i; j++)
			if(s_node[j].freq < s_node[j+1].freq){	// ����ü�� ����� �󵵼����� ��
				tempnode = s_node[j];				// �ӽ÷� ����ü�� �����س��� ���� tempnode
				s_node[j] = s_node[j+1];			// ����ü ���縦 �̿��� �󵵼� �������� �� ����ü���� ������
				s_node[j+1] = tempnode;				// ������������ ���ġ��
			}
	}
	// ���������� ����� �ڵ� ���� ���� �������� ����
	for(i=MAX-1; i>0; i--){
		for(j=0; j<i; j++)
			if(s_node[j].len > s_node[j+1].len){	// ����ü�� ����� �ڵ��� ���̵��� ��
				tempnode = s_node[j];				// �ӽ÷� ����ü�� �����س��� ���� tempnode
				s_node[j] = s_node[j+1];			// ����ü ���縦 �̿��� �󵵼� �������� �� ����ü���� ������
				s_node[j+1] = tempnode;				// ������������ ���ġ��
			}
	}
}
/* ������ �����Լ� */
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
	for(i=0 ; i<MAX ; i++)							// MAX(256) ��ŭ �ݺ�
	{
		if(s_node[i].freq != 0)						// �󵵼��� 0�� ��� ����
		{	// �� ����ü�� ������� ���
			printf("  weight=%2.d  ", s_node[i].freq);		// �󵵼� ���
			printf("depth=%2.d  ", s_node[i].len);			// �ڵ� ���� ���
			printf("char = %c  ",s_node[i].alpa);			// ���ĺ� ���
			printf("code= ");
			for(j=0 ; j<s_node[i].len;j++)			// �ڵ��� ���̸�ŭ �ݺ�
				printf("%d",s_node[i].scode[j]);	// ����� �ڵ� ���
			printf("\n");
		}
	}
	printf("\n\n");
	destruct_tree(huf_head);						// ������ Ʈ�� ���� (�޸� ����)
}

/* ���� �Լ� */
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

