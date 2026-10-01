#include <iostream>
#include <string>
#include <concepts>

//Concept Definition:
template <class T>
//keyword  name   assign sets up a local variable of Type t
concept    ILabel  =     requires(T v) {
    //Compound requirement:
    //1. The with type T must have a method named buildHtml()
    //2. Captures the expressions return type to test against a restriction
    {v.buildHtml()} ->
    //type constraint (on return type). T has a buildHtml can be implicitly convertered to std::string
                       std::convertible_to<std::string>;
};    

// PASSES - perfect match, returns a std::string
class TextLabel {
public: 
    std::string buildHtml() { return "<p>Hello from the wide world of sports.</p>";}
};

// PASSES - returns a const char* which implicitly can convert to std::string
class QuickLabel {
public:
    const char* buildHtml() { return "<div>Quick</div>";}
};

// FAILS - Method exists but returns an int that can't convert to std::string
class BrokenLabel {
public:
    int buildHtml() { return 404;}
};

//Use abbreviated function syntax to accept any ILabel type
void renderWebpage(ILabel auto label) {
    std::string html = label.buildHtml();
    std::cout << "Rendering: " << html << "\n";
}

int main() {
    TextLabel txt;
    QuickLabel qck;
    BrokenLabel brk;

    renderWebpage(txt);  //Valid
    renderWebpage(qck);  //Valid

    // COMPILER ERROR: BrokenLabel doesn't satisfy ILabel because int not convertible to string
    //renderWebpage(brk); 
    
    return 0;
}
   