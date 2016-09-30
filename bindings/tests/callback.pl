#!/usr/bin/perl -w
#

use strict;

push @INC,".";
push @INC,"./perl";
push @INC,"./.libs";

eval  { require LibIodefEasy; };
die "Could not load LibIodefEasy ($@).\nTry 'cd ./.libs && ln -s libiodefv2_perl.so LibIodefEasy.so'" if $@;

sub PrintUID
{
	print "UID is $<\n";
}

LibIodefEasy::set_perlmethod(\&PrintUID);

LibIodefEasy::test_fct();
