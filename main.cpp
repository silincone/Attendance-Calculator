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

    auto result = Subject::create(subjectName, numberOfClassAttended, numberOfClassConducted);

    if (result.has_value())
    {
        Subject subject = result.value();
        print("\n");
        println("Current percentage in {}:  {}%", subjectName, subject.currentPercentage());
        println("Percentage required in {}: {}%", subjectName, subject.requiredPercentage());
        println("Desired percentage in {}:  {}%", subjectName, subject.desiredPercentage());
        println("Classes needed in {}:      {}",  subjectName, subject.classesNeeded());
    }
    else
    {
        std::println("{}", result.error());
    }
}
