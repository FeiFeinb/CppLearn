#include "Utilities/AllHeader.h"

using namespace std;

void MapCreateTest()
{
    map<string, int> map;
    // 利用重载[]插入
    map["Zoom"] = 25;
    map.insert(make_pair<string, int>("Mike", 18));
    auto p = map.insert(make_pair<string, int>("Mike", 30));
    // 1
    cout << map.size() << endl;
    // 检测是否插入失败
    if (p.second == false)
        cout << "insert fail, map exists " << (*p.first).first << (*p.first).second << endl;

    multimap<string, int> multiMap;
    multiMap.insert(make_pair<string, int>("Jelly", 18));
    multiMap.insert(make_pair<string, int>("Jelly", 20));
    // 2
    cout << multiMap.size() << endl;
}

void SearchMapTest()
{
    map<string, int> map;
    map.insert(make_pair<string, int>("Mike", 18));
    // 只有map有重载[]运算符 ( )
    cout << "use operator[] to find unique element " << map["Mike"] << endl;

    multimap<string, int> multiMap;
    multiMap.insert(make_pair<string, int>("Jelly", 19));
    multiMap.insert(make_pair<string, int>("Jelly", 20));
    multiMap.insert(make_pair<string, int>("Zed", 22));
    multiMap.insert(make_pair<string, int>("Zed", 24));
    // 输出第一个 19
    cout << "use find to search the first element " << (*multiMap.find("Jelly")).second << endl;
    auto notFind = multiMap.find("Angle");
    if (notFind == multiMap.end())
        cout << "cant find the element Angle" << endl;


    // 输出 19 20
    cout << "use thr iterator to traverse Key" << endl;
    // 利用二分查找 从找到的第一个Jelly开始到最后一个Jelly 遍历
    for (auto start = multiMap.lower_bound("Jelly"); start != multiMap.upper_bound("Jelly"); start++)
        cout << (*start).second << endl;
}


void FindTimeTest()
{
    unordered_multiset<int> multiSet;
    multiSet.insert(10);
    multiSet.insert(20);

    // 使用类自带的Find 效率更高
    auto p1 = multiSet.find(10);
    if (p1 != multiSet.end())
        cout << "find value " << (*p1) << endl;

    // 使用STL中的全局Find 效率较低
    auto p2 = ::find(multiSet.begin(), multiSet.end(), 20);
    if (p2 != multiSet.end())
        cout << "find value " << (*p2) << endl;
}


template<typename T>
struct TestTemplate
{
};

template<>
struct TestTemplate<int>
{
    void intFunc() {}
};

template<>
struct TestTemplate<string>
{
    void TestStringFunc() {}
};

void TemplateStructTest()
{
    // 全特化测试
    TestTemplate<double> ttd;
    TestTemplate<int> tti;
    tti.intFunc();
    TestTemplate<string> tts;
    tts.TestStringFunc();
}


template<typename T1, typename T2 = bool>
struct TestPartial
{
    void Test(T1 t1, T2 t2) {}

    void MoreMotherFunc() {}
};

template<typename T2>
struct TestPartial<int, T2>
{
    void Test(int t1, T2 t2) {}

    void MoreIntFunc() {}
};

void TemplatePartialTest()
{
    TestPartial<int> ti;
    // 偏特化版本 且使用默认模板类型
    // 无法使用t调用 MoreMotherFunc()
    ti.MoreIntFunc();

    // 无特化版本 但使用默认模板类型
    TestPartial<string> ts;
    ts.Test("Mike", false);
    ts.MoreMotherFunc();

    // 无特化版本 不使用默认模板类型
    TestPartial<double, string> td;
    td.Test(3.14, "123");
    td.MoreMotherFunc();
}

template<typename T>
struct TestPointer
{
    void Value() { cout << "Value" << endl; }
};

template<typename T>
struct TestPointer<T*>
{
    void Pointer() { cout << "Pointer" << endl; }
};

template<typename T>
struct TestPointer<const T*>
{
    void ConstPointer() { cout << "ConstPointer" << endl; }
};

void TemplatePointerTest()
{
    TestPointer<int> ti;
    ti.Value();
    TestPointer<int*> pti;
    pti.Pointer();

    TestPointer<const int*>().ConstPointer();
}


void SizeTest()
{
    vector<int> v;
    vector<int> vEmpty;
    for (int i = 0; i < 1000; i++)
        v.push_back(i);
    // 一样的结果输出
    cout << "sizeof v = " << sizeof(v) << endl;
    cout << "size of vEmpty = " << sizeof(vEmpty) << endl;
}

template<typename T>
class MyList : public list<T>
{
public:
    const T& operator[](const int index) const
    {
        if (index >= this->size())
            throw "out of range";
        auto p = this->begin();
        for (int i = 0; i < index; i++, p++);
        return *p;
    }
};

void ListOperatorTest()
{
    MyList<int> ml;
    for (int i = 0; i < 200; i++)
        ml.push_back(i);

    auto p = ::find(ml.begin(), ml.end(), 199);
    *p = 300;

    // 输出0
    cout << ml[0] << endl;
    // 输出300
    cout << ml[199] << endl;
}

struct ListNode
{
    ListNode() = default;

    ListNode(const ListNode& node) = default;

    explicit ListNode(string _data, ListNode* _next = nullptr) : data(std::move(_data)), nextNode(_next) {}

    string data;
    ListNode* nextNode = nullptr;
};

class ListNodeIterator
{
public:
    ListNode* node;

    explicit ListNodeIterator(ListNode* listNode) : node(listNode) {}

    ListNodeIterator(const ListNodeIterator& copy)
    {
        node = new ListNode(*copy.node);
    }

    ~ListNodeIterator()
    {
        delete node;
    }

    string& operator*() const
    {
        return node->data;
    }

    // 前置++
    ListNodeIterator& operator++()
    {
        node = node->nextNode;
        return *this;
    }

    // 后置++
    ListNodeIterator operator++(int)
    {
        // 调用到拷贝构造函数 进行一次默认的浅拷贝 故需要自行书写拷贝构造函数 同时该拷贝构造函数不能为explicit
        ListNodeIterator temp = *this;
        // ListNodeIterator temp(*this); // 两种写法
        operator++();
        // ++(*this); // 两种写法
        return temp;
    }
};

void TestIteratorOperator()
{
    auto node2 = new ListNode("Mike");
    auto node1 = new ListNode("Jelly", node2);
    // 因为是explicit 所以不支持隐式构造
    // ListNodeIterator iterator = node1;
    ListNodeIterator iterator(node1);
    cout << *(iterator) << endl;
    cout << *(++iterator) << endl;
}

void TestIteratorTraits()
{
    // 通过value_type直接获得string类型

    vector<string>::value_type aStr = "123";
    vector<string>::iterator::value_type bStr = "123";

    // <algorithm>通过iterator_traits实现的模板偏特化 判断传进的类型是class iterator还是non-class iterator
    iterator_traits<vector<string>::iterator>::value_type a = "123";
    // 不管是non-const pointer还是const pointer 都只萃取指针的类型
    iterator_traits<string*>::value_type b = "123";
    iterator_traits<const string*>::value_type c = "123";
    // 尽管是const string* 但还是只萃取到string
    c = "1234";
}

void TestBound()
{
    vector<int> myList{1, 2, 4, 4, 4, 4, 5, 6, 8, 8};
    // 从左往右找第一个大于等于2的数 myList[1] = 2
    vector<int>::iterator lower = ::lower_bound(myList.begin(), myList.end(), 2, ::less<int>());
    cout << "下标在" << lower - myList.begin() << "的元素" << *lower << endl;
    // 从左往右找第一个大于2的数 myList[2] = 4
    // 默认是使用less<int>
    vector<int>::iterator upper = ::upper_bound(myList.begin(), myList.end(), 2);
    // 只有各element在内存中顺序排列能执行此操作 list就不行
    cout << "下标在" << upper - myList.begin() << "的元素" << *upper << endl;

    // 如果是降序序列 代表左边大于右边 即greater
    // {8, 8, 6, 5, 4, 4, 4, 4, 2, 1}
    ::sort(myList.begin(), myList.end(), ::greater<int>());
    lower = ::lower_bound(myList.begin(), myList.end(), 2, ::greater<int>());
    // 从左往右找到第一个小于等于2的数 myList[9] = 2
    cout << "下标在" << lower - myList.begin() << "的元素" << *lower << endl;
    upper = ::upper_bound(myList.begin(), myList.end(), 2, ::greater<int>());
    // 从左往右找第一个小于2的数 myList[9] = 1
    cout << "下标在" << upper - myList.begin() << "的元素" << *upper << endl;

    // 综上 less<int> 给升序数组用
    // greater<int>给降序数组用
}

class TreeNode
{
public:
    int data;

    TreeNode()
    {
        cout << "无参构造" << endl;
    }

    TreeNode(int _data) : data(_data)
    {
        cout << "有参构造" << endl;
    }

    ~TreeNode()
    {
        cout << "销毁" << endl;
    }
};

vector<string> split(const string& data)
{
    vector<string> result;
    for (int i = 0; i < data.size(); i++)
    {
        result.emplace_back();
        while (data[i] != ',')
        {
            result.back().push_back(data[i++]);
        }
    }
    return result;
}

class Any
{
public:
    template<class T>
    using StorageType = typename std::decay<T>::type;

    template<typename U,
            class=typename std::enable_if<
                    !std::is_same<StorageType<U>, Any>::value, void
            >::type>
    Any(U&& _value)
            : ptr(new Container<StorageType<U>>(std::forward<U>(_value)))
    {
    }

    Any()
            : ptr(nullptr)
    {
    }

    Any(const Any& _that)
            : ptr(_that.clone())
    {
    }

    Any(Any&& _that)
            : ptr(_that.ptr)
    {
        _that.ptr = nullptr;
    }

    template<class U>
    inline
    bool is() const
    {
        typedef StorageType<U> T;

        if (!ptr)
            return false;

        auto derived = dynamic_cast<Container <T>*> (ptr);

        return !(derived == nullptr);
    }

    template<class U>
    inline
    StorageType<U>& as() const
    {
        typedef StorageType<U> T;

        if (!ptr)
            throw std::bad_cast();

        auto container = dynamic_cast<Container <T>*> (ptr);

        if (!container)
            throw std::bad_cast();

        return container->value;
    }

    template<class U>
    inline
    StorageType<U>& value() const
    {
        return as<StorageType<U>>();
    }

    template<class U>
    inline
    operator U() const
    {
        return as<StorageType<U>>();
    }

    Any& operator=(const Any& a)
    {
        if (ptr == a.ptr)
            return *this;
        auto old_ptr = ptr;
        ptr = a.clone();
        if (old_ptr)
            delete old_ptr;
        return *this;
    }

    Any& operator=(Any&& a)
    {
        if (ptr == a.ptr)
            return *this;
        std::swap(ptr, a.ptr);
        return *this;
    }

    ~Any()
    {
        if (ptr)
            delete ptr;
    }

private:
    class ContainerBase
    {
    public:
        virtual ~ContainerBase() {}

        virtual ContainerBase* clone() const = 0;
    };

    template<typename T>
    class Container
            : public ContainerBase
    {
    public:
        template<typename U>
        Container(U&& value)
                :value(std::forward<U>(value))
        {
        }

        inline
        ContainerBase* clone() const
        {
            return new Container<T>(value);
        }

        T value;
    };

    inline
    ContainerBase* clone() const
    {
        if (ptr)
            return ptr->clone();
        else
            return nullptr;
    }

    ContainerBase* ptr;
};

class TestClass
{
public:
    int data;

    TestClass() { cout << "default create" << endl; }

    TestClass(int _data) : data(_data) { std::cout << "param create" << std::endl; }

    ~TestClass() { std::cout << "destroy" << std::endl; }
};








void f(int n)
{
    std::this_thread::sleep_for(std::chrono::seconds(n));
    std::cout << "thread " << n << " ended" << '\n';
}







// std::vector<int> vec{1, 2, 3, 4, 4, 6, 7, 8};
// std::replace_if(vec.begin(), vec.end(), std::bind(std::less<int>{}, std::placeholders::_1, 4), 100);
// for (auto i : vec)
//     std::cout << i << std::endl;
// std::vector<int> result;
// std::function<std::vector<int>(const std::vector<int>&)> func = [&](const std::vector<int>& vec) { return (std::transform(vec.begin(), vec.end(), vec.begin(), std::back_inserter(result), std::multiplies<>{}), result);};
