/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann@libiodefv2-ids.com>
*
* This file is part of the LibIodef library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

#ifndef _LIBIODEFV2_IODEFV2_CRITERIA_HXX
#define _LIBIODEFV2_IODEFV2_CRITERIA_HXX

#include <string>
#include "iodefv2.h"
#include "iodefv2.hxx"

namespace Iodef {
        class IODEFV2Criterion {
            public:
                enum IODEFV2CriterionOperatorEnum {
                        OPERATOR_NOT     = IODEFV2_CRITERION_OPERATOR_NOT,
                        OPERATOR_NOCASE  = IODEFV2_CRITERION_OPERATOR_NOCASE,

                        OPERATOR_EQUAL   = IODEFV2_CRITERION_OPERATOR_EQUAL,
                        OPERATOR_EQUAL_NOCASE = IODEFV2_CRITERION_OPERATOR_EQUAL_NOCASE,
                        OPERATOR_NOT_EQUAL = IODEFV2_CRITERION_OPERATOR_NOT_EQUAL,
                        OPERATOR_NOT_EQUAL_NOCASE = IODEFV2_CRITERION_OPERATOR_NOT_EQUAL_NOCASE,

                        OPERATOR_LESSER = IODEFV2_CRITERION_OPERATOR_LESSER,
                        OPERATOR_LESSER_OR_EQUAL = IODEFV2_CRITERION_OPERATOR_LESSER_OR_EQUAL,

                        OPERATOR_GREATER = IODEFV2_CRITERION_OPERATOR_GREATER,
                        OPERATOR_GREATER_OR_EQUAL = IODEFV2_CRITERION_OPERATOR_GREATER_OR_EQUAL,

                        OPERATOR_SUBSTR = IODEFV2_CRITERION_OPERATOR_SUBSTR,
                        OPERATOR_SUBSTR_NOCASE = IODEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE,
                        OPERATOR_NOT_SUBSTR = IODEFV2_CRITERION_OPERATOR_NOT_SUBSTR,
                        OPERATOR_NOT_SUBSTR_NOCASE = IODEFV2_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE,

                        OPERATOR_REGEX = IODEFV2_CRITERION_OPERATOR_REGEX,
                        OPERATOR_REGEX_NOCASE = IODEFV2_CRITERION_OPERATOR_REGEX_NOCASE,
                        OPERATOR_NOT_REGEX = IODEFV2_CRITERION_OPERATOR_NOT_REGEX,
                        OPERATOR_NOT_REGEX_NOCASE = IODEFV2_CRITERION_OPERATOR_NOT_REGEX_NOCASE,

                        OPERATOR_NULL = IODEFV2_CRITERION_OPERATOR_NULL,
                        OPERATOR_NOT_NULL = IODEFV2_CRITERION_OPERATOR_NOT_NULL
                };
        };

        class IODEFV2Criteria {
            private:
                iodefv2_criteria_t *_criteria;

            public:
                ~IODEFV2Criteria();
                IODEFV2Criteria();
                IODEFV2Criteria(const IODEFV2Criteria &criteria);
                IODEFV2Criteria(const char *criteria);
                IODEFV2Criteria(const std::string &criteria);
                IODEFV2Criteria(iodefv2_criteria_t *criteria);

                int match(Iodef::IODEFV2 *message) const;
                IODEFV2Criteria clone() const;
                void andCriteria(const IODEFV2Criteria &criteria);
                void orCriteria(const IODEFV2Criteria &criteria);
                const std::string toString() const;

                operator const std::string() const;
                operator iodefv2_criteria_t *() const;
                IODEFV2Criteria &operator=(const IODEFV2Criteria &criteria);
        };
};

#endif
