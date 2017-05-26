#include <iostream>
#include <string.h>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
#define TAG_SIZE unsigned short


// Check if file exists
bool fileExists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

// Used to store tags
class converter
{
	map<string, TAG_SIZE> tag;
	public:
	void init(string file);
	TAG_SIZE operator [] (string tag_name)
	{
		if(tag[tag_name] == 0)
			return -1;
		return tag[tag_name];
	}
};

// Read tags from file and assign values
void converter::init(string file)
{
	if(!fileExists(file.c_str()))
	{
		cout<<"Error, "<<file<<" does not exist!\n";
		return;
	}
	ifstream f(file.c_str(), ios::in);
	int i = 0;
	while(!f.eof())
	{
		string s;
		getline(f, s);
		tag[s] = i++;
	}
	
}

// Read
void assign_vals(string directory, converter tags)
{
	int i = 0;
	if(fileExists((directory + "save_state.sav").c_str()))
	{
		cout<<"Save found, continue from this location? (1 = Yes, 0 = No)\n";
		int opt;
		cin>>opt;
		if(opt == 0)
			remove((directory + "save_state.sav").c_str());
		else
		{
			ifstream in((directory + "save_state.sav").c_str());
			in>>i;
			++i;
		}
	}
	string word;
	for(;; ++i)
	{
		
		if(!fileExists((directory + to_string(i) + ".txt").c_str()) || !fileExists((directory + to_string(i) + ".key").c_str()))
		{
			cout<<"Reached "<<i<<".txt\n";
			break;
		}
		ifstream f((directory + to_string(i) + ".txt").c_str(), ios::in);
		ifstream vals((directory + to_string(i) + ".key").c_str(), ios::in);
		vector<TAG_SIZE> v;
		map<string, int> counter;
		while(!vals.eof())
		{
			string s;
			getline(vals, s);
			if(tags[s] != -1)
				v.push_back(tags[s]);
		}
		bool skip = false;
		while(!f.eof())
		{
			char l;
			f.read((char *)&l, sizeof(l));
			if(l == '>')
				skip = false;
			else if(l == '<')
				skip = true;
			if(skip)
				continue;
			if(l == '\'')
				continue;
			if(!isalpha(l))
			{
				if(word.length() != 0)
				{
					counter[word]++;
				}
				word.clear();
			}
			else
			{
				word += tolower(l);
			}
		}
		for(map<string, int>::iterator c_iter = counter.begin(); c_iter != counter.end(); ++c_iter)
		{
			map<TAG_SIZE, int> inp;
			string path = ("Data/" + c_iter->first + ".word");
			if(fileExists(path.c_str()))
			{
				ifstream infile(path.c_str(), ios::in | ios::binary);
				TAG_SIZE t;
				int val;
				while(infile.read((char *) &t, sizeof(t)))
				{
					infile.read((char *) &val, sizeof(val));
					inp[t] = val;
				}
				infile.close();
			}
			for(vector<TAG_SIZE>::iterator it = v.begin(); it != v.end(); ++it)
				inp[*it] += c_iter->second;
			ofstream outfile(path.c_str(), ios::out | ios::binary);
			for(map<TAG_SIZE, int>::iterator it = inp.begin(); it != inp.end(); ++it)
			{
				outfile.write((char *) &it->first, sizeof(it->first));
				outfile.write((char *) &it->second, sizeof(it->second));
			}
			outfile.close();
		}
		f.close();
		vals.close();
		ofstream out((directory + "save_state.sav").c_str(), ios::out);
		out<<i;
	}
}

int main(int argv, char *argc[])
{
	if(argv < 3)
	{
		cout<<"Argument format: Vocabulary Test_Location\n";
		return 1;
	}
	converter tag;
	tag.init(argc[1]);
	assign_vals(argc[2], tag);
	return 0;
}

