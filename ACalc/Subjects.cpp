#include "pch.h"
#include "Subjects.h"
#if __has_include("Subjects.g.cpp")
#include "Subjects.g.cpp"
#endif

namespace winrt::ACalc::implementation
{
    void Subjects::SubjectName(winrt::hstring subjectName)
    {
        m_subjectName = subjectName;
    }

    winrt::hstring Subjects::SubjectName()
    {
        return m_subjectName;
    }

    void Subjects::ClassesAttended(std::uint8_t classesAttended)
    {
        m_CA = classesAttended;
    }

    std::uint8_t Subjects::ClassesAttended()
    {
        return m_CA;
    }

    void Subjects::ClassesConducted(std::uint8_t classesConducted)
    {
        m_CC = classesConducted;
    }

    std::uint8_t Subjects::ClassesConducted()
    {
        return m_CC;
    }
}
