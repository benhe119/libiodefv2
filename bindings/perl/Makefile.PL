#
# Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
# Author: Yoann Vandoorselaere <yoann.v@libiodefv2-ids.com>
#
# This file is part of the LibIodef library.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

use Config;
use ExtUtils::MakeMaker;
use ExtUtils::MM_Unix;
use File::Copy;
use Cwd "abs_path";

my %attributs = (NAME => 'LibIodef',
		 INC => '-I../.. -I../../src/include -I../../src/include -I../../src/libiodefv2-error -I../../bindings/c++/include',
		 LIBS => ["-L../../src/.libs -llibiodefv2 -L../../bindings/c++/.libs -llibiodefv2cpp", "-L$ENV{LIBDIR} -llibiodefv2  -lgcrypt -ldl -lgpg-error -ldl  " ],
		 LDDLFLAGS => "-L../../src/.libs $Config{lddlflags}");

if ( abs_path("../..") ne abs_path("../..") ) {
    foreach my $filename ( qw/LibIodef.cxx LibIodef.pm/ ) {
	my $src = "../../bindings/perl/$filename";
	my $dst = "../../bindings/perl/$filename";
	copy($src, $dst) if ( ! -e $dst );
    }
    $attributs{"clean"} = {FILES => join(" ", map { "../../bindings/perl/$_" } qw/LibIodef.cxx LibIodef.pm Makefile/)};
}

if ( -w $Config{"sitelib"} ) {
    $attributs{"INSTALLDIRS"} = "site";
} else {
    $attributs{"PREFIX"} = "/usr/local";
}

WriteMakefile(%attributs);

my($atime, $mtime) = (stat "Makefile.PL")[8,9];
utime($atime + 1, $mtime + 1, "Makefile");


package MY;

sub install
{
    my $str = shift->SUPER::install(@_);
    my @files = ((map { "\$(INSTALLSITEARCH)/$_" } qw/perllocal.pod LibIodef.pm/),
                 (map { "\$(INSTALLSITEARCH)/auto/LibIodef/$_" } qw/LibIodef.bs LibIodef.so .packlist/),
                 '$(INSTALLMAN3DIR)/LibIodef.$(MAN3EXT)');

    $str =~ s/(uninstall_from_sitedirs.+)\n.+\n/"$1\n\t\$(RM_F) " . join(" ", @files) . "\n"/e;

    return $str;
}
