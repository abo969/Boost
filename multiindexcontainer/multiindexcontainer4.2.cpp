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

class MyContainer
{
    MyContainer_T theContainer;

public:
    void insert(MyTest* data);
    void find(const MyIndex& index);
    void print();
    void free();
};

void MyContainer::insert(MyTest* data)
{
    theContainer.insert(data);
}

void MyContainer::find(const MyIndex& index)
{
    const boost::multi_index::index<MyContainer_T, MyIndexTag>::type& indexSet = get<MyIndexTag>(theContainer);
    const boost::multi_index::index<MyContainer_T, MyIndexTag>::type::iterator iter = indexSet.find(index);
    if (indexSet.end() == iter)
    {
        index.print("not found");
        return;
    }

    (*iter)->print(", found");
}

void MyContainer::print()
{
    //obtain a reference to the index tagged by Tag
    const boost::multi_index::index<MyContainer_T, MyIndexTag>::type& indexSet = get<MyIndexTag>(theContainer);

    typedef MyContainer_T::value_type value_type;

    //dump the elements of the index to cout
    std::copy(indexSet.begin(), indexSet.end(), std::ostream_iterator<value_type>(cout));
}

void MyContainer::free()
{
    typedef MyContainer_T::value_type value_type;

    while (!theContainer.empty())
    {
        MyContainer_T::iterator iter = theContainer.begin();
        if (NULL == (*iter))
        {
            theContainer.erase(iter);
            continue;
        }

        value_type pobj = *iter;
        theContainer.erase(iter);
        delete pobj;
    }
}

//operator<< for the template to print a containter
std::ostream& operator<<(std::ostream& os, const MyTest* mytest)
{
    //mytest->print("");  //this clause can work
    os << "(" << mytest->myIndex.x << ", " << mytest->myIndex.y << ", " << mytest->myIndex.z << ") - ";
    os << "(" << mytest->myData.a << ", " << mytest->myData.b << ")" << endl;
    return os;
}

void test1(MyContainer& container)
{
    MyTest *a = new MyTest(1,1,1,10,100);
    container.insert(a);
    MyTest *b = new MyTest(1,1,2,20,200);
    container.insert(b);
    MyTest *c = new MyTest(1,1,3,30,300);
    container.insert(c);
}

void test2(MyContainer& container)
{
    MyTest *a = new MyTest(1,2,1,40,400);
    container.insert(a);
    MyTest *b = new MyTest(1,2,2,50,500);
    container.insert(b);
    MyTest *c = new MyTest(1,2,3,60,600);
    container.insert(c);
}

void test3(MyContainer& container)
{
    MyTest *a = new MyTest(1,3,1,70,700);
    container.insert(a);
    MyTest *b = new MyTest(1,3,2,80,800);
    container.insert(b);
    MyTest *c = new MyTest(1,3,3,90,900);
    container.insert(c);
}

void test4(MyContainer& container)
{
    MyTest *a = new MyTest(2,1,1,110,1000);
    container.insert(a);
    MyTest *b = new MyTest(2,1,2,220,2000);
    container.insert(b);
    MyTest *c = new MyTest(2,1,3,330,3000);
    container.insert(c);
}

void test5(MyContainer& container)
{
    MyTest *a = new MyTest(2,2,1,440,4000);
    container.insert(a);
    MyTest *b = new MyTest(2,2,2,550,5000);
    container.insert(b);
    MyTest *c = new MyTest(2,2,3,660,6000);
    container.insert(c);
}

void test6(MyContainer& container)
{
    MyTest *a = new MyTest(2,3,1,770,7000);
    container.insert(a);
    MyTest *b = new MyTest(2,3,2,880,8000);
    container.insert(b);
    MyTest *c = new MyTest(2,3,3,990,9000);
    container.insert(c);
}

void test_find(MyContainer& container)
{
    container.find(MyIndex(1,1,1));
    container.find(MyIndex(1,1,2));
    container.find(MyIndex(1,1,3));

    container.find(MyIndex(1,2,1));
    container.find(MyIndex(1,2,2));
    container.find(MyIndex(1,2,3));

    container.find(MyIndex(1,3,1));
    container.find(MyIndex(1,3,2));
    container.find(MyIndex(1,3,3));

    container.find(MyIndex(2,1,1));
    container.find(MyIndex(2,1,2));
    container.find(MyIndex(2,1,3));

    container.find(MyIndex(2,2,1));
    container.find(MyIndex(2,2,2));
    container.find(MyIndex(2,2,3));

    container.find(MyIndex(2,3,1));
    container.find(MyIndex(2,3,2));
    container.find(MyIndex(2,3,3));
}

int main()
{
    MyContainer mycontainer;
    
    test2(mycontainer);
    test4(mycontainer);
    test6(mycontainer);
    test1(mycontainer);
    test3(mycontainer);
    test5(mycontainer);

    mycontainer.print();
    cout<<endl;
    test_find(mycontainer);
    cout<<endl;
    mycontainer.free();

    return 0;
}


