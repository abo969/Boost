/**
 * boost multi index container test
 * platform: win32, visual studio 2005/2010; Linux, gcc4.1.2
 */

#include <iostream>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

using namespace std;
using namespace boost::multi_index;
using boost::multi_index_container;

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
    int x;
    int y;
    int z;
    MyData  myData;

public:

    MyTest(int x_, int y_, int z_, int a, int b):x(x_), y(y_), z(z_)
    {
        myData.a = a;
        myData.b = b;
    }

    ~MyTest()
    {
        cout << "(" << x << ", " << y << ", " << z << ") - ";
        cout << "(" << myData.a << ", " << myData.b << "), destructed" << endl;
    }

    void print(char* prompt) const
    {
        cout << "(" << x << ", " << y << ", " << z << ") - ";
        cout << "(" << myData.a << ", " << myData.b << ")" << prompt << endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyTest* mytest)
    {
        mytest->print("");
        return os;
    }

private:
    MyTest(const MyTest&);
    MyTest& operator=(const MyTest&); 
};

struct MyIndex_X_Tag{};
struct MyIndex_XY_Tag{};
struct MyIndex_XYZ_Tag{};

typedef multi_index_container<
    MyTest*,
    indexed_by<
        ordered_non_unique<tag<MyIndex_X_Tag>, 
            composite_key<MyTest, member<MyTest, int, &MyTest::x> > >,
        ordered_non_unique<tag<MyIndex_XY_Tag>, 
            composite_key<MyTest, member<MyTest, int, &MyTest::x>, member<MyTest, int, &MyTest::y> > >,
        ordered_unique<tag<MyIndex_XYZ_Tag>, 
            composite_key<MyTest, member<MyTest, int, &MyTest::x>, member<MyTest, int, &MyTest::y>,  member<MyTest, int, &MyTest::z> > >
    >
>MyContainer_T;

typedef nth_index<MyContainer_T, 0>::type MyContainer_X_T;
typedef nth_index<MyContainer_T, 1>::type MyContainer_XY_T;
typedef nth_index<MyContainer_T, 2>::type MyContainer_XYZ_T;

typedef MyContainer_T::index<MyIndex_X_Tag>::type MyContainerIndex_X_T;
typedef MyContainer_T::index<MyIndex_X_Tag>::type::iterator MyContainerIterator_X_T;
typedef std::pair<MyContainerIterator_X_T, bool> MyContainerPair_X_T;

typedef MyContainer_T::index<MyIndex_XY_Tag>::type MyContainerIndex_XY_T;
typedef MyContainer_T::index<MyIndex_XY_Tag>::type::iterator MyContainerIterator_XY_T;
typedef std::pair<MyContainerIterator_XY_T, bool> MyContainerPair_XY_T;

typedef MyContainer_T::index<MyIndex_XYZ_Tag>::type MyContainerIndex_XYZ_T;
typedef MyContainer_T::index<MyIndex_XYZ_Tag>::type::iterator MyContainerIterator_XYZ_T;
typedef std::pair<MyContainerIterator_XYZ_T, bool> MyContainerPair_XYZ_T;

typedef boost::tuples::tuple<int> MyTuple_X_T;
typedef boost::tuples::tuple<int, int> MyTuple_XY_T;
typedef boost::tuples::tuple<int, int, int> MyTuple_XYZ_T;


//a template class
template <class MultiIndexContainer_T, class MultiIndexContainerIterator_T, class Tag_T, class Data_T, class Tuple_T>
class MyContainer
{
    MultiIndexContainer_T theContainer;

public:
    void insert(Data_T* data);
    int query(const Tuple_T& tuple);
    void print();
    void free();
};

template <class MultiIndexContainer_T, class MultiIndexContainerIterator_T, class Tag_T, class Data_T, class Tuple_T>
void MyContainer<MultiIndexContainer_T, MultiIndexContainerIterator_T, Tag_T, Data_T, Tuple_T>::insert(Data_T* data)
{
    theContainer.insert(data);
}

template <class MultiIndexContainer_T, class MultiIndexContainerIterator_T, class Tag_T, class Data_T, class Tuple_T>
int MyContainer<MultiIndexContainer_T, MultiIndexContainerIterator_T, Tag_T, Data_T, Tuple_T>::query(const Tuple_T& tuple)
{
    MultiIndexContainerIterator_T it0, it1;
    boost::tie(it0, it1) = theContainer.equal_range(tuple);

    if (*it0 == *it1)
        return -1;

    while (it0 != it1)
    {
        (*it0)->print(", found");
        ++it0;
    }
    return 0;
}


template <class MultiIndexContainer_T, class MultiIndexContainerIterator_T, class Tag_T, class Data_T, class Tuple_T>
void MyContainer<MultiIndexContainer_T, MultiIndexContainerIterator_T, Tag_T, Data_T, Tuple_T>::print()
{
    const typename boost::multi_index::index<MultiIndexContainer_T, Tag_T>::type& indexSet = get<Tag_T>(theContainer);

    typedef typename MultiIndexContainer_T::value_type value_type;
    std::copy(indexSet.begin(), indexSet.end(), std::ostream_iterator<value_type>(cout));
}

template <class MultiIndexContainer_T, class MultiIndexContainerIterator_T, class Tag_T, class Data_T, class Tuple_T>
void MyContainer<MultiIndexContainer_T, MultiIndexContainerIterator_T, Tag_T, Data_T, Tuple_T>::free()
{
    typedef typename MultiIndexContainer_T::value_type value_type;

    while (!theContainer.empty())
    {
        typename MultiIndexContainer_T::iterator iter = theContainer.begin();
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


MyContainer<MyContainer_X_T, MyContainerIterator_X_T, MyIndex_X_Tag, MyTest, MyTuple_X_T> mycontainer;
MyContainer<MyContainer_X_T, MyContainerIterator_X_T, MyIndex_X_Tag, MyTest, MyTuple_X_T>& mycontainerx = mycontainer;
MyContainer<MyContainer_XY_T, MyContainerIterator_XY_T, MyIndex_XY_Tag, MyTest, MyTuple_XY_T> mycontainerxy;
MyContainer<MyContainer_XYZ_T, MyContainerIterator_XYZ_T, MyIndex_XYZ_Tag, MyTest, MyTuple_XYZ_T> mycontainerxyz;


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

void query_test1()
{
    mycontainerx.query(static_cast<MyTuple_X_T>(boost::make_tuple(1)));
    cout<<endl;

    mycontainerx.query(static_cast<MyTuple_X_T>(boost::make_tuple(2)));
    cout<<endl;

    MyTuple_X_T tuple(boost::make_tuple(3));
    if (-1 ==mycontainerx.query(tuple))
        cout << "("<< tuple.get<0>() << ") - not found" << endl;
}

void query_test2()
{
    mycontainerxy.query(static_cast<MyTuple_XY_T>(boost::make_tuple(1, 1)));
    mycontainerxy.query(static_cast<MyTuple_XY_T>(boost::make_tuple(1, 2)));
    mycontainerxy.query(static_cast<MyTuple_XY_T>(boost::make_tuple(1, 3)));

    MyTuple_XY_T tuple1(boost::make_tuple(1, 4));
    if (-1 == mycontainerxy.query(tuple1))
        cout << "("<< tuple1.get<0>() <<", "<< tuple1.get<1>() << ") - not found" << endl;
    cout<<endl;

    mycontainerxy.query(static_cast<MyTuple_XY_T>(boost::make_tuple(2, 1)));
    mycontainerxy.query(static_cast<MyTuple_XY_T>(boost::make_tuple(2, 2)));
    mycontainerxy.query(static_cast<MyTuple_XY_T>(boost::make_tuple(2, 3)));

    MyTuple_XY_T tuple2(boost::make_tuple(2, 4));
    if (-1 == mycontainerxy.query(tuple2))
        cout << "("<< tuple2.get<0>() <<", "<< tuple2.get<1>() << ") - not found" << endl;
}

void query_test3()
{
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 1, 1)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 1, 2)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 1, 3)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 2, 1)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 2, 2)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 2, 3)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 3, 1)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 3, 2)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(1, 3, 3)));
    cout<<endl;
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 1, 1)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 1, 2)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 1, 3)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 2, 1)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 2, 2)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 2, 3)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 3, 1)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 3, 2)));
    mycontainerxyz.query(static_cast<MyTuple_XYZ_T>(boost::make_tuple(2, 3, 3)));
    cout<<endl;

    MyTuple_XYZ_T tuple1(boost::make_tuple(2, 3, 4));
    if (-1 == mycontainerxyz.query(tuple1))
        cout << "("<< tuple1.get<0>() <<", "<< tuple1.get<1>() << ", " << tuple1.get<2>() <<") - not found" << endl;

    MyTuple_XYZ_T tuple2(boost::make_tuple(3, 3, 3));
    if (-1 == mycontainerxyz.query( tuple2))
        cout << "("<< tuple2.get<0>() <<", "<< tuple2.get<1>() << ", " << tuple2.get<2>() <<") - not found" << endl;
}

void query_test()
{
    query_test1();
    cout<<endl;
    query_test2();
    cout<<endl;
    query_test3();
}

int main()
{
    test2();
    test4();
    test6();
    test1();
    test3();
    test5();

    //print_container<MyIndex_X_Tag>(mycontainer);
    //cout<<endl;

    //print_container<MyIndex_XY_Tag>(mycontainer);
    //cout<<endl;

    mycontainer.print();
    cout<<endl;

    query_test();
    cout<<endl;

    mycontainer.free();

    return 0;
}


