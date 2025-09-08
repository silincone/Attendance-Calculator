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

		float currentPercentage() const
		{
			return m_currPercentage;
		}

		short classesAttended() const
		{
			return m_CA;
		}

		short totalNumberOfClasses() const
		{
			return m_TNOC;
		}

		void classesAttended(short value)
		{
			m_CA = value;
			calculateCurrentPercentage();
		}

		void totalNumberOfClasses(short value)
		{
			m_TNOC = value;
			calculateCurrentPercentage();
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
		}

		void calculateRequiredPercentage()
		{
			m_requiredPercentage = (DESIRED_PERCENTAGE > currentPercentage()) ? DESIRED_PERCENTAGE - currentPercentage() : 0.0f;
		}

	private:
		Subject(Attendance&& attendance) : Attendance(std::move(attendance))
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

			if (currPercentage >= DESIRED_PERCENTAGE) // this might happen
			{
				return 0;
			}

			short CA{ classesAttended() };
			short TNOC{ totalNumberOfClasses() };

			return static_cast<short>(std::ceil((m_desiredPercentage / 100.0f * TNOC - CA) / (1.0f - m_desiredPercentage / 100.0f)));
		}

	private:
		unsigned short m_classesNeeded;
		float m_requiredPercentage;
		float m_desiredPercentage{ AttendanceCalculator::DESIRED_PERCENTAGE };
	};
}
