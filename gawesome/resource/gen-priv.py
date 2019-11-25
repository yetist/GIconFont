#! /usr/bin/env python
# -*- encoding:utf-8 -*-
# FileName: gen.py

"This file is part of ____"

__author__   = "Wu Xiaotian"
__copyright__= "Copyright (C) 2019 Wu Xiaotian <yetist@gmail.com>"
__license__  = """
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
"""

import configparser

def gen_header():
    HEADER = """/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gawesome-priv.h: This file is part of gawesome.
 *
 * Copyright (C) 2019 yetist <yetist@yetibook>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */

#ifndef __GAWESOME_PRIV_H__
#define __GAWESOME_PRIV_H__  1

#include <glib.h>

G_BEGIN_DECLS
"""
    print(HEADER)

def gen_enum_code(items):
    print('typedef enum\n{')

    for item in items[:-1]:
        name = 'G_AWESOME_CODE_' + item[0].replace('-','_').upper()
        print('    %s = %s,' % (name, item[1]))
    name = 'G_AWESOME_CODE_' + items[-1][0].replace('-','_').upper()
    print('    %s = %s' % (name, items[-1][1]))

    print('} GAwesomeCode;\n')

def gen_array(items):
    print('struct _GAwesomeNameIcon {')
    print('    const gchar *name;')
    print('    GAwesomeCode code;')
    print('};\n')
    print('static const struct _GAwesomeNameIcon gaNameIconArray[] = {')

    for item in items[:-1]:
        name = 'G_AWESOME_CODE_' + item[0].replace('-','_').upper()
        print('    { "%s", %s },' % (item[0], name))
    name = 'G_AWESOME_CODE_' + items[-1][0].replace('-','_').upper()
    print('    { "%s", %s }' % (items[-1][0], name))
    print('};\n')

def gen_footer():
    END="""G_END_DECLS

#endif /* __GAWESOME_PRIV_H__ */"""
    print(END)

if __name__=="__main__":
    config = configparser.ConfigParser()
    config.read('fontawesome-webfont.ini')
    items = config['FontAwesome'].items()
    list = list(items)

    gen_header()
    gen_enum_code(list)
    gen_array(list)
    gen_footer()
