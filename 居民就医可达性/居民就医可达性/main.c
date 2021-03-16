# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MaxSize 1000
#define INF 10000000
//ҽԺ�ṹ����
typedef struct
{
	char sName[50];//ҽԺ����
	double x;//ҽԺ�ĺ�����
	double y;//ҽԺ��������
	int offerNum;//ҽԺ�����ṩ�����Ӧ��
	int offerNow;//ҽԺ��ǰ��Ӧ��
}Hospital;

//�����ṹ����
typedef struct
{
	char cName[50];//��������
	double x;//�����ĺ�����
	double y;//������������
	int needNum;//��������������
	int needNow;//������ǰ������
}Community;

/*�洢ҽԺ���ݺ�ҽԺ����*/
Hospital h[MaxSize];
int hn = 0;
/*ȫ�ֱ�������*/
/*�����洢�������ݺ���������*/
Community c[MaxSize];
int cn = 0;

double road[MaxSize][MaxSize];//�����洢ҽԺ�������ľ���
int v[MaxSize][MaxSize] = { 0 };//v[i][j] = 0��ʾ��(i,j)δ���ʣ�v[i][j] = 1��ʾҽԺi������j�ṩ����v[i][j] = 2��ʾ�Ѿ������޷��ṩ����

int needAll = 0;//������
double disAll = 0.0;//�ܾ���
//��ҽԺ���ݺ��������ݴ洢��ȫ�ֱ�����,ע��Ҫ�������ļ�����ANSI���뱣��
void dataIn(char * hFile, char * cFile)
{
	FILE * fH, *fC;
	int r1, r2;
	if ((fH = fopen(hFile, "r")) == NULL)
		printf("ҽԺ�ļ���ʧ�ܣ������ļ��Ƿ����");
	else
	{
		while ((r1 = fscanf(fH, "%s", h[hn].sName)) != EOF)
		{
			fscanf(fH, "%lf", &h[hn].x);
			fscanf(fH, "%lf", &h[hn].y);
			fscanf(fH, "%d", &h[hn].offerNum);
			h[hn].offerNow = h[hn].offerNum;
			++hn;
		}
		/*����ڱ����*/
		h[hn].offerNow = INF;
		h[hn].offerNum = INF;
		h[hn].sName[0] = 's';
		h[hn].sName[1] = '\0';
		++hn;
	}

	if ((fC = fopen(cFile, "r")) == NULL)
		printf("�����ļ���ʧ�ܣ������ļ��Ƿ����");
	else
	{
		while ((r2 = fscanf(fC, "%s", c[cn].cName)) != EOF)
		{
			fscanf(fC, "%lf", &c[cn].x);
			fscanf(fC, "%lf", &c[cn].y);
			fscanf(fC, "%d", &c[cn].needNum);
			c[cn].needNow = c[cn].needNum;
			++cn;
		}
	}
	fclose(fH);
	fclose(fC);
}

//����ҽԺ������ֱ�ӵľ��룬���������پ���ļ��㷽ʽ
double disCompute(double x1, double y1, double x2, double y2)
{
	return fabs(x1 - x2) + fabs(y1 - y2);
}

void comRoad()
{
	for (int i = 0; i < hn - 1; ++i)
		for (int j = 0; j < cn; ++j)
		{
			road[i][j] = disCompute(h[i].x, h[i].y, c[j].x, c[j].y);
		}
	for (int j = 0; j < cn; ++j)
	{
		road[hn - 1][j] = INF;
	}
}

//���㷨���ڷ��ؾ�������j�����ҽԺi���±�
int getClosetHos(int j)
{
	int r = 0;
	for (int i = 1; i < hn; ++i)
		if (road[i][j] < road[r][j])
			r = i;
	return r;
}

//���㷨���ڷ��ؾ���ҽԺi���������j���±�
int getClosetCom(int i)
{
	int r = 0;
	for (int j = 1; j < hn; ++i)
		if (road[i][j] < road[i][r])
			r = j;
	return r;
}

//�Բ�������15min��ҽ�ľ������ͳ�ƣ��������Ż������������ٶ�ȡ72m/min������ҽԺ����Ӧ��1080m��
void opt1()
{
	int ud[MaxSize] = { 0 };
	int m;
	for (int i = 0; i < hn - 1; ++i)
		for (int j = 0; j < cn; ++j)
		{
			if (v[i][j] != 0 && road[i][j] > 1080)
			{
				m = getClosetHos(j);
				disAll = disAll + (road[m][j] - road[i][j]) * v[i][j];
				ud[m] += v[i][j];
			}
		}
	printf("�Ż�����һ���£�\n");
	for (int i = 0; i < hn; ++i)
		if (ud[i] != 0)
			printf("ҽԺ%s���ӷ�����ʩ����%d\n", h[i].sName, ud[i]);
	printf("�Ż�����ܾ���:%lf, ������: %d, ƽ����ҽ�ɴ���:%lfmin\n", disAll, needAll, disAll / needAll / 72);
}

opt2()//�Ż�������������ҽԺ
{
	int ud[MaxSize] = { 0 };
	int m;
	for (int j = 0; j < cn; ++j)
		if (v[hn - 1][j] != 0)
		{
			m = getClosetHos(j);
			ud[m] += v[hn - 1][j];
			v[m][j] += v[hn - 1][j];
		}
	printf("��Ӧ�����㣡�Ż����������£�\n");
	for (int i = 0; i < hn - 1; ++i)
		if (ud[i] != 0)
			printf("ҽԺ%s���ӷ�����ʩ����%d\n", h[i].sName, ud[i]);

	//printf("�Ż�����ܾ���:%lf, ������: %d, ƽ����ҽ�ɴ���:%lfmin\n", disAll, needAll, disAll / needAll / 72);
	printf("�Ż���Ľ������:\n");
	FILE * fp = fopen("result2.txt", "w");
	fprintf(fp, "ҽԺ����        hX            hY          ��������    cX                CY            ����          ������\n");
	for (int i = 0; i < hn - 1; ++i)
	{
		printf("%s���ṩ�ķ���������£�\n", h[i].sName);
		for (int j = 0; j < cn; ++j)
		{
			if (v[i][j] != 0)
			{
				printf("����:%s, ������%d:, �����ľ���:%lf\n", c[j].cName, v[i][j], road[i][j]);
				fprintf(fp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", h[i].sName, h[i].x,
					h[i].y, c[j].cName, c[j].x, c[j].y, road[i][j], v[i][j]);
				disAll += road[i][j] * v[i][j];
				needAll += v[i][j];
			}
		}
	}
	printf("�Ż�����ܾ���:%lf, ������: %d, ƽ����ҽ�ɴ���:%lfmin\n", disAll, needAll, disAll / needAll / 72);
	fclose(fp);
}

void opt3()//�½�ҽԺ
{
	FILE * nfp = fopen("�½�ҽԺ��ѡ��.txt", "r");
	Hospital newHos[MaxSize];
	double newRoad[MaxSize][MaxSize];
	int nv[MaxSize][MaxSize] = { 0 };
	int nhn = 0;
	int r;
	if (nfp  == NULL)
		printf("ҽԺ�ļ���ʧ�ܣ������ļ��Ƿ����");
	else
	{
		while ((r = fscanf(nfp, "%s", newHos[nhn].sName)) != EOF)
		{
			fscanf(nfp, "%lf", &newHos[nhn].x);
			fscanf(nfp, "%lf", &newHos[nhn].y);
			fscanf(nfp, "%d", &newHos[nhn].offerNum);
			newHos[nhn].offerNow = newHos[nhn].offerNum;
			++nhn;
		}
	}
	fclose(nfp);
	for (int i = 0; i < nhn; ++i)
		for (int j = 0; j < cn; ++j)
		{
			newRoad[i][j] = disCompute(h[i].x, h[i].y, c[j].x, c[j].y);
		}

	int i, j, k, l;
	double min;
	while (1)
	{
		min = 10 * INF;
		for (i = 0; i < nhn; ++i)
		{
			if (newHos[i].offerNow != 0)
			{
				for (j = 0; j < cn; ++j)
				{
					if (v[hn - 1][j] != 0 && newRoad[i][j] < min)
					{
						min = newRoad[i][j];
						//nv[i][j] = v[hn - 1][j];
						k = i;
						l = j;
					}
				}
			}
		}
		if (min == 10 * INF)
			break;
		if (newHos[k].offerNow > v[hn - 1][l])
		{
			nv[k][l] = v[hn - 1][l];
			newHos[k].offerNow -= v[hn - 1][l];
		}
		else
		{
			nv[k][l] = newHos[k].offerNow;
			v[hn - 1][l] -= newHos[k].offerNow;
			newHos[k].offerNow = 0;
		}
	}

	FILE *nnfp = fopen("result3.txt", "w");
	fprintf(nnfp, "ҽԺ����        hX            hY          ��������    cX                CY            ����          ������\n");
	printf("�½���ҽԺ����:\n");
	for (int i = 0; i < nhn; ++i)
	{
		if (newHos[i].offerNow != newHos[i].offerNum)
		{
			printf("%s���ṩ�ķ���������£�\n", newHos[i].sName);
			for (int j = 0; j < cn; ++j)
				if (nv[i][j] != 0)
				{
					printf("����:%s, ������%d:, �����ľ���:%lf\n", c[j].cName, nv[i][j], newRoad[i][j]);
					fprintf(nnfp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", newHos[i].sName, newHos[i].x,
						newHos[i].y, c[j].cName, c[j].x, c[j].y, newRoad[i][j], nv[i][j]);
					disAll += newRoad[i][j] * nv[i][j];
					needAll += nv[i][j];
				}
		}
	}
	for (int i = 0; i < hn - 1; ++i)
	{
		printf("%s���ṩ�ķ���������£�\n", h[i].sName);
		for (int j = 0; j < cn; ++j)
		{
			if (v[i][j] != 0)
			{
				printf("����:%s, ������%d:, �����ľ���:%lf\n", c[j].cName, v[i][j], road[i][j]);
				fprintf(nnfp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", h[i].sName, h[i].x,
					h[i].y, c[j].cName, c[j].x, c[j].y, road[i][j], v[i][j]);
				disAll += road[i][j] * v[i][j];
				needAll += v[i][j];
			}
		}
	}
	printf("�ܾ���:%lf, ������: %d, ƽ����ҽ�ɴ���:%lfmin\n", disAll, needAll, disAll / needAll / 72);
	fclose(nnfp);
}

//�ɴ��Է���
void accessicilityAnalyse()
{

}
//ֱ�Ӷ�ԭʼģ�ͽ��������㷨�������д���ļ���
void solution1()
{
	int i, j, k, l;
	double min;
	int isS = 0;//�����ж��ڱ�����Ƿ��ҽԺ�ṩ����
	while (1)
	{
		min = 10 * INF;
		for (i = 0; i < hn; ++i)
		{
			if (h[i].offerNow != 0)
			{
				for (j = 0; j < cn; ++j)
				{
					if (c[j].needNow != 0 && v[i][j] == 0 && road[i][j] < min)
					{
						min = road[i][j];
						k = i;
						l = j;
					}
				}
			}
		}
		if (min == INF)
			isS = 1;
		if (min == 10 * INF)
			break;
		if (h[k].offerNow > c[l].needNow)
		{
			v[k][l] = c[l].needNow;
			h[k].offerNow -= c[l].needNow;
			c[l].needNow = 0;
		}
		else
		{
			v[k][l] = h[k].offerNow;
			c[l].needNow -= h[k].offerNow;
			h[k].offerNow = 0;
		}
	}
	
	
	if (isS == 0)//�ڱ����δ�ṩ����
	{
		FILE * fp = fopen("result1.txt", "w");
		fprintf(fp, "ҽԺ����        hX            hY          ��������    cX                CY            ����          ������\n");
		for (int i = 0; i < hn - 1; ++i)
		{
			printf("%s���ṩ�ķ���������£�\n", h[i].sName);
			for (int j = 0; j < cn; ++j)
			{
				if (v[i][j] != 0)
				{
					printf("����:%s, ������%d:, �����ľ���:%lf\n", c[j].cName, v[i][j], road[i][j]);
					fprintf(fp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", h[i].sName, h[i].x,
						h[i].y, c[j].cName, c[j].x, c[j].y, road[i][j], v[i][j]);
					disAll += road[i][j] * v[i][j];
					needAll += v[i][j];
				}
			}
		}
		printf("�ܾ���:%lf, ������: %d, ƽ����ҽ�ɴ���:%lfmin\n", disAll, needAll, disAll / needAll / 72);
		fclose(fp);
		opt1();
	}
	//�ڱ�����ṩ�˷���
	else
	{
		int op = 3;//ѡ���Ż�����
		if (op == 2)
			opt2();//�Ż�������������ҽԺ//�½�ҽԺ
		else
			opt3(); 
	}
	
}


int main()
{
	dataIn("ҽԺ.txt", "����.txt");//����¼��
	comRoad();//���������پ���
	solution1();
	while (1);
	return 0;
}