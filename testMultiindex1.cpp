/**
 * boost lexical cast test
 * platform: win32, visual studio 2005/2010; Linux, gcc4.1.2
 */

#include <string>
#include <boost/shared_ptr.hpp>

class XBase
{
public:
    enum OBJATTR;
    int Get(OBJATTR attr) const;
    int Set(OBJATTR attr, int nValue);
    int Change(OBJATTR attr, int nChange);
    int Reset(OBJATTR attr);
};

class XDerived11 : public XBase
{
};

class XDerived12 : public XDerived11
{
};

class XTestClass: public XDerived12
{
public:
    using XBase::Get;
    using XBase::Set;
    using XBase::Change;
    using XBase::Reset;

    enum OBJATTR;
    int Get(OBJATTR attr) const
    {
        return 0;
    }
    int Set(OBJATTR attr, int nValue)
    {
        return 0;
    }
    int Change(OBJATTR attr, int nChange)
    {
        Set(attr, nChange);
    }
    int Reset(OBJATTR attr)
    {
        Set(attr, 0);
    }

    enum OBJATTR
    {
        X,
        Y
    };

    void SetX(int nValue);
    int GetX() const;

    const char* GetName() const;
    void SetName(const char* cpName);

protected:
    std::string m_strName;
};

inline void XTestClass::SetX(int nValue)
{
    Set(X, nValue);
}
inline int XTestClass::GetX() const
{
    return Get(X);
}

inline const char* XTestClass::GetName() const
{
    return m_strName.c_str();
}
inline void XTestClass::SetName(const char* cpName)
{
    m_strName = cpName;
}


class XProblemClass : public XTestClass
{
public:
    using XTestClass::Get;
    using XTestClass::Set;
    using XTestClass::Change;
    using XTestClass::Reset;

    enum OBJATTR;
    int Get(OBJATTR attr) const
    {
        return 0;
    }
    int Set(OBJATTR attr, int nValue)
    {
        return 0;
    }
    int Change(OBJATTR attr, int nChange)
    {
        Set(attr, nChange);
    }
    int Reset(OBJATTR attr)
    {
        Set(attr, 0);
    }

    enum OBJATTR
    {
        X1,
        Y1
    };
};

void fun(boost::shared_ptr<XProblemClass> spA)
{

}

void fun(boost::shared_ptr<XTestClass> spA)
{

}

int main()
{
    return 0;
}

