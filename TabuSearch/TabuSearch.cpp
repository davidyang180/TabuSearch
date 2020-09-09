// TabuSearch.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
int point_number=0;
int edge_number=0;
int color_number=0;
bool **edge;
int *point_color;
int **adj_color_table;
int f=0,best_f=0;
int **tabutenure;
int **eque_change;
bool *first_color;
int *point_edge_number;
int best_delter;
int eque_number = 0;
int distribution_color=0;
void makemove(int &move_point, int &new_color, long &iter);
//先验分配生成初始解
void first_distribution(int now_point, int now_color) {
	if (!first_color[now_point]) {
		point_color[now_point] = now_color;
		first_color[now_point] = true;
		for (int i = 1; i <= point_number; i++) {
			if (edge[now_point][i]) {
				srand(time(NULL));
				distribution_color = rand() % color_number;
				while (distribution_color == now_color)
					distribution_color = rand() % color_number;
				first_distribution(i, distribution_color);
			}
		}
	}
}
void read_file(FILE *p) {
	string a;
	if (!p)
		cout << "文件读取失败！" << endl;
	while (cin >> a) {
		if (a == "edge") {
			cin >> point_number >> edge_number;
			cout << "初始化点和边为：" << point_number <<','<< edge_number << endl;
			edge = new bool* [point_number+1];
			for (int i = 0; i <= point_number;i++) {
				edge[i] = new bool[point_number + 1]{ false };
			}
			point_edge_number = new int[point_number + 1]{ 0 };
		}
		if (a == "e") {
			int i, j;
			cin >> i >> j;
			//cout << "边为：" << i << ',' << j<<endl;
			edge[i][j] = !edge[i][j];
			edge[j][i] = !edge[j][i];
			point_edge_number[i]++;
			point_edge_number[j]++;
		}
	}
	/*
	for (int i = 1; i <= point_number; i++) {
		for (int j = 1; j < i; j++) {
			if (edge[i][j])
				cout << i << ',' << j << endl;
		}
	}
	*/
	point_color = new int[point_number+1];
	adj_color_table = new int*[point_number + 1];
	for (int i = 0; i <= point_number; i++)
		adj_color_table[i] = new int[color_number] {0};
	first_color = new bool[point_number + 1]{ false };
	//随机生成结点的颜色值（可用先验生成）
	/*
	srand(time(NULL));
	for (int i = 1; i <= point_number; i++)
		point_color[i] = rand() % color_number;
	*/
	
	for (int i = 1; i <= point_number; i++) {
		first_distribution(i, (i - 1) % color_number);
	}
	

	for (int i = 1; i <= point_number; i++) {
		for (int j = i+1; j <= point_number; j++) {
			if (edge[i][j]) {
				adj_color_table[i][point_color[j]]++;
				adj_color_table[j][point_color[i]]++;
				if (point_color[i] == point_color[j])
					f++;
			}
		}
	}
	best_f = f;
	tabutenure = new int*[point_number+1];
	for (int i = 0; i <= point_number; i++)
		tabutenure[i] = new int[color_number] {0};
	eque_change = new int *[point_number* (color_number - 1)];
	for (int i = 0; i < point_number* (color_number - 1); i++) {
		eque_change[i] = new int[3]{ 0 };
	}
}
void findmove(int &move_point, int &new_color, long &iter) {
	//if (iter > 50)
	//cout << "测试2" << endl;
	best_delter=10000;
	int delter = 0;
	int best_move = 0;
	for (int i = 1; i <= point_number; i++) {
		if (adj_color_table[i][point_color[i]] > 0) {
			//if (iter > 50)
			//cout << "冲突点为" <<i<<"冲突数为:"<< adj_color_table[i][point_color[i]] << endl;
			for (int j = 0; j < color_number; j++) {
				if (point_color[i] != j) {
					delter = adj_color_table[i][j] - adj_color_table[i][point_color[i]]; //可取全局优化值
					//if (iter > 50)
					//cout <<"和预交换的颜色值"<<j<< "的局部优化值为：" << delter << endl;
					if (delter <= best_delter && (iter > tabutenure[i][j] || (f + delter) < best_f)) {
						//if (iter > 50)
						//cout << "选择了交换到颜色" << j << endl;
						if (delter < best_delter) {
							best_delter = delter;
							eque_number = 0;
						}
						eque_change[eque_number][0] = i;
						eque_change[eque_number][1] = j;
						eque_change[eque_number][2] = best_delter;
						eque_number++;
					}
					/*
					else if (((iter + 1) % 20) == 0 && (delter >= 1 && delter <= 3)) {
						eque_change[eque_number][0] = i;
						eque_change[eque_number][1] = j;
						eque_change[eque_number][2] = delter;
						eque_number++;
					}*/
					
				}
			}
		}
	}
	//if (((iter + 1) % 20) == 0)
	//	best_delter = 1;
	if (best_delter != 0) {
		srand(time(NULL));
		best_move = rand() % eque_number;
		move_point = eque_change[best_move][0];
		new_color = eque_change[best_move][1];
		best_delter= eque_change[best_move][2];
		//cout << "此时最佳局部优化为：" << best_delter << endl;
		f = f + best_delter;
		if (f < best_f) best_f = f;
		//if (iter > 50)
		//cout << best_move << "选择了将颜色" << move_point << "交换到颜色" << new_color << endl;
		makemove(move_point, new_color, iter);
	}
	
	else if (best_delter == 0) {
		//int max = 0;
		/*
		int i = 0;
		int step = 1;
		while (i < eque_number) {
			if (eque_change[i][0] == eque_change[i + step][0]) {
				step++;
			}
			else {
				srand(time(NULL));
				best_move = (rand() % step) + i;
				move_point = eque_change[best_move][0];
				new_color = eque_change[best_move][1];

				while (adj_color_table[move_point][point_color[move_point]] != adj_color_table[move_point][new_color]) {
					best_move = (rand() % step) + i;
					move_point = eque_change[best_move][0];
					new_color = eque_change[best_move][1];
					best_delter = eque_change[best_move][2];
				}

				if (adj_color_table[move_point][point_color[move_point]] == adj_color_table[move_point][new_color]) {
					f = f + best_delter;
					if (f < best_f) best_f = f;
					makemove(move_point, new_color, iter);
				}
				//if (iter > 1900)
				//cout << best_move << "选择了将颜色" << move_point << "交换到颜色" << new_color << endl;
				
				i = i + step;
				step = 1;
			}

		}*/
		
		
		for (int i = 0; i < eque_number; i++) {
			/*
			if (point_edge_number[eque_change[i][0]] > max) {
				best_move = i;
				max = point_edge_number[eque_change[i][0]];
			}*/

			move_point = eque_change[i][0];
			new_color = eque_change[i][1];
			//if (iter > 50)
			//cout << i << "选择了将颜色" << move_point << "交换到颜色" << new_color << endl;
			if(adj_color_table[move_point][point_color[move_point]] == adj_color_table[move_point][new_color])
			makemove(move_point, new_color, iter);
		}

	}

}
void makemove(int &move_point, int &new_color, long &iter) {
	//if (iter > 50)
	//cout << "进入交换" << endl;
	//if (iter > 50)
	//cout << "选择了将颜色" << move_point << "交换到颜色" << new_color << endl;
	int test = 20;
	/*
	if (iter + 1 == 2000)
		test = 50;
	else if (iter + 1 == 10000)
		test = 100;
	else if ((iter+1 % 100000) == 0)
		test = 500;
	*/
	int old_color = point_color[move_point];
	for (int i = 1; i <= point_number; i++) {
		if (edge[move_point][i]) {
			//if (iter > 50)
			//cout << "更改节点" << i << endl;
			adj_color_table[i][old_color]--;
			adj_color_table[i][new_color]++;
		}
	}
	srand(time(NULL));
	tabutenure[move_point][old_color] = iter + f + 0.6*(rand() % test);
	point_color[move_point] = new_color;
}
void print() {
	//cout << "节点颜色分配为：" << endl;
	//for (int i = 1; i <= point_number; i++)
	//	cout << "点：" << i << "的颜色：" << point_color[i] <<"冲突为："<< "  ";
	//cout << endl;
	cout << "节点冲突表为：" << endl;
	for (int i = 1; i <= point_number; i++) {
		cout << "点：" << i << "的颜色为" << point_color[i] << "  ";
		for (int j = 0; j < color_number; j++) {
			if(j== point_color[i])
			cout << "点：" << i <<"的颜色"<<j<< "冲突为：" << adj_color_table[i][j] << "  ";
		}
		cout << endl;
	}
	//cout << "节点禁忌表为：" << endl;
	//for (int i = 1; i <= point_number; i++) {
	//	for (int j = 0; j < color_number; j++) {
	//		cout << "点：" << i << "禁止到颜色"<<j<<"的时间为：" << tabutenure[i][j] << "  ";
	//	}
	//}
}
int main()
{
	int count = 12;
	const char* file_name = "./data/DSJC500.1.col";
	FILE *p = freopen(file_name, "r", stdin);
	while (count) {
		color_number = 12;
		int move_point = 0, new_color = 0;
		long iter = 0;
		read_file(p);
		cin.clear();
		rewind(p);
		clock_t start, end;
		start = clock();
		while (f != 0) {
			//cout << "测试1" << endl;
			findmove(move_point, new_color, iter);
			//if(iter>24)
			//print();
			iter++;
		}
		end = clock();
		cout << "迭代次数：" << iter << "\t 耗时=" << (end - start) << endl;

		for (int i = 0; i <= point_number; i++) {
			delete[] edge[i];
			delete[] adj_color_table[i];
			delete[] tabutenure[i];
		}
		delete[] adj_color_table;
		delete[] edge;
		delete[] tabutenure;
		for (int i = 0; i < point_number* (color_number - 1); i++) {
			delete[] eque_change[i];
		}
		delete[] eque_change;

		delete[] point_color;
		delete[] first_color;
		delete[] point_edge_number;
		
		count--;
	}
	fclose(stdin);
}
