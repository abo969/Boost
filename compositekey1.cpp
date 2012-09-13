/**
 * boost multi index container test
 * platform: win32, visual studio 2005/2010; Linux, gcc4.1.2
 */

#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
     
using namespace boost::multi_index;
using namespace std;

struct name_record
{
    string given_name;
    string family_name;
    string other_name;

public:
    name_record(string given_name_,string family_name_,string other_name_): 
        given_name(given_name_), family_name(family_name_), other_name(other_name_)
    {
    }

    string get_name() const { return given_name + " " + family_name + " " + other_name; }

    void setnew(string chg)
    {
        given_name = given_name + chg;
        family_name = family_name + chg;
    }

};

struct NameIndex{};

typedef multi_index_container<
    name_record,
    indexed_by<
        ordered_non_unique<
            tag<NameIndex>,
            composite_key<
                name_record, 
                BOOST_MULTI_INDEX_MEMBER(name_record,string, name_record::given_name),
                BOOST_MULTI_INDEX_MEMBER(name_record,string, name_record::family_name)
            >
        >
    >
> name_record_set;

typedef boost::multi_index::index<name_record_set,NameIndex>::type NameIndexType;
typedef boost::multi_index::index<name_record_set,NameIndex>::type::iterator IteratorType;

void printContainer(name_record_set & ns)
{
    cout << endl << "PrintContainer" << endl << "-------------" << endl;
    IteratorType it1 = ns.begin();
    IteratorType it2 = ns.end  ();

    while (it1 != it2)
    {
        cout<<it1->get_name()<<endl;
        it1++;
    }
    cout << "--------------" << endl << endl;
}


void modifyContainer(name_record_set & ns)
{
    cout << endl << "ModifyContainer" << endl << "-------------" << endl;
    IteratorType it3;
    IteratorType it4;

    NameIndexType & idx1 = ns.get<NameIndex>();
    IteratorType it1 = idx1.begin();
    IteratorType it2 = idx1.end();

    while (it1 != it2)
    {
        cout<<it1->get_name()<<endl;
        name_record nr = *it1;

        nr.setnew("_CHG");
        bool res = idx1.replace(it1,nr);
        cout << "result is: " << res << endl;
        it1++;
    }
    cout << "--------------" << endl << endl;
}

#include <vector>
#include <boost/iterator/counting_iterator.hpp>

void modifyContainer2(name_record_set & ns)
{
    cout << endl << "ModifyContainer" << endl << "-------------" << endl;

    NameIndexType & idx1 = ns.get<NameIndex>();
    std::vector<IteratorType> v(boost::make_counting_iterator(idx1.begin()), boost::make_counting_iterator(idx1.end()));
    std::vector<IteratorType>::iterator it1=v.begin(), it2=v.end();

    while (it1 != it2)
    {
        cout<<(*it1)->get_name()<<endl;
        name_record nr = **it1;

        nr.setnew("_CHG");
        bool res = idx1.replace(*it1,nr);
        cout << "result is: " << res << endl;
        it1++;
    }
    cout << "--------------" << endl << endl;
}

int main()
{
    name_record_set ns;

    ns.insert( name_record("Joe","Smith","ENTRY1")          );
    ns.insert( name_record("Robert","Brown","ENTRY2")       );
    ns.insert( name_record("Robert","Nightingale","ENTRY3") );
    ns.insert( name_record("Marc","Tuxedo","ENTRY4")        );

    printContainer(ns);
    modifyContainer(ns);
    printContainer(ns);

    cout<<endl<<"call modifyContainer2"<<endl;

    printContainer(ns);
    modifyContainer2(ns);
    printContainer(ns);

    return 0;
}

