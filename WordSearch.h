//
//  WordSearch.h
//  CS263-HW4
//
//  Created by Hans Dulimarta on 10/19/14.
//  Copyright (c) 2014 SoCIS. All rights reserved.
//

#ifndef __CS263_HW4__WordSearch__
#define __CS263_HW4__WordSearch__

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <stdexcept>
#include "FileSystem.h"
using namespace std;

class WordSearch {
private:
    set<string> ignored_words;
    //filename, all words w/ dupes
    map<string, vector<string>> books;
    // frequencies of words
    map<string, unsigned int> wordFreqs;
    /* TODO: declare additional data structures as needed */
    
    void read_words (const string& file_name);
    void load_ignored_words(const string& file_name);
    //filenames of files that contain any of the words we desire
    set<string> files_with_paydirt_words (const set<string>& wordset) const;
    pair<unsigned int,set<string>> largest_int_words (const map<string,unsigned int>& wordMap) const;

public:
    WordSearch ();
    WordSearch (const string& topdir, const string& ignore_file);
    
    /* number of words in all the documents,
     * excluding ignored words.
     */
    unsigned long word_count() const;
    
    /* return a set of words of certain length, return an empty set
     when no words of the requested length */
    set<string> words_of_length (int L) const;
    
    /* the first of the pair is the number of occurences,
     * the second of the pair is the word(s) that occur the most
     */
    pair<unsigned int,set<string>> most_frequent_words() const throw(length_error);
    
    /* return the file names containing the most frequent words */
    set<string> files_with_most_frequent_words() throw(length_error);
    
    /* return a set of words occuring less or equal to the given count */
    set<string> least_frequent_words (int count) const;
    
    /* return a set of file names where the above words show up */
    set<string> files_with_least_frequent_words (int count) const;
    
    /* return the most probable word after the given word */
    string most_probable_word_after (const string& word) const;
    
};
#endif /* defined(__CS263_HW4__WordSearch__) */
