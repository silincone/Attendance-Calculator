// Test the Attendance Calculator
#include <iostream>
#include <print>
#include "attendanceCalculator.h"

using std::println;

using namespace AttendanceCalculator;

int main()
{
    println("Attendance Calculator\n");

    auto result = Subject::create(17, 20);

    if (result.has_value())
    {
        Subject VCTT = result.value();
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
