package PiconTest;
# Copyright (c) 2022 Faraz V faraz@fzv.ca.
# SPDX-License-Identifier: BSD-3-Clause
use strict;
use warnings;

use Device::SerialPort;

sub run {
	my ($cmd) = @_;

	return undef unless $cmd;

	my $port = Device::SerialPort->new($ENV{"PICON_TTY"});
	return undef unless $port;

	$port->baudrate(115200);
	$port->databits(8);
	$port->parity("none");
	$port->stopbits(1);

	$port->read_char_time(20);
	$port->read_const_time(100);

	# get rid of anything that's in the buffer before issuing the command
	while (1) {
		my ($nread, $char) = $port->read(1);
		last if ($nread != 1);
	}

	$port->write("$cmd\n");

	my $result = '';
	while (1) {
		my $char=$port->read(1);
		next if $char eq "\r"; # get rid of CR

		$result .= $char;
		last  if ($char eq '#');
	}

	$result =~ s/\s*$//m;
	$result =~ s/\s*$cmd\s*//g;
	$result =~ s/#.*//g;

	return $result;
}


1;
