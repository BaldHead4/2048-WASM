#include<stdio.h>
#include<stdlib.h>
#define MAX_LEN 4
typedef struct block *Block;
struct block{
	int x;
	int y;
	int status;
};

/* 初始化 */ 
/* 将稀疏矩阵转化为二维数组 */
int **Matrix2Array(Block blocks, int len)
{
	int **datas = (int **)malloc(MAX_LEN * sizeof(int *));
	int i, j;
	for(i = 0; i < MAX_LEN; i++){
		datas[i] = (int *)malloc(MAX_LEN * sizeof(int));
	}
	for(i = 0; i < MAX_LEN; i++){
		for(j = 0; j < MAX_LEN; j++){
			datas[i][j] = 0;
		}
	}
	for(i = 0; i < len; i++){
		datas[blocks[i].x][blocks[i].y] = blocks[i].status;
	}
	return datas;
}


/* 返回值 */
/* 返回不为空值的block个数 */
int FillBlocksLen(int **datas)
{
	int i, j;
	int len = 0;
	for(i = 0; i < MAX_LEN; i++){
		for(j = 0; j < MAX_LEN; j++){
			if(datas[i][j] != 0){
				len++;
			}
		}
	}
	return len;
}

/* 返回不为空值的blocks */
Block Array2Matrix_Fill(int **datas)
{
	int len = FillBlocksLen(datas);
	Block blocks = (Block)malloc(len * sizeof(struct block));
	int i, j, k = 0;
	for(i = 0; i < MAX_LEN; i++){
		for(j = 0; j < MAX_LEN; j++){
			if(datas[i][j] != 0){
				blocks[k].x = i;
				blocks[k].y = j;
				blocks[k++].status = datas[i][j];
			}
		}
	}
	return blocks;
}

/* 返回空值的blocks */
Block Array2Matrix_None(int **datas)
{
	int len = MAX_LEN * MAX_LEN - FillBlocksLen(datas);
	Block blocks = (Block)malloc(len * sizeof(struct block));
	int i, j, k = 0;
	for(i = 0; i < MAX_LEN; i++){
		for(j = 0; j < MAX_LEN; j++){
			if(datas[i][j] == 0){
				blocks[k].x = i;
				blocks[k].y = j;
				blocks[k++].status = 0;
			}
		}
	} 
	return blocks;
}


/* 操作 */
/* 颠倒行(列) */
int *Reverse(int *data)
{
	int i;
	int *tmp = (int *)malloc(MAX_LEN * sizeof(int));
	for(i = 0; i < MAX_LEN; i++){
		tmp[MAX_LEN - i - 1] = data[i];
	}
	for(i = 0; i < MAX_LEN; i++){
		data[i] = tmp[i];
	}
	free(tmp);
	return data;
}

/* 转置二维数组 */
int **Transpose(int **datas)
{
	int **tmp = (int **)malloc(MAX_LEN * sizeof(int *));
	int i, j;
	for(i = 0; i < MAX_LEN; i++){
		tmp[i] = (int *)malloc(MAX_LEN * sizeof(int));
	}
	for(i = 0; i < MAX_LEN; i++){
		for(j = 0; j < MAX_LEN; j++){
			tmp[i][j] = datas[j][i];
		}
	}
	for(i = 0; i < MAX_LEN; i++){
		for(j = 0; j < MAX_LEN; j++){
			datas[i][j] = tmp[i][j];
		}
	}
	for(i = 0; i < MAX_LEN; i++){
		free(tmp[i]);
	}
	free(tmp);
	return datas;
}

/* 返回每一行(列)非零值 */
int *SelectNonZero(int *data)
{
	int *tmp = (int *)malloc(MAX_LEN * sizeof(int));
	int i, len = MAX_LEN - 1;
	for(i = MAX_LEN - 1; i >= 0; i--){
		if(data[i] != 0){
			tmp[len--] = data[i];
		}
	}
	for(; len >= 0; len--){
		tmp[len] = 0;
	}
	for(i = 0; i < MAX_LEN; i++){
		data[i] = tmp[i];
	}
	free(tmp);
	return data;
}

/* 对每一行(列)操作 */
int *Update(int *data)
{
	data = SelectNonZero(data);
	int i;
	for(i = MAX_LEN - 1; i >= 0; i -= 2){
		if(data[i] == data[i - 1]){
			data[i] += data[i - 1];
			data[i - 1] = 0;
		}else{
			i++;
		}
	}
	data = SelectNonZero(data);
	return data;
}

/* 向右滑动 */
int **Right(Block blocks, int len)
{
	int i;
	int **datas;
	datas = Matrix2Array(blocks, len);
	for(i = 0; i < MAX_LEN; i++){
		datas[i] = Update(datas[i]);
	}
	return datas;
}

/* 向左滑动 */
int **Left(Block blocks, int len)
{
	int i;
	int **datas;
	datas = Matrix2Array(blocks, len);
	for(i = 0; i < MAX_LEN; i++){
		datas[i] = Reverse(Update(Reverse(datas[i])));
	}
	return datas;
}

/* 向下滑动 */
int **Down(Block blocks, int len)
{
	int i;
	int **datas;
	datas = Transpose(Matrix2Array(blocks, len));
	for(i = 0; i < MAX_LEN; i++){
		datas[i] = Update(datas[i]);
	}
	return Transpose(datas);
}

/* 向上滑动 */
int **Up(Block blocks, int len)
{
	int i;
	int **datas;
	datas = Transpose(Matrix2Array(blocks, len));
	for(i = 0; i < MAX_LEN; i++){
		datas[i] = Reverse(Update(Reverse(datas[i])));
	}
	return Transpose(datas);
}

int main()
{
	struct block blocks_init[2];
	blocks_init[0].x = 2;
	blocks_init[0].y = 1;
	blocks_init[0].status = 2;
	blocks_init[1].x = 0;
	blocks_init[1].y = 1;
	blocks_init[1].status = 2;
	int len = 2;
	
	/* 测试 Matrix2Array() Right() Left()*/
	int i, j;
	int **datas;
	/* datas = Matrix2Array(blocks_init, len); */
	/* datas = Right(blocks_init, len); */
	/* datas = Left(blocks_init, len); */
	/* datas = Down(blocks_init, len); */
	datas = Up(blocks_init, len);
	for(i = 0; i < MAX_LEN; i++){
		for(j = 0; j < MAX_LEN; j++){
			printf("%d ", datas[i][j]);
		}
		printf("\n");
	}
	
	/* 测试 FillBlocksLen() Array2Matrix_Fill() Array2Matrix_None() */
	/*
	len = FillBlocksLen(datas);
	Block blocks_fill = Array2Matrix_Fill(datas);
	for(i = 0; i < len; i++){
		printf("%d ", blocks_fill[i].x);
		printf("%d ", blocks_fill[i].y);
		printf("%d\n", blocks_fill[i].status);
	}
	printf("--------------------------------------\n");
	len = MAX_LEN * MAX_LEN - FillBlocksLen(datas);
	Block blocks_none = Array2Matrix_None(datas);
	for(i = 0; i < len; i++){
		printf("%d ", blocks_none[i].x);
		printf("%d ", blocks_none[i].y);
		printf("%d\n", blocks_none[i].status);
	}
	*/
	
	/* 测试 SelectNonZero() Update() */
	/*
	int *data;
	data = (int *)malloc(MAX_LEN * sizeof(int));
	while(~scanf("%d %d %d %d",&data[0], &data[1], &data[2], &data[3])){
		data = SelectNonZero(data);
		data = Update(data);
		printf("%d %d %d %d\n",data[0], data[1], data[2], data[3]);
	}
	*/

	return 0;
}
