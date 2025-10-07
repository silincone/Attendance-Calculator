// Test the Attendance Calculator
#include <iostream>
#include <print>
#include <string>
#include "ACalcCore.h"

using std::println, std::print;

using namespace ACalcCore;

int main()
{
    std::string subjectName;
    std::uint16_t numberOfClassConducted, numberOfClassAttended;

    println("Attendance Calculator\n");

    print("\nEnter the Subject name: ");
    std::cin >> subjectName;

    print("Enter the number of Classes Conducted: "), std::cin >> numberOfClassConducted;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    print("Enter the number of Classes Attended: "), std::cin >> numberOfClassAttended;

    if (numberOfClassAttended > numberOfClassConducted)
    {
        println("ERROR! Number of classes attended ({}) is more than the classes attended ({}).", numberOfClassAttended, numberOfClassConducted);
        return 1;
    }

    auto result = Subject::create(subjectName, numberOfClassAttended, numberOfClassConducted);

    if (result.has_value())
    {
        Subject subject = result.value();
        print("\n");
        println("Current percentage in {}:  {}%", subjectName, subject.currentPercentage());
        println("Desired percentage in {}:  {}%", subjectName, subject.desiredPercentage());
        println("Required percentage in {}: {}%", subjectName, subject.requiredPercentage());
        println("Excess percentage in {}:   {}%", subjectName, subject.excessPercentage());
        println("Classes needed in {}:      {}", subjectName, subject.classesNeeded());
        println("Class overflow in {}:      {}", subjectName, subject.classOverflow());
    }
    else
    {
        std::println("{}", result.error());
    }
}
