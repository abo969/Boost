/**
 * boost lexical cast test
 * platform: win32, visual studio 2005/2010; Linux, gcc4.1.2
 */

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::lexical_cast;

//convert string to numerical number
void string_to_number()
{
    int nA = lexical_cast<int>("123456");
    double dB = lexical_cast<double>("123.456");
    cout<<"nA = "<<nA<<endl;    //cout is in <iostream>, and in namespace std
    cout<<"dB = "<<dB<<endl;
}

//convert numerical number to string
void number_to_string()
{
    double dC = 234.56;
    string strC = lexical_cast<string>(dC);
    cout<<"sC = "<<strC<<endl;
}

//conversion principle
/* lexical_cast depends char stream, stringstream, and it includes header 
   files automatically. its principle is as follows: 
   1. read source type data into char stream
   2. write char stream to destinate type
*/
void conversion_principle()
{
    float fD = lexical_cast<float>("456.78");
    cout<<"fD = "<<fD<<endl;

    //the above equals the following
    stringstream ss;
    ss<<"456.78";
    ss>>fD;
    cout<<"fD = "<<fD<<endl;
}

//conversion exception when type is wrong
void conversion_exception1()
{
    double dE = 0.0;

    try
    {
        dE = lexical_cast<float>("abcd");
    }
    catch(boost::bad_lexical_cast& e)
    {
        cout<<e.what()<<endl;
    }

    cout<<"dE = "<<dE<<endl;
}

//conversion exception when data is converted partially
void conversion_exception2()
{
    int nF = 0;

    try
    {
        nF = lexical_cast<int>("123.456");
    }
    catch(boost::bad_lexical_cast& e)
    {
        cout<<e.what()<<endl;
    }

    cout<<"nF = "<<nF<<endl;
}

//conversion precision
void conversion_precision()
{
    string sG = lexical_cast<string>(123.123456789);
    cout<<"sG = "<<sG<<endl;  //sG = 123.123456789, converted completely
    double dH = lexical_cast<double>("123.123456");
    cout<<"dH = "<<dH<<endl;  //dH = 123.123, converted partially
}

int main()
{
    string_to_number();    
    number_to_string();
    conversion_principle();
    conversion_exception1();
    conversion_exception2();
    conversion_precision();

    return 0;
}

