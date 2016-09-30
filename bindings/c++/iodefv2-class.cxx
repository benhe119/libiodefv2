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

#include <string>
#include <iodefv2.h>
#include <iodefv2-value-type.h>

#include "libiodefv2.hxx"


using namespace Iodef;



IODEFV2Class::IODEFV2Class(iodefv2_class_id_t id)
{
        _id = id;
        _depth = 0;
}


IODEFV2Class::IODEFV2Class(const IODEFV2Path &path)
{
        int i;
        IODEFV2Class root;

        for ( i = 0; i < path.getDepth(); i++ )
                root = root.get(path.getName(i));

        *this = root;
}



IODEFV2Class::IODEFV2Class(const std::string &path)
{
        *this = IODEFV2Class(IODEFV2Path(path.c_str()));
}


IODEFV2Class::IODEFV2Class(IODEFV2Class &parent, int child_id, int depth)
{
        IODEFV2Class::IODEFV2ClassElem elem;

        if ( depth >= 16 )
                throw LibIodefError(libiodefv2_error(LIBIODEFV2_ERROR_IODEFV2_PATH_DEPTH));

        _depth = depth;
        _pathelem = parent._pathelem;
        _id = iodefv2_class_get_child_class(parent._id, child_id);

        elem.parent_id = parent._id;
        elem.idx = child_id;
        _pathelem.push_back(elem);
}


std::string IODEFV2Class::toString(void)
{
        unsigned int i = 0;
        std::string s  = "IODEFV2Class(" + getName();

        do {
                if ( i > 0 )
                        s += ", ";

                try {
                        s += get(i++).toString();
                } catch(...) {
                        break;
                }
        } while ( TRUE );

        s += "\n)";

        return s;
}


bool IODEFV2Class::isList(void)
{
        if ( _pathelem.size() == 0 )
                throw LibIodefError("Already in rootclass, cannot retrieve parents info");

        return iodefv2_class_is_child_list(_pathelem.back().parent_id, _pathelem.back().idx);
}


bool IODEFV2Class::isKeyedList(void)
{
        if ( _pathelem.size() == 0 )
                throw LibIodefError("Already in rootclass, cannot retrieve parents info");

        return iodefv2_class_is_child_keyed_list(_pathelem.back().parent_id, _pathelem.back().idx);
}


Iodef::IODEFV2Value::IODEFV2ValueTypeEnum IODEFV2Class::getValueType(void)
{
        if ( _pathelem.size() == 0 )
                throw LibIodefError("Already in rootclass, cannot retrieve parents info");

        return (Iodef::IODEFV2Value::IODEFV2ValueTypeEnum) iodefv2_class_get_child_value_type(_pathelem.back().parent_id, _pathelem.back().idx);
}


std::string IODEFV2Class::getName(void)
{
        if ( _pathelem.size() == 0 )
                return iodefv2_class_get_name(_id);

        return iodefv2_class_get_child_name(_pathelem.back().parent_id, _pathelem.back().idx);
}


size_t IODEFV2Class::getDepth(void)
{
        return _pathelem.size();
}


std::string IODEFV2Class::getPath(int rootidx, int depth, const std::string &sep, const std::string &listidx)
{
        std::string out;

        if ( depth >= 0 ) {
                if ( (depth + 1) == _pathelem.size() )
                        return getName();

                IODEFV2ClassElem elem = _pathelem[depth];
                return iodefv2_class_get_child_name(elem.parent_id, elem.idx);
        }

        for ( std::vector<IODEFV2ClassElem>::iterator it = _pathelem.begin() + rootidx; it != _pathelem.end(); it++) {
                out += iodefv2_class_get_child_name((*it).parent_id, (*it).idx);

                if ( iodefv2_class_is_child_list((*it).parent_id, (*it).idx) )
                        out += listidx;

                if ( it + 1 != _pathelem.end() )
                        out += sep;
        }

        return out;
}



IODEFV2Class IODEFV2Class::get(const std::string &name)
{
        int i = iodefv2_class_find_child(_id, name.c_str());

        if ( i < 0 )
                throw LibIodefError(i);

        return get(i);
}


IODEFV2Class IODEFV2Class::get(int i)
{
        iodefv2_class_id_t cl;
        iodefv2_value_type_id_t vl;

        cl = iodefv2_class_get_child_class(_id, i);
        if ( cl < 0 ) {
                vl = (iodefv2_value_type_id_t) iodefv2_class_get_child_value_type(_id, i);
                if ( vl < 0 )
                        throw LibIodefError(vl);
        }

        return IODEFV2Class(*this, i, _depth + 1);
}



std::vector<std::string> IODEFV2Class::getEnumValues(void)
{
        int i = 0;
        const char *ret;
        std::vector<std::string> ev;

        if ( getValueType() != IODEFV2Value::TYPE_ENUM )
                throw LibIodefError("Input class is not enumeration");

        do {
                ret = iodefv2_class_enum_to_string(_id, i++);
                if ( ret )
                        ev.push_back(ret);

        } while ( ret || i == 1 ); /* entry 0 might be NULL, if the enumeration has no default value */

        return ev;
}



IODEFV2Criterion::IODEFV2CriterionOperatorEnum IODEFV2Class::getApplicableOperator(void)
{
        int ret;
        iodefv2_criterion_operator_t op;

        ret = iodefv2_value_type_get_applicable_operators((iodefv2_value_type_id_t) getValueType(), &op);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return (IODEFV2Criterion::IODEFV2CriterionOperatorEnum) ret;
}
