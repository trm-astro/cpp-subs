#include <vector>
#include <pcrecpp.h>
#include "trm_header.h"
#include "trm_subs.h"

// static member
std::string Subs::Header::start_string = "";

// Setting. Checks for validity of name and that an appropriate
// directory has ben created for it.

/**
 * Sets the value of a header item. Header items are stored as pointers
 * to Subs::Hitem objects to allow for derived types.
 * \param name name of the header item
 * \param hip pointer to a Subs::Hitem object
 */

void Subs::Header::set(const std::string& name, Subs::Hitem* hip){
    if(valid_name(name)){
	if(check_for_dir(name)){
	    // take care to delete any previous instance if it is different
	    iterator p = this->find(name);
	    if(p == this->end()){
		this->insert(std::make_pair(name,hip));
	    }else{
		if(hip != p->second){
		    delete p->second;
		    this->base::erase(p);
		}
		this->insert(std::make_pair(name,hip));
	    }      
	}else{
	    throw Header_Error("Attempt to define header item before its "
			       "directory was created in void Subs::Header::set(const "
			       "string&, Subs::Hitem* hip). \nItem = " + name);
	}
    }else{
	throw Header_Error(std::string("Invalid item name "
				       " in void Subs::Header::set(const std::string&, Subs::Hitem* hip) = [") + name + 
			   std::string("]"));
    }
}

/**
 * Sets the value of a header item forcing the creation of any directory structure
 * if necessary. Header items are stored as pointers
 * to Subs::Hitem objects to allow for derived types.
 * \param name name of the header item
 * \param hip pointer to a Subs::Hitem object
 */

void Subs::Header::set_auto(const std::string& name, Subs::Hitem* hip){
    if(valid_name(name)){
	
	// Ensure that containing directories exist
	std::string::size_type ppos = name.find('.');
	while(ppos != std::string::npos){
	    std::string dir = name.substr(0,ppos);
	    const_iterator cit = this->find_starting_with(dir);
	    if(cit == this->end())
		this->set(dir, new Subs::Hdirectory("Directory set automatically"));
	    ppos = name.find('.',++ppos);
	}
	// Delete any previous instance if it is different
	iterator p = this->find(name);
	if(p == this->end()){
	    this->insert(std::make_pair(name,hip));
	}else{
	    if(hip != p->second){
		delete p->second;
		this->base::erase(p);
	    }
	    this->insert(std::make_pair(name,hip));
	}      
	
    }else{
	throw Header_Error(std::string("Invalid item name "
				       " in void Subs::Header::set_auto(const std::string&, Subs::Hitem* hip) = [") + name + 
			   std::string("]"));
    }
}

/** Destructor. Must delete all the items pointed to by the header
 */

Subs::Header::~Header(){
    for(const_iterator p = this->begin(); p != this->end(); p++)
	delete p->second;
}

/** Copy constructor. Needed because of pointers. 
 * \param hd the header to be copied.
 */
Subs::Header::Header(const Header& hd) : base() {
    for(const_iterator p = hd.begin(); p != hd.end(); p++)
	this->insert(std::make_pair(p->first, p->second->copy()));
}

/** Assignment. Needed because of pointers. 
 * \param hd the header to be copied.
 */
Subs::Header& Subs::Header::operator=(const Header& hd){
    if(this != &hd){
	this->clear();
	for(const_iterator p = hd.begin(); p != hd.end(); p++)
	    this->insert(std::make_pair(p->first, p->second->copy()));
    }
    return *this;
}

/** Clear a header. This deletes all the items the header points
 * to and removes all items from the header.
 */
void Subs::Header::clear(){
    for(const_iterator p = this->begin(); p != this->end(); p++)
	delete p->second;
    this->base::clear();
}

/** Checked access to avoid automatic creation of header item.
 * This returns a pointer to the Subs::Hitem equivalent to a given name.
 * An exception is thrown if the item does not exist.
 * \param name the name of the header item
 * \exception Subs::Header::Header_Error exceptions are thrown if the header item does
 * not exist
 */

Subs::Hitem* Subs::Header::operator[](const std::string& name) const {
    const_iterator hit = this->find(name);
    if(hit == this->end())
	throw Header_Error("Subs::Header::operator[](const std::string&): Failed to find item = ") + name;
    else
	return hit->second;
}

/** This routine looks for a directory containing the item called 'name' in it.
 * \param name the item to search for
 * \return true/false depending on whether there is a directory containg 'name' or not
 */

bool Subs::Header::check_for_dir(const std::string &name) const {

    std::string::size_type n = name.find_last_of(dir_flag);
    if(n == std::string::npos) return true;
  
    std::string directory = name.substr(0,n);
    
    for(const_iterator p = this->begin(); p != this->end(); p++)
	if(p->first == directory && p->second->is_a_dir()) return true;

    return false;
}

/**
 * Checks a header name for validity. This means it must contain no
 * white space, start or end with the directory character flag (e.g. '.')
 * or have two or more of them in a row.
 * \param name the header name to test
 */

bool Subs::Header::valid_name(const std::string &name){ 
    std::string test(2,Header::dir_flag);
    return !(name.find(' ')  != std::string::npos ||
	     name.find('\t') != std::string::npos ||
	     name[0] == Header::dir_flag || 
	     name[name.length()-1] == Header::dir_flag ||
	     name.find(test) != std::string::npos);
}

// Prints name and lmax levels below it if it
// is a directory

void Subs::Header::print(std::ostream& s, const std::string& name, int lmax) const {
    
    const_iterator p;
    std::string::size_type fcount=0, n=0, m, count, maxl;
    
    // If name is not blank (indicating everything), check that
    // it exists
    
    if(name != ""){
	p = this->find(name);
	if(p == this->end()) {
	    std::string message = name;
	    message += " not recognised in void Subs::Header::print(std::ostream&, const std::string&, int)";
	    throw Header_Error(message);
	}
	
	// Print item, with a preceding blank line if it is a directory
	
	if(p->second->is_a_dir()){
	    maxl = name_width + Hitem::value_width;
	    s << "\n";
	}else{
	    maxl = name_width;
	}
	s << start_string;
	std::cout.fill(' ');
	std::cout.width(maxl);
	std::cout.setf(std::ios_base::left);
	s << p->first;
	if(!p->second->is_a_dir()) s << " = ";
	Subs::operator<<(s,p->second); 
	s << std::endl;
	
	while((m = p->first.find_first_of(dir_flag,n)) != std::string::npos){
	    fcount++;
	    n += m+1;
	}    
	p++;
    }else{
	p = this->begin();
	fcount = 0;
    }
    
    std::string indent(dir_indent,' ');
    
    // Print any items starting with same string which can only
    // be items in a directory called name
    
    for(const_iterator tp=p; tp != this->end(); tp++){
	if(name == "" || tp->first.find(name) == 0){
	    count = n = 0;
	    while((m = tp->first.find_first_of(dir_flag,n)) != std::string::npos){
		count++;
		n += m+1;
	    }
	    
	    count -= fcount;
	    if(lmax < 0 || count <= std::string::size_type(lmax)){
		s << start_string;
		for(std::string::size_type i=0; i<count; i++) s << indent;
		s.setf(std::ios::left);
		if(tp->second->is_a_dir()){
		    maxl = name_width + Hitem::value_width;
		}else{
		    maxl = name_width;
		}
		std::cout.fill(' ');
		std::cout.width(maxl);
		std::cout.setf(std::ios_base::left);
		if((n = tp->first.find_last_of(Header::dir_flag)) == std::string::npos)
		    s << tp->first;
		else
		    s << tp->first.substr(n+1);
		if(!tp->second->is_a_dir()) s << " = ";
		Subs::operator<<(s,tp->second);
		s << std::endl;
	    }
	}else{
	    break;
	}
    }
}

/** Writes out header in binary format
 * \param s output file stream opened for binary IO
 */
void Subs::Header::write(std::ofstream& s) const {
  size_t lmap = this->size();
  s.write((char*)&lmap,sizeof(size_t));
  if(s && lmap){
    for(const_iterator p=this->begin(); p!=this->end(); p++){
      Subs::write_string(s, p->first);  // write name of item
      Subs::Hitem::write_item(s, p->second); // write item
    }
  }
}


/** Reads header in binary format. This reads in a series of names and
 * associated Subs::Hitem objects from a binary file on disk. At its end,
 * you are left at whatever follows the header. 
 * \param s the input stream opened for binary IO.
 */

void Subs::Header::read(std::ifstream& s, bool swap_bytes) {

    // Read number of header items
    size_t lmap;
    s.read((char*)&lmap,sizeof(size_t));
    if(swap_bytes) lmap = Subs::byte_swap(lmap);
    
    if(s && lmap){
	std::string name;
	
	for(size_t i=0; i<lmap; i++){
	    
	    // read the item name
	    Subs::read_string(s,name,swap_bytes);
	    
	    // then its value
	    this->set(name, Subs::Hitem::read_item(s, swap_bytes));
	    
	}
    }
}

// skip header in binary format
void Subs::Header::skip(std::ifstream& s, bool swap_bytes) {
  size_t lmap;
  s.read((char*)&lmap, sizeof(size_t));
  if(swap_bytes) lmap = Subs::byte_swap(lmap);

  if(lmap){

    for(size_t i=0; i<lmap; i++){

      // skip the item name
      Subs::skip_string(s, swap_bytes);

      // then skip the item
      Subs::Hitem::skip_item(s, swap_bytes);

    }
  }
}


Subs::Header::const_iterator 
Subs::Header::find_next_starting_with(const std::string& name, 
				const_iterator start) const {
  for(const_iterator p=start; p != this->end(); p++)
    if(p->first.find(name) == 0) return p;
  return this->end();
}

Subs::Header::const_iterator 
Subs::Header::find_next_starting_with_dir(const std::string& dir, 
				    const_iterator start) const {
  for(const_iterator p=start; p != this->end(); p++)
    if(p->first.find(dir) == 0  && 
       (p->first.length() == dir.length() ||
	p->first[dir.length()] == dir_flag)) return p;
  return this->end();
}

Subs::Header::const_iterator 
Subs::Header::find_next_ending_with(const std::string& name, 
			      const_iterator start) const {
  for(const_iterator p=start; p != this->end(); p++)
    if(p->first.find(name) + name.length() == p->first.length()) return p;
  return this->end();
}

Subs::Header::const_iterator Subs::Header::find_next(const std::string& name, 
					 const_iterator start) const {
  for(const_iterator p=start; p != this->end(); p++)
    if(p->first.find(name) != std::string::npos) return p;
  return this->end();
}

Subs::Header::const_iterator 
Subs::Header::find_starting_with(const std::string& name) const {
  for(const_iterator p=this->begin(); p != this->end(); p++){
    if(p->first.find(name) == 0) return p;
  }
  return this->end();
}

Subs::Header::const_iterator 
Subs::Header::find_ending_with(const std::string& name) const { 
  for(const_iterator p=this->begin(); p != this->end(); p++)
    if(p->first.find(name) + name.length() == p->first.length())
      return p;
  return this->end();
}

Subs::Header::iterator 
Subs::Header::find_next_starting_with(const std::string& name, 
				iterator start){
  for(iterator p=start; p != this->end(); p++)
    if(p->first.find(name) == 0) return p;
  return this->end();
}

Subs::Header::iterator 
Subs::Header::find_next_starting_with_dir(const std::string& dir, 
				iterator start){
  for(iterator p=start; p != this->end(); p++)
    if(p->first.find(dir) == 0  && 
       (p->first.length() == dir.length() ||
	p->first[dir.length()] == dir_flag)) return p;
  return this->end();
}


Subs::Header::iterator 
Subs::Header::find_next_ending_with(const std::string& name, 
			      iterator start){
  for(iterator p=start; p != this->end(); p++)
    if(p->first.find(name) + name.length() == p->first.length()) return p;
  return this->end();
}

Subs::Header::iterator Subs::Header::find_next(const std::string& name, 
					 iterator start){
  for(iterator p=start; p != this->end(); p++)
    if(p->first.find(name) != std::string::npos) return p;
  return this->end();
}

Subs::Header::iterator 
Subs::Header::find_starting_with(const std::string& name){
  for(iterator p=this->begin(); p != this->end(); p++)
    if(p->first.find(name) == 0)
      return p;
  return this->end();
}

Subs::Header::iterator 
Subs::Header::find_ending_with(const std::string& name){ 
  for(iterator p=this->begin(); p != this->end(); p++)
    if(p->first.find(name) + name.length() == p->first.length())
      return p;
  return this->end();
}

/** This routine searches for a header item with a name that matches the supplied regular expression
 * \param the regular expression (Perl compatible)
 * \param the iterator pointing to the header item if a match is found (the first found)
 * \return 0 if no match is found, 1 is one is found 2 if 2 or more matches exist
 */
int Subs::Header::refind(const std::string& regexp, const_iterator& cit) const {

  // Compile the regular expression
  pcrecpp::RE re(regexp);
  
  int nmatch = 0;
  for(const_iterator p=this->begin(); p != this->end(); p++){
    if(re.PartialMatch(p->first)){
      nmatch++;
      if(nmatch > 1) break;
      cit = p;
    }
  }
  return nmatch;
}

/** Determines the range of iterators which
 * match the name dir regarded as a directory. A range covering
 * a single element is returned even if an item is not a directory
 * as long as it is present. The range is returned as a pair with
 * the second one one greater than the last OK iterator
 *
 * This routine uses the ordering provided by map to return as
 * soon as it fails to match.
 *
 * Complete failure is indicated by both pair members being set 
 * to the end() iterator.
 */

std::pair<Subs::Header::const_iterator,Subs::Header::const_iterator>
Subs::Header::range_in_dir(const std::string& dir) const {
  bool not_found_first = true;
  const_iterator first, last;
  for(const_iterator p=this->begin(); p != this->end(); p++){
    if(member_of_dir(p->first,dir)){
      if(not_found_first){
	not_found_first = false;
	first = last = p;
      }else{
	last = p;
      }
    }else if(!not_found_first){
      return std::pair<const_iterator,const_iterator>(first,++last);
    }
  }
  return std::pair<const_iterator,const_iterator>(this->end(),this->end());
}

/** Returns pair of iterators covering items in a given directory
 * \param dir the directory in question
 */
std::pair<Subs::Header::iterator,Subs::Header::iterator>
Subs::Header::range_in_dir(const std::string& dir) {
  bool not_found_first = true;
  iterator first=this->end(), last = first;
  for(iterator p=this->begin(); p != this->end(); p++){
    if(member_of_dir(p->first,dir)){
      if(not_found_first){
	not_found_first = false;
	first = last = p;
      }else{
	last = p;
      }
    }else if(!not_found_first){
      return std::pair<iterator,iterator>(first,++last);
    }
  }
  if(!not_found_first)
    return std::pair<iterator,iterator>(first,++last);
  else
    return std::pair<iterator,iterator>(this->end(),this->end());
}

/** Erases the element pointed to by 'name' and any sub-elements
 * of it.
 * \param name the name of the item to remove
 */
void Subs::Header::erase(const std::string& name){ 
  std::pair<iterator,iterator> pit = range_in_dir(name);
  if(pit.first != this->end()){
    std::vector<iterator> ename;
    for(iterator p=pit.first; p!=pit.second; p++){
      ename.push_back(p);
      delete p->second;
    }
    for(size_t i=0; i<ename.size(); i++)
      this->base::erase(ename[i]);
  }
}

/** Rename an element (or a directory of elements)
 * This allows one to change the name of an element. If applied
 * to a directory, all elements that stem from that directory are moved as well.
 * \param name1 old name
 * \param name2 new name
 */
void Subs::Header::move(const std::string& name1, const std::string& name2){
    
    if(name1 != name2){
	
	// check that name2 is valid
	if(!valid_name(name2)){
	    std::string message = "Invalid item name "
		" in void Subs::Header::set(const std::string&, const double&,"
		" const std::string) = [" + name2 + "]";
	    throw Header_Error(message);
	}else if(!check_for_dir(name2)){
	    std::string message = "Attempt to rename [" + name1 + "] to [" + name2
		+ "] which has no directory "
		"in void Subs::Header::move(const std::string&, const std::string&)";
	    throw Header_Error(message);
	}
	
	// check that name2 is not already set.
	if(find(name2) != this->end()){
	    std::string message = "Error in void Subs::Header::move(const std::string&, "
		"const std::string&): [";
	    message += name2;
	    message += "] already exists!";
	    throw Header_Error(message);
	}
	
	// check that names is not a sub-element of name1
	if(member_of_dir(name2,name1)){
	    std::string message = "Can't rename item = [" + name1 + 
		"] to be a sub-element of itself = [" + name2 +
		"] in void Subs::Header::move(const std::string&, const std::string&)";
	    throw Header_Error(message);
	}
	
	// Rename all items that have name1 as their directory or entire name.
	std::string tname;
	iterator p;
	std::pair<iterator,iterator> pit = range_in_dir(name1);
	if(pit.first != this->end()){
	    base translate;
	    for(p=pit.first; p!=pit.second; p++){
		tname = p->first;
		tname.replace(0,name1.length(),name2);
		translate[tname] = p->second;
		this->erase(p);
	    }
	    base::iterator it;
	    for(it=translate.begin(); it!=translate.end(); it++)
		this->insert(std::make_pair(it->first, it->second));
	}else{
	    std::string message = "Error in void Subs::Header::move(const std::string&, const std::string&): no such element as [";
	    message += name1 + "]";
	    throw Header_Error(message);
	}
    }
}

/** Determines whether name is an element of a directory dir or not
 * \param name the name to test
 * \param dir the directory to test against
 * \return true if 'name' is an element of 'dir', false if it is 
 */

bool Subs::Header::member_of_dir(const std::string& name, const std::string& dir){
  return (name.find(dir) == 0  && (name.length() == dir.length() || name[dir.length()] == Header::dir_flag));
}


/** Output operator for Header class.
 * \param s output stream
 * \param obj the Header item to list
 */
std::ostream& Subs::operator<<(std::ostream& s, const Subs::Header& obj) {
  obj.print(s,"",-1);
  return s;
}
    






