#include <iostream>
#include <string>
#include <concepts>

// Helper concept to check if optional 'string_type' alias exists
template <typename T>
concept has_string_type = requires {
    typename T::string_type;
};

//Concept Definition:
template <class T>
//keyword  name   assign sets up a local variable of Type t
concept    ILabel  =     requires(T v) {                       // Simple (Can I call this?)
    //Items of type T must have a method named buildHtml()
    {v.buildHtml()} ->
    //type constraint (on return type). T has a buildHtml can be implicitly convertered to std::string
                       std::convertible_to<std::string>;
};    

// PASSES - perfect match, returns a std::string
class TextLabel {
public: 
    using string_type = std::string; // Renamed alias!
    std::string buildHtml() { return "<p>Hello from the wide world of sports.</p>";}
};

// PASSES - returns a const char* which implicitly can convert to std::string
class QuickLabel {
public:
    using string_type = const char*; // Renamed alias!
    const char* buildHtml() { return "<div>Quick</div>";}
};

// passes because the using clause has been made optional
class LegacyLabel {
public:
    std::string buildHtml() { return "<a>Legacy Label</a>"; }
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
    LegacyLabel lgy;
    BrokenLabel brk;

    renderWebpage(txt);  //Valid
    renderWebpage(qck);  //Valid
    renderWebpage(lgy);  //Valid
    renderWebpage(txt);  //Valid

    // COMPILER ERROR: BrokenLabel doesn't satisfy ILabel because int not convertible to string
    //renderWebpage(brk); 
    
    return 0;
}
   