//
//  WordSearch.cpp
//  CS263-HW4
//
//  Created by Hans Dulimarta on 10/19/14.
//  Copyright (c) 2014 SoCIS. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>  // needed for transform()
#include <exception>
#include <math.h>
#include "WordSearch.h"

WordSearch::WordSearch() {
    /* default constructor requires no additional code */
}

WordSearch::WordSearch(const string& topdir, const string& ignore_file) {
    load_ignored_words(ignore_file);
    /* filter the directory only for files ending with "txt" */
    gvsu::FileSystem dir (topdir);
    
    for (auto entry : dir) {
        cout << "Reading words from " << entry.second << endl;
        read_words (entry.first + "/" + entry.second);
    }
}

void WordSearch::load_ignored_words(const string& fname) {
    ifstream ign_file (fname);
    if (!ign_file.is_open()) {
        throw ios_base::failure {"Unable to load ignored.txt"};
        
    }
    string word;
    while (getline(ign_file, word))
        ignored_words.insert(word);
    ign_file.close();
}

void WordSearch::read_words(const string &file_name)
{
    ifstream txt (file_name); /* file is aumatically open */
    
    string one_line;
    
    int line = 1;
    string prev = "";
    while (getline(txt, one_line)) {
        /* change to lowercase */
        transform(one_line.begin(), one_line.end(), one_line.begin(), ::tolower);
        
        /* replace non-alpha with a space */
        replace_if (one_line.begin(), one_line.end(),
        	[] (const char c) {
        	return !isalpha(c);
        }, ' ');
        istringstream tokens{one_line};
        string word;
        while (tokens >> word) {
            if (word.length() < 3) continue;
            if (ignored_words.find(word) == ignored_words.end()) {
                books.at(file_name).push_back(word);
                wordFreqs.emplace(word, 0);
                wordFreqs.at(word)++;
            }
        }
        line++;
    }
    txt.close(); /* close the file */
}


set<string> WordSearch::files_with_paydirt_words (const set<string>& wordset) const{
	set<string> files;
	bool foundWord;
	    //books in the filesystem
	    for (const auto& book : books){
	    	foundWord = false;
	    	for (unsigned int i = 0;!foundWord && i < book.second.size() ;i++){
	    		for (auto iter = wordset.begin(); iter != wordset.end(); iter++ ){
	    			if(book.second[i] == *iter){
	    				foundWord = true;
	    				files.insert(book.first);
	    				break;
	    			}
	    		}
	    	}
	    }
	return files;
}

pair<unsigned int,set<string>> WordSearch::largest_int_words (const map<string,unsigned int>& wordMap) const{
    set<string> words;
    unsigned int highInt = 0;
    for (const auto& wordVal : wordMap) {
    	if (wordVal.second > highInt) {
    		highInt = wordVal.second;
    	}
    }
    for (const auto& wordVal : wordMap) {
    	if (wordVal.second == highInt) {
    		words.insert(wordVal.first);
    	}
    }
    return make_pair(highInt, words);
}



unsigned long WordSearch::word_count() const {
    unsigned long num = 0;
    for (const auto& book : books) {
    	num += book.second.size();
    }
	return num;
}

set<string> WordSearch::words_of_length (int L) const {
    set<string> result;
    for (const auto& book : books) {
    	for (const string& word : book.second) {
    		if (word.size() == L) {
    			result.insert(word);
    		}
    	}
    }
    return result;
}

pair<unsigned int,set<string>> WordSearch::most_frequent_words() const throw (length_error) {
    return largest_int_words(wordFreqs);
}

set<string> WordSearch::least_frequent_words(int count) const {
	set<string> words;
	    for (const auto& wordFreq : wordFreqs) {
	    	if (wordFreq.second <= count) {
	    		words.insert(wordFreq.first);
	    	}
	    }
	    return words;
}

set<string> WordSearch::files_with_most_frequent_words()  throw(length_error) {
    return this->files_with_paydirt_words(this->most_frequent_words().second);
}

set<string> WordSearch::files_with_least_frequent_words(int N) const
{
    return this->files_with_paydirt_words(this->least_frequent_words(N));
}

string WordSearch::most_probable_word_after(const string& word) const {
    map<string, unsigned int> afterWords;
    for (const auto& book : books){
    	for (unsigned int i = 0; i < book.second.size() - 1; i++){
    		if(book.second[i] == word){
    			afterWords.emplace(book.second[i+1], 0);
    			afterWords.at(book.second[i+1])++;
    		}
    	}
    }
    return *(this->largest_int_words(afterWords).second.begin());
}
