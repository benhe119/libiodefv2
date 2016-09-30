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

#ifndef _LIBIODEFV2_IODEFV2_PATH_HXX
#define _LIBIODEFV2_IODEFV2_PATH_HXX

#include "iodefv2.h"
#include "iodefv2-path.h"

#include "iodefv2.hxx"
#include "libiodefv2-error.hxx"
#include "iodefv2-value.hxx"


namespace Iodef {
        class IODEFV2Class;

        class IODEFV2Path {
            private:
                iodefv2_path_t *_path;

            public:
                IODEFV2Path(const char *buffer);
                IODEFV2Path(Iodef::IODEFV2 &iodefv2, const char *buffer);

                IODEFV2Path(iodefv2_path_t *path);
                IODEFV2Path(const IODEFV2Path &path);
                ~IODEFV2Path();

                Iodef::IODEFV2Value get(const Iodef::IODEFV2 &message) const;
                void set(Iodef::IODEFV2 &message, const std::vector<Iodef::IODEFV2> &value) const;
                void set(Iodef::IODEFV2 &message, Iodef::IODEFV2 *value) const;
                void set(Iodef::IODEFV2 &message, const std::vector<Iodef::IODEFV2Value> &value) const;
                void set(Iodef::IODEFV2 &message, Iodef::IODEFV2Value *value) const;
                void set(Iodef::IODEFV2 &message, Iodef::IODEFV2Value &value) const;
                void set(Iodef::IODEFV2 &message, Iodef::IODEFV2Time &time) const;
                void set(Iodef::IODEFV2 &message, const std::string &value) const;
                void set(Iodef::IODEFV2 &message, const char *value) const;
                void set(Iodef::IODEFV2 &message, int32_t value) const;
                void set(Iodef::IODEFV2 &message, int64_t value) const;
                void set(Iodef::IODEFV2 &message, uint64_t value) const;
                void set(Iodef::IODEFV2 &message, float value) const;
                void set(Iodef::IODEFV2 &message, double value) const;

                Iodef::IODEFV2Class getClass(int depth=-1) const;
                Iodef::IODEFV2Value::IODEFV2ValueTypeEnum getValueType(int depth=-1) const;
                int setIndex(unsigned int index, int depth=-1);
                int undefineIndex(int depth=-1);
                int getIndex(int depth=-1) const;
                int makeChild(const char *child_name, unsigned int index);
                int makeParent();
                int compare(IODEFV2Path *path, int depth=-1) const;
                IODEFV2Path clone() const;

                int checkOperator(iodefv2_criterion_operator_t op) const;
                iodefv2_criterion_operator_t getApplicableOperators() const;

                const char *getName(int depth=-1) const;
                bool isAmbiguous() const;
                int hasLists() const;
                bool isList(int depth=-1) const;
                unsigned int getDepth() const;

                IODEFV2Path &operator = (const IODEFV2Path &path);
                operator iodefv2_path_t*() const;
        };
};

#endif
