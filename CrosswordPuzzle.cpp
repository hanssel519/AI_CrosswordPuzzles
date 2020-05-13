#include<iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <vector>
#include <iterator>
#include <stack>
#include <queue>
using namespace std;
class INTERESECT
{
public:
	int inter1_node;
	int inter1_place;
	int inter2_node;//第幾個
	int inter2_place;//2第幾格
	//INTERESECT();
	//~INTERESECT();	
};
class fitting
{
public:
	int sequance;
	bool if_assign;
	int len;
	int current_picked;//現在在字典裡取到的字是第幾個
	//fitting();
	//~fitting();
	
};
class PlaceAndAlphabet
{
public:
	int place;//直接存index
	char alphabet;
	//PlaceAndAlphabet();
	//~PlaceAndAlphabet();
	
};
vector<char *> words[20];
int global_count0 = 0;
int global_count1 = 0;
int global_count2 = 0;
int count_zero(char *puzzle){
	int count = 0;
	for (int i = 0; i < strlen(puzzle); ++i){
		if (puzzle[i] == ' '){
			count++;
		}
	}
	return count;
}
void format_convertion(char *puzzle, int **format){
	char* pch;
	int format_d1 = 0;
	int tmp_num;
	pch = strtok (puzzle," \n");
	while (pch != NULL)
	{
		for(int i = 1; i <= 4; i++){
			tmp_num = 0;
			/*if (strcmp(pch, "A")==0){
				cout << "A"<<endl;
				format[format_d1][i-1] = 1;//左右 = 1
			}
			else if(strcmp(pch, "D")==0){
				cout << "D"<<endl;
				format[format_d1][i-1] = 0;//上下 = 0
			}*/
			if (pch[0] == 'A'){
				//cout << "A"<<endl;
				format[format_d1][i-1] = 1;//左右 = 1
			}
			else if(pch[0] == 'D'){
				//cout << "D"<<endl;
				format[format_d1][i-1] = 0;//上下 = 0
			}
			else{//for numbers
				//cout << "len:  "<<strlen(pch)<<endl;
				//cout <<"TT:  "<<pch<<endl;
				for(int i = 0; i < strlen(pch); i++){
					tmp_num += (pch[i]-'0') * pow(10, strlen(pch)-1-i);
				}

				//cout << tmp_num<<endl;
				format[format_d1][i-1] = tmp_num;
			}
			pch = strtok (NULL, " \n");
		}
		format_d1++;
	}
	
}
void find_intersection(int set_num, int **format, vector<INTERESECT> &v){
	INTERESECT *tmp_v;
	for (int i = 0; i < set_num-1; ++i){
		for (int j = i+1; j < set_num; ++j){

			if(format[i][3]==1 && format[j][3]==0){//i:cross, j:down
				//cout << "format[j][0]: " <<format[j][0];
				if(format[j][0]>=format[i][0] && format[j][0]<=format[i][0]+format[i][2]){
					if(format[i][1]>=format[j][1] && format[i][1]<=format[j][1]+format[j][2]){
						//cout <<i<<", "<<j<<" intersect at ("<< format[j][0]<<", " << format[i][1]<<")"<<endl;
						tmp_v = new INTERESECT;
						tmp_v->inter1_node = i;
						tmp_v->inter1_place = format[j][0]-format[i][0]+1; //x
						//printf("inter1_place: %d - %d +1\n", format[j][0], format[i][0]);
						tmp_v->inter2_node = j;
						tmp_v->inter2_place = format[i][1]-format[j][1]+1;//y
						//printf("inter2_place: %d - %d +1\n", format[i][1], format[j][1]);
						//printf("%d, %d, %d, %d\n", tmp_v->inter1_node, tmp_v->inter1_place,tmp_v->inter2_node, tmp_v->inter2_place);
						v.push_back(*tmp_v);//??
					}
				}
			}
			//here!!
			if(format[i][3]==0 && format[j][3]==1){//i:down, j:cross
				if(format[j][1]>=format[i][1] && format[j][1]<=format[i][1]+format[i][2]){
					if (format[i][0]>=format[j][0] && format[i][0]<=format[j][0]+format[j][2]){
						//cout <<i<<", "<<j<<" intersect at ("<< format[i][0]<<", " << format[j][1]<<")"<<endl;
						tmp_v = new INTERESECT;
						tmp_v->inter1_node = i;
						tmp_v->inter1_place = format[j][1]-format[i][1]+1;//y
						//printf("inter1_place: %d - %d +1\n", format[j][1], format[i][1]);
						tmp_v->inter2_node = j;
						tmp_v->inter2_place = format[i][0]-format[j][0]+1;//x
						//printf("inter2_place: %d - %d +1\n", format[i][0], format[j][0]);
						//printf("%d, %d, %d, %d\n", tmp_v->inter1_node, tmp_v->inter1_place,tmp_v->inter2_node, tmp_v->inter2_place);
						v.push_back(*tmp_v);//??
					}
				}
			}
		}
	}
}
void words_manage(const char *file_name){
	char *inword;
	int count = 0;
	ifstream infile(file_name);
	while(1){
		inword = new char[30];
		if(infile.getline(inword, 29)){
			//cout << inword <<endl;
			words[strlen(inword)-1].push_back(inword);
			//cout << strlen(inword)-1<<"'s size:   "<< words[strlen(inword)-1].size() <<endl;
			//cout << "in vector:   "<< words[strlen(inword)][words[strlen(inword)].size()-1]<<endl;
			//cout << "word: " << words[strlen(inword)].
			count++;	
		}
		else{
			break;
		}
	}
	cout << "count num:  "<<count << endl;
}
void initialization(int **format, fitting *base, int set_num){
	for (int i = 0; i < set_num; ++i){
		base[i].sequance = i;
		base[i].if_assign = false; //還沒assign
		base[i].len = format[i][2];
		base[i].current_picked = 0; //還沒指向
	}	
}
int bruteforce(fitting *base, int set_num, int current_in_stack, vector<INTERESECT> &v){//words(global不用傳),current_seq為了遞迴
	//stack<fitting> s;
	//int current_in_stack = 0;
	//s.push(base[current_in_stack]);//initial
	for (int i = 0; i < set_num; ++i)
	{
		printf("sequance: %d, if_assign: %d, len: %d, current_picked: %d \n", base[i].sequance, base[i].if_assign, base[i].len, base[i].current_picked);
	}
	global_count1++;
	vector<PlaceAndAlphabet> place_alphabet;
	PlaceAndAlphabet *pa;
	if(current_in_stack == set_num){
		return 0;
	}
	int err = 0;
	printf("in bruteforce: %d\n", current_in_stack);

	if ((current_in_stack == 0)&&(base[0].current_picked >= words[base[0].len].size()-1)){
		printf("CANNOT FIND AN ANSWER!!\n");
		return 1;
	}
	

	for (int i = 0; i < v.size(); ++i){
		if (v[i].inter1_node == current_in_stack){
			if (base[v[i].inter2_node].if_assign == true){//constrain已經被限制 需要配合
				printf("important1 !!\n");
				//place = v[i].inter1_place-1;//直接事index
				//alphabet = words[base[v[i].inter2_node].len][base[v[i].inter2_node].current_picked][v[i].inter2_place-1]
				pa = new PlaceAndAlphabet;
				pa->place = v[i].inter1_place-1;
				pa->alphabet = words[base[v[i].inter2_node].len][base[v[i].inter2_node].current_picked][v[i].inter2_place-1];
				place_alphabet.push_back(*pa);//*pa or pa?
				
			}
		}
		else if(v[i].inter2_node == current_in_stack){
			if (base[v[i].inter1_node].if_assign == true){//constrain已經被限制 需要配合
				printf("important2 !!\n");
				pa = new PlaceAndAlphabet;
				pa->place = v[i].inter2_place-1;
				pa->alphabet = words[base[v[i].inter1_node].len][base[v[i].inter1_node].current_picked][v[i].inter1_place-1];
				place_alphabet.push_back(*pa);//*pa or pa?
				
			}
		}
	}
	printf("PlaceAndAlphabet長度:  %lu\n", place_alphabet.size());
	printf("base[%d].current_picked: %d\n", current_in_stack, base[current_in_stack].current_picked);
	for (int k = base[current_in_stack].current_picked; k <= words[base[current_in_stack].len].size(); ++k){
		global_count0++;
		if(k == words[base[current_in_stack].len].size()){
			printf("no answer!!\n");//找到底了都不適合
			//return 1;
			base[current_in_stack].current_picked = 0;//設回0 往parent
			base[current_in_stack].if_assign = false;
			base[current_in_stack-1].current_picked++;
			if(base[current_in_stack-1].current_picked >= words[base[current_in_stack-1].len].size()){
				if (current_in_stack-2 >= 0){
					base[current_in_stack-1].current_picked = 0;//設回0 往parent
					base[current_in_stack-1].if_assign = false;
					base[current_in_stack-2].current_picked++;
				}
				else{
					printf("CANNOT FIND AN ANSWER(by line 185)!!\n");
					return 1;
				}
			}
			//bruteforce(base, current_in_stack-1, set_num, v);
			
			err = 1;
			break;
		}
		//檢查intersection的字有沒有一樣
		if (place_alphabet.size() == 0){
			printf("沒有constraint. 直接往下做\n");
			break;//出去的機會1:不用配合別人
		}
		//測當前字串是否符合
		for(int j = 0; j < place_alphabet.size(); j++){
			if(words[base[current_in_stack].len][k][place_alphabet[j].place] != place_alphabet[j].alphabet){//single alphabet判斷
				//printf("intersect(a word): %s\n", words[base[current_in_stack].len][k]);
				//printf("intersect(single alphabet): %c, %c\n", words[base[current_in_stack].len][k][place_alphabet[j]->place], place_alphabet[j]->alphabet);
				break;
			}
			if (j == place_alphabet.size()-1){//前面都符合且找完constraint
				printf("intersect(a word): %s\n", words[base[current_in_stack].len][k]);
				printf("intersect(single alphabet): %c, %c\n", words[base[current_in_stack].len][k][place_alphabet[j].place], place_alphabet[j].alphabet);
				base[current_in_stack].current_picked = k;
				err = 2;//出去的機會2:找到符合字串
			}
		}
		if(err == 2){//出去的機會2:找到符合字串
			err = 0;
			break;
		}
	}

	if (err == 0){ //不用回parent
		printf("in node: %d, go to child\n", current_in_stack);
		base[current_in_stack].if_assign = true;
		current_in_stack++;
	}
	if(err == 1){
		if(current_in_stack > 0)
			current_in_stack--;
		else{
			return 1;
		}
	}
	if(bruteforce(base, set_num, current_in_stack, v) == 1){
		return 1;
	}
	else{
		return 0;//表示正常結束 有解
	}
}
void put_words_in_domain(fitting *base, vector<int> words_in_domain[], int set_num){
	for (int i = 0; i < set_num; ++i){
		for (int j = 0; j < words[base[i].len].size(); ++j){
			words_in_domain[i].push_back(j);//
		}
	}
}
void words_adjustment(fitting *base, vector<INTERESECT> &v, vector<int> words_in_domain[], int current_constraint_q, queue<int> &constraint_q){
	//queue<int> constraint_q;
	int tmp_alphabet[26];//紀錄要刪除的字
	int err1 = 0;
	int err2 = 0;
	int cons1[26], cons2[26];
	for (int i = 0; i < 26; ++i){
		tmp_alphabet[i] = 0;
		cons1[i] = 0;
		cons2[i] = 0;
	}
	/*
	for (int i = 0; i < v.size(); ++i){
		constraint_q.push(i);
	}*/
	for(vector<int>::iterator it=words_in_domain[v[current_constraint_q].inter1_node].begin(); it!=words_in_domain[v[current_constraint_q].inter1_node].end(); ++it){
			cons1[(words[base[v[current_constraint_q].inter1_node].len][*it][v[current_constraint_q].inter1_place-1] - 'a')] = 1;	
	}
	for(vector<int>::iterator it=words_in_domain[v[current_constraint_q].inter2_node].begin(); it!=words_in_domain[v[current_constraint_q].inter2_node].end(); ++it){
			cons2[(words[base[v[current_constraint_q].inter2_node].len][*it][v[current_constraint_q].inter2_place-1] - 'a')] = 1;	
	}
	/*
    for (int i = 0; i < words_in_domain[v[current_constraint_q].inter1_node].size(); ++i){
		cons1[(words[base[v[current_constraint_q].inter1_node].len][*it][v[current_constraint_q].inter1_place] - 'a')] = 1;
	}
	for (int i = 0; i < words_in_domain[v[current_constraint_q].inter2_node].size(); ++i){
		cons2[(words[base[v[current_constraint_q].inter2_node].len][i][v[current_constraint_q].inter2_place] - 'a')] = 1;
	}*/
	for (int i = 0; i < 26; ++i){
		if((cons1[i] == 0)||(cons2[i] == 0)){//兩個有一個沒有這個字
			tmp_alphabet[i] = 1;
		}
	}
	for(vector<int>::iterator it=words_in_domain[v[current_constraint_q].inter1_node].begin(); it!=words_in_domain[v[current_constraint_q].inter1_node].end();){
		//printf("%d\n", *it);
		//tmp_alphabet[(words[base[v[current_constraint_q].inter1_node].len][*it][v[current_constraint_q].inter1_place]-'a')] == 1
		if(tmp_alphabet[(words[base[v[current_constraint_q].inter1_node].len][*it][v[current_constraint_q].inter1_place-1]-'a')] == 1) {
			it = words_in_domain[v[current_constraint_q].inter1_node].erase(it);
			err1 = 1;
		}
		else {
			++it;
		}
	}
	for(vector<int>::iterator it=words_in_domain[v[current_constraint_q].inter2_node].begin(); it!=words_in_domain[v[current_constraint_q].inter2_node].end();){
		//printf("%d\n", *it);
		//tmp_alphabet[(words[base[v[current_constraint_q].inter1_node].len][*it][v[current_constraint_q].inter1_place]-'a')] == 1
		if(tmp_alphabet[(words[base[v[current_constraint_q].inter2_node].len][*it][v[current_constraint_q].inter2_place-1]-'a')] == 1) {
			it = words_in_domain[v[current_constraint_q].inter2_node].erase(it);
			err2 = 1;
		}
		else {
			++it;
		}
	}
	if (err1 == 1){
		for (int i = 0; i < v.size(); ++i){
			if(i != current_constraint_q){
				if ((v[i].inter1_node == v[current_constraint_q].inter1_node)||(v[i].inter2_node== v[current_constraint_q].inter1_node)){
					constraint_q.push(i);
				}
			}
		}
	}
	if (err1 == 2){
		for (int i = 0; i < v.size(); ++i){
			if(i != current_constraint_q){
				if ((v[i].inter1_node == v[current_constraint_q].inter2_node)||(v[i].inter2_node== v[current_constraint_q].inter2_node)){
					constraint_q.push(i);
				}
			}
		}
	}
	/*
	for(vector<char *>::iterator it=words[base[v[current_constraint_q].inter1_node].len].begin(); it!=words[base[v[current_constraint_q].inter1_node].len].end();){
		printf("%s\n", *it);
		if(tmp_alphabet[(*it[base[v[current_constraint_q].inter1_place]]-'a')] == 1) {
			it = words.erase(it);
		}
		else {
			++it;
		}
	}
	for(vector<char *>::iterator it=words[base[v[current_constraint_q].inter2_node].len].begin(); it!=words[base[v[current_constraint_q].inter2_node].len].end();){
		printf("%s\n", *it);
		if(tmp_alphabet[(*it[base[v[current_constraint_q].inter2_place]]-'a')] == 1) {
			it = words.erase(it);
		}
		else {
			++it;
		}
	}*/
    
}
int withAC3(fitting *base, int set_num, int current_in_stack, vector<INTERESECT> &v, vector<int> words_in_domain[]){
	global_count2++;
	for (int i = 0; i < set_num; ++i)
	{
		printf("sequance: %d, if_assign: %d, len: %d, current_picked: %d \n", base[i].sequance, base[i].if_assign, base[i].len, base[i].current_picked);
	}
	vector<PlaceAndAlphabet> place_alphabet;
	PlaceAndAlphabet *pa;
	if(current_in_stack == set_num){
		return 0;
	}
	int err = 0;
	printf("in bruteforce: %d\n", current_in_stack);

	if ((current_in_stack == 0)&&(base[0].current_picked >= words_in_domain[0].size()-1)){
		printf("CANNOT FIND AN ANSWER!!\n");
		return 1;
	}
	for (int i = 0; i < v.size(); ++i){
		if (v[i].inter1_node == current_in_stack){
			if (base[v[i].inter2_node].if_assign == true){//constrain已經被限制 需要配合
				printf("important1 !!\n");
				pa = new PlaceAndAlphabet;
				pa->place = v[i].inter1_place-1;
				pa->alphabet = words[base[v[i].inter2_node].len][words_in_domain[v[i].inter2_node][base[v[i].inter2_node].current_picked]][v[i].inter2_place-1];
				//pa->alphabet = words[base[v[i].inter2_node].len][base[v[i].inter2_node].current_picked][v[i].inter2_place-1];
				place_alphabet.push_back(*pa);
				
			}
		}
		else if(v[i].inter2_node == current_in_stack){
			if (base[v[i].inter1_node].if_assign == true){//constrain已經被限制 需要配合
				printf("important2 !!\n");
				pa = new PlaceAndAlphabet;
				pa->place = v[i].inter2_place-1;
				pa->alphabet = words[base[v[i].inter1_node].len][words_in_domain[v[i].inter1_node][base[v[i].inter1_node].current_picked]][v[i].inter1_place-1];
				//pa->alphabet = words[base[v[i].inter1_node].len][base[v[i].inter1_node].current_picked][v[i].inter1_place-1];
				place_alphabet.push_back(*pa);
				
			}
		}
	}
	printf("PlaceAndAlphabet長度:  %lu\n", place_alphabet.size());
	printf("base[%d].current_picked: %d\n", current_in_stack, base[current_in_stack].current_picked);
	for (int k = base[current_in_stack].current_picked; k <= words_in_domain[current_in_stack].size(); ++k){
		if(k == words_in_domain[current_in_stack].size()){
			printf("no answer!!\n");//找到底了都不適合
			//return 1;
			base[current_in_stack].current_picked = 0;//設回0 往parent
			base[current_in_stack].if_assign = false;
			base[current_in_stack-1].current_picked++;
			if(base[current_in_stack-1].current_picked >= words_in_domain[current_in_stack-1].size()){
				if (current_in_stack-2 >= 0){
					base[current_in_stack-1].current_picked = 0;//設回0 往parent
					base[current_in_stack-1].if_assign = false;
					base[current_in_stack-2].current_picked++;
				}
				else{
					printf("CANNOT FIND AN ANSWER(by line 185)!!\n");
					return 1;
				}
			}
			//bruteforce(base, current_in_stack-1, set_num, v);
			
			err = 1;
			break;
		}
		//檢查intersection的字有沒有一樣
		if (place_alphabet.size() == 0){
			printf("沒有constraint. 直接往下做\n");
			break;//出去的機會1:不用配合別人
		}
		//測當前字串是否符合
		for(int j = 0; j < place_alphabet.size(); j++){
			if(words[base[current_in_stack].len][words_in_domain[current_in_stack][k]][place_alphabet[j].place] != place_alphabet[j].alphabet){//single alphabet判斷
				//printf("intersect(a word): %s\n", words[base[current_in_stack].len][k]);
				//printf("intersect(single alphabet): %c, %c\n", words[base[current_in_stack].len][k][place_alphabet[j]->place], place_alphabet[j]->alphabet);
				break;
			}
			if (j == place_alphabet.size()-1){//前面都符合且找完constraint
				printf("intersect(a word): %s\n", words[base[current_in_stack].len][words_in_domain[current_in_stack][k]]);
				printf("intersect(single alphabet): %c, %c\n", words[base[current_in_stack].len][words_in_domain[current_in_stack][k]][place_alphabet[j].place], place_alphabet[j].alphabet);
				base[current_in_stack].current_picked = k;
				err = 2;//出去的機會2:找到符合字串
			}
		}
		if(err == 2){//出去的機會2:找到符合字串
			err = 0;
			break;
		}
	}

	if (err == 0){ //不用回parent
		printf("in node: %d, go to child\n", current_in_stack);
		base[current_in_stack].if_assign = true;
		current_in_stack++;
	}
	if(err == 1){
		if(current_in_stack > 0)
			current_in_stack--;
		else{
			return 1;
		}
	}
	if(withAC3(base, set_num, current_in_stack, v, words_in_domain) == 1){
		return 1;
	}
	else{
		return 0;//表示正常結束 有解
	}
}
int main(int argc, char const *argv[])
{
	char* puzzle;
	int** format;
	fitting* base;
	

	int zero_num, set_num;
	//fstream file;
	if (argc!=3)                  //判斷參數輸入是否正確
	{
		cout << "Input error!\n";
		return 1;
	}
	const char *file_name = argv[2];
	words_manage(file_name);//處理words;
	ifstream infile(argv[1]);
	
	while(1){
		puzzle = new char[1000];
		//cout << "puzzle size0: "<< sizeof(puzzle)<<endl; 
		if(infile.getline(puzzle, 1000)){
			//cout << "puzzle size1: "<< sizeof(puzzle)<<endl;
			//cout <<"puzzle: "<< puzzle<<endl;
			//cout <<"puzzle len: "<<strlen(puzzle) << endl;
			zero_num = count_zero(puzzle);
			//cout << "zero_num: "<<zero_num<<endl;
			set_num = ((zero_num/3)+1)/2;
			//cout << "set_num: "<<set_num<<endl;
			format = new int* [set_num];
			for (int i = 0; i < set_num; ++i){
				format[i] = new int[4];
			}
			format_convertion(puzzle, format);
			vector<INTERESECT> intersection;
			find_intersection(set_num, format, intersection);
			base = new fitting [set_num];
			initialization(format, base, set_num);
			vector<int> words_in_domain[set_num];
			put_words_in_domain(base, words_in_domain, set_num);
			queue<int> constraint_q;
			for (int i = 0; i < intersection.size(); ++i){
				constraint_q.push(i);
			}
			while(constraint_q.size() > 0){
				words_adjustment(base, intersection, words_in_domain, constraint_q.front(), constraint_q);
				constraint_q.pop();
			}

			if (withAC3(base, set_num, 0, intersection, words_in_domain) == 0){
				printf("withAC3's return value: 0(success)\n");
				printf("final answer for a set.\n");
				printf("------------------------------------");
				printf("\n\n");
				for (int i = 0; i < set_num; ++i){
					printf("ans %d: %s\n", i, words[base[i].len][words_in_domain[i][base[i].current_picked]]);
				}
				printf("\n\n");
				printf("------------------------------------");
			}
			else{
				printf("withAC3's return value: 1(failed)\n");
			}
			printf("\nglobal_count2: %d\n", global_count2);
		}
		else{
			break;
		}
		delete puzzle;
	}
	return 0;
}