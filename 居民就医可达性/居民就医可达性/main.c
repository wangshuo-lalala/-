# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MaxSize 1000
#define INF 10000000
//医院结构定义
typedef struct
{
	char sName[50];//医院名称
	double x;//医院的横坐标
	double y;//医院的纵坐标
	int offerNum;//医院所能提供的最大供应量
	int offerNow;//医院当前供应量
}Hospital;

//社区结构定义
typedef struct
{
	char cName[50];//社区名称
	double x;//社区的横坐标
	double y;//社区的纵坐标
	int needNum;//社区的总需求量
	int needNow;//社区当前需求量
}Community;

/*存储医院数据和医院数量*/
Hospital h[MaxSize];
int hn = 0;
/*全局变量定义*/
/*用来存储社区数据和社区总量*/
Community c[MaxSize];
int cn = 0;

double road[MaxSize][MaxSize];//用来存储医院和社区的距离
int v[MaxSize][MaxSize] = { 0 };//v[i][j] = 0表示点(i,j)未访问，v[i][j] = 1表示医院i对社区j提供服务，v[i][j] = 2表示已经访问无法提供服务

int needAll = 0;//总需求
double disAll = 0.0;//总距离
//将医院数据和社区数据存储到全局变量中,注意要将数据文件采用ANSI编码保存
void dataIn(char * hFile, char * cFile)
{
	FILE * fH, *fC;
	int r1, r2;
	if ((fH = fopen(hFile, "r")) == NULL)
		printf("医院文件打开失败，请检查文件是否存在");
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
		/*添加哨兵结点*/
		h[hn].offerNow = INF;
		h[hn].offerNum = INF;
		h[hn].sName[0] = 's';
		h[hn].sName[1] = '\0';
		++hn;
	}

	if ((fC = fopen(cFile, "r")) == NULL)
		printf("社区文件打开失败，请检查文件是否存在");
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

//计算医院和社区直接的距离，采用曼哈顿距离的计算方式
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

//该算法用于返回距离社区j最近的医院i的下标
int getClosetHos(int j)
{
	int r = 0;
	for (int i = 1; i < hn; ++i)
		if (road[i][j] < road[r][j])
			r = i;
	return r;
}

//该算法用于返回距离医院i最近的社区j的下标
int getClosetCom(int i)
{
	int r = 0;
	for (int j = 1; j < hn; ++i)
		if (road[i][j] < road[i][r])
			r = j;
	return r;
}

//对不能满足15min就医的居民进行统计，并给出优化方案，步行速度取72m/min，及到医院距离应在1080m内
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
	printf("优化方案一如下：\n");
	for (int i = 0; i < hn; ++i)
		if (ud[i] != 0)
			printf("医院%s增加服务设施数量%d\n", h[i].sName, ud[i]);
	printf("优化后的总距离:%lf, 总需求: %d, 平均就医可达性:%lfmin\n", disAll, needAll, disAll / needAll / 72);
}

opt2()//优化方案二，扩建医院
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
	printf("供应量不足！优化方案二如下：\n");
	for (int i = 0; i < hn - 1; ++i)
		if (ud[i] != 0)
			printf("医院%s增加服务设施数量%d\n", h[i].sName, ud[i]);

	//printf("优化后的总距离:%lf, 总需求: %d, 平均就医可达性:%lfmin\n", disAll, needAll, disAll / needAll / 72);
	printf("优化后的结果如下:\n");
	FILE * fp = fopen("result2.txt", "w");
	fprintf(fp, "医院名称        hX            hY          社区名称    cX                CY            距离          服务量\n");
	for (int i = 0; i < hn - 1; ++i)
	{
		printf("%s所提供的服务情况如下：\n", h[i].sName);
		for (int j = 0; j < cn; ++j)
		{
			if (v[i][j] != 0)
			{
				printf("社区:%s, 服务量%d:, 单个的距离:%lf\n", c[j].cName, v[i][j], road[i][j]);
				fprintf(fp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", h[i].sName, h[i].x,
					h[i].y, c[j].cName, c[j].x, c[j].y, road[i][j], v[i][j]);
				disAll += road[i][j] * v[i][j];
				needAll += v[i][j];
			}
		}
	}
	printf("优化后的总距离:%lf, 总需求: %d, 平均就医可达性:%lfmin\n", disAll, needAll, disAll / needAll / 72);
	fclose(fp);
}

void opt3()//新建医院
{
	FILE * nfp = fopen("新建医院候选点.txt", "r");
	Hospital newHos[MaxSize];
	double newRoad[MaxSize][MaxSize];
	int nv[MaxSize][MaxSize] = { 0 };
	int nhn = 0;
	int r;
	if (nfp  == NULL)
		printf("医院文件打开失败，请检查文件是否存在");
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
	fprintf(nnfp, "医院名称        hX            hY          社区名称    cX                CY            距离          服务量\n");
	printf("新建的医院如下:\n");
	for (int i = 0; i < nhn; ++i)
	{
		if (newHos[i].offerNow != newHos[i].offerNum)
		{
			printf("%s所提供的服务情况如下：\n", newHos[i].sName);
			for (int j = 0; j < cn; ++j)
				if (nv[i][j] != 0)
				{
					printf("社区:%s, 服务量%d:, 单个的距离:%lf\n", c[j].cName, nv[i][j], newRoad[i][j]);
					fprintf(nnfp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", newHos[i].sName, newHos[i].x,
						newHos[i].y, c[j].cName, c[j].x, c[j].y, newRoad[i][j], nv[i][j]);
					disAll += newRoad[i][j] * nv[i][j];
					needAll += nv[i][j];
				}
		}
	}
	for (int i = 0; i < hn - 1; ++i)
	{
		printf("%s所提供的服务情况如下：\n", h[i].sName);
		for (int j = 0; j < cn; ++j)
		{
			if (v[i][j] != 0)
			{
				printf("社区:%s, 服务量%d:, 单个的距离:%lf\n", c[j].cName, v[i][j], road[i][j]);
				fprintf(nnfp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", h[i].sName, h[i].x,
					h[i].y, c[j].cName, c[j].x, c[j].y, road[i][j], v[i][j]);
				disAll += road[i][j] * v[i][j];
				needAll += v[i][j];
			}
		}
	}
	printf("总距离:%lf, 总需求: %d, 平均就医可达性:%lfmin\n", disAll, needAll, disAll / needAll / 72);
	fclose(nnfp);
}

//可达性分析
void accessicilityAnalyse()
{

}
//直接对原始模型进行求解的算法并将结果写入文件中
void solution1()
{
	int i, j, k, l;
	double min;
	int isS = 0;//用来判断哨兵结点是否对医院提供服务
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
	
	
	if (isS == 0)//哨兵结点未提供服务
	{
		FILE * fp = fopen("result1.txt", "w");
		fprintf(fp, "医院名称        hX            hY          社区名称    cX                CY            距离          服务量\n");
		for (int i = 0; i < hn - 1; ++i)
		{
			printf("%s所提供的服务情况如下：\n", h[i].sName);
			for (int j = 0; j < cn; ++j)
			{
				if (v[i][j] != 0)
				{
					printf("社区:%s, 服务量%d:, 单个的距离:%lf\n", c[j].cName, v[i][j], road[i][j]);
					fprintf(fp, "%s\t%lf\t%lf\t     %s\t%lf\t    %lf  %lf\t%d\n", h[i].sName, h[i].x,
						h[i].y, c[j].cName, c[j].x, c[j].y, road[i][j], v[i][j]);
					disAll += road[i][j] * v[i][j];
					needAll += v[i][j];
				}
			}
		}
		printf("总距离:%lf, 总需求: %d, 平均就医可达性:%lfmin\n", disAll, needAll, disAll / needAll / 72);
		fclose(fp);
		opt1();
	}
	//哨兵结点提供了服务
	else
	{
		int op = 3;//选择优化方案
		if (op == 2)
			opt2();//优化方案二，扩建医院//新建医院
		else
			opt3(); 
	}
	
}


int main()
{
	dataIn("医院.txt", "社区.txt");//数据录入
	comRoad();//计算曼哈顿距离
	solution1();
	while (1);
	return 0;
}