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

    void Subjects::ClassesAttended(std::uint16_t classesAttended)
    {
        m_classesAttended = classesAttended;
    }

    std::uint16_t Subjects::ClassesAttended() const
    {
        return m_classesAttended;
    }

    void Subjects::ClassesConducted(std::uint16_t classesConducted)
    {
        m_classesConducted = classesConducted;
    }

    std::uint16_t Subjects::ClassesConducted() const
    {
        return m_classesConducted;
    }

    void Subjects::DesiredPercentage(float desiredPercentage)
    {
        m_sub->desiredPercentage(desiredPercentage);
    }

    float Subjects::DesiredPercentage() const
    {
        return m_sub->desiredPercentage();
    }

    std::uint16_t Subjects::ClassesNeeded() const
    {
        return m_sub->classesNeeded();
    }

    float Subjects::CurrentPercentage() const
    {
        return m_sub->currentPercentage();
    }

    float Subjects::RequiredPercentage() const
    {
        return m_sub->requiredPercentage();
    }
}
