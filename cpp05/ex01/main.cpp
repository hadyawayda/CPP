#include <iostream>
#include "Bureaucrat.hpp"
#include "Form.hpp"

int main() {
    try {
        Bureaucrat alice("Alice", 40);
        Form        tax("TaxForm", 30, 10);
        Form        leave("LeaveForm", 50, 25);

        std::cout << alice << "\n" 
                  << tax   << "\n"
                  << leave << "\n\n";

        // too low to sign TaxForm (requires grade ≤30)
        alice.signForm(tax);
        // OK to sign LeaveForm
        alice.signForm(leave);

        std::cout << "\nAfter attempts:\n"
                  << tax   << "\n"
                  << leave << "\n\n";

        // bump Alice’s rank and try again
        alice.incrementGrade(); // 39
        alice.incrementGrade(); // 38, … keep going
        for (int i = 0; i < 10; ++i) alice.incrementGrade(); 
        std::cout << "Alice is now grade " << alice.getGrade() << "\n";
        alice.signForm(tax);
        std::cout << tax << "\n";

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    // Test invalid form creation
    try {
        Form badHigh("BadHigh", 0, 10);
    } catch (std::exception& e) {
        std::cerr << "Form ctor exception: " << e.what() << "\n";
    }
    try {
        Form badLow("BadLow", 10, 200);
    } catch (std::exception& e) {
        std::cerr << "Form ctor exception: " << e.what() << "\n";
    }

    return 0;
}
