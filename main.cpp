// Test the Attendance Calculator
#include <iostream>
#include <print>
#include <string>
#include "attendanceCalculator.h"

using std::println, std::print;

using namespace AttendanceCalculator;

int main()
{
    std::string subjectName;
    short numberOfClassConducted, numberOfClassAttended;

    println("Attendance Calculator\n");

    print("\nEnter the Subject name: ");
    std::cin >> subjectName;

    print("Enter the number of Classes Conducted: "), std::cin >> numberOfClassConducted;
    print("Enter the number of Classes Attended: "), std::cin >> numberOfClassAttended;

    if (numberOfClassAttended > numberOfClassConducted)
    {
        println("ERROR! Number of classes attended ({}) is more than the classes attended ({}).", numberOfClassAttended, numberOfClassConducted);
        return 1;
    }

    auto result = Subject::create("VCTT", numberOfClassAttended, numberOfClassConducted);

    if (result.has_value())
    {
        Subject VCTT = result.value();
        print("\n");
        println("Current percentage in VCTT:  {}%", VCTT.currentPercentage());
        println("Percentage required in VCTT: {}%", VCTT.requiredPercentage());
        println("Desired percentage in VCTT:  {}%", VCTT.desiredPercentage());
        println("Classes needed in VCTT:      {}",  VCTT.classesNeeded());
    }
    else
    {
        std::println("{}", result.error());
    }
}
