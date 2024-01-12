#!/usr/bin/env perl

# RegComp is a small preprocessor similar to re2c --- it takes a preprocessor directive containing a regular expressison, converts that regular expression to NFA, the converts that NFA to DFA, and then compiles it to C code.
# You must RegComp a file and it will bypass any line that is not prefixed with '==='. Th lines prefixed by '===' get preprocessed into compiled RE. It requires several patameters:
# === regex::"<regex>" buff::"<buffername>" next::"<get next char in buffer"

use strict;
use warnings;
use Data::Dumper;

my @directives;
my @bypass;

while (<>) {
	if (m<^\s*===\s+regex::"(.*)"\s+buff::"(.*)"\s+next::"(.*)"\s*$>) {
		push @directives, { re => $1, buff => $2, nxt => $3 };
		push @bypass, \$directives[-1];
	} else {
		push @bypass, $_;
	}

}

sub infix_to_postfix {
    my ($infix_expression) = @_;

    my %precedence = (
        '|' => 1,        # Alternation
        '.' => 2,        # Concatenation
        '*' => 3,        # Zero or more repetitions
        '+' => 3,        # One or more repetitions
        '?' => 3,        # Optional 
	'-' => 2, 	 # Character class
    );

    my @output;
    my @stack;

    # Split the infix expression into tokens
    my @tokens = split /\s*/, $infix_expression;

    for my $token (@tokens) {
        if ($token =~ /\w|\-/) {
            push @output, $token;
        } elsif ($token eq '[') {
            push @stack, $token;
        } elsif ($token eq ']') {
	    while (@stack && $stack[-1] ne ']') {
		push @output, pop @stack;
	    }	
	} elsif ($token eq '(') {
            push @stack, $token;
        } elsif ($token eq ')') {
            while (@stack && $stack[-1] ne '(') {
                push @output, pop @stack;
            }
            pop @stack;
        } elsif ($precedence{$token}) {
            while (@stack && $precedence{$stack[-1]} >= $precedence{$token}) {
                push @output, pop @stack;
            }
            push @stack, $token;
        }
    }

    while (@stack) {
        push @output, pop @stack;  # Pop remaining operators
    }

    return join(' ', @output);
}


print infix_to_postfix("ab[a-z]b*k+");
