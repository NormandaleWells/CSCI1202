/*
	user_engagement_stats

	This program compiles MediaSpace user engagement stats to compile "attendance" stats.

	Setup:
		(1) Create assignments in D2L.
		(2) Create the classlist.txt file (see below).
		(3) Create a directory structure with a subdirectory for each assignment.
		(4) Download MediaSpace user engagement csvs to the appropriate subdirectory
		    for each assignment.

	Input:
		classlist.txt - list of students
		<assignment name>	- directory containing MediSpace data
			<csv files> - user engagement stats from MediaSpace

	Output:
		gradelist.csv - file return for import into D2L gradebook

	The classlist.txt file is a tab-deimited file with at least the following columns
		Star ID
		Tech ID
		Student name
	
	The easiest way to create this to simple open the classlist in eservices, select
	all the students, and cut and paste into a text file called classlist.txt.

	The classlist.txt file should be placed into a directory (the "base directory").
	Each subdirectory of that directory represents an assignment with the same name
	as the directory.  The .csv files from MediaSpace are then placed in the
	directory for the assignment they apply to.

	Command line parameters:
		--dir <base directory>		Default is the current directory.
		--classlist <filename>		Default is classlist.txt
		<assignment>				Default is to process every subdirectory
									in the base directory.
*/

#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Student.h"

namespace fs = std::filesystem;

using string_vec = std::vector<std::string>;

struct Command_line_params
{
	std::string base_dir;
	std::string class_list_name = "classlist.txt";
	string_vec dirs_to_process;
};

/*
	set_current_directory

	Attempts to make the given directory the current directory.
*/
void set_current_directory(const std::string& new_dir)
{
	std::error_code ec;
	fs::current_path(new_dir, ec);
	if (!ec) return;

	std::stringstream ss;
	ss << "Could not set " << new_dir << " as the current directory.";
	ss << "  Error: " << ec.message();
	throw std::runtime_error(ss.str());
}

/*
	parse_cl_params

	Parses the command-line parameters.  The only error checking done
	here is verification that any option that requires a parameter has
	one, and no unknown parameters are specfied.
*/
void parse_cl_params(int argc, char* argv[], Command_line_params& cl)
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg{ argv[i] };
		if (arg == "--dir")
		{
			i++;
			if (i == argc)
			{
				throw std::runtime_error("--dir requires a directory name.");
			}
			cl.base_dir = argv[i];
		}
		else if (arg == "--classlist")
		{
			i++;
			if (i == argc)
			{
				throw std::runtime_error("--classlist requires a file name.");
			}
			cl.class_list_name = argv[i];
		}
		else if (arg.length() >= 2 && arg.substr(0, 2) == "--")
		{
			std::stringstream ss;
			ss << arg << " is not a valid command line parameter.";
			throw std::runtime_error(ss.str());
		}
		else
		{
			cl.dirs_to_process.push_back(arg);
		}
	}
}

/*
	verify_cl_params

	Verifies that the command-line parameters are valid.  That is, it checks
	the following:

	(1)	That the base directory (if given) is valid.
	(2)	That the class list file exists and is a file.
	(3) That all the specified directories to process exist and are directories.
	
	It also has the following side-effects:

	(1)	The current directory is set to the base directory.
	(2) If no directories are given on the command line, it fills the
		dirs_to_process field to all the subdirectories of the base
		directory (non-recursively).
*/
void verify_cl_params(Command_line_params& cl)
{
	if (!cl.base_dir.empty())
	{
		set_current_directory(cl.base_dir);
	}

	fs::path class_list_path{ cl.class_list_name };
	if (!exists(class_list_path))
	{
		std::stringstream ss;
		ss << class_list_path << " does not exist.";
		throw std::runtime_error(ss.str());
	}

	fs::file_type type = fs::status(class_list_path).type();
	if (type != fs::file_type::regular)
	{
		std::stringstream ss;
		ss << class_list_path << " is not a file.\n";
		throw std::runtime_error(ss.str());
	}

	if (cl.dirs_to_process.empty())
	{
		for (const fs::directory_entry& dir_entry : fs::directory_iterator("."))
		{
			if (dir_entry.is_directory())
			{
				cl.dirs_to_process.push_back(dir_entry.path().filename().string());
			}
		}
	}

	for (const auto& dir_name : cl.dirs_to_process)
	{
		fs::path dir_path{ dir_name };
		if (!exists(dir_path))
		{
			std::stringstream ss;
			ss << dir_path << " does not exist.\n";
			throw std::runtime_error(ss.str());
		}

		type = fs::status(dir_path).type();
		if (type != fs::file_type::directory)
		{
			std::stringstream ss;
			ss << dir_path << " is not a directory.\n";
			throw std::runtime_error(ss.str());
		}
	}
}

int main(int argc, char* argv[])
try
{
	Command_line_params cl;
	parse_cl_params(argc, argv, cl);
	verify_cl_params(cl);

	student_vec students = read_class_list(cl.class_list_name);

	return 0;
}
catch (std::runtime_error& e)
{
	std::cout << e.what() << '\n';
}
catch (std::logic_error& e)
{
	std::cout << "Program defect detected:\n";
	std::cout << e.what() << '\n';
}
catch (...)
{
	std::cout << "Unknown exception thrown.\n";
}
