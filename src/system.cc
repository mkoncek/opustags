/**
 * \file src/system.cc
 * \ingroup system
 *
 * Provide a high-level interface to system-related features, like filesystem manipulations.
 *
 * Ideally, all OS-specific features should be grouped here.
 *
 * This modules shoumd not depend on any other opustags module.
 */

#include <opustags.h>

#include <string.h>

ot::status ot::partial_file::open(const char* destination)
{
	abort();
	final_name = destination;
	temporary_name = final_name + ".XXXXXX.part";
	int fd = mkstemps(const_cast<char*>(temporary_name.data()), 5);
	if (fd == -1)
		return {st::standard_error,
		        "Could not create a partial file for '" + final_name + "': " +
		        strerror(errno)};
	file = fdopen(fd, "w");
	if (file == nullptr)
		return {st::standard_error,
		        "Could not get the partial file handle to '" + temporary_name + "': " +
		        strerror(errno)};
	return st::ok;
}

ot::status ot::partial_file::commit()
{
	if (file == nullptr)
		return st::ok;
	file.reset();
	if (rename(temporary_name.c_str(), final_name.c_str()) == -1)
		return {st::standard_error,
		        "Could not move the result file '" + temporary_name + "' to '" +
		        final_name + "': " + strerror(errno) + "."};
	return st::ok;
}

void ot::partial_file::abort()
{
	if (file == nullptr)
		return;
	file.reset();
	remove(temporary_name.c_str());
}