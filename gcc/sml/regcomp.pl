

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
        '|' => 1,
        '.' => 2,
        '*' => 3,
        '+' => 3,
        '?' => 3,
    );

    my %associativity = (
        '|' => 'left',
        '.' => 'left',
        '*' => 'right',
        '+' => 'right',
        '?' => 'right',
    );

    my @output;
    my @stack;

    my @tokens = split /\s*/, $infix_expression;

    for my $token (@tokens) {
        if ($token =~ /[^\(\)\*\+\?\|\.]/) {
            push @output, $token;
        } elsif ($token eq '(') {
            push @stack, $token;
        } elsif ($token eq ')') {
            while (@stack && $stack[-1] ne '(') {
                push @output, pop @stack;
            }
            pop @stack;
        } elsif ($precedence{$token}) {
            while (
                @stack
                && (
                    $precedence{$stack[-1]} > $precedence{$token}
                    || ($precedence{$stack[-1]} == $precedence{$token}
                        && $associativity{$token} eq 'left')
                )
            ) {
                push @output, pop @stack;
            }
            push @stack, $token;
        }
    }

    while (@stack) {
        push @output, pop @stack;
    }

    return join(' ', @output);
}
