
#include "split.h"

using namespace std;

vector<string> split(const string & s) noexcept
{
	vector<string> words;

	size_t i = 0;

	while (i < s.length())
	{
		// Skip whitespace.
		while ((i < s.length()) && isspace(s[i]))
			i++;

		// Skip characters until we find whitespace.
		size_t j = i;
		while ((i < s.length()) && !isspace(s[i]))
			i++;

		// Make sure we actually found something.
		if (i != j)
			words.push_back(s.substr(j, i - j));
	}

	return words;
}

vector<string> split(const string & s, char sep) noexcept
{
	vector<string> words;

	size_t i = 0;

	while (true)
	{
		size_t j = i;
		while ((i < s.length()) && (s[i] != sep))
			i++;

		// Note that we allow zero-length strings here.
		words.push_back(s.substr(j, i - j));

		// The previous loop ended by either finding
		// a separator or the end of the string.
		// If the former, increment past the
		// separator.  If that puts us at the end
		// of the string, we have a blank string
		// to add to the output.
		if (i == s.length())
			break;

		// Note that if this increment puts us at
		// the end of the string, the next loop
		// around will provide the necessary
		// empty string.
		++i;
	}

	return words;
}
