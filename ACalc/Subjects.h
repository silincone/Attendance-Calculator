#pragma once

#include "Subjects.g.h"

namespace winrt::ACalc::implementation
{
    struct Subjects : SubjectsT<Subjects>
    {
        Subjects() = default;
        Subjects(winrt::hstring subjectName, std::uint8_t classesAttended, std::uint8_t classesConducted) : m_subjectName{ subjectName }, m_CA{ classesAttended }, m_CC{ classesConducted } {}

        void SubjectName(winrt::hstring subjectName);
        winrt::hstring SubjectName();

        void ClassesAttended(std::uint8_t classesAttended);
        std::uint8_t ClassesAttended();
        
        void ClassesConducted(std::uint8_t classesAttended);
        std::uint8_t ClassesConducted();

    private:
        winrt::hstring m_subjectName;
        std::uint8_t m_CA, m_CC;
    };
}

namespace winrt::ACalc::factory_implementation
{
    struct Subjects : SubjectsT<Subjects, implementation::Subjects>
    {
    };
}
