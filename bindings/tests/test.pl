#!/usr/bin/env perl

use LibIodef;

sub log_func {
	($level, $str) = @_;
	print("log: " . $str);
}

sub print_array($)
{
    my $arrRef = shift;
    for (my $i = 0; $i < scalar(@$arrRef); ++$i) {
        if (ref($arrRef->[$i]) eq 'ARRAY') {
            print ', ' if ($i);
            print '[';
            print_array($arrRef->[$i]);
            print ']';
        } else {
            print ', ' if ($i);
            print $arrRef->[$i];
        }
    }
    print ' ' if (!scalar(@$arrRef));
}


LibIodef::LibIodefLog::setCallback(\&log_func);

$iodefv2 = new LibIodef::IODEFV2;

print "*** IODEFV2->Set() ***\n";
$iodefv2->set("alert.classification.text", "My Message");
$iodefv2->set("alert.source(0).node.address(0).address", "s0a0");
$iodefv2->set("alert.source(0).node.address(1).address", "s0a1");
$iodefv2->set("alert.source(1).node.address(0).address", "s1a0");
$iodefv2->set("alert.source(1).node.address(1).address", "s1a1");
$iodefv2->set("alert.source(1).node.address(2).address", undef);
$iodefv2->set("alert.source(1).node.address(3).address", "s1a3");
print $iodefv2->toString();


print "\n*** IODEFV2->Get() ***\n";
print $iodefv2->get("alert.classification.text") . "\n";

print "\n\n*** Value IODEFV2->Get() ***\n";
print $iodefv2->get("alert.classification.text");

print "\n\n*** Listed Value IODEFV2->Get() ***\n";
print_array($iodefv2->get("alert.source(*).node.address(*).address"));

print "\n\n*** Object IODEFV2->Get() ***\n";
print $iodefv2->get("alert.source(0).node.address(0)");

print "\n\n*** Listed Object IODEFV2->Get() ***\n";
print_array($iodefv2->get("alert.source(*).node.address(*)"));


open FILE, ">foo.bin" or die "arg";
$iodefv2->write(FILE);
close FILE;

print "\n\n*** IODEFV2->Read() ***\n";
open FILE2, "<foo.bin" or die "arg2";
my $iodefv22 = new LibIodef::IODEFV2;
while ( $iodefv22->read(FILE2) ) {
	print $iodefv22->toString();
}

close FILE2;

print "\n*** Client ***";
$client = new LibIodef::ClientEasy("libiodefv2-lml");
$client->start();

$client->sendIODEFV2($iodefv2);
