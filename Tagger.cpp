#include <iostream>
#include <string.h>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
#define TAG_SIZE unsigned short


// A trie used to make reading words quick and efficient
struct letter
{
	letter *next[26];
	map<TAG_SIZE, int> tags;
	letter()
	{
		// Initialize trie with empty characters
		for(int i = 0; i < 26; ++i)
			next[i] = NULL;
	}
	// Function to display words gathered
	void recursify(string now);
	void save_to_file(string path);
	void read_from_file(string path);
};
bool sorter(pair<TAG_SIZE, int> a, pair<TAG_SIZE, int> b)
{
	return a.second > b.second;
}
// Output words along with tagged entities
void letter::recursify(string now)
{
	// Current letter is the end of a word
	if(!tags.empty())
	{
		// Output the word so far
		cout<<now<<" ";
		vector<pair<TAG_SIZE, int> > v;
		copy(tags.begin(), tags.end(), back_inserter(v));
		sort(v.begin(), v.end(), sorter);
		for(vector<pair<TAG_SIZE, int> >::iterator it = v.begin(); it != v.end(); ++it)
		{
			cout<<it->first<<".."<<it->second<<"  ";
		}
		cout<<"\n";
	}
	for(int i = 0; i < 26; ++i)
	{
		if(next[i] != NULL)
		{
			next[i]->recursify(now + (char)(i + 'a'));
		}
	}
}
void letter::read_from_file(ifstream file)
{
	ofstream out(path.c_str(), ios::app | ios::binary);
	out.write((char *)this, sizeof(letter));	
	if(!tags.empty())
	{
		vector<pair<TAG_SIZE, int> > v;
		copy(tags.begin(), tags.end(), back_inserter(v));
		copy(v.begin(), v.end(), std::ostreambuf_iterator<char>(out));
	}
	out.close();
	for(int i = 0; i < 26; ++i)
	{
		if(next[i] != NULL)
		{
			next[i]->save_to_file(path);
		}
	}
}
void letter::save_to_file(ofstream file)
{
	file.write((char *)this, sizeof(letter));	
	if(!tags.empty())
	{
		vector<pair<TAG_SIZE, int> > v;
		copy(tags.begin(), tags.end(), back_inserter(v));
		copy(v.begin(), v.end(), std::ostreambuf_iterator<char>(file));
	}
	out.close();
	for(int i = 0; i < 26; ++i)
	{
		if(next[i] != NULL)
		{
			next[i]->save_to_file(file);
		}
	}
}

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
	letter top;
	for(int i = 0;; ++i)
	{
		if(!fileExists((directory + to_string(i) + ".txt").c_str()) || !fileExists((directory + to_string(i) + ".key").c_str()))
		{
			cout<<"Reached "<<i<<".txt\n";
			break;
		}
		ifstream f((directory + to_string(i) + ".txt").c_str(), ios::in);
		ifstream vals((directory + to_string(i) + ".key").c_str(), ios::in);
		vector<TAG_SIZE> v;
		while(!vals.eof())
		{
			string s;
			getline(vals, s);
			if(tags[s] != -1)
				v.push_back(tags[s]);
		}
		bool skip = false;
		letter *cur = &top;
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
				if(cur != &top)
				{
					for(int i = 0; i < v.size(); ++i)
						cur->tags[v[i]]++;
				}
				cur = &top;
			}
			else
			{
				if(cur->next[tolower(l) - 'a'] == NULL)
				{
					cur->next[tolower(l) - 'a'] = new letter;
				}
				cur = cur->next[tolower(l) - 'a'];
			}
		}
		f.close();
		vals.close();
	}
	string s;
	top.recursify(s);
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













