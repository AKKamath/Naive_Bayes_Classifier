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
bool sorter(pair<TAG_SIZE, int> a, pair<TAG_SIZE, int> b)
{
	return a.second > b.second;
}
// Used to store tags
class converter
{
	map<string, TAG_SIZE> tag;
	map<TAG_SIZE, string> tag2;
	public:
	void init(string file);
	TAG_SIZE operator [] (string tag_name)
	{
		if(tag[tag_name] == 0)
			return -1;
		return tag[tag_name];
	}
	string operator [](unsigned short val)
	{
		return tag2[val];
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
	int i = 1;
	string s;
	while(getline(f, s))
	{
		tag2[i] = s;
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
		{
			remove((directory + "save_state.sav").c_str());
		}
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
		map<TAG_SIZE, int> possible_tags;
		if(!fileExists((directory + to_string(i) + ".txt").c_str()))
		{
			cout<<"Reached "<<i<<".txt\n";
			break;
		}
		ifstream f((directory + to_string(i) + ".txt").c_str(), ios::in);
		ofstream vals((directory + to_string(i) + ".key").c_str(), ios::out);
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
					string path = ("Data/" + word + ".word");
					if(fileExists(path.c_str()))
					{
						map<TAG_SIZE, int> inp;
						ifstream infile(path.c_str(), ios::in | ios::binary);
						TAG_SIZE t;
						int val;
						while(infile.read((char *) &t, sizeof(t)))
						{
							infile.read((char *) &val, sizeof(val));
							inp[t] = val;
						}
						if(inp.size() < 10)
							for(map<TAG_SIZE, int>::iterator it = inp.begin(); it != inp.end(); ++it)
								possible_tags[it->first] += it->second;
						infile.close();
					}
				}
				word.clear();
			}
			else
			{
				word += tolower(l);
			}
		}
		vector<pair<TAG_SIZE, int> > v;
		copy(possible_tags.begin(), possible_tags.end(), back_inserter(v));
		sort(v.begin(), v.end(), sorter);
		char x = 0;
		for(vector<pair<TAG_SIZE, int> >::iterator it = v.begin(); it != v.end(); ++it)
		{
			vals<<tags[it->first]<<" "<<it->second<<"\n";
			if(x > 5)
				break;
			++x;
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


















