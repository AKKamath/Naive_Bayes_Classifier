#include <iostream>
#include <string.h>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
#define TAG_SIZE unsigned int

// Check if file exists
bool fileExists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}
// Used to sort tags by probability
bool sorter(pair<TAG_SIZE, int> a, pair<TAG_SIZE, int> b)
{
	return a.second > b.second;
}

// Used to store tags
class TagStorage
{
	map<string, TAG_SIZE> tag;
	map<TAG_SIZE, string> tag2;
	public:
	// Initialize tags from file
	void init(string file);
	// Retrieve tag by name
	TAG_SIZE operator [] (string tag_name)
	{
		if(tag[tag_name] == 0)
			return -1;
		return tag[tag_name];
	}
	// Retrieve tag by ID
	string operator [](unsigned short val)
	{
		return tag2[val];
	}
};

// Read tags from file and assign values
void TagStorage::init(string file)
{
	// Tag file doesn't exist
	if(!fileExists(file.c_str()))
	{
		cout<<"Error, "<<file<<" does not exist!\n";
		return;
	}
	ifstream f(file.c_str(), ios::in);
	int i = 1;
	string s;
	// Increment ID and store
	while(getline(f, s))
	{
		tag2[i] = s;
		tag[s] = i++;
	}
}

// Read from training directory and create tag files
void assignValues(string directory, TagStorage tags)
{
	int i = 0;
	// We found a save, save time and continue?
	if(fileExists((directory + "save_state.sav").c_str()))
	{
		cout<<"Save found, continue from this location? (1 = Yes, 0 = No)\n";
		int opt;
		cin>>opt;
		// Nah, delete all data and restart
		if(opt == 0)
		{
			remove((directory + "save_state.sav").c_str());
		}
		// Continue from where we left off
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
		// File of required format doesn't exist, so we can stop
		if(!fileExists((directory + to_string(i) + ".txt").c_str()))
		{
			cout<<"Reached "<<i<<".txt\n";
			break;
		}
		// Open required files
		ifstream main_file((directory + to_string(i) + ".txt").c_str(), ios::in);
		ofstream tag_file((directory + to_string(i) + ".key").c_str(), ios::out);
		bool skip = false;
		while(!main_file.eof())
		{
			// Read character from file
			char l;
			main_file.read((char *)&l, sizeof(l));
			// Ignore anything in <> and punctuation
			if(l == '>')
				skip = false;
			else if(l == '<')
				skip = true;
			if(skip)
				continue;
			if(l == '\'')
				continue;
			// Reached end of a word
			if(!isalpha(l))
			{
				if(word.length() != 0)
				{
					// Read details of the word from file
					string path = ("Data/" + word + ".word");
					if(fileExists(path.c_str()))
					{
						// Get all the tag details
						map<TAG_SIZE, int> inp;
						ifstream infile(path.c_str(), ios::in | ios::binary);
						TAG_SIZE t;
						int val;
						while(infile.read((char *) &t, sizeof(t)))
						{
							infile.read((char *) &val, sizeof(val));
							inp[t] = val;
						}
						// Ignore words with more than 10 tags associated with them
						// Used to avoid common words like 'the', 'a', 'is'
						if(inp.size() < 10)
							for(map<TAG_SIZE, int>::iterator it = inp.begin(); it != inp.end(); ++it)
								possible_tags[it->first] += it->second;
						infile.close();
					}
				}
				word.clear();
			}
			// Append current letter to end of current word
			else
			{
				word += tolower(l);
			}
		}
		// Go through all tags obtained
		vector<pair<TAG_SIZE, int> > v;
		copy(possible_tags.begin(), possible_tags.end(), back_inserter(v));
		// Sort tags by probability
		sort(v.begin(), v.end(), sorter);
		// Output top 5 tags
		char x = 0;
		for(vector<pair<TAG_SIZE, int> >::iterator it = v.begin(); it != v.end(); ++it)
		{
			tag_file<<tags[it->first]<<" "<<it->second<<"\n";
			if(x > 5)
				break;
			++x;
		}
		main_file.close();
		tag_file.close();
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
	TagStorage tag;
	tag.init(argc[1]);
	assignValues(argc[2], tag);
	return 0;
}


















