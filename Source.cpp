#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;//jrogjqeropvqeofvnpqejfovnjqenrvpijeqrvi

typedef struct {
	long long int address;
	int var;
}cblocks;

//Print file
void printfile(int hits, int misses, int uniquea, int addrsize, vector<vector<cblocks>>c, int columns, int rows) {
	ofstream fo;
	int i, j;
	float hitrate, missrate;
	hitrate = (float)hits / addrsize;
	missrate = (float)misses / addrsize;
	fo.open("output.txt", ios::out);
	fo << "Cache_accesses = " << addrsize << endl;
	fo << "Total_number_of_unique_addresses = " << uniquea << endl;
	fo << "Total_number_of_misses = " << misses << endl;
	fo << "Total_number_of_hits = " << hits << endl;
	fo << "Miss_rate = " << missrate * 100 << endl;
	fo << "Hit_rate = " << hitrate * 100 << endl;
	fo << endl;
	fo << "--------Cache Contents---------" << endl << endl;
	for (i = 0; i < rows; i++) {
		fo << "# " << i;
		for (j = 0; j < columns; j++) {
			fo << " " << c[i][j].address;
		}
		fo << " " << endl;
	}
	fo.close();
}

//Finds the index of the given address
int findindex(vector<cblocks>c, long long int v) {
	for (int i = 0; i < c.size(); i++) {
		if (c[i].address == v) {
			return i;
		}
	}
	return -1;
}

//Finds the index with the specific RRPV
int findindexvar(vector<cblocks>c, long long int v) {
	for (int i = 0; i < c.size(); i++) {
		if (c[i].var == v) {
			return i;
		}
	}
	return -1;
}

//Replaces the address when miiss and updates the replacement variavle when hit
void replacelru(vector<cblocks>& c, long long int addr, int found) {
	int i, index;
	if (found == 0) {
		index = findindex(c, 0);
		if (index != -1) {
			for (i = 0; i < index; i++) {
				c[i].var++;
			}
			c[i].address = addr;
			c[i].var = 0;
		}
		else {
			int maxindex = INT_MIN;
			int maxvar = INT_MIN;
			for (i = 0; i < c.size(); i++) {
				if (c[i].var > maxvar) {
					maxvar = c[i].var;
					maxindex = i;
				}
			}
			c[maxindex].address = addr;
			c[maxindex].var = 0;
			for (i = 0; i < c.size(); i++) {
				if (i == maxindex) {
					continue;
				}
				c[i].var++;
			}
		}
	}
	else {
		for (i = 0; i < c.size(); i++) {
			if (c[i].address == addr) {
				c[i].var = 0;
			}
			else {
				if (c[i].address != 0) {
					c[i].var++;
				}
			}
		}
	}
}

//finds if the address hits or misses
bool LRU(vector<cblocks>& c, long long int addr) {
	int index;
	index = findindex(c, addr);
	if (index != -1) {
		replacelru(c, addr, 1);
		return true;
	}
	else {
		replacelru(c, addr, 0);
		return false;
	}
}

void replacesrrip(vector<cblocks>& c, long long int addr, int found, int bits, int indx) {
	int i, j, index;
	if (found == 0) {
		index = findindexvar(c, (pow(2, bits) - 1));
		while (index == -1) {
			for (i = 0; i < c.size(); i++) {
				c[i].var++;
			}
			index = findindexvar(c, (pow(2, bits) - 1));
		}
		c[index].address = addr;
		c[index].var = (pow(2, bits) - 2);
	}
	else {
		c[indx].var = 0;
	}
}

bool SRRIP(vector<cblocks>& c, long long int addr, int bits) {
	int index;
	index = findindex(c, addr);
	if (index != -1) {
		replacesrrip(c, addr, 1, bits, index);
		return true;
	}
	else {
		replacesrrip(c, addr, 0, bits, -1);
		return false;
	}
}

void main() {
	ifstream fi;
	vector<long long int>address;
	vector<long long int>unique;
	vector<vector<cblocks>>cache;
	vector<long long int>::iterator it;
	int blocks, ways, brp, M, found, hits, misses, i, j, index;
	bool hitormiss;
	hits = 0;
	misses = 0;
	long long int addr;
	fi.open("input.txt", ios::in);
	if (!(fi.is_open())) {
		cout << "File open failed" << endl;
		return;
	}
	//Read file and input all addresses to vector
	while (!fi.eof()) {
		fi >> addr;
		address.push_back(addr);
		it = find(unique.begin(), unique.end(), addr);
		if (it == unique.end()) {
			unique.push_back(addr);
		}
	}
	fi.close();
	//Print menu
	cout << "Give the size of cache(blocks): ";
	cin >> blocks;
	while (blocks < 1) {
		cout << "Error, give a larger number:\t";
		cin >> blocks;
	}
	cout << "\n\nType of Cache\n";
	cout << "0. Fully Associative(FA)\n";
	cout << "1. Direct Mapped(DM)\n";
	cout << "2+. Set-Associative(SA)\n";
	cout << "\nGive your choice. If you want SA give the number of sets:\t";
	cin >> ways;
	while (ways < 0) {
		cout << "Error, give another choice: ";
		cin >> ways;
	}
	if (ways == 0 || ways >= 2) {
		cout << "\n\nBlock replacement policy\n";
		cout << "1. Least-Recently Used(LRU)\n";
		cout << "2. Static Re-Reference Interval Prediction(SRRIP)\n";
		cout << "\nGive your choice: ";
		cin >> brp;
		while (brp < 1 || brp>2) {
			cout << "\nError, give another choice: ";
			cin >> brp;
		}
		if (brp == 2) {
			cout << "\nGive the number of bits of Re-Reference Prediction value(RRPV): ";
			cin >> M;
			while (M < 1) {
				cout << "\nError,give a larger number: ";
				cin >> M;
			}
		}
	}
	switch (ways) {
	case 0: {
		cache.resize(1);
		cache[0].resize(blocks);
		for (j = 0; j < cache[0].size(); j++) {
			cache[0][j].address = 0;
			cache[0][j].var = -1;
		}
		if (brp == 1) {
			for (index = 0; index < address.size(); index++) {
				hitormiss = LRU(cache[0], address[index]);
				if (hitormiss == true)
					hits++;
				else
					misses++;
			}
		}
		else {
			for (index = 0; index < address.size(); index++) {
				hitormiss = SRRIP(cache[0], address[index], M);
				if (hitormiss == true) {
					hits++;
				}
				else if (hitormiss == false) {
					misses++;
				}
			}
		}
		printfile(hits, misses, unique.size(), address.size(), cache, cache[0].size(), 1);
		break;
	}
	case 1: {
		cache.resize(blocks);
		for (i = 0; i < cache.size(); i++) {
			cache[i].resize(1);
			cache[i][0].address = 0;
		}
		for (i = 0; i < address.size(); i++) {
			index = address[i] % blocks;
			if (cache[index][0].address != address[i]) {
				cache[index][0].address = address[i];
				misses++;
			}
			else {
				hits++;
			}
		}
		printfile(hits, misses, unique.size(), address.size(), cache, 1, cache.size());
		break;
	}
	default: {
		int numofindexes = blocks / ways;
		cache.resize(numofindexes);
		for (i = 0; i < cache.size(); i++) {
			cache[i].resize(ways);
			for (j = 0; j < cache[i].size(); j++) {
				cache[i][j].address = 0;
			}
		}
		if (brp == 1) {
			for (i = 0; i < address.size(); i++) {
				index = address[i] % numofindexes;
				hitormiss = LRU(cache[index], address[i]);
				if (hitormiss == true) {
					hits++;
				}
				else if (hitormiss == false) {
					misses++;
				}
			}
		}
		else {
			for (i = 0; i < cache.size(); i++) {
				for (j = 0; j < cache[i].size(); j++) {
					cache[i][j].var = (pow(2, M) - 1);
				}
			}
			for (i = 0; i < address.size(); i++) {
				index = address[i] % numofindexes;
				hitormiss = SRRIP(cache[index], address[i], M);
				if (hitormiss == true) {
					hits++;
				}
				else if (hitormiss == false) {
					misses++;
				}
			}
		}
		printfile(hits, misses, unique.size(), address.size(), cache, cache[0].size(), cache.size());
		break;
	}
	}
}
