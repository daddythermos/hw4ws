#ifndef DULIMARTA_FILESYSTEM_H
#define DULIMARTA_FILESYSTEM_H

/*
 * MinGW users:
 * This file will not compile unless you update the following 
 * two lines in your MinGW installation.
 *
 * edit line 301 and 302 of include/io.h
 *  replace off64_t with _off64_t  (4x)
 */
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <stack>

using namespace std;
namespace gvsu {
class FileSystem {
private:
    std::string absPath;   // the absolute path to the file/directory
public:
    class gv_iter;   // forward declaration
    
    FileSystem (const std::string&);
    
    gv_iter begin() const {
        return gv_iter(absPath);
    }
    
    gv_iter end() const {
        return gv_iter();  // a nullptr entry
    }
    
    /* the iterator inner class */
    class gv_iter {
    private:
        string curr_path;
        DIR *curr_dir;
        stack<pair<DIR*,string>> dir_stack;
        dirent *entry;
    public:
        gv_iter();
        ~gv_iter();
        gv_iter(const string& path);
        
        void operator++(); // prefix increment
        bool operator== (const gv_iter&) const;
        bool operator!= (const gv_iter&) const;
        
        void operator++(int) { // postfix increment
            operator++();
        }
        
        /* 
         the dereference oper returns a pair.
         
         The first field is the leading path name
         The second field is the file name
         */
        pair<string,string> operator*() const {
            return make_pair(curr_path, entry->d_name);
        }
    };

};
}
#endif
