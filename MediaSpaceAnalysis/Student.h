
#pragma once

#include <string>
#include <vector>

struct Student
{
	std::string star_id;
	std::string tech_id;
	std::string name;
};

using student_vec = std::vector<Student>;

student_vec read_class_list(const std::string& class_list_name);
