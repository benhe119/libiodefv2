/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
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

#ifndef _LIBIODEFV2_IODEFV2_CLASS_HXX
#define _LIBIODEFV2_IODEFV2_CLASS_HXX

#include <string>
#include <vector>

#include "iodefv2-criteria.hxx"
#include "iodefv2-path.hxx"

namespace Iodef {
        class IODEFV2Class {
            private:
                class IODEFV2ClassElem {
                        public:
                                int idx;
                                iodefv2_class_id_t parent_id;
                };

                int _depth;
                iodefv2_class_id_t _id;
                std::vector<IODEFV2Class::IODEFV2ClassElem> _pathelem;

                IODEFV2Class(IODEFV2Class &parent, int child_id, int depth=0);
            public:
                IODEFV2Class(iodefv2_class_id_t id=IODEFV2_CLASS_ID_DOCUMENT);
                IODEFV2Class(const IODEFV2Path &path);
                IODEFV2Class(const std::string &path);

                size_t getDepth(void);
                IODEFV2Class get(int child);
                IODEFV2Class get(const std::string &name);

                size_t getChildCount() { return iodefv2_class_get_child_count(_id); };

                /* main object operation */
                bool isList(void);
                bool isKeyedList(void);
                std::string getName(void);
                std::string toString(void);
                Iodef::IODEFV2Value::IODEFV2ValueTypeEnum getValueType(void);
                std::string getPath(int rootidx=0, int depth=-1, const std::string &sep = ".", const std::string &listidx="");
                std::vector<std::string> getEnumValues(void);
                Iodef::IODEFV2Criterion::IODEFV2CriterionOperatorEnum getApplicableOperator(void);
        };
};

#endif
