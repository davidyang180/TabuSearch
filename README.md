# TabuSearch
TabuSearch
使用禁忌搜索算法解决图着色问题  
（之后随缘更新遗传禁忌搜索混合算法和蚁群算法）
# Result
DSJC125.1  
未优化测试：颜色数为5  
初始化点和边为：125,1472  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 1205541 | 994  
2  | 3105 | 3  
3  | 3105 | 4  
4  | 3105 | 3  
5  | 3105 | 3  
***********************************************************
DSJC125.5   
未优化测试：颜色数为17  
1、未优化  
初始化点和边为：125,7782  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 32408 | 72  
2  | 329140 | 703  
3  | 3968036 | 4940  
4  | 20953 | 46  
5  | 20953 | 58  
***********************************************************
DSJC125.9  
未优化测试：颜色数为44  
初始化点和边为：125,13922  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 2218 | 6  
2  | 2218 | 6  
3  | 2218 | 5  
4  | 2218 | 5  
5  | 2218 | 7  
***********************************************************
DSJC250.1  
未优化测试：颜色数为8  
初始化点和边为：250,55794  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 79446 | 660  
2  | 50420 | 422  
3  | 50446 | 425  
4  | 9607 | 84  
5  | 9607 | 87  
***********************************************************
DSJC250.5  
未优化测试：颜色数为28  
初始化点和边为：250,31336  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 12287307 | 39346  
2  | 8295197 | 28220  
3  | 1610321 | 5348  
4  | 24180876 | 68171  
5  | 2131009 | 5863  
***********************************************************
DSJC250.9  
未优化测试：颜色数为72  
初始化点和边为：250,55794  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 1613574 | 4263  
2  | 463545 | 1453  
3  | 4513607 | 11594  
4  | 1514768 | 3903  
5  | 1724716 | 5337  
***********************************************************
DSJC500.1  
未优化测试：颜色数为12  
初始化点和边为：500,24916  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 199324101 | 653758  
2  | 85075105 | 276255  
3  | 57106955 | 180991  
4  | 49048921 | 155768  
5  | 37840538 | 119583 
***********************************************************
DSJC500.5  
未优化测试：颜色数为50（无法达到49）  
初始化点和边为：500,125248  

测试数  | 迭代次数  | 耗时
---- | ----- | ------  
1  | 11322770 | 83973  
2  | 9825869 | 72465  
3  | 15346225 | 116680  
4  | 2704364 | 18957  
5  | 2452182 | 20003 
# 遇到问题
* 1、编写过程中忘记加入评估函数f的更新策略，而导致收敛非常缓慢，导致一开始测试所有用例时全都达不到参考颜色用例，并且迭代次数非常大；（已解决）  
* 2、程序内部存储优化比较差，使用过多的存储和分支判断，非常大的程度上影响了程序的运行速度。（待解决）  
# 相关优化
* 1、先验生成局部着色互斥的初始解集  
**有效性证明：设a1、a2、a3.....an冲突，则颜色c1、c2、c3.....cn必不相同；**  
**假设∃yiyi一解集成立，则必然∃该解集中a1......an对应的颜色c进行颜色交换等价于初始解集生成的局部互斥颜色c1......cn。**  
```cpp
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
``` 
* 2、当迭代陷入时，尽可能的移动多的不影响评价函数的局部策略点，活化当前局面；  
```cpp
else if (best_delter == 0) {
		//int max = 0;
		
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
				i = i + step;
				step = 1;
			}

		}
		
		//for (int i = 0; i < eque_number; i++) {
			/*
			if (point_edge_number[eque_change[i][0]] > max) {
				best_move = i;
				max = point_edge_number[eque_change[i][0]];
			}*/

			//move_point = eque_change[i][0];
			//new_color = eque_change[i][1];
			//if (iter > 50)
			//cout << i << "选择了将颜色" << move_point << "交换到颜色" << new_color << endl;
			//if(adj_color_table[move_point][point_color[move_point]] == adj_color_table[move_point][new_color])
			//makemove(move_point, new_color, iter);
		//}
	}
  ``` 
  * 3、迭代过程中，概率生成小异常移动策略（这里使用最简单的概率：等概率，可能对跳出局部陷入有效）  
  ```cpp
					else if (((iter + 1) % 20) == 0 && (delter >= 1 && delter <= 3)) {
						eque_change[eque_number][0] = i;
						eque_change[eque_number][1] = j;
						eque_change[eque_number][2] = delter;
						eque_number++;
					}
   ``` 
   * 4、优化禁忌表策略（关键，暂时未想到有效的方法）  
   目前使用随迭代数逐渐增加动态生成更新权值
    ```cpp
   int test = 20;
	/*
	if (iter + 1 == 2000)
		test = 50;
	else if (iter + 1 == 10000)
		test = 100;
	else if ((iter+1 % 100000) == 0)
		test = 500;
	*/
  	srand(time(NULL));
	tabutenure[move_point][old_color] = iter + f + 0.6*(rand() % test);
    ``` 
   # 总结
   * 1、经过测试先验生成局部着色互斥的初始解集有一定的优化效果，可以减少一定的迭代次数，但效果不明显，当节点数量增大时，无法抵消先验生成解集的消耗；  
   * 2、第二种优化方法，有一定的优化程度，但有时会恶化；  
   * 3、第三种优化方法，局限性太大，很难能控制好判断条件；  
   * 4、第四种优化方法，对禁忌表影响很大，应该寻找一个更优的禁忌表更新策略；
