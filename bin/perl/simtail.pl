#!/usr/bin/perl -w
#
# Program to be distributed under GPLv3 as present in LICENSE file in
# the root dir of this repository.
# Description - Simulate the tail -f command
# Usage - perl simtail.pl <filename> <n> where n is the last n lines to show
#
use strict;
use warnings;
use constant DEFAULT_N => 10;
my ($filename, $linecnt) = @ARGV;
(not defined($filename)) and die "Provide file to open";
(not defined($linecnt)) and ($linecnt = DEFAULT_N);

my $fh;
open($fh, $filename) || die "failed to open file $filename: $!";

my $lastpos      = 0; # record file pointer's last position
my $skiplines    = 0; # flag to skip first (total_lines - linecount) lines
my $lastchkdsize = 0; # record last size to know if any newlines added
my $newsize = -s $filename;
($newsize > 0) and ($skiplines = 1);

while(1) {
    # Have any new lines been added / file truncated? 
    while($newsize == $lastchkdsize) {
	sleep 1;
	if (! -f $filename) {
	    print "\n----File deleted----\n";
	    exit 1;
	}
	$newsize = -s $filename;
    }

    $lastchkdsize = $newsize;
    if ($newsize == 0) {
        print "\n----File truncated----\n";
	$lastpos = 0; # rewind to the top
        die "failed to seek: $!" unless seek($fh, $lastpos, 0);
	next;
    }
    die "failed to seek: $!" unless seek($fh, $lastpos, 0);

    my $currcnt = 1;
    my @display_data;
    while(! eof($fh)) {
	my $currline = <$fh>;
	push @display_data, $currline;
	if ($currcnt > $linecnt and $skiplines == 1) {
	    # For every push after linecnt reached, shift out the 
	    # topmost line
	    shift @display_data;
	}
	$currcnt++;
    }
    ($skiplines == 1) and ($skiplines = 0);

    $lastpos = tell($fh) || die "failed to tell: $!"; # record curr pos

    while (@display_data) {
        print shift @display_data;
    }
}
close($fh);
exit(0);
