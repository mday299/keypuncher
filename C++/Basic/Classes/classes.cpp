/*
BOTTOM LINE UP FRONT: std::move has weird behavior for types more complex
 than char, e.g., std::string

Write a program that defines a class called MyClass and makes an
 instance of MyClass in the main function.
Make MyClass have data members int, char, bool, double, and string
Put member function printmessage into MyClass and have it take a string arg

Write a program that defines a class called MyClass with one private data 
 member of type int called x and two member functions. The first member 
 function called setx(int myvalue) will set the value of x to its parameter
 myvalue. The second member function is called getx(), is of type int,
 and returns a value of x. 
Create an instance of the class and use the object to access both member
 functions.

Write a program that defines a class called MyClass with a user-defined default
 constructor and user-defined destructor. Both member functions will output a
 free-to-choose text on the standard output. 
Create an object of a class in function main.

Write a program that defines a class called MyClass with arbitrary data fields.
 Write a user-defined constructor with parameters that initializes data members.
 Write a user-defined copy constructor which copies all the members. 
 Write a user-provided move constructor (THIS IS NOT POSSIBLE with strings type!!)
Create one object of the class, called o1, and initialize it with values. 
Create another object of a class, called o2, and initialize it with object o1. 
Print out the data for both objects.

Write a program that overloads the arithmetic operator – in terms of a compound
 arithmetic operator -=. Subtract one object from the other and assign the 
 result to a third object. 
Print out the values of the resulting object’s member fields.
*/

#include <iostream>
#include <string>

class MyClass {
public:
    int myInt;
    char myChar;
    bool myBool;
    double myDouble;
    std::string myStr;

    //user defined constructor 
    MyClass(int i, char c, bool b, double d, std::string s)
     : myInt{i}, myChar(c), myBool{b}, myDouble{d}, myStr{s} {         
        std::cout << "Constructor invoked" << std::endl;
    }

    //user defined copy constructor
    MyClass(const MyClass& toCopy) 
     : myInt{toCopy.myInt}, myChar(toCopy.myChar), myBool(toCopy.myBool), 
       myDouble(toCopy.myDouble), myStr{toCopy.myStr} {
        std::cout << "Copy Constructor Invoked" << std::endl;
    }

    //user defined move constructor
    MyClass(MyClass&& otherObj) 
     : myInt(otherObj.myInt), myChar(otherObj.myChar),
       myBool(otherObj.myBool), myDouble(std::move(otherObj.myDouble)),
        //Can't move strings with std::move!!!
       myStr(std::move(otherObj.myStr)) {

        std::cout << "Move constructor invoked." << std::endl;
     }

    ~MyClass() { //user defined destructor
        std::cout << "Destructor invoked" << std::endl;
    }

    void printData() {
        std::cout << myInt << ", " << myChar << ", " << myBool << ", " << 
         myDouble << ", " << myStr << std::endl;
    }

    void printMessage (std::string mess) {
        std::cout << mess << std::endl;
    }

    MyClass& operator-=(const MyClass& rhs)
    {
        this->myInt -= rhs.myInt;
        this->myChar -= rhs.myChar; //char gets really weird 
                                    //but it should compile most of the time
        this->myBool -= rhs.myBool;
        this->myDouble -= rhs.myDouble;
        //this->myStr -= rhs.myStr; decrement operation is undefined for strings
        return *this;
    }
    friend MyClass operator-(MyClass lhs, const MyClass& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

 private:
    int x;

public:
    void setX(int myValue) {x = myValue;}
    int getX() {return x;}
};

int main() {
    //part 1
    MyClass obj(1,'a',true,44.9,"boo");

    obj.printMessage("Howdy Doody");

    //part 2
    obj.setX(346);
    std::cout << "X is: " << obj.getX() << std::endl;

    //part 3 is done above in the class

    //part 4
    MyClass o1(4, 'q', false, 245.6, "This was in object 1.");
    MyClass o2(o1);
    MyClass o3 = std::move(o1);
    o1.printData(); //The original string is no longer there 
                    //Because the string was moved and became
                    //The empty string!
    o2.printData();
    o3.printData(); 

    //part 5
    MyClass o4(34,'t',true,202.6,"foobar");
    MyClass o5(3,'u',false,28.4,"fetch");
    MyClass myresult = o4 - o5;
    myresult.printData();
    
    return 0;
}