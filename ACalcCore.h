#pragma once
#include <expected>
#include <string>
#include <format>
#include <print>
#include <cmath> // for std::ceil, std::floor

namespace ACalcCore
{
	constexpr float DESIRED_PERCENTAGE = 85.0f;

	class Attendance
	{
	public:
		Attendance() = delete;

		static std::expected<Attendance, std::string> create(std::uint16_t classesAttended, std::uint16_t classesConducted)
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

		std::uint16_t classesAttended() const
		{
			return m_CA;
		}

		std::uint16_t classesConducted() const
		{
			return m_CC;
		}

		bool classesAttended(std::uint16_t value)
		{
			if (!isOk(value, m_CC)) { return false; }

			m_CA = value;
			calculateCurrentPercentage();

			return true;
		}

		bool classesConducted(std::uint16_t value)
		{
			if (!isOk(m_CA, value)) { return false; }

			m_CC = value;
			calculateCurrentPercentage();

			return true;
		}

		bool calculateCurrentPercentage()
		{
			if (!isOk(m_CA, m_CC)) { return false; }

			m_currPercentage = static_cast<float>(m_CA) / m_CC * 100.0f;

			return true;
		}

	private:
		Attendance(std::uint16_t classesAttended, std::uint16_t classesConducted) : m_CA{ classesAttended }, m_CC{ classesConducted }
		{
			calculateCurrentPercentage();
		}

		bool isOk(std::uint16_t& classesAttended, std::uint16_t& classesConducted) const
		{
			if (classesAttended > classesConducted)
			{
				std::println("ERROR! Number of classes attended ({}) is more than the classes conducted ({}).", classesAttended, classesConducted);
				return false;
			}
			return true;
		}

	private:
		std::uint16_t m_CA, m_CC;
		float m_currPercentage;
	};

	class Subject : public Attendance
	{
	public:
		Subject() = delete;

		static std::expected<Subject, std::string> create(std::string subjectName, std::uint16_t classesAttended, std::uint16_t classesConducted)
		{
			auto result = Attendance::create(classesAttended, classesConducted);

			if (!result.has_value())
			{
				return std::unexpected(result.error());
			}

			return Subject(subjectName, std::move(result.value()));
		}

	public:
		float requiredPercentage() const
		{
			return m_requiredPercentage;
		}

		float excessPercentage() const
		{
			return m_excessPercentage;
		}

		std::uint16_t classesNeeded() const
		{
			return m_classesNeeded;
		}

		std::uint16_t classOverflow() const
		{
			return m_classOverflow;
		}

		float desiredPercentage() const
		{
			return m_desiredPercentage;
		}

		bool desiredPercentage(float value)
		{
			if (value > 100.0f or value < 0.0f) { return false; }

			m_desiredPercentage = value;
			
			calculateCurrentPercentage();
			calculateRequiredPercentage();
			calculateExcessPercentage();

			m_classesNeeded = calculateClassesNeeded();
			m_classOverflow = calculateClasseOverflow();

			return true;
		}

		void subjectName(std::string value)
		{
			m_subjectName = value;
		}

		std::string subjectName() const
		{
			return m_subjectName;
		}

	private:
		Subject(std::string subjectName, Attendance&& attendance) : m_subjectName{ subjectName }, Attendance(std::move(attendance))
		{
			m_classesNeeded = calculateClassesNeeded();
			m_classOverflow = calculateClasseOverflow();
			calculateRequiredPercentage();
			calculateExcessPercentage();
		}

		void calculateRequiredPercentage()
		{
			m_requiredPercentage = (m_desiredPercentage > currentPercentage()) ? m_desiredPercentage - currentPercentage() : 0.0f;
		}

		void calculateExcessPercentage()
		{
			m_excessPercentage = (currentPercentage() > m_desiredPercentage) ? currentPercentage() - m_desiredPercentage : 0.0f;
		}

		std::uint16_t calculateClassesNeeded()
		{
			float currPercentage{ currentPercentage() };

			if (currPercentage >= m_desiredPercentage)
			{
				return 0;
			}

			std::uint16_t CA{ classesAttended() };
			std::uint16_t TNOC{ classesConducted() };

			return static_cast<std::uint16_t>(std::ceil((m_desiredPercentage / 100.0f * TNOC - CA) / (1.0f - m_desiredPercentage / 100.0f)));
		}

		std::uint16_t calculateClasseOverflow()
		{
			float currPercentage{ currentPercentage() };

			if (currPercentage <= m_desiredPercentage)
			{
				return 0;
			}

			std::uint16_t CA{ classesAttended() };
			std::uint16_t TNOC{ classesConducted() };

			return static_cast<std::uint16_t>(std::floor((CA - m_desiredPercentage / 100.0f * TNOC) / (1.0f - m_desiredPercentage / 100.0f)));
		}

	private:
		std::uint16_t m_classesNeeded, m_classOverflow;
		float m_requiredPercentage, m_excessPercentage;
		float m_desiredPercentage{ ACalcCore::DESIRED_PERCENTAGE };
		std::string m_subjectName;
	};
}
