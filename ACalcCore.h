#pragma once
#include <expected>
#include <string>
#include <format>
#include <print>

namespace ACalcCore
{
	constexpr float DESIRED_PERCENTAGE = 85.0f;

	class Attendance
	{
	public:
		Attendance() = delete;
		
		static std::expected<Attendance, std::string> create(std::uint8_t classesAttended, std::uint8_t classesConducted)
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

		std::uint8_t classesAttended() const
		{
			return m_CA;
		}

		std::uint8_t classesConducted() const
		{
			return m_CC;
		}

		bool classesAttended(std::uint8_t value)
		{
			if (!isOk(value, m_CC)) { return false; }

			m_CA = value;
			calculateCurrentPercentage();

			return true;
		}

		bool classesConducted(std::uint8_t value)
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
		Attendance(std::uint8_t classesAttended, std::uint8_t classesConducted) : m_CA{ classesAttended }, m_CC{ classesConducted }
		{
			calculateCurrentPercentage();
		}

		bool isOk(std::uint8_t& classesAttended, std::uint8_t& classesConducted) const
		{
			if (classesAttended > classesConducted) 
			{ 
				std::println("ERROR! Number of classes attended ({}) is more than the classes conducted ({}).", classesAttended, classesConducted);
				return false; 
			}
			return true;
		}

	private:
		std::uint8_t m_CA, m_CC;
		float m_currPercentage;
	};

	class Subject : public Attendance
	{
	public:
		Subject() = delete;
		
		static std::expected<Subject, std::string> create(std::string subjectName, std::uint8_t classesAttended, std::uint8_t classesConducted)
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

		std::uint8_t classesNeeded() const
		{
			return m_classesNeeded;
		}

		float desiredPercentage() const
		{
			return m_desiredPercentage;
		}

		bool desiredPercentage(float value)
		{
			m_desiredPercentage = value;
			calculateCurrentPercentage();
			calculateRequiredPercentage();

			auto result = calculateClassesNeeded();
			if (result.has_value())
			{
				m_classesNeeded = result.value();
			}
			else
			{
				std::println("{}", result.error());
				return false;
			}

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

		void calculateRequiredPercentage()
		{
			m_requiredPercentage = (m_desiredPercentage > currentPercentage()) ? m_desiredPercentage - currentPercentage() : 0.0f;
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

		std::expected<std::uint8_t, std::string> calculateClassesNeeded()
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

			std::uint8_t CA{ classesAttended() };
			std::uint8_t TNOC{ classesConducted() };

			return static_cast<std::uint8_t>(std::ceil((m_desiredPercentage / 100.0f * TNOC - CA) / (1.0f - m_desiredPercentage / 100.0f)));
		}

	private:
		std::uint8_t m_classesNeeded;
		float m_requiredPercentage;
		float m_desiredPercentage{ ACalcCore::DESIRED_PERCENTAGE };
		std::string m_subjectName;
	};
}
