/**
 * boost multi index container test
 * platform: win32, visual studio 2005/2010; Linux, gcc4.1.2
 */

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

#include "boost/function.hpp"
#include "boost/lambda/lambda.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/ordered_index.hpp"

using namespace std;
using namespace boost::multi_index;
using boost::multi_index_container;

/* course class */
struct Course   
{
    unsigned int course_num;  //course number
    unsigned int course_hour; //course hour
    std::string  course_name; //course name

    //courses
    enum CourseNum_Enum
    {
        CourseNum_Base = 1001,
        CourseNum_Cpp   = 0,
        CourseNum_English,
        CourseNum_Maths,
        CourseNum_Computer,
        CourseNum_DataStructure
    };

    static const Course courses[5];
    Course(unsigned int num, unsigned int hour, std::string name);
};   

const Course Course::courses[5] = {
    Course(Course::CourseNum_Cpp,           50, "C plus plus" ),
    Course(Course::CourseNum_English,       60, "English"),
    Course(Course::CourseNum_Maths,         70, "Mathematics"),
    Course(Course::CourseNum_Computer,      80, "Computer principle"),
    Course(Course::CourseNum_DataStructure, 72, "data structure")
};

Course::Course(unsigned int num, unsigned int hour, std::string name):
    course_num(num), course_hour(hour), course_name(name)
{
}


/* student class */
struct Student
{
    unsigned int     stu_num;         //student number
    std::string      stu_name;        //student name
    unsigned int     stu_age;         //student age
    std::set<Course> stu_courselist;  //course list of this student

    friend Student* CreateStudent(const std::string& name, unsigned int age);

private:
    Student(unsigned int num, const std::string& name, unsigned int age);    
};

Student::Student(unsigned int num, const std::string& name, unsigned int age):
    stu_num(num), stu_name(name), stu_age(age)
{
}

Student* CreateStudent(const std::string& name, unsigned int age)
{
    static unsigned int currentnum = 20101001;  //the student number from 20101001
    return new Student(currentnum++, name, age);
}


/* define tag for variable need to be indexed */
struct stu_num_tag{};   //index: student number
struct stu_name_tag{};  //index: student name
struct stu_age_tag{};   //index: student age

/* index type is Student. indexed_by is used to create index.
   ordered_unique is used to create index whose value is unique and ordered.
   ordered_non_unique is used to create index whose value is not unique but ordered.
   tag<...> is used to instantiate stu_num_tag, etc.
   BOOST_MULTI_INDEX_MEMBER is a macro, 3 parameters: class, type, member name.
 */
typedef
boost::multi_index_container<
    Student*,
    indexed_by<
        ordered_unique<tag<stu_num_tag>,      BOOST_MULTI_INDEX_MEMBER(Student, unsigned int, stu_num)>, 
        ordered_non_unique<tag<stu_name_tag>, BOOST_MULTI_INDEX_MEMBER(Student, std::string, stu_name)>,
        ordered_non_unique<tag<stu_age_tag>,  BOOST_MULTI_INDEX_MEMBER(Student, unsigned int, stu_age)>
    >
> StudentContainer;

//this template function is from boost doc
template<typename Tag, typename MultiIndexContainer>
void print_out_by(const MultiIndexContainer& s)
{
    //obtain a reference to the index tagged by Tag
    const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& i = get<Tag>(s);

    typedef typename MultiIndexContainer::value_type value_type;

    //dump the elements of the index to cout
    std::copy(i.begin(), i.end(), std::ostream_iterator<value_type>(cout));
}

std::ostream& operator<<(std::ostream& os, const Course& cou)
{
    os<<"  course number: "<<cou.course_num<<"\thour: "<<cou.course_hour<<"\tname: "<<cou.course_name<<endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Student& stu)
{
    os<<"student number : "<<stu.stu_num<<"\nname           : "<<stu.stu_name<<"\nage            : "<<stu.stu_age<<endl;
    std::for_each(stu.stu_courselist.begin(), stu.stu_courselist.end(), cout<<boost::lambda::_1);
    os<<endl;
    return os;
}

bool operator<(const Course& lhs, const Course& rhs)
{
    return lhs.course_num < rhs.course_num;
}

void test1(StudentContainer& stusets)
{
    // insert some data
    Student *stu1 = CreateStudent("John", 20);
    stu1->stu_courselist.insert(Course::courses[Course::CourseNum_Cpp]);
    stu1->stu_courselist.insert(Course::courses[Course::CourseNum_English]);
    stusets.insert(stu1);

    Student *stu2 = CreateStudent("Tom", 22);
    stu2->stu_courselist.insert(Course::courses[Course::CourseNum_Cpp]);
    stu2->stu_courselist.insert(Course::courses[Course::CourseNum_DataStructure]);
    stu2->stu_courselist.insert(Course::courses[Course::CourseNum_Computer]);
    stusets.insert(stu2);

    Student *stu3 = CreateStudent("Mike", 19);
    stu3->stu_courselist.insert(Course::courses[Course::CourseNum_English]);
    stu3->stu_courselist.insert(Course::courses[Course::CourseNum_Maths]);
    stusets.insert(stu3);

    Student *stu4 = CreateStudent("Tom", 20);
    stu4->stu_courselist.insert(Course::courses[Course::CourseNum_Computer]);
    stu4->stu_courselist.insert(Course::courses[Course::CourseNum_Maths]);
    stusets.insert(stu4);

    //output the students order by student number
    print_out_by<stu_name_tag>(stusets);
}

void test2()
{
}

//find all students with name "Tom"
void test3(StudentContainer& stusets)
{
    //student name is used to be index
    StudentContainer::index<stu_name_tag>::type& indexOfName = stusets.get<stu_name_tag>();

    //find the lower bound and upper bound with name "Tom"
    StudentContainer::index<stu_name_tag>::type::iterator iterLower = indexOfName.lower_bound("Tom");
    StudentContainer::index<stu_name_tag>::type::iterator iterUpper = indexOfName.upper_bound("Tom");

    while (iterLower != iterUpper)
    {
        std::cout<<*iterLower;
        ++iterLower;
    }
}

int main()
{
    StudentContainer studentsets;
    
    cout<<"====== all students"<<endl<<endl;
    test1(studentsets);

    cout<<endl<<"====== all students named \"Tom\""<<endl<<endl;
    test3(studentsets);

    return 0;
}

