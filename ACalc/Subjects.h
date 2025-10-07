#pragma once

#include "Subjects.g.h"
#include "ACalcCore.h"

namespace winrt::ACalc::implementation
{
    struct Subjects : SubjectsT<Subjects>
    {
        Subjects() = default;
        Subjects(winrt::hstring subjectName, std::uint16_t classesAttended, std::uint16_t classesConducted) : m_subjectName{ subjectName }, m_classesAttended{ classesAttended }, m_classesConducted{ classesConducted } 
        {
            auto result = ACalcCore::Subject::create(winrt::to_string(subjectName), classesAttended, classesConducted);
            if (result.has_value())
            {
                m_sub = std::make_unique<ACalcCore::Subject>(std::move(result.value()));
            }
            else
            {
                m_sub = nullptr;
                throw "Could not create Subject Variable";
            }
        }

        void SubjectName(winrt::hstring subjectName);
        winrt::hstring SubjectName();

        void ClassesAttended(std::uint16_t classesAttended);
        std::uint16_t ClassesAttended() const;
        
        void ClassesConducted(std::uint16_t classesAttended);
        std::uint16_t ClassesConducted() const;

        void DesiredPercentage(float desiredPercentage);
        float DesiredPercentage() const;

        std::uint16_t ClassesNeeded() const;
        float CurrentPercentage() const;
        float RequiredPercentage() const;


    private:
        winrt::hstring m_subjectName;
        std::uint16_t m_classesAttended{0}, m_classesConducted{0};
        std::unique_ptr<ACalcCore::Subject> m_sub;
    };
}

namespace winrt::ACalc::factory_implementation
{
    struct Subjects : SubjectsT<Subjects, implementation::Subjects>
    {
    };
}
