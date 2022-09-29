/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0

#include <test/TemporaryDirectory.h>
#include <test/libsolidity/util/SoltestErrors.h>
#include <stdint.h>
#include <boost/filesystem/path_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

using namespace std;
using namespace solidity;
using namespace solidity::test;

namespace fs = boost::filesystem;

TemporaryDirectory::TemporaryDirectory(std::string const& _prefix):
	m_path(fs::temp_directory_path() / fs::unique_path(_prefix + "-%%%%-%%%%-%%%%-%%%%"))
{
	// Prefix should just be a file name and not contain anything that would make us step out of /tmp.
	soltestAssert(fs::path(_prefix) == fs::path(_prefix).stem(), "");

	fs::create_directory(m_path);
}

TemporaryDirectory::TemporaryDirectory(
	vector<boost::filesystem::path> const& _subdirectories,
	string const& _prefix
):
	TemporaryDirectory(_prefix)
{
	for (boost::filesystem::path const& subdirectory: _subdirectories)
	{
		soltestAssert(!subdirectory.is_absolute() && subdirectory.root_path() != "/", "");
		soltestAssert(
			m_path.lexically_relative(subdirectory).empty() ||
			*m_path.lexically_relative(subdirectory).begin() != "..",
			""
		);
		boost::filesystem::create_directories(m_path / subdirectory);
	}
}

TemporaryDirectory::~TemporaryDirectory()
{
	// A few paranoid sanity checks just to be extra sure we're not deleting someone's homework.
	soltestAssert(m_path.string().find(fs::temp_directory_path().string()) == 0, "");
	soltestAssert(!fs::equivalent(m_path, fs::temp_directory_path()), "");
	soltestAssert(!fs::equivalent(m_path, m_path.root_path()), "");
	soltestAssert(!m_path.empty(), "");

	boost::system::error_code errorCode;
	uintmax_t numRemoved = fs::remove_all(m_path, errorCode);
	if (errorCode.value() != boost::system::errc::success)
	{
		cerr << "Failed to completely remove temporary directory '" << m_path << "'. ";
		cerr << "Only " << numRemoved << " files were actually removed." << endl;
		cerr << "Reason: " << errorCode.message() << endl;
	}
}

TemporaryWorkingDirectory::TemporaryWorkingDirectory(fs::path const& _newDirectory):
	m_originalWorkingDirectory(fs::current_path())
{
	fs::current_path(_newDirectory);
}

TemporaryWorkingDirectory::~TemporaryWorkingDirectory()
{
	fs::current_path(m_originalWorkingDirectory);
}
