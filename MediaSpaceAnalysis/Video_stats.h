
#pragma once

#include <string>
#include <vector>

struct Video_student_stats
{
	std::string star_id;
	std::string name;
	int completion;
};

struct Video_stats
{
	std::string name;
	double length;
	std::vector<Video_student_stats> student_stats;
};

std::vector<Video_stats> read_user_engagement(const std::string& assignment);
