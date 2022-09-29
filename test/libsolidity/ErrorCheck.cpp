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
/** @file ErrorCheck.cpp
 * @author Yoichi Hirai <i@yoichihirai.com>
 * @date 2016
 */

#include <test/libsolidity/ErrorCheck.h>
#include <libsolutil/Exceptions.h>
#include <string>
#include <iostream>
#include <memory>

#include "liblangutil/Exceptions.h"
#include "liblangutil/SourceLocation.h"

using namespace std;
using namespace solidity;
using namespace solidity::langutil;
using namespace solidity::frontend;

namespace
{
std::string errorMessage(Error const& _e)
{
	return _e.comment() ? *_e.comment() : "NONE";
}
}

bool solidity::frontend::test::searchErrorMessage(Error const& _err, std::string const& _substr)
{
	if (string const* errorMessage = _err.comment())
	{
		if (errorMessage->find(_substr) == std::string::npos)
		{
			cout << "Expected message \"" << _substr << "\" but found \"" << *errorMessage << "\".\n";
			return false;
		}
		return true;
	}
	else
		cout << "Expected error message but found none." << endl;
	return _substr.empty();
}

string solidity::frontend::test::searchErrors(ErrorList const& _errors, vector<pair<Error::Type, string>> const& _expectations)
{
	auto expectations = _expectations;
	for (auto const& error: _errors)
	{
		string msg = errorMessage(*error);
		bool found = false;
		for (auto it = expectations.begin(); it != expectations.end(); ++it)
			if (msg.find(it->second) != string::npos && error->type() == it->first)
			{
				found = true;
				expectations.erase(it);
				break;
			}
		if (!found)
			return "Unexpected error: " + Error::formatErrorType(error->type()) + ": " + msg;
	}
	if (!expectations.empty())
	{
		string msg = "Expected error(s) not present:\n";
		for (auto const& expectation: expectations)
			msg += expectation.second + "\n";
		return msg;
	}

	return "";
}
