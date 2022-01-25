
#include "Video_stats.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

void error(const fs::path& path, const std::string& message)
{
	std::stringstream ss;
	ss << "Error reading " << path << ": " << message << '.';
	throw std::runtime_error(ss.str());
}

Video_stats read_video_stats(const fs::path& video_path)
{
	Video_stats stats;

	std::ifstream video_file{ video_path };
	std::string line;
	
	if (!std::getline(video_file, line))
		error(video_path, "Could not read first line");
	if (line[0] != '#')
		error(video_path, "Invalid first line");

	if (!std::getline(video_file, line))
		error(video_path, "Could not read second line");
	if (!line.starts_with("Report: User Engagement"))
		error(video_path, "Invalid second line - wrong report type");

	if (!std::getline(video_file, line))
		error(video_path, "Could not read third line");
	if (!std::getline(video_file, line))
		error(video_path, "Could not read fourth line");

	const std::string line5_start = "Filtered entries: ";
	if (!std::getline(video_file, line))
		error(video_path, "Could not read fifth line");
	if (!line.starts_with(line5_start))
		error(video_path, "Invalid fifth line - improper format");
	stats.name = line.substr(line5_start.length());

	if (!std::getline(video_file, line))
		error(video_path, "Could not read sixth line");
	if (line[0] != '#')
		error(video_path, "Invalid sixth line");

	return stats;
}

std::vector<Video_stats> read_user_engagement(const std::string& assignment)
{
	std::vector<Video_stats> videos;
	for (const fs::directory_entry& dir_entry : fs::directory_iterator(assignment))
	{
		if (dir_entry.is_regular_file() && dir_entry.path().extension().string() == ".csv")
		{
			Video_stats video_stats = read_video_stats(dir_entry.path());
			videos.push_back(std::move(video_stats));
		}
	}

	return videos;
}
