package PiconTest;
# Copyright (c) 2022 Faraz V faraz@fzv.ca.
# SPDX-License-Identifier: BSD-3-Clause
use strict;
use warnings;

use Device::SerialPort;

my $port = Device::SerialPort->new($ENV{"PICON_TTY"});

$port->baudrate(115200);
$port->databits(8);
$port->parity("none");
$port->stopbits(1);

sub run {
	my ($cmd) = @_;

	return undef unless $cmd;

	$port->write("$cmd\n");

	my $result = '';
	while (1) {
		my $byte=$port->read(1);
		next if $byte eq "\r";

		$result .= $byte;
		if ($byte eq '#') {
			last;
		}
	}

	$result =~ s/\s*Welcome.*$//m;
	$result =~ s/\s*$//m;
	$result =~ s/\s*$cmd\s*//g;
	$result =~ s/#.*//g;

	return $result;
}


1;
