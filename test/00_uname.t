use Test::More tests => 2;
# Copyright (c) 2022 Faraz V faraz@fzv.ca.
# SPDX-License-Identifier: BSD-3-Clause
use warnings;
use strict;

use lib '.';

use Test::Deep;
use PiconTest;

my $picon_version = "0.4.0";
my $rtos_version  = "V10.4.3";
my $pico_sdk_version = "1.3.0";


my %tests = (
	"uname" =>
qr/Picon Version: \Q$picon_version\E
FreeRTOS version: \Q$rtos_version\E
PICO SDK: \Q$pico_sdk_version\E
built: \w\w\w\s+\d+ 20\d\d, \d+:\d+:\d+
/m,
	"uname -a" =>
qr/Picon Version: \Q$picon_version\E
FreeRTOS version: \Q$rtos_version\E
PICO SDK: \Q$pico_sdk_version\E
built: \w\w\w\s+\d+ 20\d\d, \d+:\d+:\d+
NOR Flash ID: [A-F0-9]+
Chip ID: [a-z0-9]+
/m

);

while (my ($cmd, $expected) = each %tests) {

	my $result = PiconTest::run($cmd);

	like($result, $expected, "$cmd worked");

}

