#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <random>
#include <algorithm>


#define uint unsigned

using namespace std;

const uint c_w = 130;
const uint w = 450;
const uint h = 29;

const int maze_h = (h-1)/2;
const int maze_w = 51; // 81 = 2*41 - 1

// Flaga: JHtC4BSK{_IT_S_3AS1ER_WITH_MITM_4136}
vector<string> flag = {
"       ## ##     ##   ##     ######  ##        ########   ######  ##    ##   ####         #### ########          ######           #######     ###     ######     ##   ######## ########          ##      ## #### ######## ##     ##         ##     ## #### ######## ##     ##         ##           ##    #######   #######  ####      ",
"       ## ##     ##   ##    ##    ## ##    ##  ##     ## ##    ## ##   ##   ##             ##     ##            ##    ##         ##     ##   ## ##   ##    ##  ####   ##       ##     ##         ##  ##  ##  ##     ##    ##     ##         ###   ###  ##     ##    ###   ###         ##    ##   ####   ##     ## ##     ##    ##     ",
"       ## ##     ## ######  ##       ##    ##  ##     ## ##       ##  ##    ##             ##     ##            ##                      ##  ##   ##  ##          ##   ##       ##     ##         ##  ##  ##  ##     ##    ##     ##         #### ####  ##     ##    #### ####         ##    ##     ##          ## ##           ##     ",
"       ## #########   ##    ##       ##    ##  ########   ######  #####    ###             ##     ##             ######           #######  ##     ##  ######     ##   ######   ########          ##  ##  ##  ##     ##    #########         ## ### ##  ##     ##    ## ### ##         ##    ##     ##    #######  ########     ###    ",
" ##    ## ##     ##   ##    ##       ######### ##     ##       ## ##  ##    ##             ##     ##                  ##                ## #########       ##    ##   ##       ##   ##           ##  ##  ##  ##     ##    ##     ##         ##  #  ##  ##     ##    ##  #  ##         #########    ##          ## ##     ##    ##     ",
" ##    ## ##     ##   ## ## ##    ##       ##  ##     ## ##    ## ##   ##   ##             ##     ##            ##    ##         ##     ## ##     ## ##    ##    ##   ##       ##    ##          ##  ##  ##  ##     ##    ##     ##         ##     ##  ##     ##    ##     ##               ##     ##   ##     ## ##     ##    ##     ",
"  ######  ##     ##    ###   ######        ##  ########   ######  ##    ##   #### ####### ####    ##    #######  ######  #######  #######  ##     ##  ######   ###### ######## ##     ## #######  ###  ###  ####    ##    ##     ## ####### ##     ## ####    ##    ##     ## #######       ##   ######  #######   #######  ####      "
};

uint64_t constexpr prime_m1 = 1007012107;
uint64_t constexpr prime_m2 = 179426549;
uint64_t constexpr prime_m3 = 1500450271;


uint64_t mul_mod(uint64_t const x, uint64_t const y, uint64_t const prime) {
    return (x*y) % prime;
}

int64_t mul_inv(int64_t a, int64_t b){
	int64_t b0 = b, t, q;
	int64_t x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1){
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;

	return x1;
}


random_device rd;
uint64_t seed = rd();
mt19937 gen(seed);

const uint FLAG_EMPTY = 0;
const uint FLAG_BLOCKED = 1;
const uint FLAG_TARGET = 2;
const uint FLAG_VISITED = 4;

const pair<int,int> N = {0,-1};
const pair<int,int> S = {0,1};
const pair<int,int> E = {1,0};
const pair<int,int> W = {-1,0};

uint dir_flag(pair<int,int> dir){
	if(dir == N)
		return 1;
	if(dir == S)
		return 2;
	if(dir == E)
		return 4;
	if(dir == W)
		return 8;
	assert(false);
}

pair<int,int> opposite(pair<int, int> dir){
	return {-1*dir.first, -1*dir.second};
}

void dfs(uint cx, uint cy, vector<vector<uint>> &maze){
	vector<pair<int,int>> d = {N, S, E, W};
	shuffle(d.begin(), d.end(), gen);

	for(auto dir : d){
		int nx = cx + dir.first;
		int ny = cy + dir.second;
		if(ny >= 0 && ny <= maze_h -1 && nx >= 0 && nx <= maze_w - 1 && maze[ny][nx] == 0U){
			maze[cy][cx] |= dir_flag(dir);
			maze[ny][nx] |= dir_flag(opposite(dir));
			dfs(nx, ny, maze);
		}
	}
}

bool bdfs(int x, int y, uint dist, vector<vector<uint>> &m){
	const vector<pair<int,int>> dir = {N,S,E,W};
	m[y][x] |= FLAG_VISITED;

	bool is_accessible = false;
	for(auto d : dir){
		int nx = x+d.first;
		int ny = y+d.second;

		if(nx >= 0 && nx < (int)m[0].size() && ny >= 0 && ny < (int)m.size())
			if(!(m[ny][nx] & FLAG_VISITED) && !(m[ny][nx] & FLAG_BLOCKED))
				is_accessible |= bdfs(nx, ny, dist+1, m);
	}

	if(m[y][x] & FLAG_TARGET)
		return true;

	if(is_accessible){
		if(dist == 0)
			m[y][x] |= FLAG_BLOCKED;
		uniform_int_distribution<> dis(0,2*dist);
		if(dis(gen) == 0)
			m[y][x] |= FLAG_BLOCKED;
	}

	return (is_accessible && !(m[y][x] & FLAG_BLOCKED));
}

vector<vector<uint>> maze_to_ascii(vector<vector<uint>> &maze){
	vector<vector<uint>> result;
	for(uint y=0;y<maze_h;++y){

		{
			vector<uint> cline;
			for(uint x=0;x<maze_w;++x){
				cline.push_back(FLAG_EMPTY);
				if(x != maze_w-1){
					if(maze[y][x] & dir_flag(E))
						cline.push_back(FLAG_EMPTY);
					else
						cline.push_back(FLAG_BLOCKED);
				}
			}
			result.push_back(cline);
		}

		if(y != maze_h - 1){
			vector<uint> cline;
			for(uint x=0;x<maze_w;++x){
				if(maze[y][x] & dir_flag(S))
					cline.push_back(FLAG_EMPTY);
				else
					cline.push_back(FLAG_BLOCKED);

				if(x != maze_w-1){
					uint count_adj = cline.back();
					if(x<maze_w-1 && !(maze[y][x+1] & dir_flag(S)))
						++count_adj;
					if(!(maze[y][x] & dir_flag(E)))
						++count_adj;
					if(y < maze_h-1 && !(maze[y+1][x] & dir_flag(E)))
						++count_adj;

					if(count_adj >= 2)
						cline.push_back(FLAG_BLOCKED);
					else
						cline.push_back(FLAG_EMPTY);
				}
			}

			result.push_back(cline);
		}
	}
	return result;
}

void clear_visited(vector<vector<uint>> &m){
	for(uint j=0;j<m.size();++j)
		for(uint k=0;k<m[j].size();++k)
			m[j][k] &= ~FLAG_VISITED;
}


bool cdfs(int x, int y, vector<vector<uint>> &m){
	const vector<pair<int,int>> dir = {N,S,E,W};
	m[y][x] |= FLAG_VISITED;

	bool is_accessible = false;
	for(auto d : dir){
		int nx = x+d.first;
		int ny = y+d.second;

		if(nx >= 0 && nx < (int)m[0].size() && ny >= 0 && ny < (int)m.size()){
			if(!(m[ny][nx] & FLAG_VISITED) && !(m[ny][nx] & FLAG_BLOCKED))
				is_accessible |= cdfs(nx, ny, m);
		}
	}

	if(m[y][x] & FLAG_TARGET)
		return true;

	return is_accessible;
}

bool check_if_impassable(vector<vector<uint>> &result){
	for(uint i=0;i<h-2;++i){
		clear_visited(result);
		if(!(result[i].back() & FLAG_BLOCKED) && cdfs(result[0].size()-1, i, result))
			return false;
	}
	return true;
}

int main(){

	int pos_x = 1;
   uniform_int_distribution<> dis1(1, prime_m1-1);
   uniform_int_distribution<> dis2(1, prime_m2-1);

	char ch;
   cin >> ch;
   if(ch != 'H')
		return 0;

   uint64_t cha = dis1(gen);
   uint64_t chb = dis2(gen);
   uint64_t chc = dis1(gen);
   uint64_t chd = dis1(gen);

	cout << cha << endl;
   cout << chb << endl;
   cout << chc << endl;
   cout << chd << endl;

	uint64_t a, b;

	cin >> a;
	cin >> b;
	getchar();
	uint64_t na1 = chb ^ cha ^ a;//b ^ a ^ inv(c)
	uint64_t nb1 = chc ^ chd ^ b;//c ^ d ^ inv(b)
	uint64_t na2 = chb ^ cha ^ b;
	uint64_t nb2 = chc ^ chd ^ a;

	if((a < b) &&
		((mul_mod(chc, na1, prime_m1)==1 && mul_mod(chb, nb1, prime_m2)==1) || (mul_mod(chc, na2, prime_m1)==1 && mul_mod(chb, nb2, prime_m2)==1))){
		cout << "K" << endl;
	} else {
		cout << "F" << endl;
		return 0;
	}

	uint64_t col_key = a ^ b ^ prime_m2;


	uint fail_count = 0;
	vector<vector<uint>> result;
	do {
		vector<vector<uint>> maze(maze_h, vector<uint>(maze_w, 0));

		dfs(0,0,maze);
		result = maze_to_ascii(maze);

		for(uint i=0;i<h-2;++i)
			result[i][0] |= FLAG_TARGET;

		// This isn't always correct...
		for(uint i=0;i<h-2;++i)
			if(!(result[i].back() & FLAG_BLOCKED)){
				bdfs(result[0].size()-1,i,0,result);
				clear_visited(result);
			}
		++fail_count;
		if(fail_count > 200){
			cout << "There was a problem" << endl;
			return 0;
		}
		// DO IT TILL WE GET A CORRECT RESULT.
	} while(!check_if_impassable(result));


	/*for(auto y : result){
		for(auto x : y)
			if(x & FLAG_BLOCKED)
				cout << "#";
			else
				cout << " ";
		cout << "\n";
	}

	return 0;
	*/


	while((ch = getchar()) != EOF) {
		if(ch == 'R'){
			pos_x++;
		} else if(ch == 'L'){
			pos_x--;
		} else if(ch == 'C'){
			uint64_t line_n;
			cin >> line_n;
			line_n ^= col_key;
			line_n = mul_inv(line_n, prime_m3)-2;
			if(line_n > pos_x+c_w){
				cout << "No." << endl;
				return 0;
			}
			if(line_n == 0 || line_n == w-2){
				cout << "11111111111111111111111111111" << endl;
			} else if(line_n < 10) {
				cout << "10000000000000000000000000001" << endl;
			} else if(line_n >= 10 && line_n <= 110){
				cout << 1;
				for(uint i=0;i<h-2;++i){
					if(result[i][line_n-10] & FLAG_BLOCKED)
						cout << "1";
					else
						cout << "0";
				}
				cout << 1 << endl;
			} else if(line_n>=115&&line_n<=115+flag[0].size()) {
				cout << "1";
				for(uint i=0;i<10;++i)
					cout << "0";
				for(uint i=0;i<7;++i)
					cout << (flag[i][line_n-115] == '#' ? "1" : "0");
				for(uint i=0;i<10;++i)
					cout << "0";
				cout << "1" << endl;
			} else {
				cout << "10000000000000000000000000001" << endl;
			}
		}
	}

   return 0;
}
