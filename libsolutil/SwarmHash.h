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
/** @file SwarmHash.h
 */

#pragma once

#include <libsolutil/FixedHash.h>
#include <string>

#include "libsolutil/Common.h"
#include "libsolutil/CommonData.h"

namespace solidity::util
{

/// Compute the "swarm hash" of @a _input (OLD 0x1000-section version)
h256 bzzr0Hash(std::string const& _input);

/// Compute the "bzz hash" of @a _input (the NEW binary / BMT version)
h256 bzzr1Hash(bytes const& _input);

inline h256 bzzr1Hash(std::string const& _input)
{
	return bzzr1Hash(asBytes(_input));
}

}
