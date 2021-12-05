#!/usr/bin/env perl
use strict;
use warnings;

use String::CamelCase qw(decamelize);
#use C::Scan;
use ModPerl::CScan;
use Data::Dumper;
use File::Basename;

sub unhun {
	my ($thing, $type) = @_;

	return $thing unless $thing;

	$type ||= '';

	$type = lc $type;

	if ($type ne 'define')  {
		# remove the stupid freaking hungarian type notation
		$thing =~ s/^[a-z]+([A-Z].*)/$1/; 
	}

	if ($type eq 'define') {
		# some freaking Macros in FreeRTOS still have types
		$thing =~ s/^p?d//;
		$thing =~ s/^pc//;
		$thing =~ s/^err([A-Z])/ERR_$1/;
		$thing =~ s/^e//;
		$thing =~ s/^v?x?([A-Z].*)/$1/; 

		$thing =~ s/^INCLUDE_p?d./INCLUDE_/;
		$thing =~ s/^INCLUDE_pc/INCLUDE_/;
		$thing =~ s/^INCLUDE_e/INCLUDE_/;
		$thing =~ s/^INCLUDE_v?x?([A-Z].*)/INCLUDE_$1/; 
	}

	$thing = 'rtos_'.decamelize($thing);

	if ($type eq 'define') {
		$thing = uc $thing;
	}

	return $thing;
}


if (scalar(@ARGV) == 0) {
	print STDERR "usage : $0 <C header file>\n";
	exit;
}


my $objs = {};
# {
#     obj_name => {  type => '',  value => '' },
#
# }


for my $hfile (@ARGV) {

	if (! -r $hfile) {
		print STDERR "Failed to read $hfile\n";
		exit;
	}

	my $content;
	open FH, $hfile;
	{
		local $/;
		$content = <FH>;
	}
	close FH;

	my $temp_filename = "blahblah.h";
	open FH, "> $temp_filename";
	print FH "#include \"FreeRTOSConfig.h\"\n";
	if ($hfile ne "FreeRTOS.h") {
		print FH "#include \"FreeRTOS.h\"\n";
	}
	
	print FH "\n";
	print FH $content;
	close FH;	

	#my $c = C::Scan->new( filename => $temp_filename);
	my $c = ModPerl::CScan->new( filename => $temp_filename);

	$c->set('includeDirs' => ['../src/include',
				  '../freertos/include',
				 # '../freertos/portable/ThirdParty/GCC/RP2040/include/',
				 '../freertos/portable/ThirdParty/GCC/rpi_pico/',
				 # $ENV{PICO_SDK_PATH}."/src/boards/include/boards/",
				 # $ENV{PICO_SDK_PATH}."/src/common/pico_base/include/",
				 # $ENV{PICO_SDK_PATH}."/src/./src/rp2_common/hardware_sync/include/",
				 # $ENV{PICO_SDK_PATH}."/src/host/hardware_sync/include/",
				 # $ENV{PICO_SDK_PATH}."/src/common/pico_sync/include/",
				 ]);

	my $defs_args  = $c->get('defines_args');
	my $defs_no_args  = $c->get('defines_no_args');
	my $func    = $c->get('parsed_fdecls');
	my $inlines  = $c->get('inlines');
	my $typedefs = $c->get('typedef_hash');
	my $externs = $c->get('vdecl_hash');

	if (scalar(keys %$typedefs)) {
		for my $x (keys %$typedefs) {
			next if $x =~ /^_/;

			my $sane = unhun($x, 'type');
			if ($x ne $sane) {
				if (defined($objs->{$sane}) &&
				    ($objs->{$sane}->{type} ne 'type' ||
				     $objs->{$sane}->{value} ne $x)) {
					printf(STDERR "ERROR: symbol %s was defined as '%s' as type '%s', in %s\n", 
					       $sane, $objs->{$sane}->{value}, $objs->{$sane}->{type},
					       $objs->{$sane}->{file});
					#exit(1);
				} else {
					$objs->{$sane} = { type => 'type', value => $x, file => basename($hfile) };
				}
			}
		}
	}

	if (scalar( keys %$defs_args )) {
		for my $x (keys %$defs_args) {
			my $sane = unhun($x, 'define');
			if ($x ne $sane) {
				if (defined($objs->{$sane}) &&
				    ($objs->{$sane}->{type} ne 'define' ||
				     $objs->{$sane}->{value} ne $x)) {
					printf(STDERR "ERROR: symbol %s was defined as '%s' as type '%s', in %s\n", 
					       $sane, $objs->{$sane}->{value}, $objs->{$sane}->{type},
					       $objs->{$sane}->{file});
					#exit(1);
				} else {
					$objs->{$sane} = { type => 'define', value => $x, file => basename($hfile) };
				}
			}
		}
	}

	if (scalar( keys %$defs_no_args )) {
		for my $x (keys %$defs_no_args) {
			my $sane = unhun($x, 'define');
			if ($x ne $sane) {
				if (defined($objs->{$sane}) &&
				    ($objs->{$sane}->{type} ne 'define' ||
				     $objs->{$sane}->{value} ne $x)) {
					printf(STDERR "ERROR: symbol %s was defined as '%s' as type '%s', in %s\n", 
					       $sane, $objs->{$sane}->{value}, $objs->{$sane}->{type},
					       $objs->{$sane}->{file});
					#exit(1);
				} else {
					$objs->{$sane} = { type => 'define', value => $x, file => basename($hfile) };
				}
			}
		}
	}


	if (scalar(@$func)) {
		for my $func (@$func) {
			my $x = $func->[1];
			my $sane = unhun($x, 'func');
			if ($x ne $sane) {
				if (defined($objs->{$sane}) &&
				    ($objs->{$sane}->{type} ne 'func' ||
				     $objs->{$sane}->{value} ne $x)) {
					printf(STDERR "ERROR: symbol %s was defined as '%s' as type '%s', in %s\n", 
					       $sane, $objs->{$sane}->{value}, $objs->{$sane}->{type},
					       $objs->{$sane}->{file});
					#exit(1);
				} else {
					$objs->{$sane} = { type => 'func', value => $x, file => basename($hfile) };
				}
			}
		}
	}
	unlink $temp_filename;

}
print "#ifndef _UN_HUN_\n";
print "#define _UN_HUN_\n\n";

for my $arg (@ARGV) {
	printf("#include \"%s\"\n", basename($arg));
}

print "\n";
print "/* Macros */\n";
my $include_file = '';
for my $thing (sort keys %$objs) {
	next unless $objs->{$thing}->{type} eq 'define';

	if ($include_file ne $objs->{$thing}->{file}) {
		printf("/* from %s */\n", $objs->{$thing}->{file});
		$include_file = $objs->{$thing}->{file}
	}
	printf("#define %-44s \t %s\n", $thing, $objs->{$thing}->{value});
}
print "\n";

print "\n";
print "/* typedefs */\n";
$include_file = '';
for my $thing (sort keys %$objs) {
	next unless $objs->{$thing}->{type} eq 'type';

	if ($include_file ne $objs->{$thing}->{file}) {
		printf("/* from %s */\n", $objs->{$thing}->{file});
		$include_file = $objs->{$thing}->{file}
	}

	printf("#define %-44s \t %s\n", $thing, $objs->{$thing}->{value});
}
print "\n";

print "\n";
print "/* functions */\n";
$include_file = '';
for my $thing (sort keys %$objs) {
	next unless $objs->{$thing}->{type} eq 'func';

	if ($include_file ne $objs->{$thing}->{file}) {
		printf("/* from %s */\n", $objs->{$thing}->{file});
		$include_file = $objs->{$thing}->{file}
	}

	printf("#define %-44s \t %s\n", $thing, $objs->{$thing}->{value});
}
print "\n\n";

print "#endif\n";

