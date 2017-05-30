#include <iostream>
#include <string.h>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <stdlib.h>
using namespace std;
#define TAG_SIZE unsigned int

struct stat sb;

// Check if file exists
bool fileExists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}


// Used to store tags
class TagStorage
{
	// Stores the tags
	// string is the tagName, TAG_SIZE is the ID
	map<string, TAG_SIZE> tag;
	public:
	// Used to read from file and create tags
	void init(string file);
	// Access tags by string name
	TAG_SIZE operator [] (string tag_name)
	{
		if(tag[tag_name] == 0)
			return -1;
		return tag[tag_name];
	}
	// Output all tags
	void print();
};

// Output all tags and details
void TagStorage::print()
{
	cout<<"Name\tTag\n";
	for(map<string, TAG_SIZE>::iterator it = tag.begin(); it != tag.end(); ++it)
	{
		cout<<it->first<<"\t"<<it->second<<"\n";
	}
}

// Read tags from file and assign values
void TagStorage::init(string file)
{
	if(!fileExists(file.c_str()))
	{
		cout<<"Error, "<<file<<" does not exist!\n";
		return;
	}
	ifstream f(file.c_str(), ios::in);
	int i = 1;
	// Read tag name, increment ID and store
	string s;
	while(getline(f, s))
		tag[s] = i++;
}

// Read training data from files
void assignValues(string directory, TagStorage tags)
{
	// Start from 0.txt
	int i = 0;
	// We found a save file, save time and start from where we left off?
	if(fileExists((directory + "save_state.sav").c_str()))
	{
		cout<<"Save found, continue from this location? (1 = Yes, 0 = No)\n";
		int opt;
		cin>>opt;
		// Nah, delete all saved data and restart
		if(opt == 0)
		{
			remove((directory + "save_state.sav").c_str());
			system("rm -r Data");
		}
		// Resume from last saved location
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
		// No more files of required format found		
		if(!fileExists((directory + to_string(i) + ".txt").c_str()) || !fileExists((directory + to_string(i) + ".key").c_str()))
		{
			cout<<"Reached "<<i<<".txt\n";
			break;
		}
		// Open required files
		ifstream main_file((directory + to_string(i) + ".txt").c_str(), ios::in);
		ifstream tag_file((directory + to_string(i) + ".key").c_str(), ios::in);
		vector<TAG_SIZE> v;
		map<string, int> counter;
		string s;
		// Read tags from file
		while(getline(tag_file, s))
		{
			if(tags[s] != -1)
				v.push_back(tags[s]);
		}
		bool skip = false;
		while(!main_file.eof())
		{
			// Read character by character
			char l;
			main_file.read((char *)&l, sizeof(l));
			// Ignore lines within <> and punctuation
			if(l == '>')
				skip = false;
			else if(l == '<')
				skip = true;
			if(skip)
				continue;
			if(l == '\'')
				continue;
			// Completed a word, so we store it
			if(!isalpha(l))
			{
				if(word.length() != 0)
				{
					counter[word]++;
				}
				word.clear();
			}
			// Didn't complete a word, so concatenate last letter to current word
			else
			{
				word += tolower(l);
			}
		}
		// Check if Data directory exists, if not create it
		if (!(stat("./Data", &sb) == 0 && S_ISDIR(sb.st_mode)))
		{
			system("mkdir Data");
		}
		// Store all words encountered
		for(map<string, int>::iterator c_iter = counter.begin(); c_iter != counter.end(); ++c_iter)
		{
			map<TAG_SIZE, int> inp;
			string path = ("Data/" + c_iter->first + ".word");
			// Word has been encountered before, increment tags for that word
			if(fileExists(path.c_str()))
			{
				ifstream infile(path.c_str(), ios::in | ios::binary);
				TAG_SIZE t;
				int val;
				// Read values for each tag
				while(infile.read((char *) &t, sizeof(t)))
				{
					infile.read((char *) &val, sizeof(val));
					inp[t] = val;
				}
				infile.close();
			}
			// Increment values for each tag
			for(vector<TAG_SIZE>::iterator it = v.begin(); it != v.end(); ++it)
				inp[*it] += c_iter->second;
			ofstream outfile(path.c_str(), ios::out | ios::binary);
			// Write back new values to the file
			for(map<TAG_SIZE, int>::iterator it = inp.begin(); it != inp.end(); ++it)
			{
				outfile.write((char *) &it->first, sizeof(it->first));
				outfile.write((char *) &it->second, sizeof(it->second));
			}
			outfile.close();
		}
		main_file.close();
		tag_file.close();
		// Create a save state
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
	//tag.print();
	assignValues(argc[2], tag);
	return 0;
}

