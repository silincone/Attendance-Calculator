// Test the Attendance Calculator
#include <iostream>
#include <print>
#include "attendanceCalc.h"

using std::println;

using namespace AttendanceCalculator;

int main()
{
    println("Attendance Calculator\n");

    auto result = Subject::create(17, 20);

    if (!result.has_value())
    {
        std::println("{}", result.error());
    }
    else
    {
        Subject VCTT = result.value();
        println("Current percentage in VCTT:  {}%", VCTT.getCurrentPercentage());
        println("Percentage required in VCTT: {}%", VCTT.getRequiredPercentage());
        println("Classes needed in VCTT:      {}",  VCTT.getClassesNeeded());
    }
}
