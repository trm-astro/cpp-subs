#ifndef TRM_HEADER
#define TRM_HEADER

#include <string>
#include <map>
#include "trm_subs.h"
#include "trm_hitem.h"

namespace Subs {

  //! Class for storage of header information.

  /** This class enables hierarchical storage of heterogeneous
   * data types such as doubles, ints etc. It uses the Subs::Hitem class and its
   * derivatives to handle these, and internally a 'map' to provide lookup
   * keyed by name. Pointers to Hitem are stored to use virtual functions. 
   * Here is an example of usage:
   * \code
   * #include "subs.h"
   * #include "Header.h"
   *
   * int main(){
   *
   *  Subs::Header head;
   *  head.set("Record",           new Subs::Hint(23,"Record number"));
   *  head.set("Position",         new Subs::Hdirectory("Star's position"));
   *  head.set("Position.RA",      new Subs::Hdouble(21.0,"Right ascension"));
   *  head.set("Position.Dec",     new Subs::Hdouble(-28.0,"Declination"));
   *  head.set("Position.Equinox", new Subs::Hdouble(200.0,"Equinox"));
   *  head.set("Dwell",            new Subs::Hfloat(500.,"Exposure time (seconds)"));
   *  cout << head << endl;
   *  try{
   *   ofstream fout("junk");
   *   head.write(fout);
   *   fout.close();
   *  }
   *  catch(const string& str){
   *    cerr << str << endl;
   *  }  
   * }
   * \endcode
   *
   * In this example an item called 'Record' stores an integer
   * value of 23 representing a record number. This could later
   * be retrieved using head.get("Record",ival) as long as ival
   * was an int.
   *
   * The item called 'Position' is a directory and can store
   * sub items using the ndf-style dot construction such as
   * 'Position.RA' (a double type). The 'set' routines are
   * selected by the data types of the value, and the 'Dwell'
   * object shows how a float item can be set.
   */
  
  class Header : public std::map<std::string, Hitem*> {

  public:

    //! iterator over the Header data
    typedef std::map<std::string,Hitem*> base;

    //! iterator over the Header data
    typedef base::const_iterator const_iterator;

    //! iterator over the Header data
    typedef base::iterator iterator;
    
    //! string to start each line of header output with
    static std::string start_string; 

    //! minimum field width for header item names
    static const std::string::size_type name_width = 20;  

    //! indentation to mark directories
    static const int  dir_indent = 3;   

    //! character to indicate a new indentation level
    static const char dir_flag   = '.'; 
    
    //! default constructor of an empty header
    Header() : base() {}

    //! copy constructor
    Header(const Header& hd);

    //! Destructor
    ~Header();

    //! Assignment
    Header& operator=(const Header& hd);

    //! set an item
    void set(const std::string& name, Subs::Hitem* hip);

    //! set an item with automatic creation of upper level directories
    void set_auto(const std::string& name, Subs::Hitem* hip);

    //! indexed access to header
    Hitem* operator[](const std::string& name) const;
    
    //! Rename an item    
    void move(const std::string& name1, const std::string& name2);

    //! Clear a header completely
    void clear();

    //! Erase a named item
    void erase(const std::string& name);

    //! Erase an item referred to by an iterator
    void erase(iterator p){
      delete p->second;
      this->erase(p);
    }

    //! Checks a name for existence of a directory containing 'name'
    bool check_for_dir(const std::string& name) const;
    
    //! ASCII print out of a header
    void print(std::ostream& s, const std::string& name="", int lmax = -1) const;

    //! ASCII print out of a header
    friend std::ostream& operator<<(std::ostream& s, const Header& obj);

    //! Binary output of a header
    void write(std::ofstream& s) const;

    //! Binary input of a header
    void read(std::ifstream& s, bool swap_bytes);

    //! Skip a binary header
    static void skip(std::ifstream& s, bool swap_bytes);
    
    //! Finds next header item with name starting with string = 'name', beginning from 'start'
    const_iterator find_next_starting_with(const std::string& name,  const_iterator start) const;

    //! Finds next header item with name starting with directory = 'dir', beginning from 'start'
    const_iterator find_next_starting_with_dir(const std::string& dir, const_iterator start) const;

    //! Finds next header item with name ending with string = 'name', beginning from 'start'
    const_iterator find_next_ending_with(const std::string& name, const_iterator start) const;

    //! Finds next header item with name = 'name', beginning from 'start'
    const_iterator find_next(const std::string& name, const_iterator start) const;

    //! Finds first header item with name starting with string = 'name'
    const_iterator find_starting_with(const std::string& name) const;

    //! Finds first header item with name ending with string = 'name'
    const_iterator find_ending_with(const std::string& name) const;

    //! Regular expression searcher
    int refind(const std::string& regexp, const_iterator& cit) const;
    
    //! Finds next header item with name starting with string = 'name', beginning from 'start'
    iterator find_next_starting_with(const std::string& name, iterator start);

    //! Finds next header item with name starting with directory = 'dir', beginning from 'start'
    iterator find_next_starting_with_dir(const std::string& dir, iterator start);

    //! Finds next header item with name ending with string = 'name', beginning from 'start'
    iterator find_next_ending_with(const std::string& name, iterator start);

    //! Finds next header item with name = 'name', beginning from 'start'
    iterator find_next(const std::string& name, iterator start);

    //! Finds first header item with name starting with string = 'name'
    iterator find_starting_with(const std::string& name);

    //! Finds first header item with name ending with string = 'name'
    iterator find_ending_with(const std::string& name);

    //! Returns range of items in directory 'dir'
    std::pair<const_iterator,const_iterator> range_in_dir(const std::string& dir) const;

    //! Returns range of items in directory 'dir'
    std::pair<iterator,iterator> range_in_dir(const std::string& dir);
    
    //! Error class for header class
    class Header_Error : public Subs_Error { 
    public:
      //! Default constructor
      Header_Error () : Subs_Error() {}
      //! Constructor from a string
      Header_Error (const std::string& str) : Subs_Error(str) {}
    };

    //! Is 'name' a valid header item name
    static bool valid_name(const std::string &name);

    //! Is 'name' contained within 'dir'?
    static bool member_of_dir(const std::string& name, const std::string& dir);

  };
  
  //! ASCII output
  std::ostream& operator<<(std::ostream& s, const Subs::Header& obj);  


};



#endif
  




