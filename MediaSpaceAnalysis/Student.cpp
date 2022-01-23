
#include "Student.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "split.h"

namespace fs = std::filesystem;

student_vec read_class_list(const std::string& class_list_name)
{
	fs::path class_list_path{ class_list_name };
	std::ifstream class_list_file{ class_list_path };
	if (!class_list_file)
	{
		std::stringstream ss;
		ss << "Could not open " << class_list_name;
		throw std::runtime_error(ss.str());
	}

	student_vec students;
	std::string line;
	int num_read = 0;
	while (std::getline(class_list_file, line))
	{
		num_read++;
		std::vector<std::string> fields = split(line, '\t');
		if (fields.size() < 3)
		{
			std::stringstream ss;
			ss << "classlist line " << num_read << ": only "
					<< fields.size() << " fields.";
			throw std::runtime_error(ss.str());
		}

		if (fields[0].length() != 8)
		{
			std::stringstream ss;
			ss << "classlist line " << num_read << ": the Star ID length is "
				<< fields[0].length() << ".";
		}

		if (fields[1].length() != 8)
		{
			std::stringstream ss;
			ss << "classlist line " << num_read << ": the Tech ID length is "
				<< fields[1].length() << ".";
		}

		students.emplace_back(fields[0], fields[1], fields[2]);
	}

	return students;
}
