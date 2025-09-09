#pragma once
#include <expected>
#include <string>
#include <format>
#include <print>

namespace AttendanceCalculator
{
	constexpr float DESIRED_PERCENTAGE = 85.0f;

	class Attendance
	{
	public:
		Attendance() = delete;
		
		static std::expected<Attendance, std::string> create(unsigned short classesAttended, unsigned short classesConducted)
		{
			if (classesAttended > classesConducted)
			{
				return std::unexpected(std::format("ERROR! Number of classes attended ({}) is more than the classes conducted ({}).", classesAttended, classesConducted));
			}

			return Attendance{ classesAttended, classesConducted };
		}

		float currentPercentage() const
		{
			return m_currPercentage;
		}

		short classesAttended() const
		{
			return m_CA;
		}

		short classesConducted() const
		{
			return m_CC;
		}

		void classesAttended(short value)
		{
			m_CA = value;
			calculateCurrentPercentage();
		}

		void classesConducted(short value)
		{
			m_CC = value;
			calculateCurrentPercentage();
		}

		bool calculateCurrentPercentage()
		{
			if (m_CC == 0) { return false; }

			m_currPercentage = static_cast<float>(m_CA) / m_CC * 100.0f;

			return true;
		}

	private:
		Attendance(unsigned short classesAttended, unsigned short classesConducted) : m_CA{ classesAttended }, m_CC{ classesConducted }
		{
			calculateCurrentPercentage();
		}

	private:
		unsigned short m_CA, m_CC;
		float m_currPercentage;
	};

	class Subject : public Attendance
	{
	public:
		Subject() = delete;
		
		static std::expected<Subject, std::string> create(std::string subjectName, unsigned short classesAttended, unsigned short classesConducted)
		{
			auto result = Attendance::create(classesAttended, classesConducted);

			if (!result.has_value())
			{
				return std::unexpected(result.error());
			}
			
			return Subject(subjectName, std::move(result.value()));
		}

		float requiredPercentage() const
		{
			return m_requiredPercentage;
		}

		short classesNeeded() const
		{
			return m_classesNeeded;
		}

		float desiredPercentage() const
		{
			return m_desiredPercentage;
		}

		void desiredPercentage(float value)
		{
			m_desiredPercentage = value;
			calculateCurrentPercentage();
			calculateRequiredPercentage();

			auto result = calculateClassesNeeded();
			if (result.has_value())
			{
				m_classesNeeded = result.value();
			}
		}

		void subjectName(std::string value)
		{
			m_subjectName = value;
		}

		std::string subjectName() const
		{
			return m_subjectName;
		}

		void calculateRequiredPercentage()
		{
			m_requiredPercentage = (DESIRED_PERCENTAGE > currentPercentage()) ? DESIRED_PERCENTAGE - currentPercentage() : 0.0f;
		}

	private:
		Subject(std::string subjectName, Attendance&& attendance) : m_subjectName{ subjectName }, Attendance(std::move(attendance))
		{
			auto result = calculateClassesNeeded();

			if (result.has_value())
			{
				m_classesNeeded = result.value();
				calculateRequiredPercentage();
			}
			else
			{
				println("{}", result.error());
			}
		}

		std::expected<short, std::string> calculateClassesNeeded()
		{
			float currPercentage{ currentPercentage() };

			if (currPercentage > 100.0) // this won't happen
			{
				return std::unexpected(std::format("ERROR! Percentage ({}) is more than 100.", currPercentage));
			}

			if (currPercentage < 0.0) // this also won't happen
			{
				return std::unexpected(std::format("ERROR! Percentage ({}) is less than 0.", currPercentage));
			}

			if (currPercentage >= m_desiredPercentage) // this might happen
			{
				return 0;
			}

			short CA{ classesAttended() };
			short TNOC{ classesConducted() };

			return static_cast<short>(std::ceil((m_desiredPercentage / 100.0f * TNOC - CA) / (1.0f - m_desiredPercentage / 100.0f)));
		}

	private:
		unsigned short m_classesNeeded;
		float m_requiredPercentage;
		float m_desiredPercentage{ AttendanceCalculator::DESIRED_PERCENTAGE };
		std::string m_subjectName;
	};
}
