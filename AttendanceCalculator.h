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
		
		static std::expected<Attendance, std::string> create(unsigned short classesAttended, unsigned short totalNumberOfClasses)
		{
			if (classesAttended > totalNumberOfClasses)
			{
				return std::unexpected(std::format("ERROR! Number of classes attended ({}) is more than the total number of classes ({}).", classesAttended, totalNumberOfClasses));
			}

			return Attendance{ classesAttended, totalNumberOfClasses };
		}

		float getCurrentPercentage() const
		{
			return m_currPercentage;
		}

		short getClassesAttended() const
		{
			return m_CA;
		}

		short getTotalNumberOfClasses() const
		{
			return m_TNOC;
		}

		void setClassesAttended(short classesAttended)
		{
			m_CA = classesAttended;
		}

		void setTotalNumberOfClasses(short totalNumberOfClasses)
		{
			m_TNOC = totalNumberOfClasses;
		}

		bool calculateCurrentPercentage()
		{
			if (m_TNOC == 0) { return false; }

			m_currPercentage = static_cast<float>(m_CA) / m_TNOC * 100.0f;

			return true;
		}

	private:
		Attendance(unsigned short classesAttended, unsigned short totalNumberOfClasses) : m_CA{ classesAttended }, m_TNOC{ totalNumberOfClasses }
		{
			calculateCurrentPercentage();
		}

	private:
		unsigned short m_CA, m_TNOC;
		float m_currPercentage;
	};

	class Subject : public Attendance
	{
	public:
		Subject() = delete;
		
		static std::expected<Subject, std::string> create(unsigned short classesAttended, unsigned short totalNumberOfClasses)
		{
			auto result = Attendance::create(classesAttended, totalNumberOfClasses);

			if (!result.has_value())
			{
				return std::unexpected(result.error());
			}
			
			return Subject(std::move(result.value()));
		}

		std::expected<short, std::string> classesNeeded()
		{
			float currPercentage{ getCurrentPercentage() };

			if (currPercentage > 100.0) // this won't happen
			{
				return std::unexpected(std::format("ERROR! Percentage ({}) is more than 100.", currPercentage));
			}

			if (currPercentage < 0.0) // this also won't happen
			{
				return std::unexpected(std::format("ERROR! Percentage ({}) is less than 0.", currPercentage));
			}

			if (currPercentage >= DESIRED_PERCENTAGE) // this might happen
			{
				return 0;
			}

			short classesAttended{ getClassesAttended() };
			short totalNumberOfClasses{ getTotalNumberOfClasses() };

			return static_cast<short>(std::ceil((DESIRED_PERCENTAGE / 100.0f * totalNumberOfClasses - classesAttended) / (1.0f - DESIRED_PERCENTAGE / 100.0f)));
		}

		float getRequiredPercentage() const
		{
			return m_requiredPercentage;
		}

		short getClassesNeeded() const
		{
			return m_classesNeeded;
		}

	private:
		Subject(Attendance&& attendance) : Attendance(std::move(attendance))
		{
			auto result = classesNeeded();

			if (result.has_value())
			{
				m_classesNeeded = result.value();
				m_requiredPercentage = (DESIRED_PERCENTAGE > getCurrentPercentage()) ? DESIRED_PERCENTAGE - getCurrentPercentage() : 0.0f;
			}
			else
			{
				println("{}", result.error());
			}
		}

	private:
		unsigned short m_classesNeeded;
		float m_requiredPercentage;
	};
}
