#include <iostream>
#include <string.h>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;
#define TAG_SIZE unsigned int

// Check if file exists
bool fileExists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

// Structure of a tag
struct TagStructure
{
	TAG_SIZE id;
	string name;
	string category;
	TagStructure()
	{
		id = 0;
		name = "";
		category = "";
	}
	TagStructure(TAG_SIZE Id, string Name, string Category)
	{
		id = Id;
		name = Name;
		category = Category;
	}
};

// Used to sort tags by probability
bool sorter(pair<TAG_SIZE, int> a, pair<TAG_SIZE, int> b)
{
	return a.second > b.second;
}

// Used to store tags
class TagStorage
{
	// Stores the tags
	// string is the tag name
	map<string, TagStructure> tag;
	map<TAG_SIZE, string> tag2;
	public:
	// Initialize tags from file
	void init(string file);
	// Retrieve tag by name
	TAG_SIZE operator [] (string tag_name)
	{
		if(tag[tag_name].id == 0)
			return -1;
		return tag[tag_name].id;
	}
	// Retrieve tag by ID
	string operator [](TAG_SIZE val)
	{
		return tag2[val];
	}
	string getCategory(TAG_SIZE val)
	{
		return tag[tag2[val]].category;
	}
	// Output all tags
	void print();
};

// Output all tags and details
void TagStorage::print()
{
	cout<<"Name\tTag\n";
	for(map<string, TagStructure>::iterator it = tag.begin(); it != tag.end(); ++it)
	{
		cout<<it->first<<"\t"<<it->second.id<<"\n";
	}
}

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
	// Read tag id, create data, and store
	string s;
	while(getline(f, s, ','))
	{
		TagStructure t;
		t.id = stoi(s);
		getline(f, s, ',');
		t.name = s;
		getline(f, s);
		t.category = s;
		tag[t.name] = t;
		tag2[t.id] = t.name;
	}
}

// Read from testing directory and create tag files
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
		map<string, vector<pair<TAG_SIZE, int> > > mapper;
		for(map<TAG_SIZE, int>::iterator it = possible_tags.begin(); it != possible_tags.end(); ++it)
		{
			mapper[tags.getCategory(it->first)].push_back(pair<TAG_SIZE, int>(it->first, it->second));
		}
		for(map<string, vector<pair<TAG_SIZE, int> > >::iterator map_it = mapper.begin(); map_it != mapper.end(); ++map_it)
		{
			tag_file<<map_it->first<<"\n";
			vector<pair<TAG_SIZE, int> > v = map_it->second;
			// Sort tags by probability
			sort(v.begin(), v.end(), sorter);
			// Output top 5 tags
			string s;
			for(vector<pair<TAG_SIZE, int> >::iterator it = v.begin(); it != v.end(); ++it)
			{
				tag_file<<tags[it->first]<<" "<<it->second<<"\n";
			}
			main_file.close();
			tag_file.close();
			ofstream out((directory + "save_state.sav").c_str(), ios::out);
			out<<i;
		}
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


















