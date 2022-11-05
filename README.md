# Naive_Bayes_Classifier
A program that implements Naive Bayes' Algorithm to classify documents according to a set vocabulary.

## Setting Up
This system has been developed, and mainly works exclusively for Linux systems, specifically Ubuntu. However, the source code has been provided in *.cpp format to allow for porting to other systems as well.
Further instructions shall be given under the assumption that a Linux system is being utilized.
Although the binary files have been provided, the following commands can be used to generate them.
```
g++ Hash_Tagger.cpp -o hashtagger -std=c++11
g++ Hash_Tester.cpp -o hashtest -std=c++11
```

## Data Format
This system requires 3 main items:
1. A vocabulary which is used to assign tags or classes to documents.
2. Training data so that the system can start drawing inferences between data and tags.
3. Test data which requires classification.
   
Data inside angular brackets ***\<data\>*** will be ignored in training and test data. This is to allow for raw HTML to be given.

### Vocabulary
[Vocabulary.txt](Vocabulary.txt) shows an example case of Vocabulary. The format of vocabulary is as follows:   
ID,Name,Category  

ID - A unique number from 1 to 4 billion, used for the specific tag.   
Name - A number/string which represents the tag that will be given as input in the training data, and that will be generated as output.   
Category - The category that the tag falls under.    

### Training Data
Training data should consist of files in consecutive numerical order, starting from 0.txt to n.txt. Each text file should have a key file, 0.key to n.key which contains the tags given for that file, with each tag being on each line.

### Test Data
Similar to training data, files in consecutive numerical order, starting from 0.txt to n.txt. Key files will be generated in this case.

## Example Use
Provided is a simple walkthrough of the system, along with test files necessary to begin execution.   
[Vocabulary.txt](Vocabulary.txt) - List of languages.   
[train/](train/) - Contains various documents in different languages.   
