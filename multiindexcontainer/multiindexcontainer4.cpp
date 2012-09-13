/**
 * boost multi index container test
 * platform: win32, visual studio 2005/2010; Linux, gcc4.1.2
 */

#include <iostream>
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/ordered_index.hpp"

using namespace std;
using namespace boost::multi_index;
using boost::multi_index_container;

//define multiple index
typedef struct MyIndex
{
    int x;
    int y;
    int z;

    MyIndex(int ax = 0, int ay = 0, int az = 0): x(ax), y(ay), z(az){}

    void print(char* prompt) const
    {
        cout << "(" << x << ", " << y << ", " << z << ") - " << prompt << endl;
    }
}MyIndex;

//define data to be indexed
typedef struct
{
    int a;
    int b;
}MyData;

//define object to be indexed
class MyTest
{
public:
    MyIndex myIndex;
    MyData  myData;

public:

    MyTest(int x, int y, int z, int a, int b)
    {
        myIndex.x = x;
        myIndex.y = y;
        myIndex.z = z;

        myData.a = a;
        myData.b = b;
    }

    ~MyTest()
    {
        cout << "(" << myIndex.x << ", " << myIndex.y << ", " << myIndex.z << ") - ";
        cout << "(" << myData.a << ", " << myData.b << "), destructed" << endl;
    }

    void print(char* prompt) const
    {
        cout << "(" << myIndex.x << ", " << myIndex.y << ", " << myIndex.z << ") - ";
        cout << "(" << myData.a << ", " << myData.b << ")" << prompt << endl;
    }

private:
    MyTest(const MyTest&);
    MyTest& operator= (const MyTest&);
};

//define index tag, multi_index_container, and its type
struct MyIndexTag{};
typedef multi_index_container<
    MyTest*,
    indexed_by<
        ordered_unique<
            tag<MyIndexTag>,  member<MyTest, MyIndex, &MyTest::myIndex> > > 
>MyContainer_T;

typedef MyContainer_T::index<MyIndexTag>::type MyContainerIndex_T;
typedef MyContainer_T::index<MyIndexTag>::type::iterator MyContainerIterator_T;
typedef std::pair<MyContainerIterator_T, bool> MyContainerPair_T;

//define operator< of index for this container
bool operator<(const MyIndex& lhs, const MyIndex& rhs)
{
    if (lhs.x < rhs.x) return true;
    else if (lhs.x > rhs.x) return false;
    else if (lhs.y < rhs.y) return true;
    else if (lhs.y > rhs.y) return false;
    else if (lhs.z < rhs.z) return true;
    else if (lhs.z > rhs.z) return false;
    else return false;
}

//define a global container
MyContainer_T mycontainer;


//a template to print a container
template<typename Tag, typename MultiIndexContainer>
void print_container(const MultiIndexContainer& container)
{
    //obtain a reference to the index tagged by Tag
    const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& indexSet = get<Tag>(container);

    typedef typename MultiIndexContainer::value_type value_type;

    //dump the elements of the index to cout
    std::copy(indexSet.begin(), indexSet.end(), std::ostream_iterator<value_type>(cout));
}

//operator<< for the template to print a containter
std::ostream& operator<<(std::ostream& os, const MyTest* mytest)
{
    //mytest->print("");  //this clause can work
    os << "(" << mytest->myIndex.x << ", " << mytest->myIndex.y << ", " << mytest->myIndex.z << ") - ";
    os << "(" << mytest->myData.a << ", " << mytest->myData.b << ")" << endl;
    return os;
}

//a template to find an element in a container
template<typename Tag, typename MultiIndexContainer, typename Index>
void find_container(const MultiIndexContainer& container, const Index& index)
{
    const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& indexSet = get<Tag>(container);
    const typename boost::multi_index::index<MultiIndexContainer, Tag>::type::iterator iter = indexSet.find(index);
    if (indexSet.end() == iter)
    {
        index.print("not found");
        return;
    }

    (*iter)->print(", found");
}

//a template to free a container
template<typename MultiIndexContainer>
void free_container(MultiIndexContainer& container)
{
    typedef typename MultiIndexContainer::value_type value_type;

    while (!container.empty())
    {
        typename MultiIndexContainer::iterator iter = container.begin();
        if (NULL == (*iter))
        {
            container.erase(iter);
            continue;
        }

        value_type pobj = *iter;
        container.erase(iter);
        delete pobj;
    }
}

void test1()
{
    MyTest *a = new MyTest(1,1,1,10,100);
    mycontainer.insert(a);
    MyTest *b = new MyTest(1,1,2,20,200);
    mycontainer.insert(b);
    MyTest *c = new MyTest(1,1,3,30,300);
    mycontainer.insert(c);
}

void test2()
{
    MyTest *a = new MyTest(1,2,1,40,400);
    mycontainer.insert(a);
    MyTest *b = new MyTest(1,2,2,50,500);
    mycontainer.insert(b);
    MyTest *c = new MyTest(1,2,3,60,600);
    mycontainer.insert(c);
}

void test3()
{
    MyTest *a = new MyTest(1,3,1,70,700);
    mycontainer.insert(a);
    MyTest *b = new MyTest(1,3,2,80,800);
    mycontainer.insert(b);
    MyTest *c = new MyTest(1,3,3,90,900);
    mycontainer.insert(c);
}

void test4()
{
    MyTest *a = new MyTest(2,1,1,110,1000);
    mycontainer.insert(a);
    MyTest *b = new MyTest(2,1,2,220,2000);
    mycontainer.insert(b);
    MyTest *c = new MyTest(2,1,3,330,3000);
    mycontainer.insert(c);
}

void test5()
{
    MyTest *a = new MyTest(2,2,1,440,4000);
    mycontainer.insert(a);
    MyTest *b = new MyTest(2,2,2,550,5000);
    mycontainer.insert(b);
    MyTest *c = new MyTest(2,2,3,660,6000);
    mycontainer.insert(c);
}

void test6()
{
    MyTest *a = new MyTest(2,3,1,770,7000);
    mycontainer.insert(a);
    MyTest *b = new MyTest(2,3,2,880,8000);
    mycontainer.insert(b);
    MyTest *c = new MyTest(2,3,3,990,9000);
    mycontainer.insert(c);
}

void test_find()
{
    find_container<MyIndexTag>(mycontainer, MyIndex(1,1,1));
    find_container<MyIndexTag>(mycontainer, MyIndex(1,1,2));
    find_container<MyIndexTag>(mycontainer, MyIndex(1,1,3));

    find_container<MyIndexTag>(mycontainer, MyIndex(1,2,1));
    find_container<MyIndexTag>(mycontainer, MyIndex(1,2,2));
    find_container<MyIndexTag>(mycontainer, MyIndex(1,2,3));

    find_container<MyIndexTag>(mycontainer, MyIndex(1,3,1));
    find_container<MyIndexTag>(mycontainer, MyIndex(1,3,2));
    find_container<MyIndexTag>(mycontainer, MyIndex(1,3,3));

    find_container<MyIndexTag>(mycontainer, MyIndex(2,1,1));
    find_container<MyIndexTag>(mycontainer, MyIndex(2,1,2));
    find_container<MyIndexTag>(mycontainer, MyIndex(2,1,3));

    find_container<MyIndexTag>(mycontainer, MyIndex(2,2,1));
    find_container<MyIndexTag>(mycontainer, MyIndex(2,2,2));
    find_container<MyIndexTag>(mycontainer, MyIndex(2,2,3));

    find_container<MyIndexTag>(mycontainer, MyIndex(2,3,1));
    find_container<MyIndexTag>(mycontainer, MyIndex(2,3,2));
    find_container<MyIndexTag>(mycontainer, MyIndex(2,3,3));
}

int main()
{
    test2();
    test4();
    test6();
    test1();
    test3();
    test5();

    print_container<MyIndexTag>(mycontainer);

    cout<<endl;
    test_find();

    cout<<endl;
    free_container(mycontainer);

    return 0;
}


