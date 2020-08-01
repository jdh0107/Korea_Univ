#define _CRT_SECURE_NO_WARNINGS
#define N_WORD 12
#define _USE_MATH_DEFINES
#define WTPFactor 50
#define INF 1000000000

#include <cstdio>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include "hmm.h"
using namespace std;

namespace fs = std::experimental::filesystem;

typedef struct {
	const char* myWord;						//viterbi table�� �� element�� ���� word�� �̸�
	int index;								//�� path�� ���� ����� viterbi table������ row index
} btComp;

typedef struct {
	char* word;						//phone�� ���ϴ� �ܾ� �̸�
	hmmType* phoneModel;					//phone ������
} phoneType;

typedef struct{
	char* name;
	int phoneCount;							//�� word model�� �����ϴ� phone model�� ����
	phoneType* phoneModels;					//�� word model�� �����ϴ� phone model���� �迭
} wordModel;

typedef struct {
	int wordModelCount;						//word Model ����
	wordModel* wordModels;
} wordModelArray;							//word Model���� �迭

typedef struct {
	char* name;
	float probability;
} prior;

typedef struct {
	int priorCount;							//prior���� ����
	prior* priors;
} priorArray;					


typedef struct {
	char* prev;
	char* next;
	float probability;
} wordTranx;


///////////////////////////////////////////////////////////////
//�������� ���ڿ� �ڸ��� �Լ�. �߸� ���ڿ���(string)���� �̷���� vector�� ��ȯ
bool space(char c) 
{
	return isspace(c);
}

bool not_space(char c)
{
	return !isspace(c);
}

vector<string> split(const string& str) 
{
	typedef string::const_iterator iter;
	vector<string> ret;

	iter it_b = str.begin();

	while (it_b != str.end()) {
		//������ �ƴ� ù ����
		it_b = find_if(it_b, str.end(), not_space);
		//�� ���ڷ� �����ϴ� �ܾ��� ��
		iter it_e = find_if(it_b, str.end(), space);

		if (it_b != str.end()) {
			ret.push_back(string(it_b, it_e));
		}

		it_b = it_e;
	}

	return ret;
}
////////////////////////////////////////////////////////////////
//replace all
string replaceAll(const string &str, const string &pattern, const string &replace)
{
	string result = str;
	string::size_type pos = 0;
	string::size_type offset = 0;

	while ((pos = result.find(pattern, offset)) != string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}

	return result;
}
////////////////////////////////////////////////////////////////
//functions

hmmType* findPhone(char* name);
void createWordModel(wordModelArray& wordModels);
void createPriors(priorArray& priors);
void createWordTranxs(wordTranx* wordTranxs);
vector<string> viterbi_call(wordModelArray& wordModels, char* wordSeq, string fileLoc);
vector<string> viterbi_compute(btComp** backtrack, float** SeqProb, int r, int c, wordModelArray& wordModels, FILE* input);
float observation(stateType curState, float evi[N_DIMENSION]);
float maxViterbi_starting(float evi[N_DIMENSION], phoneType curPhone, int s, int j, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels);
float maxViterbi_Normal(float evi[N_DIMENSION], phoneType curPhone, int s, int j, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels);
float maxViterbi_midStart(float evi[N_DIMENSION], phoneType curPhone, int s, int j, wordModel thisWord, int l, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels);
float maxViterbi_Sp(float evi[N_DIMENSION], phoneType curPhone, int s, int j, wordModel thisWord, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels);
vector<string> getWordSeq(btComp** backtrack, int i, int c);
float tranxPenalty(vector<string> wordSeq);

/////////////////////////////////////////////////////////////////
//global variables
priorArray priors;
wordTranx wordTranxs[N_WORD * (N_WORD - 1)];

/////////////////////////////////////////////////////////////////


int main(void)
{

	wordModelArray wordModels;
	wordModels.wordModelCount = 0;
	wordModels.wordModels = (wordModel*)malloc(sizeof(wordModel) * 13);
	createWordModel(wordModels);
	createPriors(priors);
	createWordTranxs(wordTranxs);

	
	char* wordSeq = NULL;
	
	vector<string> answer;
	FILE* output = fopen("recognized.txt", "w");
	fprintf(output, "#!MLF!#\n");
	
	for (const fs::directory_entry& entry :
		 fs::recursive_directory_iterator(fs::current_path() / "tst")) {
		
		fs::path q = entry.path();
		if (!fs::is_directory(q)) {

			string fileLoc = q.string();
			answer = viterbi_call(wordModels, wordSeq, fileLoc);

			//process output
			int nameStart = fileLoc.find("tst");
			string newName = fileLoc.substr(nameStart, string::npos);
			newName = replaceAll(newName, "\\", "/");
			newName = replaceAll(newName, "txt", "rec");
			
			printf("%s\n", newName.c_str());

			fprintf(output, "\"");
			fprintf(output, "%s", newName.c_str());
			fprintf(output, "\"\n");
			
			for (int i = 0; i < answer.size(); i++) {
				fprintf(output, "%s\n", answer[i].c_str());
			}
			fprintf(output, ".\n");
			
		}
	}
	fclose(output);

	return 0;
}

//�ش� �̸��� phone ã��
hmmType* findPhone(const char* name)
{
	for (int i = 0; 1; i++) {
		if (strcmp(name, phones[i].name) == 0) {
			return &phones[i];
		}
	}
}

void createWordModel(wordModelArray& wordModels)
{	
	ifstream dictionary("dictionary.txt");		//read dictionary.txt

	//mC : how many word models are there? - 1
	int mC = wordModels.wordModelCount = -1;

	//list phone models of the word model
	if (dictionary.is_open()) {
		
		while (!dictionary.eof()) {
			mC++;
			//read one line from dictionary.txt and split the line using whitespaces
			string line;
			getline(dictionary, line);
			vector<string> pronounce = split(line);

			//new model's name is fist element of the vector
			wordModels.wordModels[mC].name = (char*)malloc(sizeof(char) * pronounce[0].length());
			strcpy(wordModels.wordModels[mC].name, pronounce[0].c_str());

			//c: how many phone models compose this word model?
			int c = wordModels.wordModels[mC].phoneCount = 0;

			//new model's phone models' names are 2nd ~ last element of the vector
			wordModels.wordModels[mC].phoneModels = (phoneType*)malloc(sizeof(phoneType) * pronounce.size());
			for (int i = 1; i < pronounce.size(); i++) {
				wordModels.wordModels[mC].phoneModels[c].phoneModel = findPhone(pronounce[i].c_str());
				wordModels.wordModels[mC].phoneModels[c++].word = wordModels.wordModels[mC].name;
			}

			wordModels.wordModels[mC].phoneCount = c;
		}
		wordModels.wordModelCount = mC + 1;
	}	
}

void createPriors(priorArray& priors)
{
	ifstream unigram("unigram.txt");			//read unigram.txt

	priors.priorCount = 0;
	priors.priors = (prior*)malloc(sizeof(prior) * 12);
	int pC = priors.priorCount = -1;
	if (unigram.is_open()) {
		//pC: prior ���� -1
		
		while (!unigram.eof()) {
			pC++;

			//read a line from unigram.txt
			string line;
			size_t sz;
			getline(unigram, line);
			vector<string> probability = split(line);
			if (!probability.size()) continue;

			priors.priors[pC].name = (char*)malloc(sizeof(char) * probability[0].length());
			strcpy(priors.priors[pC].name, probability[0].c_str());
			priors.priors[pC].probability = stod(probability[1], &sz);
		}
		priors.priorCount = pC;
	}
}

void createWordTranxs(wordTranx* wordTranxs)
{
	ifstream bigram("bigram.txt");				//read bigram.txt

	if (bigram.is_open()) {
		for (int i = 0; !bigram.eof(); i++) {
			//read a line from unigram.txt
			string line;
			size_t sz;
			getline(bigram, line);
			vector<string> T = split(line);
			if (!T.size()) continue;

			wordTranxs[i].prev = (char*)malloc(sizeof(char) * T[0].length());
			strcpy(wordTranxs[i].prev, T[0].c_str());

			wordTranxs[i].next = (char*)malloc(sizeof(char) * T[1].length());
			strcpy(wordTranxs[i].next, T[1].c_str());
			wordTranxs[i].probability = stod(T[2], &sz);
		}
	}
}

//viterbi algorithm caller
vector<string> viterbi_call(wordModelArray& wordModels, char* wordSeq, string fileLoc)
{
	//state ���� ����. word �𵨺��� ������ phone ���
	int stateNum = 0;
	for (int i = 0; i < wordModels.wordModelCount; i++) {
		stateNum += wordModels.wordModels[i].phoneCount * 3;
	}
	//optional silence model�� state�� 1���̹Ƿ� word���� state�� 2�� �� ����. <s>�� ���󺹱�
	stateNum = stateNum - wordModels.wordModelCount * 2 + 2;

	FILE* input = fopen(fileLoc.c_str(), "r");
	int time, dim;
	fscanf(input, "%d %d\n", &time, &dim);
	
	//ũ�⿡ �°� backtrack �迭(m') ����. ���� max state index�� ������ ����
	btComp** backtrack;
	backtrack = new btComp* [stateNum];
	for (int i = 0; i < stateNum; i++) {
		backtrack[i] = new btComp[time];
	}
	//���±��� �ְ� Ȯ�� �����ϴ� m �迭
	float** seqProb;
	seqProb = new float*[stateNum];
	for (int i = 0; i < stateNum; i++) {
		seqProb[i] = new float[time];
	}

	vector<string> bestSeq = viterbi_compute(backtrack, seqProb, stateNum, time, wordModels, input);

	fclose(input);
	return bestSeq;
}

/*
run viterbi algorithm .
best state sequence�� ã�Ƽ� �˷���. vector<string>����
*/
vector<string> viterbi_compute(btComp** backtrack, float** seqProb, int r, int c, wordModelArray& wordModels, FILE* input)
{
	float evi[N_DIMENSION] = { 0, };

	//j: row, i: column
	int j = 0, i = 0;

	//Initialize
	for (int i = 0; i < N_DIMENSION; i++) {
		fscanf(input, "%e", &evi[i]);
	}
	while (j < r) {
		
		//for all words..
		int k = 0; //for <s>
		for (int l = 0; l < wordModels.wordModels[k].phoneCount; l++) {
			phoneType curPhone = wordModels.wordModels[k].phoneModels[l];
			int s = 0;
			//state 1 of that phone
			backtrack[j][0] = { curPhone.word, -1 };
			seqProb[j++][0] = log((curPhone.phoneModel->tp[s][s + 1])) + observation(curPhone.phoneModel->state[s], evi);
			s++;

			//state 2 of that phone
			backtrack[j][0] = { curPhone.word, -1 };
			seqProb[j++][0] = log((curPhone.phoneModel->tp[s][s + 1])) + observation(curPhone.phoneModel->state[s], evi);
			s++;

			//state 3 of that phone
			backtrack[j][0] = { curPhone.word, -1 };
			seqProb[j++][0] = log((curPhone.phoneModel->tp[s][s + 1])) + observation(curPhone.phoneModel->state[s], evi);
		}

		for (int k = 1; k < wordModels.wordModelCount; k++) {
			//for all phones of that word...
			int l;
			for (l = 0; l < wordModels.wordModels[k].phoneCount - 1; l++) {
				phoneType curPhone = wordModels.wordModels[k].phoneModels[l];
				int s = 0;
				//state 1 of that phone
				backtrack[j][0] = {curPhone.word, -1};
				seqProb[j++][0] = log((curPhone.phoneModel->tp[s][s + 1])) + observation(curPhone.phoneModel->state[s], evi);
				s++;
				//state 2 of that phone
				backtrack[j][0] = { curPhone.word, -1 };
				seqProb[j++][0] = log((curPhone.phoneModel->tp[s][s + 1])) + observation(curPhone.phoneModel->state[s], evi);
				s++;
				//state 3 of that phone
				backtrack[j][0] = { curPhone.word, -1 };
				seqProb[j++][0] = log((curPhone.phoneModel->tp[s][s + 1])) + observation(curPhone.phoneModel->state[s], evi);
			}
			//last phone model is optional silence.
			backtrack[j][0] = { wordModels.wordModels[k].phoneModels[l].word, -1 };
			seqProb[j++][0] = log(wordModels.wordModels[k].phoneModels[l].phoneModel->tp[0][1]) + observation(wordModels.wordModels[k].phoneModels[l].phoneModel->state[0], evi);
		}
	}

	//Iteration for all time
	for (int t = 1; t < c; t++) {
		//j is for traversing all current time's states
		j = 0;

		//read one evidence
		for (int i = 0; i < N_DIMENSION; i++) {
			fscanf(input, "%e", &evi[i]);
		}

		//find maximum viterbi values for each state
		while (j < r) {
			int maxIndex = -1;

			//for all words..
			//for <s>
			int k = 0;
			phoneType curPhone = wordModels.wordModels[k].phoneModels[0];

			//state 1 of <s>
			maxIndex = -1;
			seqProb[j][t] = maxViterbi_starting(evi, curPhone, 0, j, maxIndex, seqProb, t, r, wordModels);
			backtrack[j][t] = { curPhone.word, maxIndex };
			j++;

			//state 2 of <s>
			maxIndex = -1;
			seqProb[j][t] = maxViterbi_Normal(evi, curPhone, 1, j, maxIndex, seqProb, t, r, wordModels);
			backtrack[j][t] = { curPhone.word, maxIndex };
			j++;

			//state 3 of <s>
			maxIndex = -1;
			seqProb[j][t] = maxViterbi_Normal(evi, curPhone, 2, j, maxIndex, seqProb, t, r, wordModels);
			backtrack[j][t] = { curPhone.word, maxIndex };
			j++;

			for (k = 1; k < wordModels.wordModelCount; k++) {
				//for all phones of that word...
				wordModel thisWord = wordModels.wordModels[k];
				int l;
				for (l = 0; l < wordModels.wordModels[k].phoneCount - 1; l++) {
					curPhone = wordModels.wordModels[k].phoneModels[l];
					int s = 0;

					//word model�� ���� phone
					if (l == 0) {
						//state 1 of that phone
						maxIndex = -1;
						seqProb[j][t] = maxViterbi_starting(evi, curPhone, s, j, maxIndex, seqProb, t, r, wordModels);
						backtrack[j][t] = { curPhone.word, maxIndex };
						s++;  j++;
					}

					//word model�� �߰� �Ǵ� �� phone
					else {
						//state 1 of that phone
						maxIndex = -1;
						seqProb[j][t] = maxViterbi_midStart(evi, curPhone, s, j, thisWord, l, maxIndex, seqProb, t, r, wordModels);
						backtrack[j][t] = { curPhone.word, maxIndex };
						s++; j++;
					}

					//state 2 of that phone
					maxIndex = -1;
					seqProb[j][t] = maxViterbi_Normal(evi, curPhone, s, j, maxIndex, seqProb, t, r, wordModels);
					backtrack[j][t] = { curPhone.word, maxIndex };
					s++; j++;
											
					//state 3 of that phone
					maxIndex = -1;
					seqProb[j][t] = maxViterbi_Normal(evi, curPhone, s, j, maxIndex, seqProb, t, r, wordModels);
					backtrack[j][t] = { curPhone.word, maxIndex };
					j++;
				}

				//last phone model of the word is optional silence.
				maxIndex = -1;
				seqProb[j][t] = maxViterbi_Sp(evi, thisWord.phoneModels[l], 0, j, thisWord, maxIndex, seqProb, t, r, wordModels);
				backtrack[j][t] = { thisWord.phoneModels[l].phoneModel->name, maxIndex };
				j++;
			}
		}
	}
	
	//�� �������� max probability state sequence�� ã���� �ȴ�.
	float maxProb = -INF;
	int maxIndex = -1;
	int maxSize = 0;
	vector<string> maxWordSeq;
	vector<string> tempWordSeq;

	for (int i = 0; i < r; i++) {
		tempWordSeq = getWordSeq(backtrack, i, c);
		//if (tempWordSeq.size() == 1) continue;

		float WTP = tranxPenalty(tempWordSeq);
		float prob = seqProb[i][c - 1] + WTPFactor * WTP;

		if (maxProb < prob) {
			maxProb = prob;
			maxIndex = i;
			maxWordSeq = tempWordSeq;
		}
	}
	return maxWordSeq;
}

//�ڱ� �ڽ�(��ü index�� j) �Ǵ� �ٷ� �� word�� �� phone�� �� �ٷ� ���� state���� ������ �� �� ����.
float maxViterbi_Normal(float evi[N_DIMENSION], phoneType curPhone, int s, int j, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels)
{
	float maxProb = -INF, temp = -INF, obsv = observation(curPhone.phoneModel->state[s], evi);
	int p = 0;

	//�ڱ� �ڽ����� �ʱ�ȭ. state�� s + 1�� �� index
	maxProb = seqProb[j][t - 1] + log(curPhone.phoneModel->tp[s + 1][s + 1]) + obsv;
	maxIndex = j;

	//�� �ٷ� �� state�� �̵�
	p = j - 1;
	temp = seqProb[p][t - 1] + log(curPhone.phoneModel->tp[s][s + 1]) + obsv;

	if (maxProb < temp) {
		maxProb = temp;
		maxIndex = p;
	}

	return maxProb;
}

//�ڱ� �ڽ�(��ü index�� j), �Ǵ� �ٸ� word�� sp�� �ƴ� ������ phone�� ������ state, sp���� �� �� ����.
float maxViterbi_starting(float evi[N_DIMENSION], phoneType curPhone, int s, int j, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels)
{
	float maxProb = -INF, temp = -INF, obsv = observation(curPhone.phoneModel->state[s], evi);
	int p = 0;

	//�ڱ� �ڽ����� �ʱ�ȭ
	maxProb = seqProb[j][t - 1] + log(curPhone.phoneModel->tp[s + 1][s + 1]) + obsv;
	maxIndex = j;

	for (int k = 0; k < wordModels.wordModelCount; k++) {
		wordModel prevWord = wordModels.wordModels[k];
		
		//<s>. sil�� 3 state���� �Ѿ�� �� ����.
		if (k == 0) {
			//<s>�� 3��° state�� ����Ű�� �̵�
			p = 2;
			//self-loop�� �ƴ� ���
			if (p != j) {
				temp = seqProb[p][t - 1] + log(prevWord.phoneModels[0].phoneModel->tp[3][4]) + log(curPhone.phoneModel->tp[s][s + 1]) + obsv;

				if (maxProb < temp) {
					maxProb = temp;
					maxIndex = p;
				}
			}
		}

		//�ٸ� word��
		else {
			phoneType prevPhone = prevWord.phoneModels[prevWord.phoneCount - 2];
			//������ phone�� 3��° state�� ����Ű�Բ� �̵�
			p += (prevWord.phoneCount - 1) * 3;
			
			//self-loop�� �ƴ� ���
			if (p != j) {
				temp = seqProb[p][t - 1] + log(prevPhone.phoneModel->tp[3][4]) + log(curPhone.phoneModel->tp[s][s + 1]) + obsv;

				if (maxProb < temp) {
					maxProb = temp;
					maxIndex = p;
				}
			}

			//sp
			p++;
			temp = seqProb[p][t - 1] + log(findPhone("sp")->tp[1][2]) + log(curPhone.phoneModel->tp[s][s + 1]) + obsv;
			
			if (maxProb < temp) {
				maxProb = temp;
				maxIndex = p;
			}
		}
	}
	return maxProb;
}

//�ڱ� �ڽ�(��ü index�� j), �Ǵ� �� word�� �ٷ� ���� phone�� 3��° state���� �� �� ����.
//l: phone ��ȣ. curPhone�� l��°.
float maxViterbi_midStart(float evi[N_DIMENSION], phoneType curPhone, int s, int j, wordModel thisWord, int l, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels)
{
	float maxProb = -INF, temp = -INF, obsv = observation(curPhone.phoneModel->state[s], evi);
	int p = 0;

	//�ڱ� �ڽ����� �ʱ�ȭ
	maxProb = seqProb[j][t - 1] + log(curPhone.phoneModel->tp[s + 1][s + 1]) + obsv;
	maxIndex = j;

	//�� word�� �ٷ� ���� phone�� 3��° state�� �̵�
	p = j - 1;
	temp = seqProb[p][t - 1] + log(thisWord.phoneModels[l - 1].phoneModel->tp[3][4]) + log(curPhone.phoneModel->tp[0][1]) + obsv;

	if (maxProb < temp) {
		maxProb = temp;
		maxIndex = p;
	}

	return maxProb;
}

/*
curState�� optional silence�� ��.
�ڱ� �ڽ�(��ü index�� j), �Ǵ� �ٷ� �� word�� ������ phone�� 3��° state (�� �ٷ� �� state)���� �� �� ����.
*/
float maxViterbi_Sp(float evi[N_DIMENSION], phoneType curPhone, int s, int j, wordModel thisWord, int& maxIndex, float** seqProb, int t, int r, wordModelArray& wordModels)
{
	float maxProb = -INF, temp = -INF, obsv = observation(curPhone.phoneModel->state[s], evi);
	int p = 0;

	//�ڱ� �ڽ����� �ʱ�ȭ
	maxProb = seqProb[j][t - 1] + log(findPhone("sp")->tp[1][1]) + obsv;
	maxIndex = j;

	//�ٷ� �� word�� ������ phone�� 3��° state�� �̵�
	p = j - 1;
	temp = seqProb[p][t - 1] + log(thisWord.phoneModels[thisWord.phoneCount - 2].phoneModel->tp[3][4]) + log(findPhone("sp")->tp[0][1]) + obsv;

	if (maxProb < temp) {
		maxProb = temp;
		maxIndex = p;
	}
	return maxProb;
}

float observation(stateType curState, float evi[N_DIMENSION])
{
	float b1 = 0, b2 = 0, result = 0;
	float proj_sig[2] = { 1.0, 1.0 };

	for (int i = 0; i < N_DIMENSION; i++) {
		b1 += pow((evi[i] - curState.pdf[0].mean[i]), 2) / curState.pdf[0].var[i];
		b2 += pow((evi[i] - curState.pdf[1].mean[i]), 2) / curState.pdf[1].var[i];
		proj_sig[0] *= pow(curState.pdf[0].var[i], 0.5);
		proj_sig[1] *= pow(curState.pdf[1].var[i], 0.5);
	}
	b1 *= -0.5;
	b2 *= -0.5;

	if (b1 > b2) {
		result = b1 + log(curState.pdf[0].weight * (1 / (pow(2 * M_PI, N_DIMENSION / 2.0) * proj_sig[0])) + curState.pdf[1].weight * (1 / (pow(2 * M_PI, N_DIMENSION / 2.0) * proj_sig[1])) * exp(b2 - b1));
		//printf("%f\n", result);
		return result;
	}
	else {
		result = b2 + log(curState.pdf[0].weight * (1 / (pow(2 * M_PI, N_DIMENSION / 2.0) * proj_sig[0])) * exp(b1 - b2) + curState.pdf[1].weight * (1 / (pow(2 * M_PI, N_DIMENSION / 2.0) * proj_sig[1])));
		//printf("%f\n", result);
		return result;
	}
}

/*
�־��� element���� backtracking�Ͽ� word sequence�� ���� �˰���.
vector�� ��� ���� string�� �ٸ� string�� ����. "sp"�� ���� ����.
*/
vector<string> getWordSeq(btComp** backtrack, int i, int c)
{
	int prev = i;
	vector<string> words;
	for (int k = c - 1; k >= 0; k--) {
		string str(backtrack[prev][k].myWord);
		//printf("%s ", str.c_str());
		if (words.empty()) {
			if(str.compare("<s>") != 0 && str.compare("sp") != 0)
				words.push_back(str);
		}
		//����ִ� �� �ƴϰ�, silence / optional silence�� �ƴ϶�� ���ڿ��� �߰�
		else if(words.back().compare(str) != 0 && str.compare("<s>") != 0 && str.compare("sp") != 0) {
			words.push_back(str);
		}
		prev = backtrack[prev][k].index;
	}
	//printf("\n");
	return words;
}

/*
Uni/bigram�� �̿��� word transition penalty�� ���
*/
float tranxPenalty(vector<string> wordSeq)
{
	float result = 0;

	for (int i = 0; i < priors.priorCount; i++) {
		if (strcmp(priors.priors[i].name, wordSeq[0].c_str()) == 0) {
			result += log(priors.priors[i].probability);
			break;
		}
	}

	for (int i = 0; i < wordSeq.size() - 1; i++) {
		for (int j = 0; j < N_WORD * (N_WORD - 1); j++) {
			if (strcmp(wordSeq[i].c_str(), wordTranxs[j].prev) == 0 && strcmp(wordSeq[i + 1].c_str(), wordTranxs[j].next) == 0) {
				result += log(wordTranxs[j].probability);
				break;
			}
		}
	}
	return result;
}