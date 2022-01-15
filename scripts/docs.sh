#!/usr/bin/env bash

#------------------------------------------------------------------------------
# Bash script to build the Solidity Sphinx documentation locally.
#
# The documentation for solidity is hosted at:
#
#     https://docs.soliditylang.org
#
# ------------------------------------------------------------------------------
# This file is part of solidity.
#
# solidity is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# solidity is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with solidity.  If not, see <http://www.gnu.org/licenses/>
#
# (c) 2016 solidity contributors.
#------------------------------------------------------------------------------

# List of supported languages
LANGUAGES=("en_US" "es_EM")

set -e
cd docs
pip3 install -r requirements.txt --upgrade --upgrade-strategy eager
sphinx-build -b gettext . _build/gettext

for i in "${LANGUAGES[@]}"; do
	sphinx-intl update -p _build/gettext -l "${i}"
	sphinx-build -nW -b html -d _build/doctrees -D language="${i}" . "_build/html/${i}"
done
cd ..
